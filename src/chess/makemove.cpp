#include "makemove.hpp"
#include "hash.hpp"
#include "move.hpp"
#include "pieces.hpp"
#include "position.hpp"
#include <cassert>
#include <iostream>

void makeMove(const Move *move, Position *pos) {
	pos->halfmoves += 1;
	int piece = move->piece();
	int cappiece = move->cappiece();
	int newepsquare = -1; // init to -1, change to ep square if there is one
	int torank = getrank(move->to());
	int prompiece = move->prom();
	int tosquare = move->to();
	int fromsquare = move->from();
	int epsquare = pos->epsquare;
	int colour = pos->tomove;

	// setPiece(pos,fromsquare, NONE, NONE);
	// remove old piece

	pos->colours[WHITE] &= ~(1ULL << fromsquare);
	pos->colours[BLACK] &= ~(1ULL << fromsquare);
	int oldpiece = pos->getPiece(fromsquare);
	pos->pieces[oldpiece] &= ~(1ULL << fromsquare);
	pos->setPiece(tosquare, pos->tomove, piece);

	if (cappiece != NONE) {
		pos->halfmoves = 0;
	}
	if (piece == PAWN && colour == WHITE) {
		pos->halfmoves = 0;
		if (torank == 7) {
			// promotion
			pos->setPiece(tosquare, colour, prompiece);
			pos->colours[WHITE] |= (1ULL << tosquare);
			if (prompiece == QUEEN) {
				pos->pieces[QUEEN] |= (1ULL << tosquare);
			} else if (prompiece == ROOK) {
				pos->pieces[ROOK] |= (1ULL << tosquare);
			} else if (prompiece == BISHOP) {
				pos->pieces[BISHOP] |= (1ULL << tosquare);
			} else if (prompiece == KNIGHT) {
				pos->pieces[KNIGHT] |= (1ULL << tosquare);
			}
		}
		if (move->to() == epsquare) {
			// pawn moves en passant
			// remove captured piece
			// setPiece(pos,epsquare - 8,NONE, NONE);
			pos->clearSquare(epsquare - 8);
		}
		if (abs(fromsquare - tosquare) == 16) {
			newepsquare = tosquare - 8;
		}
	} else if (piece == PAWN && colour == BLACK) {
		pos->halfmoves = 0;
		if (torank == 0) {
			// promotion
			pos->setPiece(tosquare, colour, prompiece);
			pos->colours[BLACK] |= (1ULL << tosquare);
			if (prompiece == QUEEN) {
				pos->pieces[QUEEN] |= (1ULL << tosquare);
			} else if (prompiece == ROOK) {
				pos->pieces[ROOK] |= (1ULL << tosquare);
			} else if (prompiece == BISHOP) {
				pos->pieces[BISHOP] |= (1ULL << tosquare);
			} else if (prompiece == KNIGHT) {
				pos->pieces[KNIGHT] |= (1ULL << tosquare);
			}
		}
		if (move->to() == epsquare) {
			// pawn moves en passant
			// remove captured piece
			// setPiece(pos,epsquare + 8, NONE, NONE);
			pos->clearSquare(epsquare + 8);
		}
		if (abs(fromsquare - tosquare) == 16) {
			// pawn moves 2 spaces forward
			newepsquare = tosquare + 8;
		}
	} else if (piece == KING && colour == WHITE) { // white king
		pos->WcastleQS = 0;
		pos->WcastleKS = 0;
		pos->Wkingpos = tosquare;

		if ((fromsquare == E1) && (tosquare == G1)) { // white kingside castling
			// clear E1
			pos->clearSquare(E1);
			pos->setPiece(G1, colour, KING);
			pos->setPiece(F1, colour, ROOK);
			// clear H1
			pos->clearSquare(H1);
			pos->Wcastled = 1;
		}

		if ((fromsquare == E1) && (tosquare == C1)) { // white queenside castling
			// clear E1
			pos->clearSquare(E1);
			pos->setPiece(C1, colour, KING);
			pos->setPiece(D1, colour, ROOK);
			// clear A1
			pos->clearSquare(A1);
			pos->Wcastled = 1;
		}
	} else if (piece == KING && colour == BLACK) { // black king
		pos->BcastleQS = 0;
		pos->BcastleKS = 0;
		pos->Bkingpos = tosquare;
		if ((fromsquare == E8) && (tosquare == G8)) { // black kingside castling
			// clear E8
			pos->clearSquare(E8);
			pos->setPiece(G8, colour, KING);
			pos->setPiece(F8, colour, ROOK);
			// clear H8
			pos->clearSquare(H8);
			pos->Bcastled = 1;
		}
		if ((fromsquare == E8) && (tosquare == C8)) { // black queenside castling
			// clear E8
			pos->clearSquare(E8);
			pos->setPiece(C8, colour, KING);
			pos->setPiece(D8, colour, ROOK);
			// clear A8
			pos->clearSquare(A8);
			pos->Bcastled = 1;
		}
	}
	if (piece == ROOK && colour == BLACK) {
		if (fromsquare == A8) {
			pos->BcastleQS = 0;
		} else if (fromsquare == H8) {
			pos->BcastleKS = 0;
		}
	} else if (piece == ROOK && colour == WHITE) {
		if (fromsquare == A1) {
			pos->WcastleQS = 0;
		} else if (fromsquare == H1) {
			pos->WcastleKS = 0;
		}
	}
	if (cappiece == ROOK && colour == WHITE) {
		if (tosquare == A8) {
			pos->BcastleQS = 0;
		} else if (tosquare == H8) {
			pos->BcastleKS = 0;
		}
	} else if (cappiece == ROOK && colour == BLACK) {
		if (tosquare == A1) {
			pos->WcastleQS = 0;
		} else if (tosquare == H1) {
			pos->WcastleKS = 0;
		}
	}

	pos->tomove = !pos->tomove;
	pos->epsquare = newepsquare;
	pos->irrevidx++;
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
	// pos->hashstack[pos->irrevidx] = generateHash(pos);
}

void unmakeMove(const Move *move, Position *pos) {
	assert(move->type() >= NORMAL && move->type() <= QSC);
	assert(move->piece() >= PAWN && move->piece() <= NONE);
	pos->irrevidx--;
	pos->epsquare = pos->irrev[pos->irrevidx].epsquare;
	pos->WcastleQS = pos->irrev[pos->irrevidx].WcastleQS;
	pos->WcastleKS = pos->irrev[pos->irrevidx].WcastleKS;
	pos->BcastleQS = pos->irrev[pos->irrevidx].BcastleQS;
	pos->BcastleKS = pos->irrev[pos->irrevidx].BcastleKS;
	pos->Wcastled = pos->irrev[pos->irrevidx].Wcastled;
	pos->Bcastled = pos->irrev[pos->irrevidx].Bcastled;
	pos->halfmoves = pos->irrev[pos->irrevidx].halfmoves;
	pos->Wkingpos = pos->irrev[pos->irrevidx].Wkingpos;
	pos->Bkingpos = pos->irrev[pos->irrevidx].Bkingpos;

	pos->tomove = !pos->tomove;
	U64 ksc_king[2], ksc_rook[2], qsc_king[2], qsc_rook[2];
	if (move->type() == KSC || move->type() == QSC) {
		ksc_king[WHITE] = 1ULL << E1 | 1ULL << G1;
		ksc_king[BLACK] = 1ULL << E8 | 1ULL << G8;

		ksc_rook[WHITE] = 1ULL << F1 | 1ULL << H1;
		ksc_rook[BLACK] = 1ULL << F8 | 1ULL << H8;

		qsc_king[WHITE] = 1ULL << E1 | 1ULL << C1;
		qsc_king[BLACK] = 1ULL << E8 | 1ULL << C8;

		qsc_rook[WHITE] = 1ULL << D1 | 1ULL << A1;
		qsc_rook[BLACK] = 1ULL << D8 | 1ULL << A8;
	}
	U64 from = 1ULL << move->from();
	U64 to = 1ULL << move->to();
	switch (move->type()) {
	case NORMAL:
		pos->pieces[move->piece()] ^= to | from;
		pos->colours[pos->tomove] ^= to | from;
		break;
	case CAPTURE:
		pos->pieces[move->piece()] ^= to | from;
		pos->colours[pos->tomove] ^= to | from;
		pos->pieces[move->cappiece()] ^= to;
		pos->colours[!pos->tomove] ^= to;
		break;
	case DOUBLE:
		pos->pieces[move->piece()] ^= to | from;
		pos->colours[pos->tomove] ^= to | from;
		break;
	case PROMO:
	case PROMO_CAPTURE:
		pos->pieces[move->piece()] ^= from;
		pos->pieces[move->prom()] ^= to;
		pos->colours[pos->tomove] ^= to | from;
		if (move->cappiece() != NONE) {
			pos->pieces[move->cappiece()] ^= to;
			pos->colours[!pos->tomove] ^= to;
		}
		break;
	case EN_PASSANT:
		pos->pieces[move->piece()] ^= to | from;
		pos->colours[pos->tomove] ^= to | from;
		if (pos->tomove == WHITE) {
			pos->pieces[PAWN] ^= to >> 8;
			pos->colours[BLACK] ^= to >> 8;
		} else {
			pos->pieces[PAWN] ^= to << 8;
			pos->colours[WHITE] ^= to << 8;
		}
		break;
	case KSC:
		pos->pieces[KING] ^= ksc_king[pos->tomove];
		pos->pieces[ROOK] ^= ksc_rook[pos->tomove];
		pos->colours[pos->tomove] ^= (ksc_king[pos->tomove] | ksc_rook[pos->tomove]);
		break;
	case QSC:
		pos->pieces[KING] ^= qsc_king[pos->tomove];
		pos->pieces[ROOK] ^= qsc_rook[pos->tomove];
		pos->colours[pos->tomove] ^= (qsc_king[pos->tomove] | qsc_rook[pos->tomove]);
		break;
	}
	// movestackidx--;
	// capstackidx--;
}

void makeMovestr(const std::string &movestr, Position *pos) {
	const auto move = pos->find_move(movestr);
	makeMove(&move, pos);
}

void unmakeMovestr(std::string move, Position *pos, int lastcap) {
	//	char startsquare[3];
	//	char endsquare[3];
	//	char prompiece[2];
	std::string startsquare, endsquare, prompiece;

	//	startsquare = move[0] + move[1];
	// endsquare += move[2] + move[3];
	// prompiece = move[4];
	startsquare += move[0];
	startsquare += move[1];
	endsquare += move[2];
	endsquare += move[3];
	prompiece += move[4];
	int startsquareidx = strsquaretoidx(startsquare);
	int endsquareidx = strsquaretoidx(endsquare);
	int cappiece = lastcap;
	int piece = pos->getPiece(endsquareidx);
	int prom = NONE;
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
	if (piece == PAWN && endsquareidx == pos->epsquare) {
		if (pos->tomove == WHITE) {
			if (startsquareidx == endsquareidx - 11 || startsquareidx == endsquareidx - 9) {
				isep = 1;
			}
		} else if (pos->tomove == BLACK) {
			if (startsquareidx == endsquareidx + 11 || startsquareidx == endsquareidx + 9) {
				isep = 1;
			}
		}
	}
	int isdouble = 0;
	if (piece == PAWN) {
		if (pos->tomove == WHITE) {
			if (getrank(startsquareidx) == 1 && getrank(endsquareidx) == 3) {
				isdouble = 1;
			}
		} else if (pos->tomove == BLACK) {
			if (getrank(startsquareidx) == 6 && getrank(endsquareidx) == 4) {
				isdouble = 1;
			}
		}
	}
	int isqsc = 0;
	int isksc = 0;
	if (piece == KING) {
		if (pos->tomove == WHITE) {
			if (startsquareidx == E1 && endsquareidx == G1)
				isksc = 1;
			else if (startsquareidx == E1 && endsquareidx == C1)
				isqsc = 1;
		}
		if (pos->tomove == BLACK) {
			if (startsquareidx == E8 && endsquareidx == G8)
				isksc = 1;
			else if (startsquareidx == E8 && endsquareidx == C8)
				isqsc = 1;
		}
	}
	int movetype = NORMAL;
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
	std::cout << "start: " << startsquareidx << " end: " << endsquareidx << "\n";
	std::cout << "piece: " << piece << " cappiece " << cappiece << "\n";
	std::cout << "type: " << movetype << "\n";
	Move moveobj = Move(startsquareidx, endsquareidx, prom, piece, cappiece, movetype);
	unmakeMove(&moveobj, pos);
}
