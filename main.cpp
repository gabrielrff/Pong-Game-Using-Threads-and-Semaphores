#include <iostream>
#include "Ball.h"
#include "Paddle.h"

#define SCREEN_W 1280
#define SCREEN_H 720

using namespace std;

void update_player_in_thread(Paddle *player)
{
    while (WindowShouldClose() == false)
    {
        mtx.lock();
        (*player).Update();
        mtx.unlock();
    }
}

int main()
{
    Ball ball;
    Paddle player1;
    Paddle player2;

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

    SetTargetFPS(60);

    thread thP1(update_player_in_thread, &player1);
    thread thP2(update_player_in_thread, &player2);

    chrono::time_point<std::chrono::system_clock> start, current;

    while (WindowShouldClose() == false)
    {
        // Logic
        switch (currentScreen)
        {
        case TITLE:
        {
            // Press enter to start game
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
                start = chrono::system_clock::now();
            }
            // Q to quit
            if (IsKeyPressed(KEY_Q))
            {
                CloseWindow();
            }
            // R to rules
            if (IsKeyPressed(KEY_R))
            {
                currentScreen = RULES;
            }
        }
        break;
        case RULES:
        {
            // Press enter to start game
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
                start = chrono::system_clock::now();
            }
            // B to go back to title
            if (IsKeyPressed(KEY_B))
            {
                currentScreen = TITLE;
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
        }
        break;
        case GAMEOVER:
        {
            // reseting
            ball.Reset();
            player1.Reset();
            player2.Reset();

            // Press enter to re-start game
            if (IsKeyPressed(KEY_ENTER))
            {
                currentScreen = GAMEPLAY;
                start = chrono::system_clock::now();
            }
            // B to go back to title
            if (IsKeyPressed(KEY_B))
            {
                currentScreen = TITLE;
            }
            // Q to quit
            if (IsKeyPressed(KEY_Q))
            {
                CloseWindow();
            }
        }
        break;
        default:
            break;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentScreen)
        {
        case TITLE:
        {
            const char *title = "PONG!";
            const char *subTitle = "CO-OP";
            const char *menu[2] = {"PRESS ENTER TO START GAME!", "[Q] QUIT\t\t[R] RULES"};
            int titleFontSize = 100;
            int subTitleFontSize = 80;
            int menuFontSize = 30;

            DrawText(title, (SCREEN_W / 2) - MeasureText(title, titleFontSize) / 2, (SCREEN_H / 2) - titleFontSize - subTitleFontSize, titleFontSize, LIGHTGRAY);
            DrawText(subTitle, (SCREEN_W / 2) - MeasureText(subTitle, subTitleFontSize) / 2, (SCREEN_H / 2) - subTitleFontSize, subTitleFontSize, LIGHTGRAY);

            for (int i = 0; i < 2; i++)
            {
                DrawText(menu[i], (SCREEN_W / 2) - MeasureText(menu[i], menuFontSize) / 2, (2 * SCREEN_H / 3) + (menuFontSize + 20) * i, menuFontSize, LIGHTGRAY);
            }
        }
        break;
        case RULES:
        {
            const char *menu[2] = {"PRESS ENTER TO START GAME!", "[B] BACK"};
            const char *rules[6] = {"[W] AND [S] TO CONTROL LEFT PADDLE", "[I] AND [K] TO CONTROL RIGHT PADDLE", "YOU CANNOT MOVE BOTH AT THE SAME TIME!", "THE GAME ACCELERATES AS YOU PLAY", "MISS THE BALL AND YOU LOSE!", "TRY TO SURVIVE AS LONG AS YOU CAN!"};
            int menuFontSize = 30;
            int rulesFontSize = 40;

            for (int i = 0; i < 6; i++)
            {
                DrawText(rules[i], (SCREEN_W / 2) - MeasureText(rules[i], rulesFontSize) / 2, (SCREEN_H / 5) + (rulesFontSize + 30) * i, rulesFontSize, LIGHTGRAY);
            }

            for (int i = 0; i < 3; i++)
            {
                DrawText(menu[i], (SCREEN_W / 2) - MeasureText(menu[i], menuFontSize) / 2, (4 * SCREEN_H / 5) + (menuFontSize + 20) * i, menuFontSize, LIGHTGRAY);
            }
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
        case GAMEOVER:
        {
            const char *gameOver = "GAME-OVER!";
            const char *scoreText = "SCORE: %.1f";
            const char *menu[2] = {"PRESS ENTER TO RE-START GAME!", "[Q] QUIT\t\t[B] TITLE"};
            int gameOverFontSize = 100;
            int scoreFontSize = 80;
            int menuFontSize = 30;

            DrawText(gameOver, (SCREEN_W / 2) - MeasureText(gameOver, gameOverFontSize) / 2, (2 * SCREEN_H / 5) - gameOverFontSize - scoreFontSize, gameOverFontSize, LIGHTGRAY);
            DrawText(TextFormat(scoreText, score.count()), (SCREEN_W / 2) - MeasureText(scoreText, scoreFontSize) / 2, (SCREEN_H / 2) - scoreFontSize, scoreFontSize, LIGHTGRAY);

            for (int i = 0; i < 2; i++)
            {
                DrawText(menu[i], (SCREEN_W / 2) - MeasureText(menu[i], menuFontSize) / 2, (2 * SCREEN_H / 3) + (menuFontSize + 20) * i, menuFontSize, LIGHTGRAY);
            }
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