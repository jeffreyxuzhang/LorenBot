#include "piece.h"

Piece::Piece(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char pieceChar, int x, int y) : team{ team }, hasMoved{ hasMoved }, cBoard{ cBoard }, pieceChar{ pieceChar }, curX{ x }, curY{ y }{}

char Piece::getPieceChar() {
	return pieceChar;
}

int Piece::getTeam() {
	return team;
}

std::vector<std::vector<std::shared_ptr<Piece>>>* Piece::getBoard() {
	return cBoard;
}

bool Piece::getMoved() {
	return hasMoved;
}
void Piece::setMoved(bool moved) {
	hasMoved = moved;
}
