
#include "scoreboard.h"

using namespace std;

Scoreboard::Scoreboard(): ws{0.0}, bs{0.0} {}

Scoreboard::~Scoreboard() {}

void Scoreboard::displayCurrScore(std::shared_ptr<Xwindow> xw) {
    xw->fillRectangle(300, 700, 100, 50, Xwindow::PowderBlue);
    xw->drawString(340, 704, "Score:");
    stringstream ss1;
    stringstream ss2;
    ss1 << ws;
    ss2 << bs;
    string q = ss1.str();
    string r = ss2.str();
        string s = "White: ";
        string t = "Black: ";
        s += q;
        t += r;
        xw->drawString(340, 720, s);
        xw->drawString(340, 736, t);
}

void Scoreboard::displayFinScore(std::shared_ptr<Xwindow> xw) {
    xw->fillRectangle(300, 680, 100, 70, Xwindow::PowderBlue);
    xw->drawString(340, 704, "Final Score:");
    stringstream ss1;
        stringstream ss2;
        ss1 << ws;
        ss2 << bs;
    string q = ss1.str();
        string r = ss2.str();
        string s = "White: ";
        string t = "Black: ";
        s += q;
        t += r;
        xw->drawString(340, 720, s);
        xw->drawString(340, 736, t);
}

float Scoreboard::getWs() {
    return ws;
}

float Scoreboard::getBs() {
    return bs;
}

void Scoreboard::setScores(float f, float g) {
    ws = f;
    bs = g;
}


