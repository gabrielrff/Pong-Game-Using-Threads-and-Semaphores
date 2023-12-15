#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include "raylib/raylib/src/raylib.h"

#define SCREEN_W 1280
#define SCREEN_H 800

using namespace std;

// globals
typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

GameScreen currentScreen = LOGO;

mutex mtx;

chrono::duration<double> score;

class Ball
{
public:
    float x, y;
    float initial_speed, speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update()
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

    void Reset()
    {
        // reseting positions
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        // reseting speed (ball goes do random direction)
        int speed_choices[2] = {-1, 1};
        speed_x = initial_speed * speed_choices[GetRandomValue(0, 1)];
        speed_y = initial_speed * speed_choices[GetRandomValue(0, 1)];
    }

    void GameOver()
    {
        currentScreen = ENDING;
    }
};

class Paddle
{
public:
    float x, y;
    float width, heigth;
    float initial_speed, speed;
    bool wasd = false;

    void Draw()
    {
        DrawRectangle(x, y, width, heigth, WHITE);
    }

    void Update()
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

    void Reset()
    {
        // reseting positions
        y = (GetScreenHeight() / 2) - heigth / 2;

        // reseting speeed
        speed = initial_speed;
    }
};

Paddle player1;
Ball ball;
Paddle player2;

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
    ball.initial_speed = 7;
    ball.speed_x = ball.initial_speed;
    ball.speed_y = ball.initial_speed;

    player1.wasd = true;
    player1.width = 25;
    player1.heigth = 120;
    player1.x = 10;
    player1.y = SCREEN_H / 2 - player1.heigth / 2;
    player1.initial_speed = 10;
    player1.speed = player1.initial_speed;

    player2.width = 25;
    player2.heigth = 120;
    player2.x = SCREEN_W - player2.width - 10;
    player2.y = SCREEN_H / 2 - player1.heigth / 2;
    player2.initial_speed = 10;
    player2.speed = player2.initial_speed;

    cout << "Starting the game" << endl;

    float acceleration = 0.05;

    InitWindow(SCREEN_W, SCREEN_H, "Pong Game!");
    int frameCounter = 0;

    SetTargetFPS(60);

    thread thP1(update_p1);
    thread thP2(update_p2);

    chrono::time_point<std::chrono::system_clock> start, current;

    while (WindowShouldClose() == false)
    {
        switch (currentScreen)
        {
        case LOGO:
        {
            frameCounter++; // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (frameCounter > 120)
            {
                currentScreen = TITLE;
            }
        }
        break;
        case TITLE:
        {
            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
                start = chrono::system_clock::now();
            }
        }
        break;
        case GAMEPLAY:
        {
            // Checking for collisions
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player1.x, player1.y, player1.width, player1.heigth}))
            {
                ball.speed_x *= -(1 + acceleration);
                player1.speed *= (1 + acceleration);
            }

            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.heigth}))
            {
                ball.speed_x *= -(1 + acceleration);
                player2.speed *= (1 + acceleration);
            }

            // Press enter to change to ENDING screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = ENDING;
            }
        }
        break;
        case ENDING:
        {
            // reseting
            ball.Reset();
            player1.Reset();
            player2.Reset();

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = TITLE;
            }
        }
        break;
        default:
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen)
        {
        case LOGO:
        {
            // TODO: Draw LOGO screen here!
            DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
        }
        break;
        case TITLE:
        {
            // TODO: Draw TITLE screen here!
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, GREEN);
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
        }
        break;
        case GAMEPLAY:
        {
            ball.Update();

            // Drawing
            ball.Draw();
            player1.Draw();
            player2.Draw();

            DrawLine(SCREEN_W / 2, 60, SCREEN_W / 2, SCREEN_H, WHITE);

            current = chrono::system_clock::now();
            score = current - start;
            DrawText(TextFormat("%.1f", score.count()), (SCREEN_W / 2) - 50, 0, 50, WHITE);
        }
        break;
        case ENDING:
        {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
            DrawText(TextFormat("SCORE: %.1f", score.count()), (SCREEN_W / 2) - 50, 0, 50, WHITE);
        }
        break;
        default:
            break;
        }

        EndDrawing();
    }

    thP1.join();
    thP2.join();

    return 0;
}