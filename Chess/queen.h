#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"

class Queen : public Piece {
public:
	Queen(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	bool move(int x, int y) override;
};
#endif
