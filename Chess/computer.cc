#include "computer.h"
#include <iostream>

std::shared_ptr<Board> Computer::theBoard;

void Computer::setBoard(std::shared_ptr<Board> b) {
	theBoard = b;
}

std::vector<int> Computer::computeMove() {
	int team = theBoard->currPlayer;
	int diff = team == 1 ? theBoard->cpuDif1 : theBoard->cpuDif2;
	switch (diff) {
	case 1:
		return computeLevel1();
	case 2:
		return computeLevel2();
	case 3:
		return computeLevel3();
	default:
		return computeLevel4(diff - 3);
	}
}

std::vector<int> Computer::computeLevel1() {
	std::vector<std::vector<int>> allMoves = theBoard->allLegalMoves();
	return allMoves[rand() % allMoves.size()];
}

std::vector<int> Computer::computeLevel2() {
	std::vector<std::vector<int>> allMoves = theBoard->allLegalMoves();
	std::vector<std::vector<int>> captureMoves;
	std::vector<std::vector<int>> checkMoves;
	for (int i = 0; i < allMoves.size(); i++) {
		std::vector<int> currMove = allMoves[i];
		std::shared_ptr<Piece> destPiece = theBoard->pieces[currMove[3]][currMove[2]];
		std::shared_ptr<Piece> origPiece = theBoard->pieces[currMove[1]][currMove[0]];
		if (destPiece->getPieceChar() != 't' && destPiece->getTeam() != theBoard->currPlayer) {
			captureMoves.emplace_back(currMove);
		}
		else {
			bool didPassant = false;
			bool didCastle = false;
			if (theBoard->passantCheck(currMove[0], currMove[1], currMove[2], currMove[3])) {
				didPassant = true;
				captureMoves.emplace_back(currMove);
				if (theBoard->currPlayer == 1 && theBoard->isCheck(theBoard->bkpos[0], theBoard->bkpos[1], -1) ||
					theBoard->currPlayer == -1 && theBoard->isCheck(theBoard->wkpos[0], theBoard->wkpos[1], 1)) {
					checkMoves.emplace_back(currMove);
				}
			}
			else if (theBoard->castleCheck(currMove[0], currMove[1], currMove[2], currMove[3])) {
				didCastle = true;
				if (theBoard->currPlayer == 1 && theBoard->isCheck(theBoard->bkpos[0], theBoard->bkpos[1], -1) ||
					theBoard->currPlayer == -1 && theBoard->isCheck(theBoard->wkpos[0], theBoard->wkpos[1], 1)) {
					checkMoves.emplace_back(currMove);
				}
			}
			else {
				theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currMove[2], currMove[3], true);
				theBoard->place('t', currMove[0], currMove[1], false);
				if (currMove.size() == 5) {
					theBoard->place(currMove[4], currMove[2], currMove[3], true);
				}
				if (theBoard->currPlayer == 1 && theBoard->isCheck(theBoard->bkpos[0], theBoard->bkpos[1], -1) ||
					theBoard->currPlayer == -1 && theBoard->isCheck(theBoard->wkpos[0], theBoard->wkpos[1], 1)) {
					checkMoves.emplace_back(currMove);
				}
			}
			theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currMove[0], currMove[1], origPiece->getMoved());
			theBoard->place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), currMove[2], currMove[3], destPiece->getMoved());
			if (didPassant) {
				theBoard->undoPassant(currMove[0], currMove[1], currMove[2], currMove[3]);
			}
			if (didCastle) {
				theBoard->undoCastle(currMove[0], currMove[1], currMove[2], currMove[3]);
			}
		}
		if (captureMoves.size() > 0) {
			return captureMoves[rand() % captureMoves.size()];
		}
		if (checkMoves.size() > 0) {
			return checkMoves[rand() % checkMoves.size()];
		}
		return allMoves[rand() % allMoves.size()];
	}
}

std::vector<int> Computer::computeLevel3() {
	std::vector<std::vector<int>> allMoves = theBoard->allLegalMoves();
	std::vector<std::vector<int>> valueMoves;
	for (int i = 0; i < allMoves.size(); i++) {
		std::vector<int> currMove = allMoves[i];
		std::vector<int> newValueMove = currMove;
		newValueMove.emplace_back(0);
		valueMoves.emplace_back(newValueMove);
		std::shared_ptr<Piece> destPiece = theBoard->pieces[currMove[3]][currMove[2]];
		std::shared_ptr<Piece> origPiece = theBoard->pieces[currMove[1]][currMove[0]];
		bool didPassant = false;
		bool didCastle = false;
		if (theBoard->castleCheck(currMove[0], currMove[1], currMove[2], currMove[3])) {
			didCastle = true;
		}
		else if (theBoard->passantCheck(currMove[0], currMove[1], currMove[2], currMove[3])) {
			didPassant = true;
		}
		else {
			theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currMove[2], currMove[3], true);
			theBoard->place('t', currMove[0], currMove[1], false);
			if (currMove.size() == 5) {
				theBoard->place(currMove[4], currMove[2], currMove[3], true);
			}
		}
		theBoard->currPlayer = -theBoard->currPlayer;
		std::vector<std::vector<int>> oppMoves = theBoard->allLegalMoves();
		bool captured = false;
		for (int j = 0; j < oppMoves.size(); j++) {
			std::vector<int> currOppMove = oppMoves[j];
			if (theBoard->passantCheck(currOppMove[0], currOppMove[1], currOppMove[2], currOppMove[3])) {
				captured = true;
				theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currOppMove[0], currOppMove[1], origPiece->getMoved());
				theBoard->place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), currOppMove[2], currOppMove[3], destPiece->getMoved());
				theBoard->undoPassant(currOppMove[0], currOppMove[1], currOppMove[2], currOppMove[3]);
				break;
			}
			std::shared_ptr<Piece> destPiece2 = theBoard->pieces[currOppMove[3]][currOppMove[2]];
			if (destPiece2->getTeam() != theBoard->currPlayer && destPiece2->getPieceChar() != 't') {
				captured = true;
				break;
			}
		}
		if (!captured) {
			valueMoves.back().back() += 4;
		}
		theBoard->currPlayer = -theBoard->currPlayer;
		if (destPiece->getPieceChar() != 't' && destPiece->getTeam() != theBoard->currPlayer) {
			valueMoves.back().back() += 2;
		}
		if (theBoard->currPlayer == 1 && theBoard->isCheck(theBoard->bkpos[0], theBoard->bkpos[1], -1) ||
			theBoard->currPlayer == -1 && theBoard->isCheck(theBoard->wkpos[0], theBoard->wkpos[1], 1)) {
			valueMoves.back().back() += 1;
		}
		theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currMove[0], currMove[1], origPiece->getMoved());
		theBoard->place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), currMove[2], currMove[3], destPiece->getMoved());
		if (didPassant) {
			theBoard->undoPassant(currMove[0], currMove[1], currMove[2], currMove[3]);
		}
		if (didCastle) {
			theBoard->undoCastle(currMove[0], currMove[1], currMove[2], currMove[3]);
		}
	}
	std::vector<std::vector<int>> maxMoves;
	int max = 0;
	for (int i = 0; i < valueMoves.size(); i++) {
		std::vector<int> currMove = valueMoves[i];
		if (currMove.back() > max) {
			std::vector<int> moveToAdd = { currMove.begin(), currMove.end() - 1 };
			maxMoves = { moveToAdd };
			max = currMove.back();
		}
		else if (currMove.back() == max) {
			std::vector<int> moveToAdd = { currMove.begin(), currMove.end() - 1 };
			maxMoves.emplace_back(moveToAdd);
		}
	}
	srand((unsigned)time(0));
	return maxMoves[rand() % maxMoves.size()];
}

int numMoves = 0;

int Computer::eval() {
	int sum = 0;
	int c = 0;
	int player = theBoard->currPlayer;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (tolower(theBoard->pieces[j][i]->getPieceChar())) {
			case 'q':
				sum += theBoard->pieces[j][i]->getTeam() * 90;
				break;
			case 'r':
				sum += theBoard->pieces[j][i]->getTeam() * 50;
				break;
			case 'b':
				sum += theBoard->pieces[j][i]->getTeam() * 30;
				break;
			case 'n':
				sum += theBoard->pieces[j][i]->getTeam() * 30;
				break;
			case 'p':
				sum += theBoard->pieces[j][i]->getTeam() * 10;
				break;
			case 't':
				c++;
				break;
			}
		}
	}
	if (theBoard->isCheck(theBoard->bkpos[0], theBoard->bkpos[1], -1)) {
		sum += 45;
	}

	if (theBoard->isCheck(theBoard->wkpos[0], theBoard->wkpos[1], 1)) {
		sum -= 45;
	}

	theBoard->currPlayer = 1;
	if (theBoard->checkmate()) {
		sum -= 6969;
	}

	theBoard->currPlayer = -1;
	if (theBoard->checkmate()) {
		sum += 6969;
	}
	theBoard->currPlayer = player;
	return sum;
}

int Computer::minimax(int currDepth, int alpha, int beta) {
	if (currDepth == 1) {
		numMoves++;
		return eval();
	}
	else {
		if (theBoard->currPlayer == -1) {
			int opMax = -696969;
			std::vector<int> maxOpMove;
			theBoard->currPlayer = -theBoard->currPlayer;
			std::vector<std::vector<int>> allMoves = theBoard->allLegalMoves();
			allMoves = orderMoves(allMoves);
			for (int j = 0; j < allMoves.size(); j++) {
				std::vector<int> currOpMove = allMoves[j];
				std::shared_ptr<Piece> destOpPiece = theBoard->pieces[currOpMove[3]][currOpMove[2]];
				std::shared_ptr<Piece> origOpPiece = theBoard->pieces[currOpMove[1]][currOpMove[0]];
				bool didPassant = false;
				bool didCastle = false;
				if (theBoard->castleCheck(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3])) {
					didCastle = true;
				}
				else if (theBoard->passantCheck(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3])) {
					didPassant = true;
				}
				else {
					theBoard->place(origOpPiece->getTeam() == 1 ? toupper(origOpPiece->getPieceChar()) : origOpPiece->getPieceChar(), currOpMove[2], currOpMove[3], true);
					theBoard->place('t', currOpMove[0], currOpMove[1], false);
					if (currOpMove.size() == 5) {
						theBoard->place(currOpMove[4], currOpMove[2], currOpMove[3], true);
					}
				}
				int opCurr = minimax(currDepth - 1, alpha, beta);

				theBoard->place(origOpPiece->getTeam() == 1 ? toupper(origOpPiece->getPieceChar()) : origOpPiece->getPieceChar(), currOpMove[0], currOpMove[1], origOpPiece->getMoved());
				theBoard->place(destOpPiece->getTeam() == 1 ? toupper(destOpPiece->getPieceChar()) : destOpPiece->getPieceChar(), currOpMove[2], currOpMove[3], destOpPiece->getMoved());
				if (didPassant) {
					theBoard->undoPassant(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3]);
				}
				if (didCastle) {
					theBoard->undoCastle(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3]);
				}

				if (opCurr > opMax) {
					opMax = opCurr;
					maxOpMove = currOpMove;
				}
				if (opMax >= beta) {
					break;
				}
				if (opMax > alpha) {
					alpha = opCurr;
				}
			}
			theBoard->currPlayer = -theBoard->currPlayer;
			//std::cout << maxOpMove[0] << " " << maxOpMove[1] << " " << maxOpMove[2] << " " << maxOpMove[3] << std::endl;
			return opMax;
		}
		else if (theBoard->currPlayer == 1) {
			int opMin = 696969;
			std::vector<int> minOpMove;
			theBoard->currPlayer = -theBoard->currPlayer;
			std::vector<std::vector<int>> allMoves = theBoard->allLegalMoves();
			allMoves = orderMoves(allMoves);
			for (int j = 0; j < allMoves.size(); j++) {
				std::vector<int> currOpMove = allMoves[j];
				std::shared_ptr<Piece> destOpPiece = theBoard->pieces[currOpMove[3]][currOpMove[2]];
				std::shared_ptr<Piece> origOpPiece = theBoard->pieces[currOpMove[1]][currOpMove[0]];
				bool didPassant = false;
				bool didCastle = false;
				if (theBoard->castleCheck(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3])) {
					didCastle = true;
				}
				else if (theBoard->passantCheck(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3])) {
					didPassant = true;
				}
				else {
					theBoard->place(origOpPiece->getTeam() == 1 ? toupper(origOpPiece->getPieceChar()) : origOpPiece->getPieceChar(), currOpMove[2], currOpMove[3], true);
					theBoard->place('t', currOpMove[0], currOpMove[1], false);
					if (currOpMove.size() == 5) {
						theBoard->place(currOpMove[4], currOpMove[2], currOpMove[3], true);
					}
				}
				int opCurr = minimax(currDepth - 1, alpha, beta);

				theBoard->place(origOpPiece->getTeam() == 1 ? toupper(origOpPiece->getPieceChar()) : origOpPiece->getPieceChar(), currOpMove[0], currOpMove[1], origOpPiece->getMoved());
				theBoard->place(destOpPiece->getTeam() == 1 ? toupper(destOpPiece->getPieceChar()) : destOpPiece->getPieceChar(), currOpMove[2], currOpMove[3], destOpPiece->getMoved());
				if (didPassant) {
					theBoard->undoPassant(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3]);
				}
				if (didCastle) {
					theBoard->undoCastle(currOpMove[0], currOpMove[1], currOpMove[2], currOpMove[3]);
				}

				if (opCurr < opMin) {
					opMin = opCurr;
					minOpMove = currOpMove;
				}
				if (opMin <= alpha) {
					break;
				}
				if (opMin < beta) {
					beta = opCurr;
				}
			}
			theBoard->currPlayer = -theBoard->currPlayer;
			//std::cout << minOpMove[0] << " " << minOpMove[1] << " " <<  minOpMove[2] << " " << minOpMove[3] << std::endl;
			return opMin;
		}
	}
}

std::vector<int> Computer::computeLevel4(int depth) {
	int alpha = -6969;
	int beta = 6969;
	int startTime = std::time(0);
	std::cout << "Thinking";
	int max = -696969;
	std::vector<int> maxMove;
	int min = 696969;
	std::vector<int> minMove;
	int curr = 0;
	std::vector<std::vector<int>> allMoves = theBoard->allLegalMoves();
	allMoves = orderMoves(allMoves);
	numMoves = 0;
	for (int i = 0; i < allMoves.size(); i++) {
		std::vector<int> currMove = allMoves[i];
		//std::cout << currMove[0] << " " << currMove[1] << " " << currMove[2] << " " << currMove[3] << std::endl;
		std::shared_ptr<Piece> destPiece = theBoard->pieces[currMove[3]][currMove[2]];
		std::shared_ptr<Piece> origPiece = theBoard->pieces[currMove[1]][currMove[0]];
		bool didPassant = false;
		bool didCastle = false;
		if (theBoard->castleCheck(currMove[0], currMove[1], currMove[2], currMove[3])) {
			didCastle = true;
		}
		else if (theBoard->passantCheck(currMove[0], currMove[1], currMove[2], currMove[3])) {
			didPassant = true;
		}
		else {
			theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currMove[2], currMove[3], true);
			theBoard->place('t', currMove[0], currMove[1], false);
			if (currMove.size() == 5) {
				theBoard->place(currMove[4], currMove[2], currMove[3], true);
			}
		}
		curr = minimax(depth, alpha, beta);
		//std::cout << curr << std::endl;
		theBoard->place(origPiece->getTeam() == 1 ? toupper(origPiece->getPieceChar()) : origPiece->getPieceChar(), currMove[0], currMove[1], origPiece->getMoved());
		theBoard->place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), currMove[2], currMove[3], destPiece->getMoved());
		if (didPassant) {
			theBoard->undoPassant(currMove[0], currMove[1], currMove[2], currMove[3]);
		}
		if (didCastle) {
			theBoard->undoCastle(currMove[0], currMove[1], currMove[2], currMove[3]);
		}
		if (theBoard->currPlayer == 1) {
			if (curr > max) {
				max = curr;
				maxMove = currMove;
			}
			if (max >= beta) {
				break;
			}
			if (max > alpha) {
				alpha = curr;
			}
		}

		if (theBoard->currPlayer == -1) {
			if (curr < min) {
				min = curr;
				minMove = currMove;
			}
			if (min <= alpha) {
				break;
			}
			if (min < beta) {
				beta = curr;
			}
		}
		std::cout << ".";
	}
	std::cout << std::endl;
	std::cout << "Evaluated " << numMoves << " possibilities in "  << std::time(0) - startTime << " seconds." << std::endl;
	if (theBoard->currPlayer == 1) {
		maxMove = { maxMove.begin(), maxMove.end() - 1 };
		return maxMove;
	}
	else {
		minMove = { minMove.begin(), minMove.end() - 1 };
		return minMove;
	}
}

std::vector<std::vector<int>> Computer::orderMoves(std::vector<std::vector<int>> allMoves) {
	std::vector<std::vector<int>> orderedMoves;
	int team = theBoard->currPlayer;
	for (int i = 0; i < allMoves.size(); i++) {
		std::vector<int> currMove = allMoves[i];
		currMove.emplace_back(0);
		std::shared_ptr<Piece> destPiece = theBoard->pieces[currMove[3]][currMove[2]];
		std::shared_ptr<Piece> origPiece = theBoard->pieces[currMove[1]][currMove[0]];
		int origVal = 0;
		int destVal = 0;
		switch (origPiece->getPieceChar()) {
		case 'p':
			origVal = 10;
			break;
		case 'b':
			origVal = 30;
			break;
		case 'n':
			origVal = 30;
			break;
		case 'r':
			origVal = 50;
			break;
		case 'q':
			origVal = 90;
			break;
		}
		if (destPiece->getPieceChar() != 't' && destPiece->getTeam() != theBoard->currPlayer) {
			switch (destPiece->getPieceChar()) {
			case 'p':
				destVal = 10;
				break;
			case 'b':
				destVal = 30;
				break;
			case 'n':
				origVal = 30;
				break;
			case 'r':
				destVal = 50;
				break;
			case 'q':
				destVal = 90;
				break;
			}
			currMove.back() += 10 + (origVal - destVal);
		}
		if (currMove[3] + team > 0 && currMove[3] + team < 7 && 
			(currMove[2] - 1 >= 0 &&
			theBoard->pieces[currMove[3] + team][currMove[2] - 1]->getPieceChar() == 'p' &&
			theBoard->pieces[currMove[3] + team][currMove[2] - 1]->getTeam() != team ||
			currMove[2] + 1 <= 0 &&
			theBoard->pieces[currMove[3] + team][currMove[2] + 1]->getPieceChar() == 'p' &&
			theBoard->pieces[currMove[3] + team][currMove[2] + 1]->getTeam() != team)) {
			currMove.back() -= origVal;
		}
		orderedMoves.emplace_back(currMove);
	}
	std::sort(orderedMoves.begin(), orderedMoves.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
		return a.back() > b.back();
		});
	//return allMoves;
	return orderedMoves;
}
