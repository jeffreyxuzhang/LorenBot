
#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include <vector>
#include <string>
#include <sstream>
#include "window.h"
#include <memory>

class Scoreboard {
	float ws = 0.0;
	float bs = 0.0;

public:
	Scoreboard();
	~Scoreboard();
	void displayCurrScore(std::shared_ptr<Xwindow> xw);
	void displayFinScore(std::shared_ptr<Xwindow> xw);

	float getWs();
	float getBs();
	void setScores(float f, float g);
};
#endif
