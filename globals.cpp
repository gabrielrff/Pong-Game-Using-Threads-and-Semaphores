#include "globals.h"

using namespace std;

GameScreen currentScreen = TITLE;

mutex mtx;

chrono::duration<double> score;