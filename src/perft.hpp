#ifndef PERFT_HPP
#define PERFT_HPP

#include "chess/move.hpp"
#include "chess/position.hpp"

extern Move movestack[12];
extern int movestackidx, capstackidx;
extern int capstack[12];
U64 perft(Position *pos, int depth);
U64 sperft(Position *pos, int depth);

#endif
