#include "perft.hpp"
#include "chess/attacks.hpp"
#include "chess/makemove.hpp"
#include "chess/move.hpp"
#include "chess/movegen.hpp"
#include "chess/position.hpp"
#include <iostream>

U64 perft(Position *pos, int depth) {
	assert(pos);
	assert(depth >= 0);

	if (depth == 0)
		return 1;

	U64 nodes = 0;

	Move moves[MAX_MOVES];
	const int n_moves = genMoves(pos, moves, 0);

	for (int i = 0; i < n_moves; i++) {
		makeMove(&moves[i], pos);

		if (isAttacked(pos, pos->theirKing(), pos->tomove)) {
			unmakeMove(&moves[i], pos);
			continue;
		}

		nodes += perft(pos, depth - 1);
		unmakeMove(&moves[i], pos);
	}

	return nodes;
}

U64 sperft(Position *pos, int depth) {
	assert(pos);
	assert(depth >= 0);

	if (depth == 0)
		return 1;

	U64 total_nodes = 0;
	Move moves[MAX_MOVES];
	const int n_moves = genMoves(pos, moves, 0);
	for (int i = 0; i < n_moves; i++) {
		// Position origpos = *pos;
		makeMove(&moves[i], pos);

		if (isAttacked(pos, pos->theirKing(), pos->tomove)) {
			unmakeMove(&moves[i], pos);
			continue;
		}

		U64 nodes = perft(pos, depth - 1);
		total_nodes += nodes;
		std::cout << moves[i].string() << " - " << nodes << "\n";
		unmakeMove(&moves[i], pos);
	}
	return total_nodes;
}
