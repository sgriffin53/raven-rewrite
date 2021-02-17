#include "draw.hpp"
#include "bitboards.hpp"
#include "hash.hpp"
#include "position.hpp"
#include <cassert>
#include <iostream>

int isThreefold(Position *pos) {
	assert(pos);
	assert(pos->halfmoves >= 0);
	U64 curposhash = pos->hashstack[pos->irrevidx];
	if (pos->halfmoves <= 4)
		return 0;
	int numrepeats = 0;
	for (int i = (pos->irrevidx); (i > (pos->irrevidx - 1 - pos->halfmoves - 1)) && i >= 0; i -= 2) {
		if (pos->hashstack[i] == curposhash) {
			numrepeats++;
			if (numrepeats >= 3) {
				return 1;
			}
		}
	}
	return 0;
}

int isInsufficientMaterial(Position *pos) {
	if (!(pos->pieces[PAWN] | pos->pieces[KNIGHT] | pos->pieces[BISHOP] | pos->pieces[ROOK] | pos->pieces[QUEEN])) {
		// king vs king
		return 1;
	}
	if (!(pos->pieces[PAWN] | pos->pieces[KNIGHT] | pos->pieces[ROOK] | pos->pieces[QUEEN])) {
		if (__builtin_popcountll(pos->pieces[BISHOP]) == 1) {
			// KB vs K
			return 1;
		} else if (__builtin_popcountll(pos->colours[BLACK] & pos->pieces[BISHOP]) == 1 && __builtin_popcountll(pos->colours[WHITE] & pos->pieces[BISHOP]) == 1) {
			if (pos->colours[BLACK] & pos->pieces[BISHOP] & BBlightsquares && pos->colours[WHITE] & pos->pieces[BISHOP] & BBlightsquares) {
				// both have only LSB
				return 1;
			}
			if (pos->colours[BLACK] & pos->pieces[BISHOP] & BBdarksquares && pos->colours[WHITE] & pos->pieces[BISHOP] & BBdarksquares) {
				// both have only DSB
				return 1;
			}
		}
	}
	if (!(pos->pieces[PAWN] | pos->pieces[BISHOP] | pos->pieces[ROOK] | pos->pieces[QUEEN])) {
		if (__builtin_popcountll(pos->pieces[KNIGHT]) == 1) {
			// KN vs K
			return 1;
		}
	}
	return 0;
}
