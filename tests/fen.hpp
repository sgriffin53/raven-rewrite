#ifndef TEST_FEN_HPP
#define TEST_FEN_HPP

#include "../src/chess/position.hpp"
#include <string>

namespace test {

bool fen() {
	const std::string fens[] = {
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", "rnbqkbnr/pp2pppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 1 3", "rn1qkbnr/pp1bpppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4",
		"rnb1kbnr/pppp1ppp/4p3/8/5PPq/8/PPPPP2P/RNBQKBNR w KQkq - 1 3",			"2r5/p2k4/2b4n/2Pqp3/p1PPNp1p/4RPp1/1K2RPB1/3Q4 w - - 0 1",		   "2r3nr/4kp1p/N3p1pb/7P/b1p1PqP1/B4P2/P2P1K1R/RQ6 b - - 0 1",
		"r1bqkbnr/ppppp1pp/2n5/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3",		"rnbqkbnr/pppp1ppp/8/8/3Pp3/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 3",
	};

	for (const auto &fen : fens) {
		Position pos(fen);
		if (pos.getFen() != fen) {
			return false;
		}
	}

	return true;
}

} // namespace test

#endif
