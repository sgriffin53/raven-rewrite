#ifndef TEST_PERFT_HPP
#define TEST_PERFT_HPP

#include "../src/chess/position.hpp"
#include "../src/perft.hpp"

#include <string>
#include <vector>

namespace test {

bool perft() {
	std::pair<std::string, std::vector<int>> tests[] = {
		{{"startpos"}, {1, 20, 400, 8902, 197281}},
		{{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"}, {1, 48, 2039, 97862}},
	};

	for (auto &[fen, nodes] : tests) {
		Position pos(fen);
		for (auto i = 0; i < nodes.size(); ++i) {
			const auto result = perft(&pos, i);
			if (result != nodes.at(i)) {
				return false;
			}
		}
	}

	return true;
}

} // namespace test

#endif
