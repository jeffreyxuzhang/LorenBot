#include "pawn.h"
#include <vector>
#include <algorithm>

Pawn::Pawn(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : Piece(team, hasMoved, cBoard, pieceChar, x, y) {}

bool Pawn::move(int x, int y) {
	// checks if tile at (x, y) is on the Board;
	if ((x < 0 || y < 0 || x >= 8 || y >= 8)) {
		return false;
	}

	// checks if tile at (x, y) has a piece on the same team
	if (team == (*cBoard)[y][x]->getTeam() && (*cBoard)[y][x]->getPieceChar() != 't') {
		return false;
	}

	// moving forwards
	if (curX == x && (*cBoard)[y][x]->getPieceChar() == 't') {
		if (!hasMoved && (team == 1 && curY == 1 || team == -1 && curY == 6)) {
			if ((y - curY) * team != 1 && (y - curY) * team != 2) {
				return false;
			}
		}
		else {
			if ((y - curY) * team != 1) {
				return false;
			}
		}
		return true;
	}
	else { //capturing diagonally
		if ((y - curY) * team == 1 && abs(x - curX) == 1) {
			if (-team == (*cBoard)[y][x]->getTeam() && (*cBoard)[y][x]->getPieceChar() != 't') {
				return true;
			}
		}
		return false;
	}
}

