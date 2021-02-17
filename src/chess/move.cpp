#include "move.hpp"
#include "position.hpp"

std::string Move::string() const {
	std::string returnstring;
	char startsquarefile = (char)(getfile(from()) + 97);
	char startsquarerank = (char)(getrank(from()) + 49);
	char endsquarefile = (char)(getfile(to()) + 97);
	char endsquarerank = (char)(getrank(to()) + 49);

	returnstring += startsquarefile;
	returnstring += startsquarerank;
	returnstring += endsquarefile;
	returnstring += endsquarerank;

	if (prom() == KNIGHT)
		returnstring += 'n';
	else if (prom() == BISHOP)
		returnstring += 'b';
	else if (prom() == ROOK)
		returnstring += 'r';
	else if (prom() == QUEEN)
		returnstring += 'q';

	return returnstring;
}
