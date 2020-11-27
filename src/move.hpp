#ifndef MOVE_H
#define MOVE_H

#include <string>

#define MAX_MOVES 2048

enum MOVETYPE{ NORMAL, DOUBLE, CAPTURE, EN_PASSANT, PROMO, PROMO_CAPTURE , KSC, QSC };

class Move {
public:
	int from;
	int to;
	int prom;
	int piece;
	int cappiece;
	int type;
};
std::string movetostr(Move move);

#endif