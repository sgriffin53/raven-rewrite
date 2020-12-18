#include "PST.hpp"
#include "chess/position.hpp"

int PSTval(int colour, const char piece, const int square, const char gamestage) {
	int stage = 0;
	if (gamestage == 'E') stage = 1;
	int flipsquare = fileranktosquareidx(getfile(square), 7 - getrank(square));
	if (colour == BLACK) return -PST[piece][stage][flipsquare];
	else return PST[piece][stage][square];
}