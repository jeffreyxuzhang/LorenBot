#ifndef KING_H
#define KING_H
#include "piece.h"
#include <algorithm>


class Rook;
class King : public Piece {
public:
	King(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	void castle(Rook* r);
	bool move(int x, int y) override;
};
#endif
