#ifndef COMPUTER_H
#define COMPUTER_H
#include "board.h"
#include <ctime>

class Computer {
	static std::shared_ptr<Board> theBoard;
public:
	std::vector<int> static computeMove();
	std::vector<int> static computeLevel1();
	std::vector<int> static computeLevel2();
	std::vector<int> static computeLevel3();
	std::vector<int> static computeLevel4(int depth);
	int static eval();
	int static minimax(int currDepth, int alpha, int beta);
	void static setBoard(std::shared_ptr<Board> b);
	std::vector<std::vector<int>> static orderMoves(std::vector<std::vector<int>> allMoves);
};
#endif
