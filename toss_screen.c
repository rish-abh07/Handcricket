#include "toss_screen.h"
#include "choose_bat_ball.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>  // for strcmp
#include <math.h>    // for cosf, fabsf, DEG2RAD
#include "Montserrat_Fonts.h"
#include "globalsVar.h"

bool isUserWonToss = false;
bool isComputerTurn= false;
static Vector2 headCenter = {250, 300};
static Vector2 tailCenter = {450, 300};
static float coinRadius = 50.0f;

static bool tossDone = false;
static int tossResult = -1;

static const char *userChoiceText = NULL;

static float coinAngle = 0.0f;
static float animationSpeed = 720.0f; // degrees per second
static float animationTime = 0.0f;
static float animationDuration = 1.0f; // seconds
static bool isAnimating = false;
static Rectangle HeadBtn = { 100,100,100,40};

static Rectangle smallBackgroundRect={ 250, 250, 400, 300 };
Color smallBackgroundRectColor = (Color){104, 50, 146, 255};
void InitTossScreen(void) {
    tossDone = false;
    tossResult = -1;
    smallBackgroundRect.width = GetScreenWidth()/2.5;
    smallBackgroundRect.height = GetScreenHeight()/2.5;
    smallBackgroundRect.x = (GetScreenWidth()-smallBackgroundRect.width)/2;
    smallBackgroundRect.y = (GetScreenHeight()-smallBackgroundRect.height)/2;
    HeadBtn.width = smallBackgroundRect.width/3;
    HeadBtn.x = smallBackgroundRect.x + (smallBackgroundRect.width/2 - HeadBtn.width)/2;
    HeadBtn.y = smallBackgroundRect.y + (smallBackgroundRect.height - HeadBtn.height)*0.90; 
    userChoiceText = NULL;
    isAnimating = false;
    animationTime = 0.0f;
    coinAngle = 0.0f;
    srand(time(0));
}

void UpdateTossScreen(GameState* state) {
    Vector2 mousePos = GetMousePosition();

    if (!tossDone && !isAnimating && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        float dx1 = mousePos.x - headCenter.x;
        float dy1 = mousePos.y - headCenter.y;
        float dx2 = mousePos.x - tailCenter.x;
        float dy2 = mousePos.y - tailCenter.y;

        if ((dx1 * dx1 + dy1 * dy1) <= (coinRadius * coinRadius)) {
            userChoiceText = "You chose Heads";
            isAnimating = true;
            animationTime = 0.0f;
            coinAngle = 0.0f;
        } else if ((dx2 * dx2 + dy2 * dy2) <= (coinRadius * coinRadius)) {
            userChoiceText = "You chose Tails";
            isAnimating = true;
            animationTime = 0.0f;
            coinAngle = 0.0f;
        }
    }

    if (isAnimating) {
        float dt = GetFrameTime();
        animationTime += dt;
        coinAngle += animationSpeed * dt;

        if (coinAngle > 360.0f) coinAngle -= 360.0f;

        if (animationTime >= animationDuration) {
            tossResult = rand() % 2;
         

            bool userChoseHeads = (strcmp(userChoiceText, "You chose Heads") == 0);
            isUserWonToss = userChoseHeads ? (tossResult == 0) : (tossResult == 1);
            isAnimating = false;
            tossDone = true;
        }
    }

    if (tossDone && IsKeyPressed(KEY_ENTER)) {
         if(isUserWonToss){
        InitChooseBatBall(false); // User won toss, they choose Bat or Ball
        }
        else {
            InitChooseBatBall(true); // Computer chooses Bat or Ball
        }
        *state = STATE_BAT_BOWL;
    }
}

void DrawTossScreen(int screenWidth, int screenHeight) {
    DrawRectangleRounded(smallBackgroundRect, 0.1, 10, smallBackgroundRectColor);
    Vector2 textSize = MeasureTextEx(montserratTitle, "TOSS TIME", 40,1);
    Vector2 titlePosition = {(screenWidth - textSize.x)/2,smallBackgroundRect.y - 100};
    DrawTextEx(montserratTitle,"TOSS TIME", titlePosition, 40,1, WHITE);
    Vector2 subHeadSize = MeasureTextEx(subHeadLight,"Choose Head or Tail", 20,0); // match font size to DrawText
    float subHeadX = titlePosition.x + (textSize.x - subHeadSize.x)/2;
    DrawTextEx(subHeadLight,"Choose Head or Tail",(Vector2){ subHeadX, titlePosition.y + 50}, 20,0, YELLOW);

    if (!tossDone) {
        if (isAnimating) {
            float scaleY = fabsf(cosf(coinAngle * DEG2RAD));
            Color coinColor = (cosf(coinAngle * DEG2RAD) > 0) ? GOLD : GRAY;
            const char* faceText = (cosf(coinAngle * DEG2RAD) > 0) ? "Heads" : "Tails";

            Rectangle coin = {
                350 - coinRadius,
                300 - coinRadius * scaleY,
                coinRadius * 2,
                coinRadius * 2 * scaleY
            };

            DrawEllipse(coin.x + coinRadius, coin.y + coinRadius * scaleY, coinRadius, coinRadius * scaleY, coinColor);

            if (scaleY > 0.5f) {
                DrawText(faceText, coin.x + coinRadius - 20, coin.y + coinRadius * scaleY - 10, 20, BLACK);
            }
        } else {
            DrawRectangleRounded(HeadBtn,0.1f,5,DARKBLUE);
            DrawCircleV(headCenter, coinRadius, GOLD);
            DrawText("Heads", headCenter.x - 20, headCenter.y - 10, 20, BLACK);
            DrawCircleV(tailCenter, coinRadius, GRAY);
            DrawText("Tails", tailCenter.x - 20, tailCenter.y - 10, 20, BLACK);
        }
    } else {
        Color finalColor = (tossResult == 0) ? GOLD : GRAY;
        const char* resultFace = (tossResult == 0) ? "Heads" : "Tails";

        DrawCircleV((Vector2){350, 300}, coinRadius, finalColor);
        DrawText(resultFace, 330, 290, 20, BLACK);

        DrawText(userChoiceText, 220, 375, 20, MAROON);
        DrawText("Toss Result", 220, 200, 20, DARKGREEN);
        DrawText("Press ENTER to continue", 220, 425, 20, DARKGRAY);

        if (isUserWonToss) {
            DrawText("You WON the Toss!", 220, 400, 24, GREEN);
        } else {
            DrawText("You LOST the Toss!", 220, 400, 24, RED);
        }
    }
}
