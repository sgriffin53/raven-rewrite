#include "hash.hpp"
#include "position.hpp"
#include <cassert>
#include <stdlib.h>

U64 pieceHash[13][64];
U64 turnHash;
U64 castleHash[4];

#define RAND_64 ((uint64_t)(rand() & 0x7FFF) ^ ((uint64_t)(rand() & 0x7FFF) << 15) ^ ((uint64_t)(rand() & 0x7FFF) << 30) ^ ((uint64_t)(rand() & 0x7FFF) << 45) ^ ((uint64_t)(rand() & 0xF) << 60))

void initZobrist() {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 64; j++) {
			pieceHash[i][j] = RAND_64;
		}
	}

	turnHash = RAND_64;

	for (int i = 0; i < 4; i++) {
		castleHash[i] = RAND_64;
	}
}

int pieceintval(int inpiece, int colour) {
	if (colour == BLACK) {
		switch (inpiece) {
		case PAWN:
			return 12;
		case KNIGHT:
			return 1;
		case BISHOP:
			return 2;
		case ROOK:
			return 3;
		case QUEEN:
			return 4;
		case KING:
			return 5;
		}
	} else if (colour == WHITE) {
		switch (inpiece) {
		case PAWN:
			return 6;
		case KNIGHT:
			return 7;
		case BISHOP:
			return 8;
		case ROOK:
			return 9;
		case QUEEN:
			return 10;
		case KING:
			return 11;
		}
	}
	return 0;
}
U64 generateHash(Position *pos) {
	assert(pos);
	U64 zobrist = 0;
	U64 BBoccupied = pos->colours[BLACK] | pos->colours[WHITE];
	while (BBoccupied != 0) {
		int square = __builtin_ctzll(BBoccupied);
		BBoccupied &= BBoccupied - 1;
		int sqpiece = getPiece(pos, square);
		int sqcol = getColour(pos, square);
		int piece = pieceintval(sqpiece, sqcol);
		zobrist ^= pieceHash[piece][square];
	}
	if (pos->tomove == WHITE) {
		zobrist ^= turnHash;
	}

	if (pos->epsquare != -1) {
		zobrist ^= pieceHash[0][pos->epsquare];
	}

	if (pos->WcastleKS)
		zobrist ^= castleHash[0];
	if (pos->WcastleQS)
		zobrist ^= castleHash[1];
	if (pos->BcastleKS)
		zobrist ^= castleHash[2];
	if (pos->BcastleQS)
		zobrist ^= castleHash[3];

	return zobrist;
}
