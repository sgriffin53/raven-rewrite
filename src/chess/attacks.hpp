#ifndef ATTACKS_H
#define ATTACKS_H

#include "bitboards.hpp"
#include "libchess/src/libchess/movegen.hpp"
#include "position.hpp"

const int BPdirs[2][2] = {{-1, -1}, {+1, -1}};
const int WPdirs[2][2] = {{-1, +1}, {+1, +1}};

int isAttacked(Position *pos, int square, int colour);
int isCheck(Position *pos);

U64 BBpawnSinglePushW(U64 BBwpawns, U64 BBunoccupied);
U64 BBpawnDoublePushW(U64 BBwpawns, U64 BBunoccupied);

U64 BBpawnSinglePushB(U64 BBbpawns, U64 BBunoccupied);

U64 BBpawnDoublePushB(U64 BBbpawns, U64 BBunoccupied);

U64 BBpawnEastAttacksW(U64 wpawns);
U64 BBpawnWestAttacksW(U64 wpawns);

U64 BBpawnEastAttacksB(U64 bpawns);
U64 BBpawnWestAttacksB(U64 bpawns);

U64 pawn_attacksW(U64 BBpiece);
U64 pawn_attacksB(U64 BBpiece);

U64 king_attacks(U64 BBpiece);
U64 knight_attacks(U64 BBpiece);
U64 rook_attacks(U64 BBpiece, U64 BBocc);
U64 bishop_attacks(U64 BBpiece, U64 BBocc);
U64 queen_attacks(U64 BBpiece, U64 BBocc);
U64 BBkingattacks(U64 BBpiece);
U64 BBknightattacks(U64 BBpiece);

#endif
