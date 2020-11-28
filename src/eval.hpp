#ifndef EVAL_HPP
#define EVAL_HPP

#include "position.hpp"

void evalMaterial(Position *pos, int *openingEval, int *endgameEval);
int pieceval(const char inpiece);
int finalEval(Position *pos, int *openingEval, int *endgameEval);
int taperedEval(Position *pos);
void evalPST(Position *pos, int *openingEval, int *endgameEval);

#endif