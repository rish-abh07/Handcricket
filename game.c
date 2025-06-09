#include "game.h"
#include "toss_screen.h"
#include "choose_bat_ball.h"
#include "play_screen.h"
#include "globalsVar.h"
#include "menu_screen.h"
//#include "summary screen.h"

static GameState currentState;
int screenWidth;
int screenHeight;
void InitGame(void) {
    InitWindow(800, 600, "Hand Cricket Game");
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    SetTargetFPS(60);  // Set target FPS for smoother rendering
    currentState = STATE_MENU;
    InitTossScreen();
    InitMenuScreen();
   // Initialize with user not winning toss
}

    void UpdateGame(){
         switch(currentState){
            case STATE_MENU: UpdateMenuScreen(&currentState); break;
            case STATE_TOSS: UpdateTossScreen(&currentState); break;
            case STATE_BAT_BOWL: UpdateChooseBatBall(&currentState); break;
           case STATE_PLAY: UpdatePlayScreen(&currentState); break;
           // case STATE_SUMMARY: UpdateSummaryScreen(&currentState); break;
    }
}

void DrawGame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);  // Good contrast

    switch(currentState){
        case STATE_MENU: DrawMenuScreen(screenWidth,screenHeight); break;
        case STATE_TOSS: DrawTossScreen(); break;
        case STATE_BAT_BOWL:    DrawChooseBatBallScreen(); break;
        case STATE_PLAY: DrawPlayScreen(); break;
       // case STATE_SUMMARY: DrawSummaryScreen(); break;
    }

    EndDrawing();
}

void CloseGame(void) {
     UnloadChooseBatBall();  
    CloseWindow();
}