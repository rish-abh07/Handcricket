#include "game.h"
#include "toss_screen.h"
#include "choose_bat_ball.h"
#include "play_screen.h"
#include "globalsVar.h"
#include "math.h"
#include "menu_screen.h"
#include "fade.h"
#include "summary_screen.h"
#include "ui_utils.h"
#include <stdlib.h>
//#include "summary screen.h"
static Texture2D backgroundTexture;
static GameState currentState;
int screenWidth = 800;
int screenHeight = 600;



void InitGame(void) {
    InitWindow(800, 600, "Hand Cricket Game");
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    SetTargetFPS(60);  // Set target FPS for smoother rendering
    backgroundTexture = LoadTexture("asset/menu_background.png");
    currentState = STATE_MENU;
    LoadGameFont();
    InitTossScreen();
    InitMenuScreen();
   // Initialize with user not winning toss
    InitFade(&fade); 
}

void UpdateGame(){
      
         switch(currentState){
            case STATE_MENU: UpdateMenuScreen(&currentState); break;
            case STATE_TOSS: UpdateTossScreen(&currentState); break;
            case STATE_BAT_BOWL: UpdateChooseBatBall(&currentState); break;
           case STATE_PLAY: UpdatePlayScreen(&currentState); break;
            case STATE_SUMMARY: UpdateSummaryScreen(&currentState); break;
    }
}

void DrawGame(void) {
    BeginDrawing();
    ClearBackground(BLACK);  // Good contrast
     // Calculate scale to preserve aspect ratio (letterbox/pillarbox logic)
    float scale = fminf((float)screenWidth / backgroundTexture.width,
                        (float)screenHeight / backgroundTexture.height);

    Rectangle dest = {
        (screenWidth - backgroundTexture.width * scale) / 2,
        (screenHeight - backgroundTexture.height * scale) / 2,
        backgroundTexture.width * scale,
        backgroundTexture.height * scale
    };

    DrawTexturePro(backgroundTexture,
                   (Rectangle){ 0, 0, backgroundTexture.width, backgroundTexture.height },
                   dest,
                   (Vector2){ 0, 0 },
                   0.0f,
                   WHITE);

        
    switch(currentState){
        case STATE_MENU: DrawMenuScreen(screenWidth,screenHeight); break;
        case STATE_TOSS: DrawTossScreen(screenWidth,screenHeight); break;
        case STATE_BAT_BOWL:    DrawChooseBatBallScreen(); break;
        case STATE_PLAY: DrawPlayScreen(); break;
       case STATE_SUMMARY: DrawSummaryScreen(); break;
    }
    
    EndDrawing();
}

void CloseGame(void) {
     UnloadChooseBatBall(); 
     UnloadGameFont(); 
    CloseWindow();
}


