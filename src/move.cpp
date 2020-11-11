#ifndef MOVE_H
#define MOVE_H
#include "move.h"
std::string movetostr(Move move) {
	std::string returnstring;
	char startsquarefile = (char)(getfile(move.from) + 97);
	char startsquarerank = (char)(getrank(move.from) + 49);
	char endsquarefile = (char)(getfile(move.to) + 97);
	char endsquarerank = (char)(getrank(move.to) + 49);
	char prom = 0;
	if (move.prom == KNIGHT) prom = 'n';
	else if (move.prom == BISHOP) prom = 'b';
	else if (move.prom == ROOK) prom = 'r';
	else if (move.prom == QUEEN) prom = 'q';
	//std::cout << startsquarefile << "\n";
	returnstring += startsquarefile;
	returnstring += startsquarerank;
	returnstring += endsquarefile;
	returnstring += endsquarerank;
	returnstring += prom;
	return returnstring;
}
#endif