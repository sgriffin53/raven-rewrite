#ifndef TEST_MOVESTRING_HPP
#define TEST_MOVESTRING_HPP

#include "../src/chess/move.hpp"
#include "../src/chess/movegen.hpp"
#include "../src/chess/position.hpp"
#include <string>

namespace test {

bool moveString() {
	const std::string fens[] = {
		"startpos",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
		"rnbqkbnr/pp2pppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 1 3",
		"rn1qkbnr/pp1bpppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4",
		"rnb1kbnr/pppp1ppp/4p3/8/5PPq/8/PPPPP2P/RNBQKBNR w KQkq - 1 3",
		"2r5/p2k4/2b4n/2Pqp3/p1PPNp1p/4RPp1/1K2RPB1/3Q4 w - - 0 1",
	};

	Position pos;

	for (const auto &fen : fens) {
		parsefen(&pos, fen);

		Move moves[MAX_MOVES];
		const int n_moves = genMoves(&pos, moves, 0);

		for (int i = 0; i < n_moves; i++) {
			const auto move_string = moves[i].string();
			const auto new_move = pos.find_move(move_string);
			if (new_move != moves[i]) {
				return false;
			}
		}
	}

	return true;
}

} // namespace test

#endif
