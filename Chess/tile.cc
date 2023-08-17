#include "tile.h"

Tile::Tile(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : Piece(team, hasMoved, cBoard, pieceChar, x, y){}

bool Tile::move(int x, int y) {
	return true;
}
