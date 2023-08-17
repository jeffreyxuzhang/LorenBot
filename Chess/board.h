#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <string>
#include "pawn.h"
#include "queen.h"
#include "knight.h"
#include "bishop.h"
#include "rook.h"
#include "king.h"
#include "piece.h"
#include "window.h"
#include <memory>
#include "scoreboard.h"

class Board {
	std::vector<std::vector<std::shared_ptr<Piece>>> pieces;
	// 1 - w, -1 - b
	int currPlayer;
	// true = human, false = computer.
	bool player1;
	bool player2;
	int cpuDif1;
	int cpuDif2;
	std::vector<int> bkpos;
	std::vector<int> wkpos;
	std::vector<int> passantPos;
	// Graphical Display
	friend class Computer;

public:
	Board();
	Board(const Board& other);
	void setPlayers(std::string p1, std::string p2);
	void place(char pieceChar, int x, int y, bool moved);
	bool isCheck(int x, int y, int team) const;
	bool checkmate();
	bool stalemate();
	std::vector<std::vector<std::shared_ptr<Piece>>> getPieces() const;
	void printBoard() const;
	void displayConsts(std::shared_ptr<Xwindow> xw);
	void displayPieces(std::shared_ptr<Xwindow> xw);
	void displayMessages(std::shared_ptr<Xwindow> xw, bool running, bool resigned);
	int getCurrPlayer();
	void setCurrPlayer(int p);
	bool setupCheck();
	void setDefault();
	bool makeMove(int x1, int y1, int x2, int y2);
	std::vector<std::vector<int>> allLegalMoves();
	std::vector<int> getWkPos();
	std::vector<int> getBkPos();
	bool castleCheck(int x1, int y1, int x2, int y2);
	bool passantCheck(int x1, int y1, int x2, int y2);
	bool checkForPromote(int x, int y);
	bool isWhiteHuman();
	bool isBlackHuman();
	void undoCastle(int x1, int y1, int x2, int y2);
	void undoPassant(int x1, int y1, int x2, int y2);
};

#endif
