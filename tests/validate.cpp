#include "validate.hpp"

#include "../src/chess/attacks.hpp"
#include "../src/chess/makemove.hpp"
#include "../src/chess/move.hpp"
#include "../src/chess/movegen.hpp"
#include "../src/chess/position.hpp"
#include <string>

bool search(Position *pos, const int depth) {
	assert(pos);
	assert(depth >= 0);

	if (!pos->validate()) {
		return false;
	}

	if (depth == 0) {
		return true;
	}

	Move moves[MAX_MOVES];
	const int n_moves = genMoves(pos, moves, 0);

	for (int i = 0; i < n_moves; i++) {
		makeMove(&moves[i], pos);

		if (isAttacked(pos, pos->theirKing(), pos->tomove)) {
			unmakeMove(&moves[i], pos);
			continue;
		}

		const bool valid = search(pos, depth - 1);

		if (!valid) {
			return false;
		}

		unmakeMove(&moves[i], pos);
	}

	return true;
}

namespace test {

bool validate() {
	const std::string fens[] = {
		"startpos",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		"rnbqkbnr/pp2pppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 1 3",
		"rn1qkbnr/pp1bpppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4",
		"rnb1kbnr/pppp1ppp/4p3/8/5PPq/8/PPPPP2P/RNBQKBNR w KQkq - 1 3",
		"2r5/p2k4/2b4n/2Pqp3/p1PPNp1p/4RPp1/1K2RPB1/3Q4 w - - 0 1",
		"rnbqkb1r/pppppppp/8/8/3Pn3/8/PPPKPPPP/RNBQ1BNR w kq - 3 3",
	};

	for (const auto &fen : fens) {
		Position pos(fen);

		if (!search(&pos, 4)) {
			return false;
		}
	}

	return true;
}

} // namespace test
