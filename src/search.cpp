#include "search.hpp"
#include "eval.hpp"
#include "move.hpp"
#include <ctime>
#include <iostream>
#include "globals.hpp"
#include "movegen.hpp"
#include "makemove.hpp"
#include "attacks.hpp"
#include "tests.hpp"
#include "draw.hpp"
#include "hash.hpp"

#define ONE_PLY 4
#define MAX_MOVES 2048

clock_t getClock() {
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return ((clock_t)tp.tv_sec * 1000 + tp.tv_nsec / 1000000);
}
int outOfTime(clock_t endtime) {
	static int counter = 0;

	if (counter == 0) {
		counter = 512;
		return getClock() >= endtime;
	}
	--counter;
	return 0;
}

int alphaBeta(Position *pos, int alpha, int beta, int depthleft, int nullmove, int ply, Move *pv, clock_t endtime, int cut) {
	if (ply > seldepth) seldepth = ply;
	if (outOfTime(endtime)) {
		return NO_SCORE;
	}
	// TODO: draw detection
	if (depthleft <= 0) {
		//dspBoard(pos);
		//std::cout << "eval: " << taperedEval(pos) << "\n";
		return taperedEval(pos);
	}
	if (pos->halfmoves >= 100) return 0;
	if (isInsufficientMaterial(pos)) return 0;
	if (isThreefold(pos)) return 0;
	int bestscore = INT_MIN;
	Move bestmove;
	Move moves[MAX_MOVES];
	int legalmoves = 0;
	int incheck = isCheck(pos);
	int num_moves = genMoves(pos, moves, 0);
	for (int i = 0;i < num_moves;i++) {
		//Position origpos = *pos;
		makeMove(&moves[i], pos);
		if (pos->Wkingpos < 0 || pos->Wkingpos > 63 || pos->Bkingpos < 0 || pos->Bkingpos > 63) {
			dspBoard(pos);
		}
		pos->tomove = !pos->tomove;
		if (isCheck(pos)) {
			pos->tomove = !pos->tomove;
			unmakeMove(&moves[i],pos);
			continue;
		}
		pos->tomove = !pos->tomove;
		legalmoves++;
		nodesSearched++;
		int score = -alphaBeta(pos, -beta, -alpha, depthleft - ONE_PLY, 0, ply + 1, pv, endtime, !cut);
		//std::cout << "move: " << movetostr(moves[i]) << " score: " << score << "\n";
		unmakeMove(&moves[i],pos);
		//Position after = *pos;
		//if (!posMatch(&origpos, &after)) {
		//	dspBoard(&origpos);
		//	std::cout << "move: " << movetostr(moves[i]) << "\n";
		//}
		if (score == -NO_SCORE) return NO_SCORE;
		if (score > bestscore) {
			bestscore = score;
			bestmove = moves[i];
		}
		if (score > alpha) {
			alpha = bestscore;
		}
		if (alpha >= beta) {
			break;
		}
	}
	if (legalmoves == 0) {
		if (incheck) {
			return -MATE_SCORE + ply; // checkmate
		}
		else {
			return 0; // stalemate
		}
	}
	*pv = bestmove;
	return bestscore;
}

Move search(Position pos, int searchdepth, int movetime, int strictmovetime) {
	Move pv;
	int score = 0;
	int time_spentms = 0;
	double time_spent;
	nodesSearched = 0;
	seldepth = 0;
	Move bestmove;
	const clock_t begin = getClock();
	clock_t endtime = begin + movetime;
	for (int d = 1;d <= searchdepth;d++) {
		time_spentms = getClock() - begin;
		bestmove = pv;
		score = alphaBeta(&pos, -MATE_SCORE, MATE_SCORE, d * ONE_PLY, 0, 0, &pv, endtime, 0);
		if (d > 1 && score == NO_SCORE) {
			break;
		}
		bestmove = pv;
		time_spentms = getClock() - begin;
		time_spent = time_spentms / 1000.0;
		int nps = nodesSearched / time_spent;
		std::cout << "info";
		std::cout << " depth " << d;
		std::cout << " seldepth " << seldepth;
		std::cout << " nodes " << nodesSearched;
		std::cout << " time " << (int)(time_spent*1000);
		if (time_spent > 0) {
			std::cout << " nps " << nps;
		}
		std::cout << " score cp " << score;
		std::cout << " pv " << movetostr(pv);
		std::cout << "\n";
	}
	time_spentms = getClock() - begin;
	time_spent = time_spentms / 1000.0;
	std::cout << "info time " << (int)(time_spent*1000) << "\n";
	std::cout << "bestmove " << movetostr(bestmove) << "\n";
	return pv;
}
Move randmove(Position pos) {
	// returns a random root move
	
	srand(time(NULL));
	Move moves[2048];
	int num_moves = genMoves(&pos, moves, 0);
	int idx;
	Move move;
	bool valid = false;
	while (!valid) { // loop until we find a legal move
		idx = rand() % num_moves;
		move = moves[idx];
		makeMove(&move, &pos);
		pos.tomove = !pos.tomove;
		if (isCheck(&pos)) {
			pos.tomove = !pos.tomove;
			unmakeMove(&move, &pos);
			continue;
		}
		pos.tomove = !pos.tomove;
		unmakeMove(&move, &pos);
		valid = true;
	}
	std::cout << "info";
	std::cout << " depth 1";
	std::cout << " seldepth 1";
	std::cout << " nodes 0";
	std::cout << " time 0";
	//if (time_spent > 0) {
	//	std::cout << " nps 0";
	//}
	std::cout << " score cp 0";
	std::cout << " pv " << movetostr(moves[idx]);
	std::cout << "\n";
	std::cout << "bestmove " << movetostr(moves[idx]);
	std::cout << "\n";
	return moves[idx];
}