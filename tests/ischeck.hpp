#ifndef TEST_ISCHECK_HPP
#define TEST_ISCHECK_HPP

#include "../src/chess/attacks.hpp"
#include "../src/chess/position.hpp"
#include "../src/perft.hpp"

#include <string>

namespace test {

bool isCheck() {
	const std::pair<std::string, bool> tests[] = {
		{"startpos", false},
		{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", false},
		{"rnbqkbnr/pp2pppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 1 3", true},
		{"rn1qkbnr/pp1bpppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4", false},
		{"rnb1kbnr/pppp1ppp/4p3/8/5PPq/8/PPPPP2P/RNBQKBNR w KQkq - 1 3", true},
	};

	Position pos;

	for (auto &[fen, checked] : tests) {
		parsefen(&pos, fen);
		if (checked != isCheck(&pos)) {
			return false;
		}
	}

	return true;
}

} // namespace test

#endif
