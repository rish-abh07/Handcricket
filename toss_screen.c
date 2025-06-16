#include "toss_screen.h"
#include "choose_bat_ball.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>  // for strcmp
#include <math.h>    // for cosf, fabsf, DEG2RAD
#include "Montserrat_Fonts.h"
#include "ui_utils.h"
#include "globalsVar.h"

bool isUserWonToss = false;
bool isComputerTurn= false;
static Vector2 CoinCenter = {250, 300};
static float coinRadius = 60.0f;

static bool tossDone = false;
static int tossResult = -1;

static const char *userChoiceText = NULL;

static float coinAngle = 0.0f;
static float animationSpeed = 720.0f; // degrees per second
static float animationTime = 0.0f;
static float animationDuration = 1.0f; // seconds
static bool isAnimating = false;


static Rectangle HeadBtn = { 100,100,100,40};
static Rectangle TailBtn = { 100,100,100,40};
static Rectangle contBtn = {100,100,140,40};
static Rectangle smallBackgroundRect={ 250, 250, 400, 300 };
Color smallBackgroundRectColor = (Color){104, 50, 146, 255};
static Texture2D headIcon;
static Texture2D tailIcon; 
void InitTossScreen(void) {
  headIcon = LoadTexture("asset/Head.png");
tailIcon = LoadTexture("asset/Tail.png");
    coinRadius = 0.1*GetScreenHeight();
    int gap = 0.020 * GetScreenWidth();
    tossDone = false;
    tossResult = -1;
    isUserWonToss = false;
    isComputerTurn= false;
       userChoiceText = NULL; 
    //uilogic
    smallBackgroundRect.width = GetScreenWidth()/2.5;
    smallBackgroundRect.height = GetScreenHeight()/2.5;
    smallBackgroundRect.x = (GetScreenWidth()-smallBackgroundRect.width)/2;
    smallBackgroundRect.y = (GetScreenHeight()-smallBackgroundRect.height)/2;
    HeadBtn.width = smallBackgroundRect.width/3;
    HeadBtn.x = smallBackgroundRect.x + (smallBackgroundRect.width/2 - HeadBtn.width)/2 +gap ;
    HeadBtn.y = smallBackgroundRect.y + (smallBackgroundRect.height - HeadBtn.height)*0.80;
    TailBtn.width = HeadBtn.width;
    TailBtn.x = smallBackgroundRect.x + 0.75*smallBackgroundRect.width - TailBtn.width/2 - gap;
    TailBtn.y = HeadBtn.y;
    //continuie btn 
    HeadBtn.width = smallBackgroundRect.width/3;
    contBtn.x = smallBackgroundRect.x + (smallBackgroundRect.width - contBtn.width)/2;


    //coin logic 
    CoinCenter.x = smallBackgroundRect.x + (smallBackgroundRect.width)/2;
    CoinCenter.y = smallBackgroundRect.y  + (HeadBtn.y-smallBackgroundRect.y)/2;
    userChoiceText = NULL;
    isAnimating = false;
    animationTime = 0.0f;
    coinAngle = 0.0f;
    srand(time(0));
}

void UpdateTossScreen(GameState* state) {
    Vector2 mousePos = GetMousePosition();

    if (!tossDone && !isAnimating && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
       

        if (CheckCollisionPointRec(mousePos, HeadBtn)) {
            userChoiceText = "You chose Heads";
            isAnimating = true;
            animationTime = 0.0f;
            coinAngle = 0.0f;
        } else if (CheckCollisionPointRec(mousePos, TailBtn)) {
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

    if (tossDone && CheckCollisionPointRec(mousePos, contBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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
    DrawRectangleRounded(smallBackgroundRect, 0.2, 10, smallBackgroundRectColor);
    Vector2 textSize = MeasureTextEx(montserratTitle, "TOSS TIME", 40,1);
    Vector2 titlePosition = {(screenWidth - textSize.x)/2,smallBackgroundRect.y - 100};
    DrawTextEx(montserratTitle,"TOSS TIME", titlePosition, 40,1, WHITE);
    Vector2 subHeadSize = MeasureTextEx(subHeadLight,"Choose Head or Tail", 20,0); // match font size to DrawText
    float subHeadX = titlePosition.x + (textSize.x - subHeadSize.x)/2;
    DrawTextEx(subHeadLight,"Choose Head or Tail",(Vector2){ subHeadX, titlePosition.y + 50}, 20,0, YELLOW);

    Color hoverHead = (Color){37, 95, 235, 255};
    Color hoverTail = (Color){ 222, 41, 41,255};
    Color headColor = (Color){37, 95, 235, 230};
    Color tailsColor = (Color){222, 43, 43, 230};
    if (!tossDone) {
        if (isAnimating) {
            float scaleY = fabsf(cosf(coinAngle * DEG2RAD));
            Color coinColor = (cosf(coinAngle * DEG2RAD) > 0) ? (Color){ 255, 203, 0, 255 }  : (Color){ 255, 203, 0, 100 } ;
            const Texture2D faceTexture = (cosf(coinAngle * DEG2RAD) > 0) ? headIcon : tailIcon;

            Rectangle coin = {
                CoinCenter.x - coinRadius,
                CoinCenter.y - coinRadius * scaleY,
                coinRadius * 2,
                coinRadius * 2 * scaleY
            };

            DrawEllipse(coin.x + coinRadius, coin.y + coinRadius * scaleY, coinRadius, coinRadius * scaleY, coinColor);

            if (scaleY > 0.5f) {
                Vector2 facePos = {
                    coin.x + coinRadius - faceTexture.width / 2,
                    coin.y + coinRadius * scaleY - faceTexture.height / 2
                };
                DrawTextureEx(faceTexture, facePos, 0.0f, 1.0f, WHITE);
            }
        }
        else{
            DrawCircle(CoinCenter.x,CoinCenter.y,coinRadius,GOLD);
            DrawTexture(headIcon, CoinCenter.x - headIcon.width / 2,CoinCenter.y - headIcon.height / 2,WHITE);
        }
       
    } else {
        Color finalColor = (tossResult == 0) ? (Color){ 255, 203, 0, 255 } : (Color){ 255, 203, 0, 100 };
        const Texture2D resultTexture = (tossResult == 0) ? headIcon : tailIcon;
        Vector2 resultPos = {
            CoinCenter.x - resultTexture.width / 2,
            CoinCenter.y - resultTexture.height / 2
        };
        DrawCircleV(CoinCenter, coinRadius, finalColor);
        DrawTextureEx(resultTexture, resultPos, 0.0f, 1.0f, WHITE);
        DrawRectangleRounded(contBtn, 0.3,10,DARKBLUE);
      // === TEXT SETTINGS ===
            int fontSize = 24;
            float spacing = 2.0f;

            // === TEXT CONTENT ===
            const char *labelText = "RESULT:";
            const char *resultMsg = isUserWonToss ? "YOU WON" : "YOU LOST";
            Color resultColor = isUserWonToss ? YELLOW : RED;

            // === MEASURE TEXTS ===
            Vector2 labelSize = MeasureTextEx(buttonFontMedium, labelText, fontSize, spacing);
            Vector2 resultSize = MeasureTextEx(montserratTitle, resultMsg, fontSize, spacing);
            float totalTextWidth = labelSize.x + 10 + resultSize.x;

            // === POSITION RESULT TEXT CENTERED ===
            float centerX = screenWidth / 2.0f;
            float resultY = screenHeight * 0.65f;  // Adjust vertical placement as needed
            float startX = centerX - (totalTextWidth / 2.0f);

            // === DRAW RESULT TEXT ===
            DrawTextEx(buttonFontMedium, labelText, (Vector2){startX, resultY}, fontSize, spacing, WHITE);
            DrawTextEx(montserratTitle, resultMsg, (Vector2){startX + labelSize.x + 10, resultY}, fontSize, spacing, resultColor);

            // === CONTINUE BUTTON SETTINGS ===
            contBtn.width = 180;
            contBtn.height = 45;
            contBtn.x = centerX - contBtn.width / 2;
            contBtn.y = resultY + fontSize + 20; // Spacing below result text

            // === DRAW CONTINUE BUTTON ===
            DrawRectangleRounded(contBtn, 0.3f, 10, DARKBLUE);

            // === DRAW "CONTINUE" TEXT CENTERED IN BUTTON ===
            Vector2 continueTextSize = MeasureTextEx(buttonFontMedium, "CONTINUE", 20, 2);
            DrawTextEx(buttonFontMedium, "CONTINUE",
                (Vector2){
                    contBtn.x + (contBtn.width - continueTextSize.x) / 2,
                    contBtn.y + (contBtn.height - continueTextSize.y) / 2
                },
                20, 2, WHITE);


    }
    hoverSize(HeadBtn, headColor,hoverHead, 20, 1.2f, "Head", WHITE, buttonFontMedium, 0, headIcon, 0, 0);
    hoverSize(TailBtn, tailsColor,hoverTail, 20, 1.2f, "Tail", WHITE, buttonFontMedium, 0, tailIcon, 0, 0);
}
