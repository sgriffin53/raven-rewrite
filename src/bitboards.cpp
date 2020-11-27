#include "position.hpp"
#include "bitboards.hpp"

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