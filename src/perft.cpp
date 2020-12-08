#include "perft.hpp"
#include "position.hpp"
#include "move.hpp"
#include "makemove.hpp"
#include "attacks.hpp"
#include "movegen.hpp"
#include <iostream>

Move movestack[12];
int movestackidx, capstackidx;
int capstack[12];

U64 perft(Position *pos, int depth) {
	assert(pos);
	assert(depth>=0);

	if (depth == 0) return 1;

	U64 nodes = 0;
	Move moves[MAX_MOVES];
	const int n_moves = genMoves(pos,moves, 0);
	for (int i = 0; i < n_moves;i++) {
		//Position origpos = *pos;
		makeMove(&moves[i], pos);
		pos->tomove = !pos->tomove;
		
		const int incheck = isCheck(pos);
		if (incheck) {
			pos->tomove = !pos->tomove;
			unmakeMove(&moves[i], pos);
			continue;
		}
		pos->tomove = !pos->tomove;
		nodes += perft(pos, depth - 1);
		//Position aftermake = *pos;
		unmakeMove(&moves[i], pos);
		/*
		if (!posMatch(pos, &origpos)) {
			
			printf("orig pos:\n");
			 
			 
			std::cout << "move: " << movetostr(moves[i]) << "\n";
			//std::cout << "move type: " << moves[i].type << "\n";
			std::cout << "move history: " << movestackidx;
			for (int j = 0;j < depth;j++) {
				std::cout << movetostr(movestack[j]) << " "; 
			}
			std::cout << "\n";
			std::cout << "orig pos\n";
			dspBoard(&origpos);
			std::cout << "after make\n";
			dspBoard(&aftermake);
			//printf("after unmake:\n");
			std::cout << "after unmake:\n";
			dspBoard(pos);
			std::cout << "---\noriginal white bb:\n";
			dspBB(origpos.colours[WHITE]);
			std::cout << "after white bb:\n";
			dspBB(pos->colours[WHITE]);
			 
		//	return 0;
		}
		 */
	}

	return nodes;
}

U64 sperft(Position *pos, int depth) {
	assert(pos);
	assert(depth>=0);

	if (depth == 0) return 1;

	U64 total_nodes = 0;
	Move moves[MAX_MOVES];
	const int n_moves = genMoves(pos,moves, 0);
	for (int i = 0; i < n_moves;i++) {
		//Position origpos = *pos;
		makeMove(&moves[i], pos);
		pos->tomove = !pos->tomove;
		
		const int incheck = isCheck(pos);
		if (incheck) {
			pos->tomove = !pos->tomove;
			unmakeMove(&moves[i], pos);
			continue;
		}
		pos->tomove = !pos->tomove;
		U64 nodes = perft(pos, depth - 1);
		total_nodes += nodes;
		std::cout << movetostr(moves[i]) << " - " << nodes << "\n";
		//Position aftermake = *pos;
		unmakeMove(&moves[i], pos);
	}
	return total_nodes;
}