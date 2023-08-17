#ifndef TILE_H
#define TILE_H
#include "piece.h"

class Tile : public Piece {

public:
	Tile(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	bool move(int x, int y) override;
};
#endif
