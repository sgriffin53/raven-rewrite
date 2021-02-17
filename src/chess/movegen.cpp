#include "movegen.hpp"
#include "attacks.hpp"
#include "bitboards.hpp"
#include "move.hpp"
#include "position.hpp"
#include <cassert>
#include <iostream>

int genKingMoves(Position *pos, int square, Move *moves, int forqsearch) {
	U64 BBattacks = 0ULL;
	int num_moves = 0;
	assert(square >= 0 && square <= 64);
	BBattacks = king_attacks(square);
	BBattacks = BBattacks & ~pos->colours[pos->tomove];
	if (forqsearch)
		BBattacks = BBattacks & pos->colours[!pos->tomove];
	while (BBattacks != 0) {
		int movesquare = __builtin_ctzll(BBattacks);
		BBattacks &= BBattacks - 1;
		const auto cappiece = pos->getPiece(movesquare);
		if (cappiece != NONE)
			moves[num_moves] = Move(square, movesquare, NONE, KING, cappiece, CAPTURE);
		else
			moves[num_moves] = Move(square, movesquare, NONE, KING, cappiece, NORMAL);
		num_moves++;
	}
	// castling moves
	// white castling
	if (!forqsearch) {
		if (pos->tomove == WHITE) {
			// King side castling
			if ((pos->WcastleKS == 1) && pos->getPiece(F1) == NONE && pos->getPiece(G1) == NONE && !isAttacked(pos, E1, BLACK) && !isAttacked(pos, F1, BLACK) && !isAttacked(pos, G1, BLACK)) {
				// Add move
				moves[num_moves] = Move(E1, G1, NONE, KING, NONE, KSC);
				num_moves += 1;
			}
			// Queenside castling
			if ((pos->WcastleQS == 1) && pos->getPiece(D1) == NONE && pos->getPiece(C1) == NONE && pos->getPiece(B1) == NONE && !isAttacked(pos, E1, BLACK) && !isAttacked(pos, D1, BLACK) && !isAttacked(pos, C1, BLACK)) {
				// Add move
				moves[num_moves] = Move(E1, C1, NONE, KING, NONE, QSC);
				num_moves += 1;
			}
		}
		// black castling
		else if (pos->tomove == BLACK) {
			// Kingside castling
			if ((pos->BcastleKS == 1) && pos->getPiece(F8) == NONE && pos->getPiece(G8) == NONE && !isAttacked(pos, E8, WHITE) && !isAttacked(pos, F8, WHITE) && !isAttacked(pos, G8, WHITE)) {
				// Add move
				if (!forqsearch) {
					moves[num_moves] = Move(E8, G8, NONE, KING, NONE, KSC);
					num_moves += 1;
				}
			}
			// Queenside castling
			if ((pos->BcastleQS == 1) && pos->getPiece(D8) == NONE && pos->getPiece(C8) == NONE && pos->getPiece(B8) == NONE && !isAttacked(pos, E8, WHITE) && !isAttacked(pos, D8, WHITE) && !isAttacked(pos, C8, WHITE)) {
				// Add move
				moves[num_moves] = Move(E8, C8, NONE, KING, NONE, QSC);
				num_moves += 1;
			}
		}
	}
	return num_moves;
}

int genKnightMoves(Position *pos, int square, Move *moves, int forqsearch) {
	U64 BBattacks = 0ULL;
	int num_moves = 0;
	BBattacks = knight_attacks(square);
	BBattacks = BBattacks & ~pos->colours[pos->tomove];
	if (forqsearch)
		BBattacks &= pos->colours[!pos->tomove];
	// dspBB(BBattacks);
	while (BBattacks != 0) {
		int movesquare = __builtin_ctzll(BBattacks);
		BBattacks &= BBattacks - 1;
		const auto cappiece = pos->getPiece(movesquare);
		if (cappiece != NONE)
			moves[num_moves] = Move(square, movesquare, NONE, KNIGHT, cappiece, CAPTURE);
		else
			moves[num_moves] = Move(square, movesquare, NONE, KNIGHT, cappiece, NORMAL);
		num_moves++;
	}
	return num_moves;
}

int genBishopMoves(Position *pos, int square, Move *moves, int forqsearch) {
	U64 BBattacks = 0ULL;
	int num_moves = 0;
	U64 BBoccupancy = (pos->colours[WHITE] | pos->colours[BLACK]);
	BBattacks = bishop_attacks(square, BBoccupancy);
	BBattacks = BBattacks & ~pos->colours[pos->tomove];
	if (forqsearch)
		BBattacks &= BBattacks & pos->colours[!pos->tomove];
	while (BBattacks != 0) {
		int movesquare = __builtin_ctzll(BBattacks);
		BBattacks &= BBattacks - 1;
		const auto cappiece = pos->getPiece(movesquare);
		if (cappiece != NONE)
			moves[num_moves] = Move(square, movesquare, NONE, BISHOP, cappiece, CAPTURE);
		else
			moves[num_moves] = Move(square, movesquare, NONE, BISHOP, cappiece, NORMAL);
		num_moves++;
	}
	return num_moves;
}

int genRookMoves(Position *pos, int square, Move *moves, int forqsearch) {
	U64 BBattacks = 0ULL;
	int num_moves = 0;
	U64 BBoccupancy = (pos->colours[WHITE] | pos->colours[BLACK]);
	BBattacks = rook_attacks(square, BBoccupancy);
	BBattacks = BBattacks & ~pos->colours[pos->tomove];
	if (forqsearch)
		BBattacks &= BBattacks & pos->colours[!pos->tomove];
	while (BBattacks != 0) {
		int movesquare = __builtin_ctzll(BBattacks);
		BBattacks &= BBattacks - 1;
		const auto cappiece = pos->getPiece(movesquare);
		if (cappiece != NONE)
			moves[num_moves] = Move(square, movesquare, NONE, ROOK, cappiece, CAPTURE);
		else
			moves[num_moves] = Move(square, movesquare, NONE, ROOK, cappiece, NORMAL);
		num_moves++;
	}
	return num_moves;
}

int genQueenMoves(Position *pos, int square, Move *moves, int forqsearch) {
	U64 BBattacks = 0ULL;
	int num_moves = 0;
	U64 BBoccupancy = (pos->colours[WHITE] | pos->colours[BLACK]);
	BBattacks = queen_attacks(square, BBoccupancy);
	BBattacks = BBattacks & ~pos->colours[pos->tomove];
	if (forqsearch)
		BBattacks &= BBattacks & pos->colours[!pos->tomove];
	while (BBattacks != 0) {
		int movesquare = __builtin_ctzll(BBattacks);
		BBattacks &= BBattacks - 1;
		const auto cappiece = pos->getPiece(movesquare);
		if (cappiece != NONE)
			moves[num_moves] = Move(square, movesquare, NONE, QUEEN, cappiece, CAPTURE);
		else
			moves[num_moves] = Move(square, movesquare, NONE, QUEEN, cappiece, NORMAL);
		num_moves++;
	}
	return num_moves;
}

int genAllPawnMoves(Position *pos, int square, Move *moves, int forqsearch) {
	// generates all pawn moves
	U64 BBoccupied = pos->colours[WHITE] | pos->colours[BLACK];
	U64 BBwhitepawns = pos->colours[WHITE] & pos->pieces[PAWN];
	U64 BBblackpawns = pos->colours[BLACK] & pos->pieces[PAWN];
	int num_moves = 0;
	char piece = PAWN;
	if (pos->tomove == WHITE) {
		// double pushes
		if (!forqsearch) {
			U64 BBstartpawns = BBwhitepawns & BBrank2;
			U64 BBdoublepushes = (BBstartpawns << 16) & ~(BBoccupied);
			while (BBdoublepushes) {
				int targetsquare = __builtin_ctzll(BBdoublepushes);
				BBdoublepushes &= BBdoublepushes - 1;
				U64 BBrank3blocked = ((1ULL << targetsquare) >> 8) & (BBoccupied);
				if (BBrank3blocked)
					continue;
				int sourcesquare = targetsquare - 16;
				moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, NONE, DOUBLE);
				num_moves++;
			}
		}
		// single pushes

		U64 BBpawns = BBwhitepawns;
		U64 BBsinglepushes = (BBpawns << 8) & ~(BBoccupied);
		while (BBsinglepushes) {
			int targetsquare = __builtin_ctzll(BBsinglepushes);
			BBsinglepushes &= BBsinglepushes - 1;
			int sourcesquare = targetsquare - 8;
			if (getrank(targetsquare) == 7) {
				moves[num_moves] = Move(sourcesquare, targetsquare, QUEEN, piece, NONE, PROMO);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, ROOK, piece, NONE, PROMO);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, BISHOP, piece, NONE, PROMO);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, KNIGHT, piece, NONE, PROMO);
				num_moves++;
			} else {
				if (!forqsearch) {
					moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, NONE, NORMAL);
					num_moves++;
				}
			}
		}

		// left capture

		U64 BBleftcaptures = noWeOne(BBpawns) & pos->colours[BLACK];
		while (BBleftcaptures) {
			int targetsquare = __builtin_ctzll(BBleftcaptures);
			BBleftcaptures &= BBleftcaptures - 1;
			int sourcesquare = targetsquare - 7;
			if (getrank(targetsquare) == 7) {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, QUEEN, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, ROOK, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, BISHOP, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, KNIGHT, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
			} else {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, cappiece, CAPTURE);
				num_moves++;
			}
		}

		// right capture

		U64 BBrightcaptures = noEaOne(BBpawns) & pos->colours[BLACK];
		while (BBrightcaptures) {
			int targetsquare = __builtin_ctzll(BBrightcaptures);
			BBrightcaptures &= BBrightcaptures - 1;
			int sourcesquare = targetsquare - 9;
			if (getrank(targetsquare) == 7) {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, QUEEN, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, ROOK, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, BISHOP, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, KNIGHT, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
			} else {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, cappiece, CAPTURE);
				num_moves++;
			}
		}
	} else {
		// black to move

		// double pushes
		U64 BBstartpawns = BBblackpawns & BBrank7;
		U64 BBdoublepushes = (BBstartpawns >> 16) & ~(BBoccupied);
		if (!forqsearch) {
			while (BBdoublepushes) {
				int targetsquare = __builtin_ctzll(BBdoublepushes);
				BBdoublepushes &= BBdoublepushes - 1;
				U64 BBrank3blocked = ((1ULL << targetsquare) << 8) & (BBoccupied);
				if (BBrank3blocked)
					continue;
				int sourcesquare = targetsquare + 16;
				moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, NONE, DOUBLE);
				num_moves++;
			}
		}
		// single pushes

		U64 BBpawns = BBblackpawns;
		U64 BBsinglepushes = (BBpawns >> 8) & ~(BBoccupied);
		while (BBsinglepushes) {
			int targetsquare = __builtin_ctzll(BBsinglepushes);
			BBsinglepushes &= BBsinglepushes - 1;
			int sourcesquare = targetsquare + 8;
			if (getrank(targetsquare) == 0) {
				moves[num_moves] = Move(sourcesquare, targetsquare, QUEEN, piece, NONE, PROMO);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, ROOK, piece, NONE, PROMO);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, BISHOP, piece, NONE, PROMO);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, KNIGHT, piece, NONE, PROMO);
				num_moves++;
			} else {
				if (!forqsearch) {
					moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, NONE, NORMAL);
					num_moves++;
				}
			}
		}
		// left capture

		U64 BBleftcaptures = soWeOne(BBpawns) & pos->colours[WHITE];
		while (BBleftcaptures) {
			int targetsquare = __builtin_ctzll(BBleftcaptures);
			BBleftcaptures &= BBleftcaptures - 1;
			int sourcesquare = targetsquare + 9;
			if (getrank(targetsquare) == 0) {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, QUEEN, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, ROOK, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, BISHOP, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, KNIGHT, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
			} else {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, cappiece, CAPTURE);
				num_moves++;
			}
		}
		// right capture

		U64 BBrightcaptures = soEaOne(BBpawns) & pos->colours[WHITE];
		while (BBrightcaptures) {
			int targetsquare = __builtin_ctzll(BBrightcaptures);
			BBrightcaptures &= BBrightcaptures - 1;
			int sourcesquare = targetsquare + 7;
			if (getrank(targetsquare) == 0) {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, QUEEN, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, ROOK, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, BISHOP, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
				moves[num_moves] = Move(sourcesquare, targetsquare, KNIGHT, piece, cappiece, PROMO_CAPTURE);
				num_moves++;
			} else {
				char cappiece = pos->getPiece(targetsquare);
				moves[num_moves] = Move(sourcesquare, targetsquare, NONE, piece, cappiece, CAPTURE);
				num_moves++;
			}
		}
	}
	return num_moves;
}

int genMoves(Position *pos, Move *moves, int forqsearch) {
	assert(pos);
	assert(moves);
	int num_moves = 0;
	U64 BBallpieces;
	if (pos->tomove == WHITE)
		BBallpieces = pos->colours[WHITE];
	else
		BBallpieces = pos->colours[BLACK];

	// en passant moves

	if (pos->epsquare != -1) {
		const int x = getfile(pos->epsquare);
		const int y = getrank(pos->epsquare);
		if (pos->tomove == BLACK) {
			for (int i = 0; i < 2; i++) {
				const int nx = x + BPdirs[i][0];
				const int ny = y - BPdirs[i][1];
				// Borders
				if (nx < 0 || nx > 7) {
					continue;
				}
				const int idx = fileranktosquareidx(nx, ny);
				const char piece = pos->getPiece(idx);

				if (piece == PAWN && pos->getColour(idx) == BLACK) {
					// Add move
					if (!forqsearch) {
						moves[num_moves] = Move(idx, pos->epsquare, NONE, piece, NONE, EN_PASSANT);
						num_moves++;
					}
				}
			}
		} else if (pos->tomove == WHITE) {
			for (int i = 0; i < 2; i++) {
				const int nx = x + WPdirs[i][0];
				const int ny = y - WPdirs[i][1];
				// Borders
				if (nx < 0 || nx > 7) {
					continue;
				}
				const int idx = fileranktosquareidx(nx, ny);
				const char piece = pos->getPiece(idx);
				if (piece == PAWN && pos->getColour(idx) == WHITE) {
					// Add move
					if (!forqsearch) {
						moves[num_moves] = Move(idx, pos->epsquare, NONE, piece, NONE, EN_PASSANT);
						num_moves++;
					}
				}
			}
		}
	}

	BBallpieces &= ~pos->pieces[PAWN];

	while (BBallpieces != 0) {
		int square = __builtin_ctzll(BBallpieces);
		char piece = pos->getPiece(square);
		switch (piece) {
		case KING:
			num_moves += genKingMoves(pos, square, &moves[num_moves], forqsearch);
			break;
		case KNIGHT:
			num_moves += genKnightMoves(pos, square, &moves[num_moves], forqsearch);
			break;
		case BISHOP:
			num_moves += genBishopMoves(pos, square, &moves[num_moves], forqsearch);
			break;
		case ROOK:
			num_moves += genRookMoves(pos, square, &moves[num_moves], forqsearch);
			break;
		case QUEEN:
			num_moves += genQueenMoves(pos, square, &moves[num_moves], forqsearch);
			break;
		}
		BBallpieces &= BBallpieces - 1;
	}
	U64 BBstmPawns = pos->colours[pos->tomove] & pos->pieces[PAWN]; // side to move pawns
	if (BBstmPawns)
		num_moves += genAllPawnMoves(pos, 0, &moves[num_moves], forqsearch);
	return num_moves;
}
