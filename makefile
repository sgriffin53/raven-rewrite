SOURCES = src/attacks.cpp src/bitboards.cpp src/main.cpp src/makemove.cpp src/move.cpp src/movegen.cpp src/perft.cpp src/position.cpp src/uci.cpp src/libchess/src/movegen.cpp src/search.cpp src/eval.cpp src/PST.cpp src/globals.cpp src/hash.cpp src/draw.cpp src/sort.cpp
TEST_SRC = tests/main.cpp src/attacks.cpp src/bitboards.cpp src/draw.cpp src/globals.cpp src/hash.cpp src/makemove.cpp src/move.cpp src/movegen.cpp src/perft.cpp src/position.cpp src/libchess/src/movegen.cpp
DFLAGS  = -std=c++17 -Wpedantic -Wall -Wextra -Wshadow
RFLAGS  = -std=c++17 -flto -Ofast -O3 -DNDEBUG
release:
	g++ $(SOURCES) $(RFLAGS) -o raven
debug:
	g++ $(SOURCES) $(DFLAGS) -o raven-debug
fast:
	g++ $(SOURCES) -o raven-fast
tests:
	g++ $(TEST_SRC) $(DFLAGS) -o bin/tests
