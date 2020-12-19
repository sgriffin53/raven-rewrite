SOURCES = $(wildcard src/*.cpp) $(wildcard src/chess/*.cpp) $(wildcard src/search/*.cpp) src/chess/libchess/src/movegen.cpp
TEST_SRC = $(wildcard tests/*.cpp) $(wildcard src/chess/*.cpp) src/chess/libchess/src/movegen.cpp src/perft.cpp
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
