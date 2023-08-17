#ifndef KNIGHT_H
#define KNIGHT_H
#include "piece.h"

class Knight : public Piece {
public:
	Knight(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	bool move(int x, int y) override;
};
#endif
