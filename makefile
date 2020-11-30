SOURCES = src/attacks.cpp src/bitboards.cpp src/main.cpp src/makemove.cpp src/move.cpp src/movegen.cpp src/perft.cpp src/position.cpp src/tests.cpp src/uci.cpp src/libchess/src/movegen.cpp src/search.cpp src/eval.cpp src/PST.cpp src/misc.cpp src/globals.cpp src/hash.cpp src/draw.cpp src/sort.cpp
DFLAGS  = -std=c++17 -Wpedantic -Wall -Wextra -Wshadow
RFLAGS  = -std=c++17 -flto -Ofast -O3 -DNDEBUG
release:
	g++ $(SOURCES) $(RFLAGS) -o raven
debug:
	g++ $(SOURCES) $(DFLAGS) -o raven-debug
fast:
	g++ $(SOURCES) -o raven-fast