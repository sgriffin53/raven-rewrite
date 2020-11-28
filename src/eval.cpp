#include "eval.hpp"
#include "position.hpp"
#include "PST.hpp"
#include <cassert>

int piecevalues[7] = { 110, 300, 300, 525, 900, 9999, 0 };

void evalMaterial(Position *pos, int *openingEval, int *endgameEval) {
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
}
int pieceval(const char inpiece) {
	return piecevalues[inpiece];
}
void evalPST(Position *pos, int *openingEval, int *endgameEval) {
	
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
	
}
int finalEval(Position *pos, int *openingEval, int *endgameEval) {
	
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
	return eval;
}

int taperedEval(Position *pos) {
	assert(pos);
	
	int openingEval = 0;
	int endgameEval = 0;
	evalPST(pos, &openingEval, &endgameEval);
	evalMaterial(pos, &openingEval, &endgameEval);
	
	return finalEval(pos, &openingEval, &endgameEval);
}