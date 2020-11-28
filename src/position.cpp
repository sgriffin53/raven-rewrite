#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include "position.hpp"
#include "attacks.hpp"
#include "hash.hpp"
#include "globals.hpp"
#include "draw.hpp"

int fileranktosquareidx(int file,int rank) {
	return (rank) * 8 + file;
}
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
	return fileranktosquareidx(file,rank);
}
char getPiece(Position *pos, int sq) {
	assert(pos);
	assert(sq >= 0 && sq <= 63);
	U64 BBsquare = (1ULL << sq);
	for (int i = PAWN;i <= KING;i++) {
		if (pos->pieces[i] & BBsquare) return i;
	}
	return NONE;
}

int getColour(Position *pos, int sq) {
	U64 BBsquare = (1ULL << sq);
	for (int i = 0;i < 2;i++) {
		if (pos->colours[i] & BBsquare) return i;
	}
	return NONE;
}

void setPiece(Position *pos, int sq, int colour, char piece) {
	assert(pos);
	assert(sq >= 0 && sq <= 63);
	U64 BBsquare = (1ULL << sq);
	//clear bitboard of old square
	int oldpiece = getPiece(pos,sq);
	pos->pieces[oldpiece] &= ~BBsquare;
	//clear white and black piece bitboards of that square
	pos->colours[WHITE] &= ~BBsquare;
	pos->colours[BLACK] &= ~BBsquare;
	if (piece != NONE) pos->pieces[piece] |= BBsquare;
	if (piece == KING) {
		if (colour == WHITE) pos->Wkingpos = sq;
		else if (colour == BLACK) pos->Bkingpos = sq;
	}
	pos->colours[colour] |= BBsquare;
	
}
void clearSquare(Position *pos, int sq) {
	// clears a square of a given piece
	pos->colours[WHITE] &= ~(1ULL << sq);
	pos->colours[BLACK] &= ~(1ULL << sq);
	int oldpiece = getPiece(pos,sq);
	pos->pieces[oldpiece] &= ~(1ULL << sq);
}
void dspBB(U64 BB) {
	
	std::cout << "  +---+---+---+---+---+---+---+---+\n";
	std::cout << "8 |";
	
	int rank = 0;
	int file = 0;
	int sq = 0;
	for (rank = 7;rank >= 0;rank--) {
		for (file = 0;file <= 7;file++) {
			sq = fileranktosquareidx(file,rank);
			if ((1ULL << sq) & BB) {
				std::cout << " 1 |";
			}
			else {
				std::cout << " 0 |";
			}
		}
		std::cout << "\n";
		std::cout << "  +---+---+---+---+---+---+---+---+\n";
		if ((sq/8) != 0) {
			std::cout << (sq/8);
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
	for (rank = 7;rank >= 0;rank--) {
		for (file = 0;file <= 7;file++) {
			sq = fileranktosquareidx(file,rank);
			char piece = getPiece(pos,sq);
			if (piece == NONE) piece = ' ';
			else if (piece == PAWN) piece = 'P';
			else if (piece == KNIGHT) piece = 'N';
			else if (piece == BISHOP) piece = 'B';
			else if (piece == ROOK) piece = 'R';
			else if (piece == QUEEN) piece = 'Q';
			else if (piece == KING) piece = 'K';
			if (getColour(pos, sq) == BLACK) piece = tolower(piece);
			std::cout << " " << piece << " |";
		}
		std::cout << "\n";
		std::cout << "  +---+---+---+---+---+---+---+---+\n";
		if ((sq/8) != 0) {
			std::cout << (sq/8);
			std::cout << " |";
		}
	}
	std::cout << "    A   B   C   D   E   F   G   H  \n";
	std::cout << "Side to move: ";
	if (pos->tomove == 0) std::cout << "Black";
	else std::cout << "White";
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


void parsefen(Position *pos, std::string ofen) {
	if (ofen == "startpos") {
		parsefen(pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		return;
	}
	std::string fen = ofen;

	// set blank position
	for (int i = 0;i < 2;i++) {
		pos->colours[i] = 0ULL;
	}
	for (int i = 0;i < 6;i++) {
		pos->pieces[i] = 0ULL;
	}
	pos->epsquare = -1;
	pos->WcastleKS = 0;
	pos->WcastleQS = 0;
	pos->BcastleKS = 0;
	pos->BcastleQS = 0;
	
	int numtokens;
	
	std::string line, intermediate;
	//std::string intermediate; 

	line = ofen;
  
	// Vector of string to save tokens 
	std::vector <std::string> tokens; 
  
	// stringstream class check1 
	std::stringstream check1(line); 
	// Tokenizing w.r.t. space ' ' 
	while(getline(check1, intermediate, ' ')) 
	{ 
		numtokens++;
		tokens.push_back(intermediate); 
	} 
	
	
	int j = 0;
	for (int i = 0;i < (int)tokens[0].length();i++) {
		char letter = tokens[0][i];
		
		//get rank and file of A1 = 0 board from A8 = 0 board
		int realrank = 7 - (j/8);
		int realfile = j % 8;
		int a = fileranktosquareidx(realfile,realrank);
		switch (letter) {
			case 'p': {
				pos->pieces[PAWN] |= (1ULL << a);
				pos->colours[BLACK] |= (1ULL << a);
				break;
			}
			case 'n': {
				pos->pieces[KNIGHT] |= (1ULL << a);
				pos->colours[BLACK] |= (1ULL << a);
				break;
			}
			case 'b': {
				pos->pieces[BISHOP] |= (1ULL << a);
				pos->colours[BLACK] |= (1ULL << a);
				break;
			}
			case 'r': {
				pos->pieces[ROOK] |= (1ULL << a);
				pos->colours[BLACK] |= (1ULL << a);
				break;
			}
			case 'q': {
				pos->pieces[QUEEN] |= (1ULL << a);
				pos->colours[BLACK] |= (1ULL << a);
				break;
			}
			case 'k': {
				pos->pieces[KING] |= (1ULL << a);
				pos->colours[BLACK] |= (1ULL << a);
				pos->Bkingpos = a;
				break;
			}
			case 'P': {
				pos->pieces[PAWN] |= (1ULL << a);
				pos->colours[WHITE] |= (1ULL << a);
				break;
			}
			case 'N': {
				pos->pieces[KNIGHT] |= (1ULL << a);
				pos->colours[WHITE] |= (1ULL << a);
				break;
			}
			case 'B': {
				pos->pieces[BISHOP] |= (1ULL << a);
				pos->colours[WHITE] |= (1ULL << a);
				break;
			}
			case 'R': {
				pos->pieces[ROOK] |= (1ULL << a);
				pos->colours[WHITE] |= (1ULL << a);
				break;
			}
			case 'Q': {
				pos->pieces[QUEEN] |= (1ULL << a);
				pos->colours[WHITE] |= (1ULL << a);
				break;
			}
			case 'K': {
				pos->pieces[KING] |= (1ULL << a);
				pos->colours[WHITE] |= (1ULL << a);
				pos->Wkingpos = a;
				break;
			}
			case '/': j--; break;
			case '1' : break;
			case '2' : j++; break;
			case '3' : j+=2; break;
			case '4' : j+=3; break;
			case '5' : j+=4; break;
			case '6' : j+=5; break;
			case '7' : j+=6; break;
			case '8' : j+=7; break;
		}
		j++;
	}
	
	if (tokens[1] == "w") pos->tomove = WHITE;
	if (tokens[1] == "b") pos->tomove = BLACK;

	for (size_t i = 0;i < tokens[2].length();i++) {
		if (tokens[2][i] == 'K') pos->WcastleKS = 1;
		else if (tokens[2][i] == 'Q') pos->WcastleQS = 1;
		else if (tokens[2][i] == 'k') pos->BcastleKS = 1;
		else if (tokens[2][i] == 'q') pos->BcastleQS = 1;
	}

	if (numtokens >= 4 && tokens[3] != "-") {
		//en passant square given
		pos->epsquare = strsquaretoidx(tokens[3]);
	}
	else pos->epsquare = -1;

	//std::cout << numtokens << "\n";
	if (numtokens < 5 || tokens[4] == "-") pos->halfmoves = 0;
	else pos->halfmoves = std::stoi(tokens[4]);
	pos->irrevidx = 0;
	pos->irrev[pos->irrevidx].epsquare = pos->epsquare;
	pos->irrev[pos->irrevidx].WcastleQS = pos->WcastleQS;
	pos->irrev[pos->irrevidx].WcastleKS = pos->WcastleKS;
	pos->irrev[pos->irrevidx].BcastleQS = pos->BcastleQS;
	pos->irrev[pos->irrevidx].BcastleKS = pos->BcastleKS;
	pos->irrev[pos->irrevidx].Wcastled = pos->Wcastled;
	pos->irrev[pos->irrevidx].Bcastled = pos->Bcastled;
	pos->irrev[pos->irrevidx].halfmoves = pos->halfmoves;
	pos->irrev[pos->irrevidx].Wkingpos = pos->Wkingpos;
	pos->irrev[pos->irrevidx].Bkingpos = pos->Bkingpos;
	pos->hashstack[pos->irrevidx] = generateHash(pos);
}