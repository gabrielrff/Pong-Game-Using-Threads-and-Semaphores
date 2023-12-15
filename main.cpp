#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include "raylib/raylib/src/raylib.h"

#define SCREEN_W 1280
#define SCREEN_H 800

using namespace std;

int player_score = 0;

mutex mtx;

// void sem_wait(int *semaphore)
// {
//     while ((*semaphore) < 0)
//     {
//         cout << *semaphore << endl;
//         continue;
//     }
//     (*semaphore)--;
// }

// void sem_signal(int *semaphore)
// {
//     (*semaphore)++;
// }

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth() || x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)]; // Quando resetada a bola vai pra um lado aleatório
        speed_y *= speed_choices[GetRandomValue(0, 1)];
        // Tratar a hora de resetar a bola pra ver qual retangulo é pra mexer
    }
};

class Paddle
{
public:
    float x, y;
    float width, heigth;
    int speed;
    bool wasd = false;

    void Draw()
    {
        DrawRectangle(x, y, width, heigth, WHITE);
    }

    void Update()
    {
        if ((wasd && IsKeyDown(KEY_W)) || (!wasd && IsKeyDown(KEY_I)))
        { // Aqui onde mexemos os retângulos
            y = y - speed;
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        else if ((wasd && IsKeyDown(KEY_S)) || (!wasd && IsKeyDown(KEY_K)))
        {
            y = y + speed;
            this_thread::sleep_for(chrono::milliseconds(10));
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
};

Paddle player1;
Ball ball;
Paddle player2;

// void update_ball()
// {
//     while (WindowShouldClose() == false)
//     {
//         ball.Update();
//     }
// }

void update_p1()
{
    while (WindowShouldClose() == false)
    {
        mtx.lock();
        player1.Update();
        mtx.unlock();
    }
}

void update_p2()
{
    while (WindowShouldClose() == false)
    {
        mtx.lock();
        player2.Update();
        mtx.unlock();
    }
}

int main()
{
    ball.radius = 21;
    ball.x = SCREEN_W / 2;
    ball.y = SCREEN_H / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player1.wasd = true;
    player1.width = 25;
    player1.heigth = 120;
    player1.x = 10;
    player1.y = SCREEN_H / 2 - player1.heigth / 2;
    player1.speed = 6;

    player2.width = 25;
    player2.heigth = 120;
    player2.x = SCREEN_W - player2.width - 10;
    player2.y = SCREEN_H / 2 - player1.heigth / 2;
    player2.speed = 6;

    cout << "Starting the game" << endl;

    InitWindow(SCREEN_W, SCREEN_H, "Pong Game!");
    SetTargetFPS(60);

    // int semaphore = 0;

    // thread thBall(update_ball);
    thread thP1(update_p1);
    thread thP2(update_p2);

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        ball.Update();
        // player1.Update();
        // player2.Update();

        // Checking for collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player1.x, player1.y, player1.width, player1.heigth}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.heigth}))
        {
            ball.speed_x *= -1;
        }

        // Drawing
        ball.Draw();
        player1.Draw();
        player2.Draw();
        ClearBackground(BLACK);
        DrawLine(SCREEN_W / 2, 0, SCREEN_W / 2, SCREEN_H, WHITE);
        EndDrawing();

        // sem_signal(&semaphore);
    }

    // thBall.join();
    thP1.join();
    thP2.join();

    return 0;
}