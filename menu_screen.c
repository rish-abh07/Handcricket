#include "menu_screen.h"
#include "globalsVar.h"
#include "raylib.h"
#include "math.h"
#include "ui_utils.h"
#include <stdlib.h>
#include "game.h"
#include "toss_screen.h"
float buttonWidth = 300;
float buttonHeight = 40;
int buttonGap = 20;

static Rectangle startButton = { 300, 250, 300, 40};
static Rectangle leaderBoardButton = {300, 300, 300, 40};
static Rectangle settingButton = {300, 350, 300, 40};
static Rectangle howToPlayButton = { 300, 400, 300, 40 };
static Texture2D backgroundTexture;



MenuIcons menuIcon;

void InitMenuScreen(){
    startButton.x = (screenWidth - buttonWidth)/2;
    leaderBoardButton.x = (screenWidth - buttonWidth)/2;
    settingButton.x = (screenWidth - buttonWidth)/2;
    howToPlayButton.x = (screenWidth - buttonWidth)/2;
    
   
     
     //icon load
     menuIcon.play = LoadTexture("asset/play.png");
     menuIcon.leaderboard = LoadTexture("asset/leaderboard.png");
     menuIcon.settings = LoadTexture("asset/set.png");
     menuIcon.howToPlay = LoadTexture("asset/howtoPlay.png");
     
}

void UpdateMenuScreen(GameState *state)
{
    Vector2 mouse = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, startButton)) {
            InitTossScreen();
            *state = STATE_TOSS; // Transition to toss state
            

        } else if (CheckCollisionPointRec(mouse, howToPlayButton)) {
            CloseWindow(); // Exit the game
        }
    }
}

void DrawMenuScreen(int screenWidth, int screenHeight)
{
     

   
    // hoverColors
    Color hoverStart = (Color){32, 191, 90, 255};
    Color hoverLeader = (Color){ 41, 104, 237,255};
    Color hoverSetting = (Color){149, 55, 235, 255} ; 
    Color hoverExit = (Color){237, 93, 14, 255} ;

     //buttonColor
     Color startButtonColor = (Color){32, 191, 90, 230};
     Color leaderBoardButtonColor = (Color){ 41, 104, 237, 230};
     Color settingButtonColor = (Color){149, 55, 235, 230};
     Color howToPlayButtonColor = (Color){ 237, 93, 14, 230};

     Vector2 textSize = MeasureTextEx(montserratTitle, "HAND CRICKET", 40,1);
     Vector2 titlePosition = {(screenWidth - textSize.x)/2,startButton.y - 100};
     DrawTextEx(montserratTitle,"HAND CRICKET", titlePosition, 40,1, WHITE);
    Vector2 subHeadSize = MeasureTextEx(subHeadLight,"The Ultimate Finger Game", 20,0); // match font size to DrawText
    float subHeadX = titlePosition.x + (textSize.x - subHeadSize.x)/2;
    DrawTextEx(subHeadLight,"The Ultimate Finger Game",(Vector2){ subHeadX, titlePosition.y + 50}, 20,0, YELLOW);


    hoverSize(startButton, startButtonColor,hoverStart, 20, 1.2f,"NEW GAME", WHITE, buttonFontMedium, 0, menuIcon.play, 0, 0);
    hoverSize(leaderBoardButton, leaderBoardButtonColor,hoverLeader, 20,1.2f, "LEADERBOARD", WHITE, buttonFontMedium,0,menuIcon.leaderboard, 0, 0);
    hoverSize(settingButton, settingButtonColor,hoverSetting, 20, 1.2f, "SETTING", WHITE, buttonFontMedium,0, menuIcon.settings, 0, 0);
    hoverSize(howToPlayButton, howToPlayButtonColor,hoverExit, 20, 1.2f, "HOW TO PLAY", WHITE, buttonFontMedium,0, menuIcon.howToPlay,  0, 0);
}
void UnloadMenuScreen() {
   
    UnloadTexture(menuIcon.play);
    UnloadTexture(menuIcon.leaderboard);
    UnloadTexture(menuIcon.settings);
    UnloadTexture(menuIcon.howToPlay);
}

