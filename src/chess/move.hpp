#ifndef MOVE_H
#define MOVE_H

#include <cassert>
#include <string>

#define MAX_MOVES 2048

enum MOVETYPE { NORMAL, DOUBLE, CAPTURE, EN_PASSANT, PROMO, PROMO_CAPTURE, KSC, QSC };

class Move {
  public:
	Move() : m_from{}, m_to{}, m_prom{}, m_piece{}, m_cappiece{}, m_type{} {}

	Move(const int from, const int to, const int prom, const int piece, const int cappiece, const int type) : m_from{from}, m_to{to}, m_prom{prom}, m_piece{piece}, m_cappiece{cappiece}, m_type{type} {
		assert(0 <= from && from <= 63);
		assert(0 <= to && to <= 63);
		assert(from != to);
	}

	int from() const { return m_from; }

	int to() const { return m_to; }

	int prom() const { return m_prom; }

	int piece() const { return m_piece; }

	int cappiece() const { return m_cappiece; }

	int type() const { return m_type; }

  private:
	int m_from;
	int m_to;
	int m_prom;
	int m_piece;
	int m_cappiece;
	int m_type;
};

std::string movetostr(Move move);

#endif
