#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "position.hpp"
#include "move.hpp"
#include "eval.hpp"
#include "move.hpp"
#include <ctime>

clock_t getClock();
int outOfTime(clock_t endtime);
Move search(Position pos, int searchdepth, int movetime, int strictmovetime);
int alphaBeta(Position *pos, int alpha, int beta, int depthleft, int nullmove, int ply, Move *pv, clock_t endtime, int cut);
Move randmove(Position pos);
#endif