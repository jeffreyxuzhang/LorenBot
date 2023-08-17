#include "knight.h"

Knight::Knight(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : Piece(team, hasMoved, cBoard, pieceChar, x, y) {}

bool Knight::move(int x, int y) {
	int row = y;
	int col = x;
	int curRow = curY;
	int curCol = curX;
	int rowDif = std::abs(row - curRow);
	int colDif = std::abs(col - curCol);

	// checks if tile at (x, y) is on the Board;
	if ((x < 0 || y < 0 || x >= 8 || y >= 8)) {
		return false;
	}

	// checks if tile at (x, y) has a piece on the same team
	if (team == (*cBoard)[y][x]->getTeam() && (*cBoard)[y][x]->getPieceChar() != 't') {
		return false;
	}

	// checks if tile at (x, y) is a valid knight move
	if (!(std::max(rowDif, colDif) == 2 && std::min(rowDif, colDif) == 1)) {
		return false;
	}

	return true;
}




