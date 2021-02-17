#ifndef HASH_HPP
#define HASH_HPP

#include "position.hpp"

void initZobrist();
int pieceintval(int inpiece, int colour);
U64 generateHash(Position *pos);

#endif
