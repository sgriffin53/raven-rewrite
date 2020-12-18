#include "position.hpp"
#include "bitboards.hpp"

U64 BBpasserLookup[2][64];
U64 BBpawnshieldLookup[2][64];

U64 northOne(U64 BB) {
	return BB << 8;
}
U64 southOne(U64 BB) {
	return BB >> 8;
}
U64 westOne(U64 BB) {
	return (BB >> 1) & ~BBfileH;
	//else return 0ULL;
}
U64 eastOne(U64 BB) {
	return (BB << 1) & ~BBfileA;
}

U64 noNoEa(U64 BB) {return (BB << 17) & ~BBfileA;}
U64 noEaEa(U64 BB) {return (BB << 10) & ~BBfileAB;}
U64 soEaEa(U64 BB) {return (BB >>  6) & ~BBfileAB;}
U64 soSoEa(U64 BB) {return (BB >> 15) & ~BBfileA;}
U64 noNoWe(U64 BB) {return (BB << 15) & ~BBfileH;}
U64 noWeWe(U64 BB) {return (BB <<  6) & ~BBfileGH;}
U64 soWeWe(U64 BB) {return (BB >> 10) & ~BBfileGH;}
U64 soSoWe(U64 BB) {return (BB >> 17) & ~BBfileH;}

U64 soEaOne(U64 BB) {
	return (BB >> 7) & ~BBfileA;
}
U64 soWeOne(U64 BB) {
	return (BB >> 9) & ~BBfileH;
}
U64 noEaOne(U64 BB) {
	return (BB << 9) & ~BBfileA;
}
U64 noWeOne(U64 BB) {
	return (BB << 7) & ~BBfileH;
}

void genLookups() {
	for (int i = 0; i < 64;i++) {
		
		// generate passed pawn lookups
		
		// white
		
		int rank = getrank(i);
		U64 BBpiece = (1ULL << i);
		U64 BBmidsquare = (1ULL << i);
		U64 BBchecksquares = 0ULL;
		
		while (rank < 6) {
			BBchecksquares |= noWeOne(BBmidsquare);
			BBchecksquares |= northOne(BBmidsquare);
			BBchecksquares |= noEaOne(BBmidsquare);
			BBmidsquare = northOne(BBmidsquare);
			rank++;
		}
		BBpasserLookup[WHITE][i] = BBchecksquares;
		
		// black
		
		BBmidsquare = (1ULL << i);
		BBchecksquares = 0ULL;
		rank = getrank(i);
		while (rank > 1) {
			BBchecksquares |= soWeOne(BBmidsquare);
			BBchecksquares |= southOne(BBmidsquare);
			BBchecksquares |= soEaOne(BBmidsquare);
			BBmidsquare = southOne(BBmidsquare);
			rank--;
		}
		BBpasserLookup[BLACK][i] = BBchecksquares;
		
		// king fills
		
		// pawn shields
		
		U64 BBpawnshield = noWeOne(1ULL << i) | northOne(1ULL << i) | noEaOne(1ULL << i);
		BBpawnshield |= northOne(BBpawnshield);
		BBpawnshieldLookup[WHITE][i] = BBpawnshield;
		
		BBpawnshield = soWeOne(1ULL << i) | southOne(1ULL << i) | soEaOne(1ULL << i);
		BBpawnshield |= southOne(BBpawnshield);
		BBpawnshieldLookup[BLACK][i] = BBpawnshield;
	}
}