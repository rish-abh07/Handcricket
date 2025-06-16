#include "play_screen.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include "ui_utils.h"
#include "Montserrat_Fonts.h"
#include "math.h"
#include "summary_screen.h"
#define NUM_BUTTONS 6
#define BALLS_PER_INNINGS 6
#define MAX_HAND_CHOICES 7 // 1-6 + 0
static Texture2D homeIcon ;
static Texture2D handTextures[MAX_HAND_CHOICES]; // 0 to 6
static Texture2D *currentUserHand = NULL;
static Texture2D *currentComputerHand = NULL;

int userInnings1Score = 0;
int computerInnings1Score = 0;


Rectangle buttons[NUM_BUTTONS];
Color buttonColors[NUM_BUTTONS] = {
    BLUE, GREEN, YELLOW, ORANGE, RED, PURPLE
};
Rectangle userChoiceRecord[BALLS_PER_INNINGS];
Rectangle computerChoiceRecord[BALLS_PER_INNINGS];

int userBallRecord[BALLS_PER_INNINGS] = {0};
int computerBallRecord[BALLS_PER_INNINGS] = {0};

static int currentBall = 0;
static int userScore = 0;
static int computerScore = 0;
static int target = 0;

static bool isUserBatting = false;
static bool isFirstInnings = true;
static bool matchOver = false;

static const char *screenMessage = NULL;
static Rectangle scoreBackgroundRect={ 250, 250, 400, 300 };
static Rectangle headingBackgroundRect={ 250, 250, 400, 300 };

static bool showWicketPopup = false;
static float wicketPopupTimer = 0.0f;
const float WICKET_POPUP_DURATION = 1.5f;  // seconds
  Rectangle backButton = {
       0,0,0,0
    };

void InitButtons() {
    int width = 50, height = 50, gap = 20;
    int totalWidth = NUM_BUTTONS * width + (NUM_BUTTONS - 1) * gap;

    int startX = (GetScreenWidth() - totalWidth) / 2;
    int startY = GetScreenHeight() - 150;  // 100 pixels above bottom

    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i] = (Rectangle){ startX + i * (width + gap), startY, width, height };
    }
}


void InituserChoiceRecord() {
    int startX = 25, startY = headingBackgroundRect.y+headingBackgroundRect.height + 5, width = 40, height = 40, gap = 10;
    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        userChoiceRecord[i] = (Rectangle){ startX + i * (width + gap), startY, width, height };
    }
}

void InitComputerChoiceRecord() {
    int width = 40, height = 40, gap = 10;
    int totalWidth = BALLS_PER_INNINGS * width + (BALLS_PER_INNINGS - 1) * gap;
    int startX = GetScreenWidth() - totalWidth - 25;
    int startY = headingBackgroundRect.y + headingBackgroundRect.height + 5;

    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        computerChoiceRecord[i] = (Rectangle){ startX + i * (width + gap), startY, width, height };
    }
}
void DrawAndHandleBackToHomeButton() {
  

    hoverSize( backButton, DARKGRAY, LIGHTGRAY, 20, 1.2f, "MENU", WHITE, buttonFontMedium, 0, homeIcon, 0,0);
   
    
}


int ComputerChoice() {
    return rand() % 6 + 1;
}

bool isWicket(int shot, int ball) {
    return shot == ball;
}

void HandtextureLoad(int choice, bool isUser) {
    if (choice < 0 || choice > 6) choice = 0;
    if (isUser) {
        currentUserHand = &handTextures[choice];
    } else {
        currentComputerHand = &handTextures[choice];
    }
}

void UnloadHandTextures() {
    for (int i = 0; i < MAX_HAND_CHOICES; i++) {
        UnloadTexture(handTextures[i]);
    }
}

void InitPlayScreen(bool userBatFirst) {
    if(userBatFirst){
        screenMessage = "You Are batting" ;}
    else{
        screenMessage = "Computer is batting";
    }
    isUserBatting = userBatFirst;
    isFirstInnings = true;
    matchOver = false;
    currentBall = 0;
    userScore = 0;
    computerScore = 0;
    target = 0;
        scoreBackgroundRect.width = GetScreenWidth();
        scoreBackgroundRect.height = GetScreenWidth()/8.0f;
        scoreBackgroundRect.x = (GetScreenWidth()-scoreBackgroundRect.width)/2;
        scoreBackgroundRect.y = 0;
        headingBackgroundRect.width = GetScreenWidth();
        headingBackgroundRect.height = GetScreenWidth()/18.0f;
        headingBackgroundRect.x = scoreBackgroundRect.x;
        headingBackgroundRect.y = scoreBackgroundRect.y+scoreBackgroundRect.height;
    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        userBallRecord[i] = 0;
        computerBallRecord[i] = 0;
    }

    InitButtons();
    InituserChoiceRecord();
    InitComputerChoiceRecord();

    const char *textureFiles[MAX_HAND_CHOICES] = {
        "asset/zero.png",  // index 0
        "asset/One.png",   // index 1
        "asset/two.png",   // index 2
        "asset/three.png", // index 3
        "asset/four.png",  // index 4
        "asset/five.png",  // index 5
        "asset/six.png"   // index 6
    };
    homeIcon = LoadTexture("asset/home.png");

    for (int i = 0; i < MAX_HAND_CHOICES; i++) {
        handTextures[i] = LoadCircularTexture(textureFiles[i]);
    }
    handTextures[0] = LoadCircularTexture(textureFiles[0]);
    currentUserHand = &handTextures[0];
    currentComputerHand = &handTextures[0];
    if (handTextures[0].id == 0) {
        TraceLog(LOG_ERROR, "Failed to load hand textures!");
    }    
         backButton.x =(GetScreenWidth() - 100) / 2;
                 backButton.y = GetScreenHeight() - 75;
        backButton.width = 100;
        backButton.height = 40;
}


void ballRecordReset(int *record, int size) {
    for (int i = 0; i < size; i++) {
        record[i] = 0;
    }
}

void HandleUserChoice(int userChoice) {
    if (matchOver || currentBall >= BALLS_PER_INNINGS) return;

    int computerChoice = ComputerChoice();

    if (isUserBatting) {
        userBallRecord[currentBall] = userChoice;
        computerBallRecord[currentBall] = computerChoice;
        HandtextureLoad(userChoice, true);
        HandtextureLoad(computerChoice, false);

        if (isWicket(userChoice, computerChoice)) {
            if (isFirstInnings) {
                screenMessage = "WICKET! Now Computer bats.";
                showWicketPopup = true;
                wicketPopupTimer = WICKET_POPUP_DURATION;
                isUserBatting = false;
                isFirstInnings = false;
                target = userScore;
                userInnings1Score = userScore;
          
                currentBall = 0;
                ballRecordReset(userBallRecord, BALLS_PER_INNINGS);
                ballRecordReset(computerBallRecord, BALLS_PER_INNINGS);

                userScore = 0;
                computerScore = 0;
               
                currentUserHand = &handTextures[0];
                currentComputerHand = &handTextures[0];
            } else {
                screenMessage = "You are OUT!";
                showWicketPopup = true;
                wicketPopupTimer = WICKET_POPUP_DURATION;
                matchOver = true;
            }
            return;
        }

        userScore += userChoice;

        // Check for win if chasing
        if (!isFirstInnings && userScore > target) {
           screenMessage = "You Win!";
             InitSummaryScreen(screenMessage, userScore, computerInnings1Score);
              matchOver = true;
             return;
        }

        currentBall++;

        if (currentBall == BALLS_PER_INNINGS) {
            if (isFirstInnings) {
                screenMessage = "Innings Over. Now Computer bats.";
                isUserBatting = false;
                isFirstInnings = false;
                target = userScore;
                userInnings1Score = userScore;
                currentBall = 0;
                ballRecordReset(userBallRecord, BALLS_PER_INNINGS);
                ballRecordReset(computerBallRecord, BALLS_PER_INNINGS);

                userScore = 0;
                computerScore = 0;

                currentUserHand = &handTextures[0];
                currentComputerHand = &handTextures[0];
            } else {
                // Second innings ends, decide winner
                if (userScore == target) {
                    screenMessage = "Match Tied!";
                    InitSummaryScreen(screenMessage,userScore,computerInnings1Score);
                } else if (userScore > target) {
                    screenMessage = "You Win!";
                    InitSummaryScreen(screenMessage,userScore,computerInnings1Score);
                } else {
                    screenMessage = "Computer Wins!";
                     InitSummaryScreen(screenMessage,userScore,computerInnings1Score);
                }
                matchOver = true;
            }
        }

    } else { // computer is batting
        userBallRecord[currentBall] = userChoice;
        computerBallRecord[currentBall] = computerChoice;
        HandtextureLoad(userChoice, true);
        HandtextureLoad(computerChoice, false);

        if (isWicket(computerChoice, userChoice)) {
            if (isFirstInnings) {
                screenMessage = "WICKET! Now You bat.";
                isUserBatting = true;
                isFirstInnings = false;
                target = computerScore;
                computerInnings1Score = computerScore;
                currentBall = 0;
                ballRecordReset(userBallRecord, BALLS_PER_INNINGS);
                ballRecordReset(computerBallRecord, BALLS_PER_INNINGS);

                userScore = 0;
                computerScore = 0;

                currentUserHand = &handTextures[0];
                currentComputerHand = &handTextures[0];
            } else {
                screenMessage = "Computer OUT!";
                matchOver = true;
            }
            return;
        }

        computerScore += computerChoice;

        // Check for win if chasing
        if (!isFirstInnings && computerScore > target) {
            screenMessage = "Computer Wins!";
              InitSummaryScreen(screenMessage, userInnings1Score, computerScore);
              matchOver = true;
             return;
        }

        currentBall++;

        if (currentBall == BALLS_PER_INNINGS) {
            if (isFirstInnings) {
                screenMessage = "Innings Over. Now You bat.";
                isUserBatting = true;
                isFirstInnings = false;
                target = computerScore;

                currentBall = 0;
                ballRecordReset(userBallRecord, BALLS_PER_INNINGS);
                ballRecordReset(computerBallRecord, BALLS_PER_INNINGS);

                userScore = 0;
                computerScore = 0;

                currentUserHand = &handTextures[0];
                currentComputerHand = &handTextures[0];
            } else {
                // Second innings ends, decide winner
                if (computerScore == target) {
                   
                    screenMessage = "Match Tied!";
                    InitSummaryScreen(screenMessage,userInnings1Score,computerScore);
                } else if (computerScore > target) {
                    screenMessage = "Computer Wins!";
                    InitSummaryScreen(screenMessage,userInnings1Score,computerScore);
                } else {
                    screenMessage = "You Win!";
                    InitSummaryScreen(screenMessage,userInnings1Score,computerScore);
                }
                matchOver = true;
            }
        }
    }
}

void UpdatePlayScreen(GameState *state) {
    if (showWicketPopup) {
        wicketPopupTimer -= GetFrameTime();
        if (wicketPopupTimer <= 0) {
            showWicketPopup = false;
        }
    }
     if (matchOver && !showWicketPopup) {
    *state = STATE_SUMMARY;
    return;
}

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                HandleUserChoice(i + 1);
                break;
            }
        }
        if(CheckCollisionPointRec(GetMousePosition(),backButton)){
             *state = STATE_MENU;
        }
    }
   
    
 
    
}

void DrawPlayScreen() {
    DrawHorizontalGradientBox(scoreBackgroundRect, (Color) {23, 41, 97, 255}, (Color){ 60, 21, 95, 255}, 0.0f, (Color){84,24,135,255}) ;
    DrawHorizontalGradientBox(headingBackgroundRect, (Color) {59, 55, 157, 255}, (Color){ 75, 45, 155, 255}, 0.0f, (Color){84,24,135,255}) ;
     Vector2 textCentreBall = TextCentrPos("BALL",buttonFontMedium,scoreBackgroundRect,20,0,0,-25,WHITE);
     DrawTextEx(buttonFontMedium, "BALL",textCentreBall, 20, 0, WHITE);
     DrawTextEx(montserratTitle,TextFormat("%d/%d",currentBall,BALLS_PER_INNINGS),(Vector2){textCentreBall.x-10.0f,textCentreBall.y+20}, 40, 0, WHITE);
     DrawTextEx(buttonFontMedium,"Your Score",(Vector2){25,textCentreBall.y}, 20,0, MAROON);
     DrawTextEx(montserratTitle,TextFormat("%d",userScore),(Vector2){50,textCentreBall.y+20}, 40, 0, RED);
     DrawTextEx(buttonFontMedium,"Computer Score",(Vector2){scoreBackgroundRect.width - 150, textCentreBall.y}, 20,0, BLUE);
     DrawTextEx(montserratTitle,TextFormat("%d",computerScore),(Vector2){ scoreBackgroundRect.width - 75,textCentreBall.y+20}, 40, 0, BLUE);
     //DrawText("Choose your shot (1-6):", 100, 350, 20, DARKBLUE);
    for (int i = 0; i < NUM_BUTTONS; i++) {
      Color baseColor = buttonColors[i];
      Color hoverColor = Fade(baseColor, 0.7f); // Slight fade on hover

      Color drawColor = CheckCollisionPointRec(GetMousePosition(), buttons[i]) ? hoverColor : baseColor;

      DrawRectangleRounded(buttons[i], 0.3f, 10, drawColor);

      char label[2] = { '1' + i, '\0' };
      Vector2 textWidth = MeasureTextEx(buttonFontMedium,label, 20, 0 );
      DrawTextEx(buttonFontMedium,label,(Vector2) {buttons[i].x + (buttons[i].width - textWidth.x)/2, buttons[i].y + 15}, 20, 0, BLACK);
    }

    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        DrawRectangleRounded(userChoiceRecord[i], 0.2, 5, RED);
        DrawTextEx(buttonFontMedium,TextFormat("%d", userBallRecord[i]),(Vector2){ userChoiceRecord[i].x + 10, userChoiceRecord[i].y + 10}, 20, 0, WHITE);

        DrawRectangleRounded(computerChoiceRecord[i], 0.2, 5,BLUE);
        DrawTextEx(buttonFontMedium,TextFormat("%d", computerBallRecord[i]),(Vector2){ computerChoiceRecord[i].x + 10, computerChoiceRecord[i].y + 10}, 20, 0, WHITE);
    }
    if (showWicketPopup) {
    const char* wicketText = "WICKET!";
    int fontSize = 40;
    int textWidth = MeasureText(wicketText, fontSize);
    Vector2 center = { GetScreenWidth()/2.0f - textWidth/2.0f, GetScreenHeight()/2.0f - fontSize/2.0f };
    
         DrawText(wicketText, center.x, center.y, fontSize, RED);
    }
    
    if (screenMessage) {
        Vector2 textSize = MeasureTextEx(buttonFontMedium, screenMessage, 20, 0);
        DrawTextEx(buttonFontMedium, screenMessage, (Vector2){headingBackgroundRect.x + (headingBackgroundRect.width - textSize.x)/2, 
        headingBackgroundRect.y + (headingBackgroundRect.height - textSize.y)/2}, 20,0, WHITE);}
    if (!isFirstInnings){
        Vector2 textPos = TextCentrPos("Target:%d",buttonFontMedium,scoreBackgroundRect,20,0,0,+35,WHITE);
        DrawTextEx(buttonFontMedium, TextFormat("Target: %d",target), textPos, 20, 0, WHITE);
    }
     int spacing = 100;
    int handY = (buttons[0].y - currentUserHand->height) / 2 + currentUserHand->height/1.5;

    if (currentUserHand) {
        int userX = GetScreenWidth() / 4.5 - currentUserHand->width / 2;
        int userCenterX = userX + currentUserHand->width / 2;
        int centerY = handY + currentUserHand->height / 2;

        DrawCircle(userCenterX, centerY, currentUserHand->width / 1.5, RED);
        DrawTexture(*currentUserHand, userX, handY, WHITE);
    }

    if (currentComputerHand) {
        int compX = GetScreenWidth() * .78 - currentComputerHand->width / 2;
        int compCenterX = compX + currentComputerHand->width / 2;
        int centerY = handY + currentComputerHand->height / 2;

        DrawCircle(compCenterX, centerY, currentComputerHand->width / 1.5, BLUE);
        DrawTexture(*currentComputerHand, compX, handY, WHITE);
    }

DrawAndHandleBackToHomeButton();

}


