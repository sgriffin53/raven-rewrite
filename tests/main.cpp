#include "ischeck.hpp"
#include "perft.hpp"
#include <iomanip>
#include <iostream>

int main() {
	std::cout << std::boolalpha;

	std::cout << "Raven tests\n\n";
	std::cout << "Perft: " << test::perft() << "\n";
	std::cout << "isCheck: " << test::isCheck() << "\n";
	return 0;
}
