#include "sort.hpp"
#include <cassert>

int capval(int piece) {
	switch (piece) {
	case PAWN:
		return 1;
	case BISHOP:
		return 2;
	case KNIGHT:
		return 2;
	case ROOK:
		return 3;
	case QUEEN:
		return 4;
	case KING:
		return 5;
	}
	assert(0);
	return 0;
}

int mvvlva(char piece, char cappiece) { return 10 * capval(cappiece) - capval(piece); }

void sortMoves(Position *pos, Move *moves, const int num_moves, Move *TTmove, int ply) {
	int scores[MAX_MOVES];
	for (int i = 0; i < num_moves; i++) {
		scores[i] = 0;
		int cappiece = moves[i].cappiece();
		int piece = moves[i].piece();

		if (cappiece != NONE && cappiece > piece) {
			scores[i] = 1000000 + mvvlva(piece, cappiece);
		} else if (cappiece != NONE && cappiece <= piece) {
			scores[i] = 700000 + mvvlva(piece, cappiece);
		}
	}

	// selection sort

	int i, j;
	for (i = 0; i < num_moves - 1; i++) {
		int min_idx = i;
		for (j = i + 1; j < num_moves; j++) {
			if (scores[j] > scores[min_idx])
				min_idx = j;
		}
		int scorecopy = scores[i];
		scores[i] = scores[min_idx];
		scores[min_idx] = scorecopy;

		Move movecopy = moves[i];
		moves[i] = moves[min_idx];
		moves[min_idx] = movecopy;
	}
}
