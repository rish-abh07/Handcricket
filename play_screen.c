#include "play_screen.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define NUM_BUTTONS 6
#define BALLS_PER_INNINGS 6
#define MAX_HAND_CHOICES 7 // 1-6 + 0

static Texture2D handTextures[MAX_HAND_CHOICES]; // 0 to 6
static Texture2D *currentUserHand = NULL;
static Texture2D *currentComputerHand = NULL;

Rectangle buttons[NUM_BUTTONS];
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

void InitButtons() {
    int startX = 100, startY = 500, width = 50, height = 50, gap = 20;
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i] = (Rectangle){ startX + i * (width + gap), startY, width, height };
    }
}

void InituserChoiceRecord() {
    int startX = 100, startY = 100, width = 40, height = 40, gap = 10;
    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        userChoiceRecord[i] = (Rectangle){ startX + i * (width + gap), startY, width, height };
    }
}

void InitComputerChoiceRecord() {
    int startX = 400, startY = 100, width = 40, height = 40, gap = 10;
    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        computerChoiceRecord[i] = (Rectangle){ startX + i * (width + gap), startY, width, height };
    }
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
    screenMessage = "Play!";
    isUserBatting = userBatFirst;
    isFirstInnings = true;
    matchOver = false;
    currentBall = 0;
    userScore = 0;
    computerScore = 0;
    target = 0;

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
        "asset/six .png"   // index 6
    };

    for (int i = 0; i < MAX_HAND_CHOICES; i++) {
        handTextures[i] = LoadTexture(textureFiles[i]);
    }

    currentUserHand = &handTextures[0];
    currentComputerHand = &handTextures[0];
    if (handTextures[0].id == 0) {
        TraceLog(LOG_ERROR, "Failed to load hand textures!");
    }    
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
                isUserBatting = false;
                isFirstInnings = false;
                target = userScore;

                currentBall = 0;
                ballRecordReset(userBallRecord, BALLS_PER_INNINGS);
                ballRecordReset(computerBallRecord, BALLS_PER_INNINGS);

                userScore = 0;
                computerScore = 0;

                currentUserHand = &handTextures[0];
                currentComputerHand = &handTextures[0];
            } else {
                screenMessage = "You are OUT!";
                matchOver = true;
            }
            return;
        }

        userScore += userChoice;

        // Check for win if chasing
        if (!isFirstInnings && userScore > target) {
            screenMessage = "You Win!";
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
                } else if (userScore > target) {
                    screenMessage = "You Win!";
                } else {
                    screenMessage = "Computer Wins!";
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
                } else if (computerScore > target) {
                    screenMessage = "Computer Wins!";
                } else {
                    screenMessage = "You Win!";
                }
                matchOver = true;
            }
        }
    }
}

void UpdatePlayScreen(GameState *state) {
    if (matchOver) return;

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                HandleUserChoice(i + 1);
                break;
            }
        }
    }
}

void DrawPlayScreen() {
    ClearBackground(GREEN);
    for (int i = 0; i < NUM_BUTTONS; i++) {
        Color color = CheckCollisionPointRec(GetMousePosition(), buttons[i]) ? LIGHTGRAY : DARKGRAY;
        DrawRectangleRec(buttons[i], color);
        char label[2] = { '1' + i, '\0' };
        int textWidth = MeasureText(label, 20);
        DrawText(label, buttons[i].x + (buttons[i].width - textWidth)/2, buttons[i].y + 10, 20, BLACK);
    }

    for (int i = 0; i < BALLS_PER_INNINGS; i++) {
        DrawRectangleRec(userChoiceRecord[i], RED);
        DrawText(TextFormat("%d", userBallRecord[i]), userChoiceRecord[i].x + 10, userChoiceRecord[i].y + 10, 20, WHITE);

        DrawRectangleRec(computerChoiceRecord[i], BLUE);
        DrawText(TextFormat("%d", computerBallRecord[i]), computerChoiceRecord[i].x + 10, computerChoiceRecord[i].y + 10, 20, WHITE);
    }

    DrawText("Choose your shot (1-6):", 100, 350, 20, DARKBLUE);
    if (screenMessage) DrawText(screenMessage, 100, 300, 20, DARKGRAY);

    DrawText(TextFormat("Your Score: %d", userScore), 100, 200, 20, MAROON);
    DrawText(TextFormat("Computer Score: %d", computerScore), 500, 200, 20, BLUE);
    DrawText(TextFormat("Ball: %d / %d", currentBall + 1, BALLS_PER_INNINGS), 100, 250, 20, DARKGRAY);

    if (!isFirstInnings)
        DrawText(TextFormat("Target: %d", target + 1), 300, 180, 20, ORANGE);

    if (currentUserHand) DrawTexture(*currentUserHand, 150, 400, WHITE);
    if (currentComputerHand) DrawTexture(*currentComputerHand, 550, 400, WHITE);
}
