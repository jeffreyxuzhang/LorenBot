#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include <cmath>
#include <memory>

class Piece {
protected:
	int team;
	int curX;
	int curY;
	bool hasMoved;
	std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard;
	char pieceChar;
public:
	//ctor
	Piece(int team, bool hasMoved, std::vector<std::vector<std::shared_ptr<Piece>>>* cBoard, char piece, int x, int y);
	// pure virtual class for Piece
	virtual bool move(int x, int y) = 0;
public:
	char getPieceChar();
	int getTeam();
	std::vector<std::vector<std::shared_ptr<Piece>>>* getBoard();
	bool getMoved();
	void setMoved(bool moved);
};
#endif
