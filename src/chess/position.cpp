#include "position.hpp"
#include "attacks.hpp"
#include "draw.hpp"
#include "hash.hpp"
#include "pieces.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int fileranktosquareidx(int file, int rank) { return (rank)*8 + file; }

int getrank(int square) {
	assert(square >= 0 && square <= 63);
	return square / 8;
}

int getfile(int square) {
	assert(square >= 0 && square <= 63);
	return square % 8;
}

int strsquaretoidx(std::string square) {
	int file = (int)square[0] - 97;
	int rank = (int)square[1] - 49;
	return fileranktosquareidx(file, rank);
}

char Position::getPiece(const int sq) const {
	assert(sq >= 0 && sq <= 63);
	U64 BBsquare = (1ULL << sq);
	for (int i = PAWN; i <= KING; i++) {
		if (pieces[i] & BBsquare)
			return i;
	}
	return NONE;
}

int Position::getColour(int sq) const {
	U64 BBsquare = (1ULL << sq);
	for (int i = 0; i < 2; i++) {
		if (colours[i] & BBsquare)
			return i;
	}
	return NONE;
}

void Position::setPiece(int sq, int colour, char piece) {
	assert(sq >= 0 && sq <= 63);
	U64 BBsquare = (1ULL << sq);
	// clear bitboard of old square
	int oldpiece = getPiece(sq);
	pieces[oldpiece] &= ~BBsquare;
	// clear white and black piece bitboards of that square
	colours[WHITE] &= ~BBsquare;
	colours[BLACK] &= ~BBsquare;
	if (piece != NONE)
		pieces[piece] |= BBsquare;
	if (piece == KING) {
		if (colour == WHITE)
			Wkingpos = sq;
		else if (colour == BLACK)
			Bkingpos = sq;
	}
	colours[colour] |= BBsquare;
}

void Position::clearSquare(int sq) {
	// clears a square of a given piece
	colours[WHITE] &= ~(1ULL << sq);
	colours[BLACK] &= ~(1ULL << sq);
	int oldpiece = getPiece(sq);
	pieces[oldpiece] &= ~(1ULL << sq);
}

void dspBB(U64 BB) {
	std::cout << "  +---+---+---+---+---+---+---+---+\n";
	std::cout << "8 |";

	int rank = 0;
	int file = 0;
	int sq = 0;
	for (rank = 7; rank >= 0; rank--) {
		for (file = 0; file <= 7; file++) {
			sq = fileranktosquareidx(file, rank);
			if ((1ULL << sq) & BB) {
				std::cout << " 1 |";
			} else {
				std::cout << " 0 |";
			}
		}
		std::cout << "\n";
		std::cout << "  +---+---+---+---+---+---+---+---+\n";
		if ((sq / 8) != 0) {
			std::cout << (sq / 8);
			std::cout << " |";
		}
	}
	std::cout << "    A   B   C   D   E   F   G   H  \n";
}

void dspBoard(Position *pos) {
	std::cout << "\n";
	std::cout << "  +---+---+---+---+---+---+---+---+\n";
	std::cout << "8 |";

	int rank = 0;
	int file = 0;
	int sq = 0;
	for (rank = 7; rank >= 0; rank--) {
		for (file = 0; file <= 7; file++) {
			sq = fileranktosquareidx(file, rank);
			char piece = pos->getPiece(sq);
			if (piece == NONE)
				piece = ' ';
			else if (piece == PAWN)
				piece = 'P';
			else if (piece == KNIGHT)
				piece = 'N';
			else if (piece == BISHOP)
				piece = 'B';
			else if (piece == ROOK)
				piece = 'R';
			else if (piece == QUEEN)
				piece = 'Q';
			else if (piece == KING)
				piece = 'K';
			if (pos->getColour(sq) == BLACK)
				piece = tolower(piece);
			std::cout << " " << piece << " |";
		}
		std::cout << "\n";
		std::cout << "  +---+---+---+---+---+---+---+---+\n";
		if ((sq / 8) != 0) {
			std::cout << (sq / 8);
			std::cout << " |";
		}
	}
	std::cout << "    A   B   C   D   E   F   G   H  \n";
	std::cout << "Side to move: ";
	if (pos->tomove == 0)
		std::cout << "Black";
	else
		std::cout << "White";
	std::cout << "\n";
	std::cout << "EP Square: " << pos->epsquare;
	std::cout << "\n";
	std::cout << "White King pos: " << pos->Wkingpos;
	std::cout << "\n";
	std::cout << "Black King pos: " << pos->Bkingpos;
	std::cout << "\n";
	std::cout << "White castling QS: " << pos->WcastleQS;
	std::cout << "\n";
	std::cout << "White castling KS: " << pos->WcastleKS;
	std::cout << "\n";
	std::cout << "Black castling QS: " << pos->BcastleQS;
	std::cout << "\n";
	std::cout << "Black castling KS: " << pos->BcastleKS;
	std::cout << "\n";
	std::cout << "Half moves: " << pos->halfmoves;
	std::cout << "\n";
	std::cout << "Hash: " << generateHash(pos);
	std::cout << "\n";
	std::cout << "In check: " << isCheck(pos);
	std::cout << "\n";
	std::cout << "Is threefold: " << isThreefold(pos);
	std::cout << "\n";
}

Position::Position(const std::string &fen) { setFen(fen); }

void Position::setFen(const std::string &fen) {
	if (fen == "startpos") {
		setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		return;
	}

	// set blank position
	for (int i = 0; i < 2; i++) {
		colours[i] = 0ULL;
	}
	for (int i = 0; i < 6; i++) {
		pieces[i] = 0ULL;
	}
	epsquare = -1;
	WcastleKS = 0;
	WcastleQS = 0;
	BcastleKS = 0;
	BcastleQS = 0;

	int numtokens = 0;

	std::string line, intermediate;
	// std::string intermediate;

	line = fen;

	// Vector of string to save tokens
	std::vector<std::string> tokens;

	// stringstream class check1
	std::stringstream check1(line);
	// Tokenizing w.r.t. space ' '
	while (getline(check1, intermediate, ' ')) {
		numtokens++;
		tokens.push_back(intermediate);
	}

	int j = 0;
	for (int i = 0; i < (int)tokens[0].length(); i++) {
		char letter = tokens[0][i];

		// get rank and file of A1 = 0 board from A8 = 0 board
		int realrank = 7 - (j / 8);
		int realfile = j % 8;
		int a = fileranktosquareidx(realfile, realrank);
		switch (letter) {
		case 'p': {
			pieces[PAWN] |= (1ULL << a);
			colours[BLACK] |= (1ULL << a);
			break;
		}
		case 'n': {
			pieces[KNIGHT] |= (1ULL << a);
			colours[BLACK] |= (1ULL << a);
			break;
		}
		case 'b': {
			pieces[BISHOP] |= (1ULL << a);
			colours[BLACK] |= (1ULL << a);
			break;
		}
		case 'r': {
			pieces[ROOK] |= (1ULL << a);
			colours[BLACK] |= (1ULL << a);
			break;
		}
		case 'q': {
			pieces[QUEEN] |= (1ULL << a);
			colours[BLACK] |= (1ULL << a);
			break;
		}
		case 'k': {
			pieces[KING] |= (1ULL << a);
			colours[BLACK] |= (1ULL << a);
			Bkingpos = a;
			break;
		}
		case 'P': {
			pieces[PAWN] |= (1ULL << a);
			colours[WHITE] |= (1ULL << a);
			break;
		}
		case 'N': {
			pieces[KNIGHT] |= (1ULL << a);
			colours[WHITE] |= (1ULL << a);
			break;
		}
		case 'B': {
			pieces[BISHOP] |= (1ULL << a);
			colours[WHITE] |= (1ULL << a);
			break;
		}
		case 'R': {
			pieces[ROOK] |= (1ULL << a);
			colours[WHITE] |= (1ULL << a);
			break;
		}
		case 'Q': {
			pieces[QUEEN] |= (1ULL << a);
			colours[WHITE] |= (1ULL << a);
			break;
		}
		case 'K': {
			pieces[KING] |= (1ULL << a);
			colours[WHITE] |= (1ULL << a);
			Wkingpos = a;
			break;
		}
		case '/':
			j--;
			break;
		case '1':
			break;
		case '2':
			j++;
			break;
		case '3':
			j += 2;
			break;
		case '4':
			j += 3;
			break;
		case '5':
			j += 4;
			break;
		case '6':
			j += 5;
			break;
		case '7':
			j += 6;
			break;
		case '8':
			j += 7;
			break;
		}
		j++;
	}

	if (tokens[1] == "w")
		tomove = WHITE;
	if (tokens[1] == "b")
		tomove = BLACK;

	for (size_t i = 0; i < tokens[2].length(); i++) {
		if (tokens[2][i] == 'K')
			WcastleKS = 1;
		else if (tokens[2][i] == 'Q')
			WcastleQS = 1;
		else if (tokens[2][i] == 'k')
			BcastleKS = 1;
		else if (tokens[2][i] == 'q')
			BcastleQS = 1;
	}

	if (numtokens >= 4 && tokens[3] != "-") {
		// en passant square given
		epsquare = strsquaretoidx(tokens[3]);
	} else
		epsquare = -1;

	if (numtokens >= 5)
		halfmoves = std::stoi(tokens[4]);
	else
		halfmoves = 0;

	if (numtokens >= 6)
		fullmoves = std::stoi(tokens[5]);
	else
		fullmoves = 0;

	irrevidx = 0;
	irrev[irrevidx].epsquare = epsquare;
	irrev[irrevidx].WcastleQS = WcastleQS;
	irrev[irrevidx].WcastleKS = WcastleKS;
	irrev[irrevidx].BcastleQS = BcastleQS;
	irrev[irrevidx].BcastleKS = BcastleKS;
	irrev[irrevidx].Wcastled = Wcastled;
	irrev[irrevidx].Bcastled = Bcastled;
	irrev[irrevidx].halfmoves = halfmoves;
	irrev[irrevidx].Wkingpos = Wkingpos;
	irrev[irrevidx].Bkingpos = Bkingpos;
	hashstack[irrevidx] = generateHash(this);
}

std::string Position::getFen() const {
	const char pieceChars[2][6] = {
		{'p', 'n', 'b', 'r', 'q', 'k'},
		{'P', 'N', 'B', 'R', 'Q', 'K'},
	};

	std::string fen = "";

	// Pieces
	for (int r = 7; r >= 0; r--) {
		int spaces = 0;

		for (int f = 0; f < 8; ++f) {
			const int sq = 8 * r + f;
			const int piece = getPiece(sq);
			const int colour = getColour(sq);

			if (piece == NONE) {
				spaces++;
			} else {
				if (spaces > 0) {
					fen += std::to_string(spaces);
					spaces = 0;
				}
				fen += pieceChars[colour][piece];
			}
		}

		if (spaces > 0) {
			fen += std::to_string(spaces);
		}

		if (r > 0) {
			fen += "/";
		}
	}

	// Side to move
	if (tomove == WHITE) {
		fen += " w ";
	} else {
		fen += " b ";
	}

	// Castling permissions
	const bool hasCastling = WcastleQS | WcastleKS | BcastleKS | BcastleQS;
	if (!hasCastling) {
		fen += "-";
	} else {
		if (WcastleQS) {
			fen += "K";
		}
		if (WcastleKS) {
			fen += "Q";
		}
		if (BcastleKS) {
			fen += "k";
		}
		if (BcastleQS) {
			fen += "q";
		}
	}

	// En passant
	if (epsquare != -1) {
		const int file = getfile(epsquare);
		const int rank = getrank(epsquare);
		fen += " ";
		fen += static_cast<char>(file + 97);
		fen += static_cast<char>(rank + 49);
	} else {
		fen += " -";
	}

	// Halfmove counter
	fen += " " + std::to_string(halfmoves);

	// Fullmove counter
	fen += " " + std::to_string(fullmoves);

	return fen;
}

Move Position::find_move(const std::string &movestr) const {
	std::string startsquare, endsquare, prompiece;

	startsquare += movestr[0];
	startsquare += movestr[1];
	endsquare += movestr[2];
	endsquare += movestr[3];
	prompiece += movestr[4];
	int startsquareidx = strsquaretoidx(startsquare);
	int endsquareidx = strsquaretoidx(endsquare);
	char cappiece = getPiece(endsquareidx);
	char piece = getPiece(startsquareidx);
	char prom = NONE;

	switch (prompiece[0]) {
	case 'q':
		prom = QUEEN;
		break;
	case 'r':
		prom = ROOK;
		break;
	case 'b':
		prom = BISHOP;
		break;
	case 'n':
		prom = KNIGHT;
		break;
	}

	int isep = 0;
	if (piece == PAWN && endsquareidx == epsquare) {
		if (tomove == WHITE) {
			if (startsquareidx == endsquareidx - 11 || startsquareidx == endsquareidx - 9) {
				isep = 1;
			}
		} else if (tomove == BLACK) {
			if (startsquareidx == endsquareidx + 11 || startsquareidx == endsquareidx + 9) {
				isep = 1;
			}
		}
	}

	int isdouble = 0;
	if (piece == PAWN) {
		if (tomove == WHITE) {
			if (getrank(startsquareidx) == 1 && getrank(endsquareidx) == 3) {
				isdouble = 1;
			}
		} else if (tomove == BLACK) {
			if (getrank(startsquareidx) == 6 && getrank(endsquareidx) == 4) {
				isdouble = 1;
			}
		}
	}

	int isqsc = 0;
	int isksc = 0;
	if (piece == KING) {
		if (tomove == WHITE) {
			if (startsquareidx == E1 && endsquareidx == G1)
				isksc = 1;
			else if (startsquareidx == E1 && endsquareidx == C1)
				isqsc = 1;
		}
		if (tomove == BLACK) {
			if (startsquareidx == E8 && endsquareidx == G8)
				isksc = 1;
			else if (startsquareidx == E8 && endsquareidx == C8)
				isqsc = 1;
		}
	}

	char movetype = NORMAL;
	if (cappiece != NONE && prom != NONE)
		movetype = PROMO_CAPTURE;
	else if (prom != NONE)
		movetype = PROMO;
	else if (isep)
		movetype = EN_PASSANT;
	else if (isdouble)
		movetype = DOUBLE;
	else if (isksc)
		movetype = KSC;
	else if (isqsc)
		movetype = QSC;
	else if (cappiece != NONE)
		movetype = CAPTURE;

	return Move(startsquareidx, endsquareidx, prom, piece, cappiece, movetype);
}

bool Position::validate() const {
	// Check for colour overlap
	if (colours[WHITE] & colours[BLACK]) {
		return false;
	}

	// Check for piece overlaps
	for (int i = PAWN; i < KING; i++) {
		for (int j = i + 1; j <= KING; j++) {
			if (pieces[i] & pieces[j]) {
				return false;
			}
		}
	}

	// No pawns on the 1st or 8th ranks
	if (pieces[PAWN] & (BBrank1 | BBrank8)) {
		return false;
	}

	// Check king positions
	if (Wkingpos != __builtin_ctzll(colours[WHITE] & pieces[KING])) {
		return false;
	}
	if (Bkingpos != __builtin_ctzll(colours[BLACK] & pieces[KING])) {
		return false;
	}

	// For each colour...
	for (int c = 0; c <= 1; c++) {
		const int numPawns = __builtin_popcountll(colours[c] & pieces[PAWN]);
		const int numKnights = __builtin_popcountll(colours[c] & pieces[KNIGHT]);
		const int numBishops = __builtin_popcountll(colours[c] & pieces[BISHOP]);
		const int numRooks = __builtin_popcountll(colours[c] & pieces[ROOK]);
		const int numQueens = __builtin_popcountll(colours[c] & pieces[QUEEN]);
		const int numKings = __builtin_popcountll(colours[c] & pieces[KING]);
		const int total = numPawns + numKnights + numBishops + numRooks + numQueens + numKings;

		// 16 pieces max
		if (total > 16) {
			return false;
		}

		// 8 pawns max
		if (numPawns > 8) {
			return false;
		}

		// 10 knights max
		if (numKnights > 10) {
			return false;
		}

		// 10 bishops max
		if (numBishops > 10) {
			return false;
		}

		// 10 rooks max
		if (numRooks > 10) {
			return false;
		}

		// 9 queens max
		if (numQueens > 9) {
			return false;
		}

		// 1 king
		if (numKings != 1) {
			return false;
		}
	}

	return true;
}
