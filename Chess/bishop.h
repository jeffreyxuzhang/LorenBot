#ifndef BISHOP_H
#define BISHOP_H
#include "piece.h"

class Bishop : public Piece {
public:
	Bishop(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	bool move(int x, int y) override;
};
#endif
