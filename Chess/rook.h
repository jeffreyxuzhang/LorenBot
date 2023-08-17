#ifndef ROOK_H
#define ROOK_H
#include "piece.h"

class Rook : public Piece {
public:
	Rook(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	bool move(int x, int y) override;
};
#endif
