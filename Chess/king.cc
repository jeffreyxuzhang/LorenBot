#include "king.h"

King::King(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : Piece(team, hasMoved, cBoard, pieceChar, x, y) {}

void King::castle(Rook* r) {}

bool King::move(int x, int y) {
	// checks if tile at (x, y) is on the Board;
	if ((x < 0 || y < 0 || x >= 8 || y >= 8)) {
		return false;
	}

	// checks if tile at (x, y) has a piece on the same team
	if (team == (*cBoard)[y][x]->getTeam() && (*cBoard)[y][x]->getPieceChar() != 't') {
		return false;
	}
	
	// checks if tile at (x, y) is a valid king move
	if (std::max(std::abs(curX - x), std::abs(curY - y)) > 1) {
		return false;
	}

	return true;

}




