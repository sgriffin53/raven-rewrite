release:
	g++ -std=c++17 -DNDEBUG -flto -Ofast -static src/main.cpp src/position.cpp src/uci.cpp -o raven