#include "libchess/src/libchess/movegen.hpp"
#include "position.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <climits>

#include "perft.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "bitboards.hpp"
#include "attacks.hpp"
#include "movegen.hpp"
#include "eval.hpp"
#include "search.hpp"
#include "globals.hpp"
#include "hash.hpp"
#include <fstream>

void UCI_Listen() {
	initZobrist();
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
		else if (tokens[0] == "isready") {
			std::cout << "readyok\n";
		}
		else if (tokens[0] == "quit") {
			keeprunning = false;
		}
		else if (tokens[0] == "board") {
			dspBoard(&pos);
		}
		else if (tokens[0] == "eval") {
			std::cout << "eval: " << taperedEval(&pos) << "\n";
		}
		else if (tokens[0] == "go") {

			int searchdepth = 100;
			//movetime = 2147483646;
			int movetime = INT_MAX / 2;

			if (numtokens >= 3 && tokens[1] == "depth") {
				searchdepth = std::stoi(tokens[2]);
			}
			assert(searchdepth >= 1);
			int strictmovetime = 0;
			wtime = -1;
			btime = -1;
			movestogo = 40;
			for (int i = 1;i < numtokens;i++) {
				if (tokens[i] == "wtime") {
					wtime = std::stoi(tokens[i+1]);
				}
				if (tokens[i] == "btime") {
					btime = std::stoi(tokens[i+1]);
				}
				if (tokens[i] == "movestogo") {
					movestogo = std::stoi(tokens[i+1]);
				}
			}
			if (pos.tomove == WHITE) {
				if (wtime != -1) movetime = wtime / std::min(25, std::max(2, movestogo));
			}
			else {
				if (btime != -1) movetime = btime / std::min(25, std::max(2, movestogo));
			}
			if (numtokens >= 3 && tokens[1] == "movetime") {
				movetime = std::stoi(tokens[2]) * .95;
				strictmovetime = 1;
			}

			nodesSearched = 0;

			search(pos,searchdepth,movetime, strictmovetime);
			//randmove(pos); // for random moves
		}
		else if (numtokens >= 2 && tokens[0] == "perft") {
			int depth;
			U64 pnodes;
			U64 nps;
			depth = std::stoi(tokens[1]);
			movestackidx = 0;
			capstackidx = 0;
			for (int i = 1;i <= depth;i++) {
				clock_t begin = clock();
				pnodes = perft(&pos,i);
				clock_t end = clock();
				double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
				nps = pnodes / time_spent;
				if (nps != 0) {
					std::cout << "info depth " << i << " nodes " << pnodes << " time " << (int)(1000 * time_spent) << " nps " << nps << "\n";
				}
				else {
					std::cout << "info depth " << i << " nodes " << pnodes << " time " << (int)(1000 * time_spent) << "\n";
				}
			}

			//printf("nodes %" PRIu64 "\n", pnodes);
			std::cout << "nodes " << pnodes << "\n";
		}
		else if (numtokens >= 2 && tokens[0] == "sperft") {
			int depth = std::stoi(tokens[1]);
			sperft(&pos,depth);
		}
		else if (tokens[0] == "legalmoves") {
			Move moves[MAX_MOVES];
			int num_moves = genMoves(&pos,moves, 0);
			int j;
			std::cout << num_moves << " num moves\n";
			std::cout << "--\n";
			for (j = 0;j < num_moves;j++) {
				std::cout << j << " - " << movetostr(moves[j]) << "\n";
			}
		}
		else if (numtokens >= 2 && (tokens[0] == "moves" || tokens[0] == "move")) {
			for (int i = 1;i < numtokens;i++) {
				std::cout << "making move " << tokens[i] << "\n";
				// make move
				std::string capsquare = "";
				capsquare+= tokens[i][2];
				capsquare+= tokens[i][3];
				makeMovestr(tokens[i], &pos);
				
			}
		}
		else if (tokens[0] == "hashstack") {
			for (int i = 0;i <= pos.irrevidx;i++) {
				std::cout << "hash at move " << i << ": " << pos.hashstack[i] << "\n";
			}
		}
		else if (tokens[0] == "unmove") {
			//unmakeMovestr(movetostr(movestack[movestackidx-1]), &pos, capstack[capstackidx]);
		}
		else if (tokens[0] == "movehist") {
			//std::cout << movestackidx;
			//for (int j = 0;j < movestackidx;j++) {
			//	std::cout << movetostr(movestack[j]) << " "; 
			//}
		}
		else if (numtokens >= 2 && tokens[0] == "position") {
			if (tokens[1] == "startpos") { // position startpos
				parsefen(&pos,"startpos");
				
				if (numtokens > 3 && tokens[2] == "moves") {
					for (int i = 3;i < numtokens;i++) {
						//std::cout << "making move " << tokens[i] << "\n";
						// make move
						makeMovestr(tokens[i], &pos);
					}
				}
				 
			}
			else if (numtokens >= 3 && tokens[1] == "fen") { // position fen
				// parse position from fen
				std::string fen = "";
				bool readingfen = true;
				for (int i = 2;i < numtokens;i++) {
					// loop over remaining tokens
					if (!readingfen) {
						// read moves list
						//std::cout << "making move " << tokens[i] << "\n";
						// make move
						makeMovestr(tokens[i], &pos);
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