#include "bishop.h"


Bishop::Bishop(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : Piece(team, hasMoved, cBoard, pieceChar, x, y) {}

bool Bishop::move(int x, int y) {
	int row = y;
	int col = x;
	int curRow = curY;
	int curCol = curX;
	int rowDif = row - curRow;
	int colDif = col - curCol;

	// checks if tile at (x, y) is on the Board;
	if ((x < 0 || y < 0 || x >= 8 || y >= 8)) {
		return false;
	}

	// checks if tile at (x, y) has a piece on the same team
	if (team == (*cBoard)[y][x]->getTeam() && (*cBoard)[y][x]->getPieceChar() != 't') {
		return false;
	}

	// checks for diagonal movement
	if (std::abs(rowDif) != std::abs(colDif)) {
		return false;
	}

	for (int i = 1; i < std::abs(rowDif); i++) {
		if ((*cBoard)[curY + rowDif / std::abs(rowDif) * i][curX + colDif / std::abs(colDif) * i]->getPieceChar() != 't') {
			return false;
		}
	}

	return true;
}

