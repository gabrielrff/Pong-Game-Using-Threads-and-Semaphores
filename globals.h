#ifndef GLOBALS_H
#define GLOBALS_H

#include <mutex>
#include <chrono>

using namespace std;

// globals
typedef enum GameScreen
{
    TITLE = 0,
    RULES,
    GAMEPLAY,
    GAMEOVER
} GameScreen;

extern GameScreen currentScreen;

extern mutex mtx;

chrono::duration<double> extern score;

#endif