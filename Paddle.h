#ifndef PADDLE_H
#define PADDLE_H

#include "raylib/raylib/src/raylib.h"
#include "globals.h"
#include <thread>
#include <chrono>

class Paddle
{
public:
    float x, y;
    float width, heigth;
    float initial_speed, speed;
    bool wasd = false;

    void Draw();
    void Update();
    void Reset();
};

#endif