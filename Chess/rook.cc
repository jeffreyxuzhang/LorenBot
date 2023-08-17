#include "piece.h"
#include "rook.h"
#include <algorithm>
#include <iostream>

Rook::Rook(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : Piece(team, hasMoved, cBoard, pieceChar, x, y) {}

bool Rook::move(int x, int y) {
	// checks if tile at (x, y) is on the Board;
	if ((x < 0 || y < 0 || x >= 8 || y >= 8)) {
		return false;
	}

	// checks if tile at (x, y) has a piece on the same team
	if (team == (*cBoard)[y][x]->getTeam() && (*cBoard)[y][x]->getPieceChar() != 't') {
		return false;
	}

	// moving vertically
	if (curX == x) {
		for (int i = std::min(curY, y) + 1; i < std::max(curY, y); i++) {
			if ((*cBoard)[i][x]->getPieceChar() != 't') {
				return false;
			}
		}
	}
	// moving horizontally
	else if (curY == y) { 
		for (int i = std::min(curX, x) + 1; i < std::max(curX, x); i++) {
			if ((*cBoard)[y][i]->getPieceChar() != 't') {
				return false;
			}
		}
	}
	else {
		return false;
	}
	return true;

}
