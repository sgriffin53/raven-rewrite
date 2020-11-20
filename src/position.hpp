#ifndef POSITION_H
#define POSITION_H
#include <string>

typedef unsigned long long U64;

#define WHITE 1
#define BLACK 0

enum PIECE{ PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE };
class Irreversible {
public:
	int epsquare;
	int WcastleQS;
	int WcastleKS;
	int BcastleKS;
	int BcastleQS;
	int Wcastled;
	int Bcastled;
	int halfmoves;
	int Wkingpos;
	int Bkingpos;
};
class Position {
public:
	U64 pieces[6];
	U64 colours[2];
	int tomove;
	int epsquare;
	int Wkingpos;
	int Bkingpos;
	int halfmoves;
	int fullmoves;
	int WcastleQS;
	int WcastleKS;
	int BcastleQS;
	int BcastleKS;
	int Wcastled;
	int Bcastled;
	Irreversible irrev[2048];
	int irrevidx;
};
void parsefen(Position *pos, std::string ofen);
#endif