#include "Ball.h"

void Ball::Draw()
{
    DrawCircle(x, y, radius, WHITE);
}

void Ball::Update()
{
    if (currentScreen == GAMEPLAY)
    {

        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth() || x - radius <= 0)
        {
            GameOver();
        }
    }
}

void Ball::Reset()
{
    // reseting positions
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    // reseting speed (ball goes do random direction)
    int speed_choices[2] = {-1, 1};
    speed_x = initial_speed * speed_choices[GetRandomValue(0, 1)];
    speed_y = initial_speed * speed_choices[GetRandomValue(0, 1)];
}

void Ball::GameOver()
{
    currentScreen = GAMEOVER;
}