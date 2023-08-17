#ifndef PAWN_H
#define PAWN_H
#include "piece.h"
#include "queen.h"
#include "knight.h"
#include "bishop.h"
#include "rook.h"
class Pawn : public Piece {
public:
	Pawn(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	bool move(int x, int y) override;
};
#endif
