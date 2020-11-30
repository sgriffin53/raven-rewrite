#ifndef SORT_HPP
#define SORT_HPP

#include "move.hpp"
#include "position.hpp"

void sortMoves(Position *pos, Move *moves, const int num_moves, Move *TTmove, int ply);
int mvvlva(char piece, char cappiece);
int capval(int piece);

#endif