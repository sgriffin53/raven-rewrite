#include "position.hpp"
#include "globals.hpp"

U64 nodesSearched;
int seldepth;
int strictmovetime;
int wtime, btime, movestogo;
U64 BBpasserLookup[2][64];
U64 BBpawnshieldLookup[2][64];