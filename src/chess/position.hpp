#ifndef POSITION_H
#define POSITION_H
#include <string>

typedef unsigned long long U64;

#define WHITE 1
#define BLACK 0
#define MATE_SCORE 9999
#define NO_SCORE 10000
#define MAX_PLY 8192

void initLookups();

enum PIECE{ PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE };

enum SQUARE{A1,B1,C1,D1,E1,F1,G1,H1,
			A2,B2,C2,D2,E2,F2,G2,H2,
			A3,B3,C3,D3,E3,F3,G3,H3,
			A4,B4,C4,D4,E4,F4,G4,H4,
			A5,B5,C5,D5,E5,F5,G5,H5,
			A6,B6,C6,D6,E6,F6,G6,H6,
			A7,B7,C7,D7,E7,F7,G7,H7,
			A8,B8,C8,D8,E8,F8,G8,H8};

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
	U64 hashstack[MAX_PLY];
};
void dspBB(U64 BB);
int fileranktosquareidx(int file,int rank);
int getrank(int square);
int getfile(int square);
char getPiece(Position *pos, int sq);
int strsquaretoidx(std::string square);
int getColour(Position *pos, int sq);
void setPiece(Position *pos, int sq, int colour, char piece);
void clearSquare(Position *pos, int sq);
void dspBoard(Position *pos);
void parsefen(Position *pos, std::string ofen);
#endif