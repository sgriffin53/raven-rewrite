#include "libchess/src/libchess/movegen.hpp"
#include "position.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "move.hpp"
#include "makemove.hpp"
#include "tests.hpp"

void UCI_Listen() {
	Position pos;
	parsefen(&pos,"startpos");
	bool keeprunning = true;
	while (keeprunning) {
		std::string line, intermediate;
		
		// read input
		getline(std::cin, line);
      
		// Vector of string to save tokens 
		std::vector <std::string> tokens;
		// stringstream class check1 
		std::stringstream check1(line);
		int numtokens = 0;
		// Tokenizing w.r.t. space ' ' 
		while(getline(check1, intermediate, ' ')) 
		{ 
			tokens.push_back(intermediate); 
			numtokens++;
		}
		
		// uci commands
		
		if (tokens[0] == "uci") {
			std::cout << "id name Raven 1.20\nid author Steve Griffin\n";
			//std::cout << "option name Hash type spin default 32 min 32 max 256\n";
			std::cout << "uciok\n";
		}
		else if (tokens[0] == "quit") {
			keeprunning = false;
		}
		else if (tokens[0] == "board") {
			dspBoard(&pos);
		}
		else if (tokens[0] == "test") {
			std::cout << "Running tests.\n";
			testMakeMove();
		}
		else if (numtokens >= 2 && tokens[0] == "moves") {
			for (int i = 1;i < numtokens;i++) {
				std::cout << "making move " << tokens[i] << "\n";
				// make move
				makeMovestr(tokens[i], &pos);
			}
		}
		else if (numtokens >= 2 && tokens[0] == "position") {
			if (tokens[1] == "startpos") {
				parsefen(&pos,"startpos");
				
				if (numtokens > 3 && tokens[2] == "moves") {
					for (int i = 3;i < numtokens;i++) {
						std::cout << "making move " << tokens[i] << "\n";
						// make move
						
					}
				}
				 
			}
			else if (numtokens >= 3 && tokens[1] == "fen") {
				// parse position from fen
				std::string fen = "";
				bool readingfen = true;
				for (int i = 2;i < numtokens;i++) {
					// loop over remaining tokens
					if (!readingfen) {
						// read moves list
						std::cout << "making move " << tokens[i] << "\n";
						// make move
					}
					
					if (tokens[i] == "moves") {
						// parse fen and get ready for the moves list
						parsefen(&pos, fen);
						readingfen = false;
					}
					
					if (readingfen) {
						// add next section of fen string
						fen = fen + tokens[i];
						if (i != (numtokens - 1)) fen = fen + " ";
					}
				}
				if (readingfen) {
					// parse fen since we haven't already
					parsefen(&pos, fen);
				}
			}
		}
	}
}