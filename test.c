#include "include\\raylib.h"
#include <stdlib.h>
#include <time.h>

typedef enum{
    STATE_TOSS,
    STATE_BAT_BOWL,
    STATE_RESULT,
} GameState;

GameState currentState = STATE_TOSS;
bool userWonToss = false;
bool tossDone = false;
const int screenWidth = 800;
const int screenHeight = 600;

Rectangle headsBtn = {200,200,150,50};
Rectangle tailsBtn = {450,200,150,50};

const char* tossResult = NULL;

int main(void) {
    // Correct window dimensions: Width x Height
    InitWindow(screenWidth, screenHeight, "Hand Cricket Toss");
    SetTargetFPS(60);  // Set target FPS for smoother rendering
    srand(time(NULL));  // Seed for random number generation
    SetTargetFPS(60);  // Optional: makes drawing smoother

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);  // Good contrast

        if(currentState == STATE_TOSS){
            DrawText("Choose Heads or Tails", 250 , 50, 20 , BLACK);
            DrawRectangleRec(headsBtn, LIGHTGRAY);
            DrawText("Heads", headsBtn.x + 50, headsBtn.y + 10, 20, BLACK);
            DrawRectangleRec(tailsBtn, LIGHTGRAY);
            DrawText("Tails", tailsBtn.x + 50, tailsBtn.y + 10, 20, BLACK);

            Vector2 mousePos = GetMousePosition();

            if(!tossDone && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(CheckCollisionPointRec(mousePos, headsBtn)){
                    userWonToss = (rand() % 2 == 0);  // Randomly decide if user wins toss
                    tossResult = userWonToss?"You chose Heads! you won the toss":"You lost the toss";
                    tossDone = true;
                } else if(CheckCollisionPointRec(mousePos, tailsBtn)){
                    userWonToss = (rand() % 2 == 1);  // Randomly decide if user wins toss
                    tossResult = userWonToss?"You chose Tails! you won the toss":"You lost the toss";
                    tossDone = true;
                }
            }
            if(tossDone){
                DrawText(tossResult, 250, 300, 20, userWonToss ? GREEN : RED);
                DrawText("Press SPACE to continue", 250, 350, 20, BLACK);
                if(IsKeyPressed(KEY_SPACE)){
                    currentState = STATE_BAT_BOWL;
                }
            }
        }
        else if(currentState == STATE_BAT_BOWL){
            DrawText("CHoose To Bat or Bowl", 250, 50, 20, BLACK);
            DrawText("Press ESC to exit", 250, 400, 20, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}