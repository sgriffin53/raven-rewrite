#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "../chess/move.hpp"
#include "../chess/position.hpp"
#include "eval.hpp"
#include <ctime>
#include <vector>

clock_t getClock();
int outOfTime(clock_t endtime);
Move search(Position pos, int searchdepth, int movetime, int strictmovetime);
int alphaBeta(Position *pos, int alpha, int beta, int depthleft, int nullmove, int ply, Move *pv, clock_t endtime, std::vector<Move> &fullPV, int cut);
Move randmove(Position pos);
int qSearch(Position *pos, int alpha, int beta, int ply, clock_t endtime);

#endif
