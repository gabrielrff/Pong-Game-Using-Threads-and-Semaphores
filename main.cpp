#include <iostream>
#include <raylib.h>

using namespace std;


int player_score = 0;


class Ball{
public:
    float x,y;
    int speed_x, speed_y;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius,WHITE);
    }

    void Update(){
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

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];//Quando resetada a bola vai pra um lado aleatório
        speed_y *= speed_choices[GetRandomValue(0,1)];
        //Tratar a hora de resetar a bola pra ver qual retangulo é pra mexer

    }

};

class Paddle{
public:
float x,y;
float width, heigth;
int speed;


void Draw(){
    DrawRectangle(x,y,width, heigth, WHITE);
}

void Update(){

    if (IsKeyDown(KEY_UP)){//Aqui onde mexemos os retângulos
        y = y - speed;
    }
    if (IsKeyDown(KEY_DOWN)){
        y = y + speed;
    }

    if (y <= 0){
        y = 0;
    }
    if (y + heigth >= GetScreenHeight()){
        y = GetScreenHeight() - heigth;
    }
}

};

Ball ball;
Paddle player1;
Paddle player2;

int main () {

    cout << "Starting the game" << endl;

    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width,screen_height,"Pong Game!");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player1.width = 25;
    player1.heigth = 120;
    player1.x = 10;
    player1.y = screen_height/2 - player1.heigth/2;
    player1.speed = 6;

    player2.width = 25;
    player2.heigth = 120;
    player2.x = screen_width - player1.width - 10;
    player2.y = screen_height/2 - player1.heigth/2;
    player2.speed = 6;

    while (WindowShouldClose() == false){ 
        BeginDrawing();
        //Updating
        ball.Update();
        player1.Update();
        player2.Update();


        //Checking for collisions
        if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player1.x,player1.y,player1.width,player1.heigth})){
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player2.x,player2.y,player2.width,player2.heigth})){
            ball.speed_x *= -1;
        }
        
        //Drawing
        ClearBackground(BLACK);
        DrawLine(screen_width/2,0,screen_height/2,screen_height,WHITE);
        ball.Draw();
        player1.Draw();
        player2.Draw();

        EndDrawing();
    }
    
    
    return 0;
}