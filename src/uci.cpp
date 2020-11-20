#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "position.hpp"
void UCI_Listen() {
	Position pos;
	parsefen(&pos,"startpos");
	bool keeprunning = true;
	while (keeprunning) {
		std::string line, intermediate;
		
		// read input
		getline(std::cin, line);
      
		// Vector of string to save tokens 
		std::vector <std::string> tokens;
		// stringstream class check1 
		std::stringstream check1(line);
		int numtokens = 0;
		// Tokenizing w.r.t. space ' ' 
		while(getline(check1, intermediate, ' ')) 
		{ 
			tokens.push_back(intermediate); 
			numtokens++;
		}
	}
}