#ifndef EVAL_HPP
#define EVAL_HPP

#include "chess/position.hpp"

struct mobreturn {
	int mobility;
	int mobility2; // number of squares that can be reached in 2 moves but not 1
	int kingattacks;
	int kingattackers;
	int pstO;
	int pstE;
	int unsafe;
	int centre;
	int homerowsattacks;
};
struct mobreturn Nmobility(Position *pos, int side);
struct mobreturn Bmobility(Position *pos, int side);
struct mobreturn Rmobility(Position *pos, int side);
struct mobreturn Qmobility(Position *pos, int side);
void evalMaterial(Position *pos, int *openingEval, int *endgameEval);
int pieceval(const char inpiece);
int finalEval(Position *pos, int *openingEval, int *endgameEval);
int taperedEval(Position *pos);
void evalPST(Position *pos, int *openingEval, int *endgameEval);
void evalPawns(Position *pos, int *openingEval, int *endgameEval);
void evalMobility(Position *pos, int *openingEval, int *endgameEval);

#endif