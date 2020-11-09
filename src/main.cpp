#include "libchess/src/libchess/movegen.hpp"
#include "position.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main() {
	
	//libchess::Bitboard a = 0ULL;
	//U64 occ = 1ULL << E5;
	//U64 b = libchess::movegen::bishop_moves(D4, occ);
	//dspBB(b);
	Position pos;
	//setPiece(&pos, A1, NONE, NONE);
	//parsefen(&pos, "startpos");
	//dspBoard(&pos);
	bool keeprunning = true;
	while (keeprunning) { // input loop
		std::string line, intermediate;
		//std::string intermediate; 
	
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
	return 0;
}

