#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "move.hpp"
#include "position.hpp"

int genAllPawnMoves(Position *pos, int square, Move *moves, int forqsearch);
int genKingMoves(Position *pos, int square, Move *moves, int forqsearch);
int genKnightMoves(Position *pos, int square, Move *moves, int forqsearch);
int genBishopMoves(Position *pos, int square, Move *moves, int forqsearch);
int genRookMoves(Position *pos, int square, Move *moves, int forqsearch);
int genQueenMoves(Position *pos, int square, Move *moves, int forqsearch);
int genMoves(Position *pos, Move *moves, int forqsearch);
#endif