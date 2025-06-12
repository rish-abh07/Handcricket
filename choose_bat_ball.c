#include "choose_bat_ball.h"
#include "toss_screen.h"
#include "play_screen.h"
#include "Montserrat_Fonts.h"
#include "ui_utils.h"
#include "globalsVar.h"
#include <math.h>
#include "fade.h"
static const char *computerChoice = NULL;
bool computerTurn = false;
static Vector2 orignalBatCenter = {250, 300};
static Vector2 orignalBallCenter = {450, 300};
static Rectangle batBtn;
static Rectangle ballBtn; 
static float choiceRadius = 50.0f;
static bool choiceDone = false;
static const char *choiceText = NULL;
static bool isUserBat = false;
static Texture2D batTexture;
static Texture2D ballTexture;

// Animation variables
static float choiceAnimationTime = 0.0f;
static bool isChoiceAnimating = false;
static int animationHighlightToggle = 0;
static Rectangle contBtn = {100,100,140,40};
void InitChooseBatBall(bool isComputerTurn) {
    choiceDone = false;
    choiceText = NULL;
    batBtn.width = screenWidth/1.5;
    batBtn.height = batBtn.width/3.5;
    batBtn.x = (screenWidth - batBtn.width)/2;
    batBtn.y = screenHeight * 0.15f + 90.0f ;
    ballBtn =(Rectangle){batBtn.x, batBtn.y + batBtn.height + 20.0f, batBtn.width, batBtn.height};
    if (isComputerTurn) {
        computerTurn = true;
        isChoiceAnimating = true;
        choiceAnimationTime = 0.0f;
        animationHighlightToggle = 0;
        computerChoice = NULL;
        choiceDone = false;
    } else {
        choiceText = "Choose Bat or Ball";
    }

    batTexture = LoadTexture("asset/bat.png");
    ballTexture = LoadTexture("asset/ball.png");

    if (batTexture.id == 0 || ballTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load textures!");
    }
}

void UnloadChooseBatBall(void) {
    UnloadTexture(batTexture);
    UnloadTexture(ballTexture);
}

void UpdateChooseBatBall(GameState *state) {
    // Computer is making choice
    if (computerTurn && isChoiceAnimating) {
        choiceAnimationTime += GetFrameTime();
        if (choiceAnimationTime >= 2.0f) {
            isChoiceAnimating = false;
            if (rand() % 2 == 0) {
                computerChoice = "Bat";
                isUserBat = false;
            } else {
                computerChoice = "Ball";
                isUserBat = true;
            }
            choiceDone = true;
            StartFade(&fade, 1.0f, false); // Start fade out after computer finishes choice
        } else {
            animationHighlightToggle = fmod(choiceAnimationTime, 0.4f) < 0.2f ? 0 : 1;
        }
    }

    // User is making choice
    if (!computerTurn) {
        Vector2 mousePos = GetMousePosition();
        if (!choiceDone) {
            if (CheckCollisionPointRec(mousePos, batBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                choiceText = "You chose Bat";
                isUserBat = true;
                choiceDone = true;
                StartFade(&fade, 1.0f, false); // Start fade out immediately
            } else if (CheckCollisionPointRec(mousePos, ballBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                choiceText = "You chose Ball";
                isUserBat = false;
                choiceDone = true;
                StartFade(&fade, 1.0f, false); // Start fade out immediately
            }
        }
    }

    // Transition after fade out
    if (choiceDone && fade.done && !fade.fadeIn) {
        *state = STATE_PLAY;
        StartFade(&fade, 1.0f, true);  // Fade in next screen
        UnloadChooseBatBall();
        InitPlayScreen(isUserBat);
    }
}


void DrawChooseBatBallScreen(void) {
    Texture2D empty = {0};

    if (computerTurn) {
        // Headline
        Vector2 textSize = MeasureTextEx(montserratTitle, "COMPUTER WILL MAKE CHOICE", 40, 1);
        Vector2 titlePosition = {(screenWidth - textSize.x) / 2, screenHeight * 0.15f};
        DrawTextEx(montserratTitle, "COMPUTER WILL MAKE  CHOICE", titlePosition, 40, 1, WHITE);

        // Subhead
        Vector2 subHeadSize = MeasureTextEx(subHeadLight, "Computer won the toss! Computer will choose", 20, 0);
        float subHeadX = titlePosition.x + (textSize.x - subHeadSize.x) / 2;
        DrawTextEx(subHeadLight, "Computer won the toss! Computer will choose", (Vector2){subHeadX, titlePosition.y + 50}, 20, 0, YELLOW);

        // Animate choice
        float pulse = 1.0f + 0.05f * sinf(GetTime() * 6.0f);
        float scale = isChoiceAnimating ? pulse : 1.2f;
        Rectangle btn = (isChoiceAnimating ? (animationHighlightToggle == 0 ? batBtn : ballBtn)
                                           : (strcmp(computerChoice, "Bat") == 0 ? batBtn : ballBtn));

        Rectangle scaledBtn = {
            btn.x + btn.width * (1.0f - scale) / 2,
            btn.y + btn.height * (1.0f - scale) / 2,
            btn.width * scale,
            btn.height * scale
        };
        DrawRectangleRounded(scaledBtn, 0.3, 10, YELLOW);

        // Draw buttons
        DrawRectangleRounded(batBtn, 0.2, 10, ORANGE);
        DrawRectangleRounded(ballBtn, 0.2, 10, (Color){14, 150, 139, 255});
        btnIconTextCentered("Bat First", batBtn.x, batBtn.y, batBtn.width, batBtn.height, 40, WHITE, montserratTitle, 0, empty, 0, 10.0f);
        btnIconTextCentered("Ball First", ballBtn.x, ballBtn.y, ballBtn.width, ballBtn.height, 40, WHITE, montserratTitle, 0, empty, 0, 10.0f);
        btnIconTextCentered("Set a target score", batBtn.x, batBtn.y, batBtn.width, batBtn.height, 20, WHITE, montserratTitle, 0, empty, 0, 35.0f);
        btnIconTextCentered("Restrict opponent's score", ballBtn.x, ballBtn.y, ballBtn.width, ballBtn.height, 20, WHITE, montserratTitle, 0, empty, 0, 35.0f);
        CenterIconInButton(batTexture, batBtn, 50, 0, -30.0f);
        CenterIconInButton(ballTexture, ballBtn, 50, 0, -35.0f);

    } else {
        // User UI
        Vector2 textSize = MeasureTextEx(montserratTitle, "MAKE YOUR CHOICE", 40, 1);
        Vector2 titlePosition = {(screenWidth - textSize.x) / 2, screenHeight * 0.15f};
        DrawTextEx(montserratTitle, "MAKE YOUR CHOICE", titlePosition, 40, 1, WHITE);

        Vector2 subHeadSize = MeasureTextEx(subHeadLight, "You won the toss! What do you want to do?", 20, 0);
        float subHeadX = titlePosition.x + (textSize.x - subHeadSize.x) / 2;
        DrawTextEx(subHeadLight, "You won the toss! What do you want to do?", (Vector2){subHeadX, titlePosition.y + 50}, 20, 0, YELLOW);

        // Hoverable buttons
        hoverSize(batBtn, (Color){255, 165, 0, 230}, ORANGE, 30, 1.2f, "Bat First", WHITE, montserratTitle, 0, empty, 0, 10.0f);
        hoverSize(ballBtn, (Color){14, 150, 139, 230}, (Color){14, 150, 139, 255}, 30, 1.2f, "Bowl First", WHITE, montserratTitle, 0, empty, 0, 10.0f);
        btnIconTextCentered("Set a target score", batBtn.x, batBtn.y, batBtn.width, batBtn.height, 20, WHITE, montserratTitle, 0, empty, 0, 35.0f);
        btnIconTextCentered("Restrict opponent's score", ballBtn.x, ballBtn.y, ballBtn.width, ballBtn.height, 20, WHITE, montserratTitle, 0, empty, 0, 35.0f);
        CenterIconInButton(batTexture, batBtn, 50, 0, -30.0f);
        CenterIconInButton(ballTexture, ballBtn, 50, 0, -35.0f);
    }
}
