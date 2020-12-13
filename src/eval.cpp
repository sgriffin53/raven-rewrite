#include "eval.hpp"
#include "position.hpp"
#include "PST.hpp"
#include "attacks.hpp"
#include <cassert>
#include <iostream>


static const int safety_table[9][30] =
{
/*0p*/{0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
/*1p*/{0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
/*2p*/{0,  1,  2,  4,   7,  11,  16,  22,  29,  37,  46,  56,  67,  79,  92, 106, 121, 137, 154, 172, 191, 211, 232, 254, 277, 301, 326, 352, 379, 400},
/*3p*/{0,  2,  5,  9,  14,  20,  27,  35,  44,  54,  65,  77,  90, 104, 119, 135, 152, 170, 189, 209, 230, 252, 275, 299, 324, 350, 377, 400, 400, 400},
/*4p*/{0,  4,  8, 13,  19,  26,  34,  43,  53,  64,  76,  89, 103, 118, 134, 151, 169, 188, 208, 229, 251, 274, 298, 323, 349, 376, 400, 400, 400, 400},
/*5p*/{0,  8, 16, 25,  35,  46,  58,  71,  85, 100, 116, 133, 151, 170, 190, 211, 233, 256, 280, 305, 331, 358, 386, 400, 400, 400, 400, 400, 400, 400},
/*6p*/{0, 16, 26, 37,  49,  62,  76,  91, 107, 124, 142, 161, 181, 202, 224, 247, 271, 296, 322, 349, 377, 400, 400, 400, 400, 400, 400, 400, 400, 400},
/*7p*/{0, 32, 44, 57,  71,  86, 102, 119, 137, 156, 176, 197, 219, 242, 266, 291, 317, 344, 372, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400},
/*8p*/{0, 64, 78, 93, 109, 126, 144, 163, 183, 204, 226, 249, 273, 298, 324, 351, 379, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400}
};

// Evaluation values

int SideToMove_mg = 24;
int SideToMove_eg = 27;
int OppKingProximity = 16;
int MyKingProximity = -10;
int PawnChain_mg = 4;
int PawnChain_eg = 2;
int DoubledPawn_mg = 8;
int DoubledPawn_eg = 7;
int IsolatedPawn_mg = 22;
int IsolatedPawn_eg = 20;
int ZeroOrEightPawns_mg = 10;
int ZeroOrEightPawns_eg = 10;
int PawnShield = 27;
int PawnsInCentre_mg = 14;
int PawnsInCentre_eg = 0;
int PawnsAttackingCentre_mg = 9;
int PawnsAttackingCentre_eg = 8;
int ConnectedKnights_mg = 2;
int ConnectedKnights_eg = 37;
int KnightsProtectedByPawn_mg = 7;
int KnightsProtectedByPawn_eg = 16;
int ClosedKnights_mg = 50;
int ClosedKnights_eg = 50;
int BishopPair_mg = 28;
int BishopPair_eg = 106;
int BishopsProtectedByPawn_mg = 6;
int BishopsProtectedByPawn_eg = 12;
int OpenBishops_mg = 0;
int OpenBishops_eg = 0;
int RookOpenFile_mg = 60;
int RookOpenFile_eg = 2;
int RookSemiOpenFile_mg = 27;
int RookSemiOpenFile_eg = 24;
int RookSameFileQueen_mg = 20;
int RookSameFileQueen_eg = 0;
int RookOn7th_mg = 0;
int RookOn7th_eg = 11;
int QueenOn7th_mg = 0;
int QueenOn7th_eg = 35;
int KingPawnlessFlank_mg = 17;
int KingPawnlessFlank_eg = 95;
int ImbalanceFactor = 180;


int FreePawnRankBonus[8] = {0, 0, 10, 20, 40, 60, 80, 120 };
int PassedRankBonus[8] = { 0, 0, 0, 18, 52, 108, 186, 0 };
int PassedFileBonus_mg[8] = { 25, 11, -14, -14, -14, -14, 11, 25 };
int PassedFileBonus_eg[8] = { 20, 15, 5, -7, -7, 5, 15, 20 };

// Minor piece attack bonus

int minorAttackBonus_mg[6] = {6, 14, 14, 20, 22, 0};
int minorAttackBonus_eg[6] = {10, 18, 18, 28, 30, 0};

// Piece mobility

int knightMgMobility[9] = {-15, -5, -1, 2, 5, 7, 9, 11, 13};

// (10 * x Pow 0.5) - 15};

int knightEgMobility[9] = {-30, -10, -2, 4, 10, 14, 18, 22, 26};

// (20 * x Pow 0.5) - 30};

int bishopMgMobility[14] = {-25, -11, -6, -1, 3, 6, 9, 12, 14, 17, 19, 21, 23, 25};

// (14 * x Pow 0.5) - 25};

int bishopEgMobility[14] = {-50, -22, -11, -2, 6, 12, 18, 24, 29, 34, 38, 42, 46, 50};

// (28 * x Pow 0.5) - 50};

int rookMgMobility[15] = {-10, -4, -2, 0, 2, 3, 4, 5, 6, 8, 8, 9, 10, 11, 12};

// (6 * x Pow 0.5) - 10};

int rookEgMobility[15] = {-50, -22, -11, -2, 6, 12, 18, 24, 29, 34, 38, 42, 46, 50, 54};

// (28 * x Pow 0.5) - 50};

int queenMgMobility[28] = {-10, -6, -5, -4, -2, -2, -1, 0, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6,

7, 7, 8, 8, 9, 9, 10, 10, 10};

// (4 * x Pow 0.5) - 10};

int queenEgMobility[28] = {-50, -30, -22, -16, -10, -6, -2, 2, 6, 10, 13, 16, 19, 22, 24,

27, 30, 32, 34, 37, 39, 41, 43, 45, 47, 50, 51, 53}; 

// (20 * x Pow 0.5) - 50};

const int arrCenterManhattanDistance[64] = { // char is sufficient as well, also unsigned
  6, 5, 4, 3, 3, 4, 5, 6,
  5, 4, 3, 2, 2, 3, 4, 5,
  4, 3, 2, 1, 1, 2, 3, 4,
  3, 2, 1, 0, 0, 1, 2, 3,
  3, 2, 1, 0, 0, 1, 2, 3,
  4, 3, 2, 1, 1, 2, 3, 4,
  5, 4, 3, 2, 2, 3, 4, 5,
  6, 5, 4, 3, 3, 4, 5, 6
};
int piecevalues[7] = { 110, 300, 300, 525, 900, 9999, 0 };

void evalMaterial(Position *pos, int *openingEval, int *endgameEval) {
	assert(pos);
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	int num_BP = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[PAWN]);
	int num_BN = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[KNIGHT]);
	int num_BB = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[BISHOP]);
	int num_BR = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[ROOK]);
	int num_BQ = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[QUEEN]);
	int num_WP = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[PAWN]);
	int num_WN = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[KNIGHT]);
	int num_WB = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[BISHOP]);
	int num_WR = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[ROOK]);
	int num_WQ = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[QUEEN]);
	
	// Piece values
    int white_pieces = num_WP*pieceval(PAWN)   +
                       num_WN*pieceval(KNIGHT) +
                       num_WB*pieceval(BISHOP) +
                       num_WR*pieceval(ROOK)   +
                       num_WQ*pieceval(QUEEN);

    int black_pieces = num_BP*pieceval(PAWN)   +
                       num_BN*pieceval(KNIGHT) +
                       num_BB*pieceval(BISHOP) +
                       num_BR*pieceval(ROOK)   +
                       num_BQ*pieceval(QUEEN);

    *openingEval += white_pieces - black_pieces;
	*endgameEval += white_pieces - black_pieces;
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
}
int pieceval(const char inpiece) {
	return piecevalues[inpiece];
}

void evalPawns(Position *pos, int *openingEval, int *endgameEval) {
	assert(pos);
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
	// Evaluate pawns
	
	U64 BBwhitePP = 0ULL;
	U64 BBblackPP = 0ULL;
	
	// white pawns
	
	U64 BBwhitepawns = (pos->colours[WHITE] & pos->pieces[PAWN]);
	int num_WP = __builtin_popcountll(BBwhitepawns);
	while (BBwhitepawns) {
		int square = __builtin_ctzll(BBwhitepawns);
		BBwhitepawns &= BBwhitepawns - 1;
		U64 BBpiece = 1ULL << square;
		U64 BBfilemask = BBfileA << getfile(square);
		
		// pawn chain bonus
		
		// doubled pawns

		// isolated pawns
	}
	
	// black pawns
	
	U64 BBblackpawns = (pos->colours[BLACK] & pos->pieces[PAWN]);
	int num_BP = __builtin_popcountll(BBblackpawns);
	while (BBblackpawns) {
		int square = __builtin_ctzll(BBblackpawns);
		//BBblackpawns &= ~(1ULL << square);
		BBblackpawns &= BBblackpawns - 1;
		U64 BBpiece = 1ULL << square;
		U64 BBfilemask = BBfileA << getfile(square);
		
		// pawn chain bonus
		
		// doubled pawns

		// isolated pawns
	}
	
	// penalty for zero or eight pawns

	// pawn shield
	
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
}

void evalMobility(Position *pos, int *openingEval, int *endgameEval) {
	assert(pos);
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
	// Evaluates mobility, king attackers, and NBRQ PST
	
	int kingattackers = 0;
	int kingattacks = 0;
	// white
	
	int idx = 0;
	double centremult = 0.5;
	double hrattackbonus = 1;
	struct mobreturn WNmobility = Nmobility(pos,WHITE);
	idx = std::min(8, std::max(0, WNmobility.mobility));
	*openingEval += knightMgMobility[idx] / 2;
	*endgameEval += knightEgMobility[idx] / 2;
	kingattacks +=  WNmobility.kingattacks;
	kingattackers += WNmobility.kingattackers;
	
	struct mobreturn WBmobility = Bmobility(pos,WHITE);
	idx = std::min(13, std::max(0, WBmobility.mobility));
	*openingEval += bishopMgMobility[idx] / 2;
	*endgameEval += bishopEgMobility[idx] / 2;
	kingattacks += WBmobility.kingattacks;
	kingattackers += WBmobility.kingattackers;
	
	struct mobreturn WRmobility = Rmobility(pos,WHITE);
	idx = std::min(14, std::max(0, WRmobility.mobility));
	*openingEval += rookMgMobility[idx] / 2;
	*endgameEval += rookEgMobility[idx] / 2;
	kingattacks += 2 * WRmobility.kingattacks;
	kingattackers += WRmobility.kingattackers;
	
	struct mobreturn WQmobility = Qmobility(pos,WHITE);
	idx = std::min(27, std::max(0, WQmobility.mobility));
	*openingEval += queenMgMobility[idx] / 2;
	*endgameEval += queenEgMobility[idx] / 2;
	kingattacks += 4 * WQmobility.kingattacks;
	kingattackers += WQmobility.kingattackers;
	
	// prevent overflow
	
	if (kingattackers >= 8) kingattackers = 8;
	if (kingattacks >= 29) kingattacks = 29;
	
	//*openingEval += safety_table[kingattackers][kingattacks];
	//*endgameEval += safety_table[kingattackers][kingattacks];
	
	kingattacks = 0;
	kingattackers = 0;
	// black
	struct mobreturn BNmobility = Nmobility(pos,BLACK);
	idx = std::min(8, std::max(0, BNmobility.mobility));
	*openingEval -= knightMgMobility[idx] / 2;
	*endgameEval -= knightEgMobility[idx] / 2;
	kingattacks += BNmobility.kingattacks;
	kingattackers += BNmobility.kingattackers;
	
	struct mobreturn BBmobility = Bmobility(pos,BLACK);
	idx = std::min(13, std::max(0, BBmobility.mobility));
	*openingEval -= bishopMgMobility[idx] / 2;
	*endgameEval -= bishopEgMobility[idx] / 2;
	kingattacks += BBmobility.kingattacks;
	kingattackers += BBmobility.kingattackers;
	
	struct mobreturn BRmobility = Rmobility(pos,BLACK);
	idx = std::min(14, std::max(0, BRmobility.mobility));
	*openingEval -= rookMgMobility[idx] / 2;
	*endgameEval -= rookEgMobility[idx] / 2;
	kingattacks += 2 * BRmobility.kingattacks;
	kingattackers += BRmobility.kingattackers;
	
	struct mobreturn BQmobility = Qmobility(pos,BLACK);
	idx = std::min(27, std::max(0, BQmobility.mobility));
	*openingEval -= queenMgMobility[idx] / 2;
	*endgameEval -= queenEgMobility[idx] / 2;
	kingattacks += 4 * BQmobility.kingattacks;
	kingattackers += BQmobility.kingattackers;
	
	// prevent overflow
	
	if (kingattackers >= 8) kingattackers = 8;
	if (kingattacks >= 29) kingattacks = 29;
	
	//*openingEval -= safety_table[kingattackers][kingattacks];
	//*endgameEval -= safety_table[kingattackers][kingattacks];
	
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
}
void evalPST(Position *pos, int *openingEval, int *endgameEval) {
	assert(pos);
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
	U64 BBpieces = pos->colours[WHITE] | pos->colours[BLACK];
	
	while (BBpieces != 0) {
		int square = __builtin_ctzll(BBpieces);
		//BBoccupied &= ~(1ULL << square);
		BBpieces &= BBpieces - 1;
		char piece = getPiece(pos,square);
		int col = getColour(pos, square);
		*openingEval += PSTval(col, piece,square,'O');
		*endgameEval += PSTval(col, piece,square,'E');
	}
	
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
}
int finalEval(Position *pos, int *openingEval, int *endgameEval) {
	
	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
	// get number of pieces for calculating phase for final eval
	
	int num_BP = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[PAWN]);
	int num_BN = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[KNIGHT]);
	int num_BB = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[BISHOP]);
	int num_BR = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[ROOK]);
	int num_BQ = __builtin_popcountll(pos->colours[BLACK] & pos->pieces[QUEEN]);
	int num_WP = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[PAWN]);
	int num_WN = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[KNIGHT]);
	int num_WB = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[BISHOP]);
	int num_WR = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[ROOK]);
	int num_WQ = __builtin_popcountll(pos->colours[WHITE] & pos->pieces[QUEEN]);
	
	int pawnPhase = 0;
	int knightPhase = 1;
	int bishopPhase = 1;
	int rookPhase = 2;
	int queenPhase = 4;
	
	int totalPhase = pawnPhase * 16 + knightPhase * 4 + bishopPhase*4 + rookPhase*4 + queenPhase*2;
	int phase = totalPhase;

	phase -= num_WP * pawnPhase;
	phase -= num_WN * knightPhase;
	phase -= num_WB * bishopPhase;
	phase -= num_WR * rookPhase;
	phase -= num_WQ * queenPhase;
	phase -= num_BP * pawnPhase;
	phase -= num_BN * knightPhase;
	phase -= num_BB * bishopPhase;
	phase -= num_BR * rookPhase;
	phase -= num_BQ * queenPhase;

	phase = (phase * 256 + (totalPhase / 2)) / totalPhase;
	
	int eval = ((*openingEval * (256 - phase)) + (*endgameEval * phase)) / 256;

	if (pos->tomove == BLACK) eval = -eval;

	assert(*openingEval >= -MATE_SCORE && *openingEval <= MATE_SCORE);
	assert(*endgameEval >= -MATE_SCORE && *endgameEval <= MATE_SCORE);
	
	return eval;
}

int taperedEval(Position *pos) {
	assert(pos);
	
	int openingEval = 0;
	int endgameEval = 0;
	evalPST(pos, &openingEval, &endgameEval);
	evalMaterial(pos, &openingEval, &endgameEval);
	//evalPawns(pos, &openingEval, &endgameEval);
	evalMobility(pos, &openingEval, &endgameEval);
	return finalEval(pos, &openingEval, &endgameEval);
}


struct mobreturn Nmobility(Position *pos, int side) {
	U64 BBsidepieces;
	U64 BBkingzone;
	U64 BBkingonly;
	U64 BBattackedbypawns;
	if (side == WHITE) {
		BBsidepieces = pos->colours[WHITE];
		BBkingonly = pos->pieces[KING] & pos->colours[BLACK];
		BBkingzone = BBpawnshieldLookup[BLACK][pos->Bkingpos];
		BBattackedbypawns = pawn_attacksB(pos->pieces[PAWN] & pos->colours[BLACK]);
	}
	else {
		BBsidepieces = pos->colours[BLACK];
		BBkingonly = pos->pieces[KING] & pos->colours[WHITE];
		BBkingzone = BBpawnshieldLookup[WHITE][pos->Wkingpos];
		BBattackedbypawns = pawn_attacksW(pos->pieces[PAWN] & pos->colours[WHITE]);
	}
	U64 BBallowed = ~BBsidepieces;
	U64 BBoccupied = pos->colours[WHITE] | pos->colours[BLACK];
	U64 BBmoves = 0;
	U64 BBcopy = 0;
	int from = 0;
	
	int kzattacks = 0;
	int kzattackers = 0;
	char piece = KNIGHT;
	int centre = 0;
	// Knights
	BBcopy = pos->pieces[KNIGHT] & BBsidepieces;
	while(BBcopy)
	{
		from = __builtin_ctzll(BBcopy);
		//BBmoves |= BBknightattacks(1ULL << from) & BBallowed;
		U64 BBnewmoves = knight_attacks(from) & BBallowed;
		BBmoves |= BBnewmoves;
		U64 BBkzattacks = BBnewmoves & BBkingzone;
		centre += __builtin_popcountll(BBcentre & BBnewmoves);
		kzattacks += __builtin_popcountll(BBkzattacks);
		if (BBkzattacks) kzattackers++;
		BBcopy &= BBcopy-1;
	}
	struct mobreturn returnstruct;
	returnstruct.kingattacks = kzattacks;
	returnstruct.mobility = __builtin_popcountll(BBmoves);
	returnstruct.kingattackers = kzattackers;
	returnstruct.unsafe = __builtin_popcountll(BBmoves & BBattackedbypawns);
	returnstruct.centre = centre;
	return returnstruct;
}
struct mobreturn Bmobility(Position *pos, int side) {
	U64 BBsidepieces;
	U64 BBkingzone;
	U64 BBkingonly;
	U64 BBattackedbypawns;
	if (side == WHITE) {
		BBsidepieces = pos->colours[WHITE];
		BBkingonly = pos->pieces[KING] & pos->colours[BLACK];
		BBkingzone = BBpawnshieldLookup[BLACK][pos->Bkingpos];
		BBattackedbypawns = pawn_attacksB(pos->pieces[PAWN] & pos->colours[BLACK]);
	}
	else {
		BBsidepieces = pos->colours[BLACK];
		BBkingonly = pos->pieces[KING] & pos->colours[WHITE];
		BBkingzone = BBpawnshieldLookup[WHITE][pos->Wkingpos];
		BBattackedbypawns = pawn_attacksW(pos->pieces[PAWN] & pos->colours[WHITE]);
	}
	int kzattackers = 0;
	int kzattacks = 0;
	U64 BBallowed = ~BBsidepieces;
	U64 BBoccupied = pos->colours[WHITE] | pos->colours[BLACK];
	U64 BBmoves = 0;
	U64 BBcopy = 0;
	int from = 0;
	int centre = 0;
	char piece = BISHOP;
	BBcopy = pos->pieces[BISHOP] & BBsidepieces;
	while(BBcopy)
	{
		from = __builtin_ctzll(BBcopy);
		U64 BBnewmoves = bishop_attacks(from, BBoccupied) & BBallowed;
		BBmoves |= BBnewmoves;
		U64 BBkzattacks = BBnewmoves & BBkingzone;
		kzattacks += __builtin_popcountll(BBkzattacks);
		centre += __builtin_popcountll(BBcentre & BBnewmoves);
		if (BBkzattacks) kzattackers++;
		BBcopy &= BBcopy-1;
	}
	struct mobreturn returnstruct;
	returnstruct.kingattacks = kzattacks;
	returnstruct.mobility = __builtin_popcountll(BBmoves);
	returnstruct.kingattackers = kzattackers;
	returnstruct.unsafe = __builtin_popcountll(BBmoves & BBattackedbypawns);
	returnstruct.centre = centre;
	return returnstruct;
}
struct mobreturn Rmobility(Position *pos, int side) {
	U64 BBsidepieces;
	U64 BBkingzone;
	U64 BBkingonly;
	U64 BBattackedbypawns;
	if (side == WHITE) {
		BBsidepieces = pos->colours[WHITE];
		BBkingonly = pos->pieces[KING] & pos->colours[BLACK];
		BBkingzone = BBpawnshieldLookup[BLACK][pos->Bkingpos];
		BBattackedbypawns = pawn_attacksB(pos->pieces[PAWN] & pos->colours[BLACK]);
	}
	else {
		BBsidepieces = pos->colours[BLACK];
		BBkingonly = pos->pieces[KING] & pos->colours[WHITE];
		BBkingzone = BBpawnshieldLookup[WHITE][pos->Wkingpos];
		BBattackedbypawns = pawn_attacksW(pos->pieces[PAWN] & pos->colours[WHITE]);
	}
	int kzattackers = 0;
	int kzattacks = 0;
	U64 BBallowed = ~BBsidepieces;
	U64 BBoccupied = pos->colours[WHITE] | pos->colours[BLACK];
	U64 BBmoves = 0;
	U64 BBcopy = 0;
	char piece = ROOK;
	int from = 0;
	int centre = 0;
	BBcopy = pos->pieces[ROOK] & BBsidepieces;
	while(BBcopy)
	{
		from = __builtin_ctzll(BBcopy);
		U64 BBnewmoves = rook_attacks(from, BBoccupied) & BBallowed;
		BBmoves |= BBnewmoves;
		U64 BBkzattacks = BBnewmoves & BBkingzone;
		kzattacks += __builtin_popcountll(BBkzattacks);
		centre += __builtin_popcountll(BBcentre & BBnewmoves);
		if (BBkzattacks) kzattackers++;
		BBcopy &= BBcopy-1;
	}
	struct mobreturn returnstruct;
	returnstruct.kingattacks = kzattacks;
	returnstruct.mobility = __builtin_popcountll(BBmoves);
	returnstruct.kingattackers = kzattackers;
	returnstruct.unsafe = __builtin_popcountll(BBmoves & BBattackedbypawns);
	returnstruct.centre = centre;
	return returnstruct;
}
struct mobreturn Qmobility(Position *pos, int side) {
	U64 BBsidepieces;
	U64 BBkingzone;
	U64 BBkingonly;
	U64 BBattackedbypawns;
	if (side == WHITE) {
		BBsidepieces = pos->colours[WHITE];
		BBkingonly = pos->pieces[KING] & pos->colours[BLACK];
		BBkingzone = BBpawnshieldLookup[BLACK][pos->Bkingpos];
		BBattackedbypawns = pawn_attacksB(pos->pieces[PAWN] & pos->colours[BLACK]);
	}
	else {
		BBsidepieces = pos->colours[BLACK];
		BBkingonly = pos->pieces[KING] & pos->colours[WHITE];
		BBkingzone = BBpawnshieldLookup[WHITE][pos->Wkingpos];
		BBattackedbypawns = pawn_attacksW(pos->pieces[PAWN] & pos->colours[WHITE]);
	}
	int kzattackers = 0;
	int kzattacks = 0;
	U64 BBallowed = ~BBsidepieces;
	U64 BBoccupied = pos->colours[WHITE] | pos->colours[BLACK];
	U64 BBmoves = 0;
	U64 BBcopy = 0;
	char piece = QUEEN;
	int from = 0;
	int centre = 0;
	BBcopy = pos->pieces[QUEEN] & BBsidepieces;
	while(BBcopy)
	{
		from = __builtin_ctzll(BBcopy);
		U64 BBnewmoves = queen_attacks(from, BBoccupied) & BBallowed;
		BBmoves |= BBnewmoves;
		U64 BBkzattacks = BBnewmoves & BBkingzone;
		kzattacks += __builtin_popcountll(BBkzattacks);
		centre += __builtin_popcountll(BBcentre & BBnewmoves);
		if (BBkzattacks) kzattackers++;
		BBcopy &= BBcopy-1;
	}
	struct mobreturn returnstruct;
	returnstruct.kingattacks = kzattacks;
	returnstruct.mobility = __builtin_popcountll(BBmoves);
	returnstruct.kingattackers = kzattackers;
	returnstruct.unsafe = __builtin_popcountll(BBmoves & BBattackedbypawns);
	returnstruct.centre = centre;
	return returnstruct;
}