#ifndef TEST_KINGPOS_HPP
#define TEST_KINGPOS_HPP

#include "../src/chess/makemove.hpp"
#include "../src/chess/movegen.hpp"
#include "../src/chess/pieces.hpp"
#include "../src/chess/position.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace test {

bool kingPos() {
	std::string line, intermediate;
	std::ifstream ReadFile("../tests/suites/vajolet.txt");
	int j = 0;
	while (getline(ReadFile, line)) {
		j++;
		// Vector of string to save tokens
		std::vector<std::string> tokens;
		// stringstream class check1
		std::stringstream check1(line);
		int numtokens = 0;
		// Tokenizing w.r.t. space ' '
		while (getline(check1, intermediate, ',')) {
			tokens.push_back(intermediate);
			numtokens++;
		}
		std::string fen = tokens[0];
		Position pos(fen);
		Move moves[MAX_MOVES];
		int num_moves = genMoves(&pos, moves, 0);
		for (int i = 0; i < num_moves; i++) {
			// if (moves[i].piece != KING)
			//	continue;

			int origtomove = pos.tomove;
			makeMove(&moves[i], &pos);
			U64 ctzllKingPos = __builtin_ctzll(pos.pieces[KING] & pos.colours[origtomove]);
			U64 kingPos = pos.Wkingpos;
			if (origtomove == BLACK)
				kingPos = pos.Bkingpos;
			if (ctzllKingPos != kingPos)
				return false;
			unmakeMove(&moves[i], &pos);
		}
	}
	ReadFile.close();
	return true;
}

} // namespace test

#endif
