#include <iostream>
#include <iomanip>
#include "perft.hpp"

int main() {
    std::cout << std::boolalpha;   

    std::cout << "Raven tests\n\n";
    std::cout << "Perft: " << test::perft() << "\n";
    return 0;
}
