#ifndef MOVE_H
#define MOVE_H

#define MAX_MOVES 2048

enum MOVETYPE{ NORMAL, DOUBLE, CAPTURE, EN_PASSANT, PROMO, PROMO_CAPTURE , KSC, QSC };

class Move {
public:
	int from;
	int to;
	char prom;
	char piece;
	char cappiece;
	char type;
};
std::string movetostr(Move move);

#endif