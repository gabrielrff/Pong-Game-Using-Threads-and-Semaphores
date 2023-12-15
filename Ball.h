#ifndef BALL_H
#define BALL_H

#include "raylib/raylib/src/raylib.h"
#include "globals.h"

class Ball
{

public:
    float x, y;
    float initial_speed, speed_x, speed_y;
    int radius;

    void Draw();
    void Update();
    void Reset();
    void GameOver();
};

#endif