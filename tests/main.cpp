#include "ischeck.hpp"
#include "kingpos.hpp"
#include "perft.hpp"
#include <iomanip>
#include <iostream>

int main() {
	std::cout << std::boolalpha;

	std::cout << "Raven tests\n\n";
	std::cout << "Perft: " << test::perft() << "\n";
	std::cout << "isCheck: " << test::isCheck() << "\n";
	std::cout << "Kingpos: " << test::kingPos() << "\n";
	return 0;
}
