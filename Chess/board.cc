#include "board.h"
#include "tile.h"
#include <iostream>
#include <string>

Board::Board() : currPlayer{ 1 }, passantPos{ -1, -1 } {
	for (int i = 0; i < 8; i++) {
		// new vector
		std::vector<std::shared_ptr<Piece>> rows;
		for (int j = 0; j < 8; j++) {
			std::shared_ptr<Piece> t = std::make_shared<Tile>(1, false, &pieces, 't', j, i);
			rows.emplace_back(t);
		}
		pieces.emplace_back(rows);
	}
}

Board::Board(const Board &other): currPlayer{-other.currPlayer}, player1{other.player1}, player2{other.player2}, cpuDif1{other.cpuDif1}, cpuDif2{other.cpuDif2}, bkpos{other.bkpos}, wkpos{other.wkpos}, passantPos{other.passantPos} {
	for (int i = 0; i < 8; i++) {
		// new vector
		std::vector<std::shared_ptr<Piece>> rows;
		for (int j = 0; j < 8; j++) {
			std::shared_ptr<Piece> t = std::make_shared<Tile>(1, false, &pieces, 't', j, i);
			rows.emplace_back(t);
		}
		pieces.emplace_back(rows);
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			place(other.pieces[i][j]->getTeam() == 1 ? toupper(other.pieces[i][j]->getPieceChar()) : other.pieces[i][j]->getPieceChar(), j, i, other.pieces[i][j]->getMoved());
		}
	}
}

bool Board::isWhiteHuman() {
	return player1;
}

bool Board::isBlackHuman() {
	return player2;
}

void Board::setPlayers(std::string p1, std::string p2) {
	if (p1[0] == 'c') {
		player1 = false;
		cpuDif1 = p1[8] - '0';
	}
	else {
		player1 = true;
	}

	if (p2[0] == 'c') {
		player2 = false;
		cpuDif2 = p2[8] - '0';
	}
	else {
		player2 = true;
	}
}

void Board::place(char pieceChar, int x, int y, bool moved) {
	switch (tolower(pieceChar)) {
	case 'k':
		pieces[y][x] = std::make_shared<King>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		if (pieceChar < 'a') {
			wkpos = { x, y };
		}
		else {
			bkpos = { x, y };
		}
		break;
	case 'q':
		pieces[y][x] = std::make_shared<Queen>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		break;
	case 'n':
		pieces[y][x] = std::make_shared<Knight>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		break;
	case 'b':
		pieces[y][x] = std::make_shared<Bishop>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		break;
	case 'r':
		pieces[y][x] = std::make_shared<Rook>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		break;
	case 'p':
		pieces[y][x] = std::make_shared<Pawn>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		if (pieceChar < 'a' && pieces[y-1][x]->getPieceChar() == 't' && pieces[y][x]->getPieceChar() == 't' ||
			pieceChar >= 'a' && pieces[y+1][x]->getPieceChar() == 't' && pieces[y][x]->getPieceChar() == 't') {
			passantPos = { x, y };
		}
		else {
			passantPos = { -1, -1 };
		}
		break;
	case 't':
		pieces[y][x] = std::make_shared<Tile>(pieceChar < 'a' ? 1 : -1, moved, & pieces, tolower(pieceChar), x, y);
		break;
	}
}

bool Board::isCheck(int x, int y, int team) const{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j]->getPieceChar() != 't' && pieces[i][j]->getTeam() != team) {
				if (pieces[i][j]->move(x, y)) {
					return true;
				}
			}
		}
	}
	return false;
}

std::vector<std::vector<int>> Board::allLegalMoves() {
	std::vector<std::vector<int>> allMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j]->getPieceChar() != 't' && pieces[i][j]->getTeam() == currPlayer) {
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						std::shared_ptr<Piece> destPiece = pieces[k][l];
						if (castleCheck(j, i, l, k)) {
							std::vector<int> newMove = { j, i, l, k };
							allMoves.emplace_back(newMove);
							place(pieces[k][l]->getTeam() == 1 ? toupper(pieces[k][l]->getPieceChar()) : pieces[k][l]->getPieceChar(), j, i, pieces[k][l]->getMoved());
							place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), l, k, destPiece->getMoved());
							undoCastle(j, i, l, k);
						}
						else if (passantCheck(j, i, l, k)) {
							std::vector<int> newMove = { j, i, l, k };
							allMoves.emplace_back(newMove);
							place(pieces[k][l]->getTeam() == 1 ? toupper(pieces[k][l]->getPieceChar()) : pieces[k][l]->getPieceChar(), j, i, pieces[k][l]->getMoved());
							place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), l, k, destPiece->getMoved());
							undoPassant(j, i, l, k);
						}
						else if (pieces[i][j]->move(l, k)) {
							place(pieces[i][j]->getTeam() == 1 ? toupper(pieces[i][j]->getPieceChar()) : pieces[i][j]->getPieceChar(), l, k, pieces[i][j]->getMoved());
							place('t', j, i, false);
							if (currPlayer == 1) {
								if (!isCheck(wkpos[0], wkpos[1], currPlayer)) {
									std::vector<int> newMove = { j, i, l, k };
									if (checkForPromote(l, k)) {
										newMove = { j, i, l, k, 'Q' };
										allMoves.emplace_back(newMove);
										newMove = { j, i, l, k, 'B' };
										allMoves.emplace_back(newMove);
										newMove = { j, i, l, k, 'K' };
										allMoves.emplace_back(newMove);
										newMove = { j, i, l, k, 'R' };
									} 
									allMoves.emplace_back(newMove);
								}
							}
							else {
								if (!isCheck(bkpos[0], bkpos[1], currPlayer)) {
									std::vector<int> newMove = { j, i, l, k };
									if (checkForPromote(l, k)) {
										newMove = { j, i, l, k, 'q' };
										allMoves.emplace_back(newMove);
										newMove = { j, i, l, k, 'b' };
										allMoves.emplace_back(newMove);
										newMove = { j, i, l, k, 'k' };
										allMoves.emplace_back(newMove);
										newMove = { j, i, l, k, 'r' };
									}
									allMoves.emplace_back(newMove);
								}
							}
							place(pieces[k][l]->getTeam() == 1 ? toupper(pieces[k][l]->getPieceChar()) : pieces[k][l]->getPieceChar(), j, i, pieces[k][l]->getMoved());
							place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), l, k, destPiece->getMoved());
						}
					}
				}
			}
		}
	}
	return allMoves;
}

bool Board::checkmate() {

	if (allLegalMoves().size() > 0) {
		std::vector<std::vector<int>> vec = allLegalMoves();
		return false;
	}
	if (currPlayer == 1) {
		if (isCheck(wkpos[0], wkpos[1], 1)) {
			return true;
		}
	}
	else {
		if (isCheck(bkpos[0], bkpos[1], -1)) {
			return true;
		}
	}
	return false;
}

bool Board::stalemate() {
	int c = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j]->getPieceChar() != 't') {
				c++;
			}
		}
	}
	if (c == 2) {
		return true;
	}
	if (allLegalMoves().size() > 0) {
		return false;
	}
	if (currPlayer == 1) {
		if (isCheck(wkpos[0], wkpos[1], 1)) {
			return false;
		}
	}
	else {
		if (isCheck(wkpos[0], wkpos[1], -1)) {
			return false;
		}
	}
	return true;
}

std::vector<std::vector<std::shared_ptr<Piece>>> Board::getPieces() const {
	return pieces;
}

void Board::printBoard() const {
	for (int i = 0; i < 8; i++) {
		std::cout << 8 - i << " ";
		for (int j = 0; j < 8; j++) {
			if (pieces[7 - i][j]->getPieceChar() == 't') {
				Tile* t = (Tile*)pieces[7 - i][j].get();
				std::cout << (i % 2 != j % 2 ? ' ' : '_');
			}
			else {
				std::cout << (char)(pieces[7 - i][j]->getTeam() == 1 ? toupper(pieces[7 - i][j]->getPieceChar()) : pieces[7 - i][j]->getPieceChar());
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "  abcdefgh" << std::endl;
}


void Board::displayConsts(std::shared_ptr<Xwindow> xw) {
        // background
        xw->fillRectangle(0, 0, 768, 768, Xwindow::PowderBlue);
        // letters and numbers
        for (int i = 0; i < 8; i++) {
                char c = (8 - i) + '0';
                char d = i + 65;
                std::string s(1, c);
                std::string t(1, d);
                xw->drawString(96, 160 + (64 * i), s);
                xw->drawString(160 + (64 * i), 672, t);
        }
}

void Board::displayPieces(std::shared_ptr<Xwindow> xw) {
        // empty board
        for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                        if (i % 2 != j % 2) {
                                xw->fillRectangle(128 + (64 * j), 128 + (64 * i), 64, 64, Xwindow::Brown);
                        }
                        else {
                                xw->fillRectangle(128 + (64 * j), 128 + (64 * i), 64, 64, Xwindow::LightSalmon);
                        }
                }
        }
        // pieces
        for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                        if (pieces[i][j]->getPieceChar() != 't') {
                                if (pieces[i][j]->getTeam() == 1) {
                                        char c = toupper(pieces[i][j]->getPieceChar());
                                        std::string s(1, c);
                                        xw->drawString(160 + (64 * j), 160 + (64 * (7 - i)), s);
                                }
                                else {
                                        char c = pieces[i][j]->getPieceChar();
                                        std::string s(1, c);
                                        xw->drawString(160 + (64 * j), 160 + (64 * (7 - i)), s);
                                }
                        }
                }
        }
}

void Board::displayMessages(std::shared_ptr<Xwindow> xw, bool running, bool resigned) {
        // redraws background
        xw->fillRectangle(250, 0, 250, 125, Xwindow::PowderBlue);
	// displays winner if someone resigns
	if (resigned) {
		if (currPlayer == 1) {
			xw->drawString(360, 96, "Black wins!");
                } else {
                 	xw->drawString(360, 96, "White wins!");
		}
	}
        // displays invalid setup
	else if (!running) {
		bool valid = true;
	        int bk = 0;
	        int wk = 0;
	        for (int i = 0; i < 8; i++) {
               		 for (int j = 0; j < 8; j++) {
                        	if (pieces[i][j]->getPieceChar() == 'k') {
                                	if (pieces[i][j]->getTeam() == 1) {
                                        	wk += 1;
                                	}	
                                	else {
                                        	bk += 1;
                               	 	}
                                	if (isCheck(j, i, pieces[i][j]->getTeam())) {	
						xw->drawString(320, 96, "King in check.");
                                        	xw->drawString(320, 64, "Invalid Setup.");
						valid = false;
                                	}
                        	}
                        	if (pieces[i][j]->getPieceChar() == 'p' && (i == 0 || i == 7)) {
                                	xw->drawString(320, 96, "Pawn in wrong row.");
					xw->drawString(320, 64, "Invalid Setup.");
					valid = false;
                        	}
                	}
        	}
        	if (!(wk == 1 && bk == 1)) {
                	xw->drawString(320, 96, "Invalid # of kings.");
			xw->drawString(320, 64, "Invalid Setup.");
			valid = false;
        	}
		if (valid) {
			xw->drawString(360, 64, "Finished Setup.");
		}
		if (stalemate() && valid) {
			xw->drawString(360, 96, "Stalemate!");
		}
	}
	else {
	//displays checkmate
        	if (checkmate()) {
                	if(currPlayer == 1) {
                        	xw->drawString(320, 96, "Checkmate! Black wins!");
                	}else {
                        	xw->drawString(320, 96, "Checkmate! White wins!");
                	}
        	}
        // displays stalemate
        	else if (stalemate()) {
                	xw->drawString(360, 96, "Stalemate!");
        	}
        // displays White check
        	else if (currPlayer == 1 && isCheck(wkpos[0], wkpos[1], 1)) {
                	xw->drawString(330, 96, "White is in check.");
                	xw->drawString(340, 64, "White's turn");
        	}
        // displays Black check
        	else if (currPlayer == -1 && isCheck(bkpos[0], bkpos[1], -1)) {
                	xw->drawString(330, 96, "Black is in check.");
                	xw->drawString(340, 64, "Black's turn.");
        	}
        	else {
                	if (currPlayer == 1) {
                        	xw->drawString(340, 64, "White's turn");
                	} else {
                        	xw->drawString(340, 64, "Black's turn");
                	}
        	}
	}
}


int Board::getCurrPlayer() {
	return currPlayer;
}

void Board::setCurrPlayer(int p) {
	currPlayer = p;
}

bool Board::setupCheck() {
	int bk = 0;
	int wk = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j]->getPieceChar() == 'k') {
				if (pieces[i][j]->getTeam() == 1) {
					wk += 1;
				}
				else {
					bk += 1;
				}
				if (isCheck(j, i, pieces[i][j]->getTeam())) {
					std::cout << "King at " << j << " " << i << " in check" << std::endl;
					return false;
				}
			}
			if (pieces[i][j]->getPieceChar() == 'p' && (i == 0 || i == 7)) {
				std::cout << "Pawn in " << i << "th row" << std::endl;
				return false;
			}
		}
	}
	if (!(wk == 1 && bk == 1)) {
		std::cout << "Invalid # of kings" << std::endl;
		return false;
	}
	return true;
}

void Board::setDefault() {
	for (int i = 0; i < 8; i++) {
		place('P', i, 1, false);
		place('p', i, 6, false);
	}
	place('K', 4, 0, false);
	place('k', 4, 7, false);
	place('Q', 3, 0, false);
	place('q', 3, 7, false);
	place('R', 0, 0, false);
	place('R', 7, 0, false);
	place('r', 0, 7, false);
	place('r', 7, 7, false);
	place('N', 1, 0, false);
	place('N', 6, 0, false);
	place('n', 1, 7, false);
	place('n', 6, 7, false);
	place('B', 2, 0, false);
	place('B', 5, 0, false);
	place('b', 2, 7, false);
	place('b', 5, 7, false);
}

bool Board::makeMove(int x1, int y1, int x2, int y2) {
	int team = currPlayer;
	if (pieces[y1][x1]->getPieceChar() == 't') {
		std::cout << "No piece here." << std::endl;
		return false;
	}
	if (pieces[y1][x1]->getTeam() != team) {
		std::cout << "Wrong piece colour." << std::endl;
		return false;
	}
	if (pieces[y1][x1]->move(x2, y2)) {
		std::shared_ptr<Piece> destPiece = pieces[y2][x2];
		place(team == 1 ? toupper(pieces[y1][x1]->getPieceChar()) : pieces[y1][x1]->getPieceChar(), x2, y2, true);
		place('t', x1, y1, false);
		if (team == 1 && !isCheck(wkpos[0], wkpos[1], team) || team == -1 && !isCheck(bkpos[0], bkpos[1], team)) {
			return true;
		}
		place(pieces[y2][x2]->getTeam() == 1 ? toupper(pieces[y2][x2]->getPieceChar()) : pieces[y2][x2]->getPieceChar(), x1, y1, pieces[y2][x2]->getMoved());
		place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), x2, y2, destPiece->getMoved());
		std::cout << "Move leaves king in check." << std::endl;
		return false;
	}
	else {
		if (castleCheck(x1, y1, x2, y2) || passantCheck(x1, y1, x2, y2)) {
			return true;
		}

		std::cout << "Invalid move." << std::endl;
		return false;
	}

}

std::vector<int> Board::getWkPos() {
	return wkpos;
}

std::vector<int> Board::getBkPos() {
	return bkpos;
}

bool Board::castleCheck(int x1, int y1, int x2, int y2) {
	std::shared_ptr<Piece> king = pieces[y1][x1];
	int team = pieces[y1][x1]->getTeam();
	if (king->getPieceChar() != 'k') {
		return false;
	}
	if (king->getMoved() || x1 != 4) {
		return false;
	}
	if (isCheck(x1, y1, king->getTeam())) {
		return false;
	}
	if (team == 1) {
		if (x2 == 2 && y2 == 0) {
			std::shared_ptr<Piece> rook = pieces[0][0];
			if (rook->getPieceChar() == 'r' && !rook->getMoved() && !isCheck(1, 0, team) && !isCheck(2, 0, team)) {
				bool occ = false;
				for (int i = 1; i <= 3; i++) {
					if (pieces[0][i]->getPieceChar() != 't') {
						occ = true;
						break;
					}
				}
				if (!occ) {
					place(team == 1 ? toupper(king->getPieceChar()) : king->getPieceChar(), x2, y2, true);
					place(rook->getTeam() == 1 ? toupper(rook->getPieceChar()) : rook->getPieceChar(), 3, 0, true);
					place('t', x1, y1, false);
					place('t', 0, 0, false);
					return true;
				}
			}
		}
		if (x2 == 6 && y2 == 0) {
			std::shared_ptr<Piece> rook = pieces[0][7];
			if (rook->getPieceChar() == 'r' && !rook->getMoved() && !isCheck(5, 0, team) && !isCheck(6, 0, team)) {
				bool occ = false;
				for (int i = 5; i <= 6; i++) {
					if (pieces[0][i]->getPieceChar() != 't') {
						occ = true;
						break;
					}
				}
				if (!occ) {
					place(team == 1 ? toupper(king->getPieceChar()) : king->getPieceChar(), x2, y2, true);
					place(rook->getTeam() == 1 ? toupper(rook->getPieceChar()) : rook->getPieceChar(), 5, 0, true);
					place('t', x1, y1, false);
					place('t', 7, 0, false);
					return true;
				}
			}
		}
	}
	else {
		if (x2 == 2 && y2 == 7) {
			std::shared_ptr<Piece> rook = pieces[7][0];
			if (rook->getPieceChar() == 'r' && !rook->getMoved() && !isCheck(1, 7, team) && !isCheck(2, 7, team)) {
				bool occ = false;
				for (int i = 1; i <= 3; i++) {
					if (pieces[7][i]->getPieceChar() != 't') {
						occ = true;
						break;
					}
				}
				if (!occ) {
					place(team == 1 ? toupper(king->getPieceChar()) : king->getPieceChar(), x2, y2, true);
					place(rook->getTeam() == 1 ? toupper(rook->getPieceChar()) : rook->getPieceChar(), 3, 7, true);
					place('t', x1, y1, false);
					place('t', 0, 7, false);
					return true;
				}
			}
		}
		if (x2 == 6 && y2 == 7) {
			std::shared_ptr<Piece> rook = pieces[7][7];
			if (rook->getPieceChar() == 'r' && !rook->getMoved() && !isCheck(5, 7, team) && !isCheck(6, 7, team)) {
				bool occ = false;
				for (int i = 5; i <= 6; i++) {
					if (pieces[7][i]->getPieceChar() != 't') {
						occ = true;
						break;
					}
				}
				if (!occ) {
					place(team == 1 ? toupper(king->getPieceChar()) : king->getPieceChar(), x2, y2, true);
					place(rook->getTeam() == 1 ? toupper(rook->getPieceChar()) : rook->getPieceChar(), 5, 7, true);
					place('t', x1, y1, false);
					place('t', 7, 7, false);
					return true;
				}
			}
		}
	}
	return false;
}

bool Board::passantCheck(int x1, int y1, int x2, int y2) {
	std::shared_ptr<Piece> ourPawn = pieces[y1][x1];
	int team = ourPawn->getTeam();
	if (passantPos[0] != -1) {
		if (pieces[passantPos[1]][passantPos[0]]->getTeam() != team) {
			if (std::abs(x1 - passantPos[0]) == 1 && y1 == passantPos[1] && x2 == passantPos[0] && y2 == passantPos[1] + team) {
				std::shared_ptr<Piece> destPiece = pieces[passantPos[1]][passantPos[0]];
				place('t', x1, y1, false);
				place('t', passantPos[0], passantPos[1], false);
				place(team == 1 ? toupper(ourPawn->getPieceChar()) : ourPawn->getPieceChar(), x2, y2, true);
				if (team == 1 && isCheck(wkpos[0], wkpos[1], 1) || team == -1 && isCheck(bkpos[0], bkpos[1], -1)) {
					place(team == 1 ? toupper(ourPawn->getPieceChar()) : ourPawn->getPieceChar(), x1, y1, ourPawn->getMoved());
					place(destPiece->getTeam() == 1 ? toupper(destPiece->getPieceChar()) : destPiece->getPieceChar(), passantPos[0], passantPos[1], destPiece->getMoved());
					place('t', x2, y2, false);
					return false;
				}
				passantPos = { -1, -1 };
				return true;
			}
		}
	}
	return false;
}

bool Board::checkForPromote(int x, int y) {
	std::shared_ptr<Piece> pawn = pieces[y][x];
	if (pieces[y][x]->getPieceChar() != 'p' || pieces[y][x]->getTeam() != currPlayer) {
		return false;
	}
	return (pawn->getTeam() == 1 && y == 7 || pawn->getTeam() == -1 && y == 0);	
}

void Board::undoCastle(int x1, int y1, int x2, int y2) {
	if (x2 == 2) {
		std::shared_ptr<Piece> rook = pieces[y2][3];
		place(rook->getTeam() == 1 ? toupper(rook->getPieceChar()) : rook->getPieceChar(), 0, y2, false);
		place('t', 3, y2, false);
	}
	if (x2 == 6) {
		std::shared_ptr<Piece> rook = pieces[y2][5];
		place(rook->getTeam() == 1 ? toupper(rook->getPieceChar()) : rook->getPieceChar(), 7, y2, false);
		place('t', 5, y2, false);
	}
}

void Board::undoPassant(int x1, int y1, int x2, int y2) {
	std::shared_ptr<Piece> killingPawn = pieces[y1][x1];
	place(killingPawn->getTeam() == 1 ? 'p' : 'P', x2, y1, true);
}

