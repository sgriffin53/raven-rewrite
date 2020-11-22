#include <vector>
#include <sstream>
#include <string>
#include <cassert>
#include "position.hpp"

void parsefen(Position *pos, std::string ofen) {
	if (ofen == "startpos") {
		parsefen(pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		return;
	}
	std::string fen = ofen;
	
	int numtokens;
	
	std::string line, intermediate;
	//std::string intermediate; 

	line = ofen;
  
	// Vector of string to save tokens 
	std::vector <std::string> tokens; 
  
	// stringstream class check1 
	std::stringstream check1(line); 
	// Tokenizing w.r.t. space ' ' 
	while(getline(check1, intermediate, ' ')) 
	{ 
		numtokens++;
		tokens.push_back(intermediate); 
	} 
	
}