
#include "attacks.hpp"
#include "position.hpp"
#include <iostream>

int isCheck(Position *pos) {
	if (pos->tomove == WHITE) return isAttacked(pos, pos->Wkingpos, !pos->tomove);
	else return isAttacked(pos, pos->Bkingpos, !pos->tomove);
}
int isAttacked(Position *pos,int square, int colour) {
	assert(pos);
	//assert(square >= 0 && square <= 63);
	assert(colour == WHITE || colour == BLACK);
	// colour is colour of attacking side

	// check if being attacked by colour's king
	U64 BBpiece = (1ULL << square); // get bitboard of piece on square
	U64 BBattacks;
	// pawn attacks
	if (colour == WHITE) BBattacks = pawn_attacksB(BBpiece);
	else BBattacks = pawn_attacksW(BBpiece);
	BBattacks = BBattacks & (pos->pieces[PAWN] & pos->colours[colour]);
	if (BBattacks) {
		//piece being attacked by pawn
		return 1;
	}
	// get king attack squares
	BBattacks |= king_attacks(square);
	BBattacks = BBattacks & (pos->pieces[KING] & pos->colours[colour]);
	if (BBattacks) {
		//piece being attacked by king
		return 1;
	}
	// get knight attack squares
	BBattacks = knight_attacks(square);
	// get attack squares that have colour knights on them
	BBattacks = BBattacks & (pos->pieces[KNIGHT] & pos->colours[colour]);
	if (BBattacks) {
		//piece being attacked by knight
		return 1;
	}
	// rook and queen attacks
	U64 BBoccupancy = pos->colours[WHITE] | pos->colours[BLACK];
	BBattacks = rook_attacks(square,BBoccupancy);
	BBattacks = BBattacks & ~pos->colours[!colour]; // mask out non colour pieces
	BBattacks = BBattacks & (pos->pieces[ROOK] | pos->pieces[QUEEN]) & pos->colours[colour];
	if (BBattacks) {
		//piece being attacked by rook
		return 1;
	}
	// bishop and queen attacks
	BBattacks = bishop_attacks(square,BBoccupancy);
	BBattacks = BBattacks & ~pos->colours[!colour]; // mask out black pieces
	BBattacks = BBattacks & (pos->pieces[BISHOP] | pos->pieces[QUEEN]) & pos->colours[colour];
	if (BBattacks) {
		//piece being attacked by bishop
		return 1;
	}
	 
	/*
	// queen attacks
	BBattacks = Bmagic(square,BBoccupancy) | Rmagic(square,BBoccupancy);
	BBattacks = BBattacks & ~pos->colours[!colour]; // mask out black pieces
	BBattacks = BBattacks & (pos->pieces[QUEEN] & pos->colours[colour]);
	if (BBattacks) {
		//piece being attacked by queen
		return 1;
	}
	 */
	return 0;
}
U64 BBpawnEastAttacksW(U64 wpawns) {return noEaOne(wpawns);}
U64 BBpawnWestAttacksW(U64 wpawns) {return noWeOne(wpawns);}

U64 BBpawnEastAttacksB(U64 bpawns) {return soEaOne(bpawns);}
U64 BBpawnWestAttacksB(U64 bpawns) {return soWeOne(bpawns);}


U64 BBpawnSinglePushW(U64 BBwpawns, U64 BBunoccupied) {
   return northOne(BBwpawns) & BBunoccupied;
}

U64 BBpawnDoublePushW(U64 BBwpawns, U64 BBunoccupied) {
   U64 BBsinglePushes = BBpawnSinglePushW(BBwpawns, BBunoccupied);
   return northOne(BBsinglePushes) & BBunoccupied & BBrank4;
}

U64 BBpawnSinglePushB(U64 BBbpawns, U64 BBunoccupied) {
   return southOne(BBbpawns) & BBunoccupied;
}

U64 BBpawnDoublePushB(U64 BBbpawns, U64 BBunoccupied)  {
   U64 BBsinglePushes = BBpawnSinglePushB(BBbpawns, BBunoccupied);
   return southOne(BBsinglePushes) & BBunoccupied & BBrank5;
}


U64 pawn_attacksW(U64 BBpiece) {
	return BBpawnEastAttacksW(BBpiece) | BBpawnWestAttacksW(BBpiece);
}
U64 pawn_attacksB(U64 BBpiece) {
	return BBpawnEastAttacksB(BBpiece) | BBpawnWestAttacksB(BBpiece);
}
U64 king_attacks(U64 BBpiece) {
	return libchess::movegen::king_moves(BBpiece);
}
U64 knight_attacks(U64 BBpiece) {
	return libchess::movegen::knight_moves(BBpiece);
}
U64 rook_attacks(U64 BBpiece, U64 BBocc) {
	return libchess::movegen::rook_moves(BBpiece, BBocc);
}
U64 bishop_attacks(U64 BBpiece, U64 BBocc) {
	return libchess::movegen::bishop_moves(BBpiece, BBocc);
}
U64 queen_attacks(U64 BBpiece, U64 BBocc) {
	return libchess::movegen::queen_moves(BBpiece, BBocc);
}
U64 BBkingattacks(U64 BBpiece) {
	// get king attack squares
	U64 BBattacks = eastOne(BBpiece) | westOne(BBpiece); // east and west one
	BBpiece |= BBattacks; // set piece BB to attacks
	BBattacks |= northOne(BBpiece) | southOne(BBpiece); // north, south, nw, ne, sw, se one
	return BBattacks;
}
U64 BBknightattacks(U64 BBpiece) {
	assert(__builtin_popcountll(BBpiece) >= 0);
	U64 BBattacks = noNoWe(BBpiece);
	BBattacks |= noNoEa(BBpiece);
	BBattacks |= noEaEa(BBpiece);
	BBattacks |= soEaEa(BBpiece);
	BBattacks |= soSoEa(BBpiece);
	BBattacks |= soSoWe(BBpiece);
	BBattacks |= soWeWe(BBpiece);
	BBattacks |= noWeWe(BBpiece);
	return BBattacks;
}