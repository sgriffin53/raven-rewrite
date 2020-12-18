#ifndef LIBCHESS_MOVEGEN_HPP
#define LIBCHESS_MOVEGEN_HPP

#include "bitboard.hpp"
#include "square.hpp"
#include "../../../position.hpp"

namespace libchess::movegen {

U64 knight_moves(const int sq);
U64 bishop_moves(const int sq, const U64 &occ);
U64 rook_moves(const int sq, const U64 &occ);
U64 queen_moves(const int sq, const U64 &occ);
U64 king_moves(const int sq);

}  // namespace libchess::movegen

#endif
