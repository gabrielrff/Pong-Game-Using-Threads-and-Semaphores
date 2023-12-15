#include "Paddle.h"

void Paddle::Draw()
{
    DrawRectangle(x, y, width, heigth, WHITE);
}

void Paddle::Update()
{
    if ((wasd && IsKeyDown(KEY_W)) || (!wasd && IsKeyDown(KEY_I)))
    {
        y -= speed;
        this_thread::sleep_for(chrono::milliseconds(5));
    }
    else if ((wasd && IsKeyDown(KEY_S)) || (!wasd && IsKeyDown(KEY_K)))
    {
        y += speed;
        this_thread::sleep_for(chrono::milliseconds(5));
    }

    if (y <= 0)
    {
        y = 0;
    }
    if (y + heigth >= GetScreenHeight())
    {
        y = GetScreenHeight() - heigth;
    }
}

void Paddle::Reset()
{
    // reseting positions
    y = (GetScreenHeight() / 2) - heigth / 2;

    // reseting speeed
    speed = initial_speed;
}