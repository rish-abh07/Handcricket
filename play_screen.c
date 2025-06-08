#include "play_screen.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define NUM_BUTTONS 6
Rectangle buttons[NUM_BUTTONS]; // Array to hold button rectangles
Rectangle userChoiceRecord[NUM_BUTTONS];
Rectangle computerChoiceRecord[NUM_BUTTONS];
void InitButtons() {
    // Initialize button rectangles for shots 1 to 6
    int startX = 100; // Starting X position for buttons
    int startY = 400;
    int width = 50;
    int height = 50;
    int gap = 20; // Gap between button
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].x = startX + i * (width+gap); // Position buttons horizontally
        buttons[i].y = 500;           // Fixed vertical position
        buttons[i].width = width;        // Button width
        buttons[i].height = height;       // Button height
    }
}
void InituserChoiceRecord(){
     int startX = 100; // Starting X position for buttons
    int startY = 100;
    int width = 40;
    int height = 40;
    int gap = 10; // Gap between button
    for (int i = 0; i < NUM_BUTTONS; i++) {
        userChoiceRecord[i].x = startX + i * (width+gap); // Position buttons horizontally
        userChoiceRecord[i].y = 100;           // Fixed vertical position
        userChoiceRecord[i].width = width;        // Button width
        userChoiceRecord[i].height = height;       // Button height
    }
}
void InitComputerChoiceRecord(){
     int startX = 400; // Starting X position for buttons
    int startY = 100;
    int width = 40;
    int height = 40;
    int gap = 10; // Gap between button
    for (int i = 0; i < NUM_BUTTONS; i++) {
        computerChoiceRecord[i].x = startX + i * (width+gap); // Position buttons horizontally
        computerChoiceRecord[i].y = 100;           // Fixed vertical position
        computerChoiceRecord[i].width = width;        // Button width
        computerChoiceRecord[i].height = height;       // Button height
    }
}

int userChoosedNumber;
int userBallRecord[NUM_BUTTONS]; // Record of user's choices
int computerBallRecord[NUM_BUTTONS]; // Record of user's choices
static int shot = 0; // User's shot choice
static int ball =0;
static int ballsRemaining = 6; // Number of balls per innings
static int currentBall = 0; // Current ball index
static int target = 0;
static int userScore = 0; // User's score
static int computerScore = 0; // Computer's score
static bool isUserBatting = false; // Flag to check if user is batting
static bool isFirstInnings = true; // Flag to check if it's the first innings
static bool inputReceived = false; // Flag to check if input is received

static int innings = 0; // Current innings number
static bool turnComplete = false;
static bool matchOver = false;

static const char *screenMessage = NULL; // Message to display on the screen

void InitPlayScreen(bool userBatFirst) {
    screenMessage = "Play!!!";
    isUserBatting = userBatFirst; // Set batting status based on user choice
    InitButtons(); // Initialize buttons for shots
    InituserChoiceRecord(); // Initialize user choice record
    InitComputerChoiceRecord();
}

int iswicket(int shot, int ball) {
    // Simple wicket logic: if shot equals ball, it's a wicket
    if(shot == ball) {
        return 1; // Wicket
    }
    else {
        return 0; // Not a wicket
    }
}
void HandleUserChoice(int userChoice){
      if (matchOver) return; // safety

    if (isUserBatting) {
        shot = userChoice;
        ball = ComputerChoice();

        if (iswicket(shot, ball)) {
            screenMessage = "WICKET! Switching sides.";
            isUserBatting = false;
            isFirstInnings = false;
            currentBall = 0;
        } else {
            userScore += shot;
            userBallRecord[currentBall] = shot;
            computerBallRecord[currentBall] = ball;
            currentBall++;
        }

        if (currentBall >= ballsRemaining) {
            // End of first innings
            if (isFirstInnings) {
                isUserBatting = false;
                isFirstInnings = false;
                currentBall = 0;
                screenMessage = "Innings over! Now Computer bats.";
            } else if (userScore > target) {
                matchOver = true;
                screenMessage = "You Win!";
            }
        }

    } else { // computer batting
        ball = userChoice;
        shot = ComputerChoice();

        if (iswicket(shot, ball)) {
            screenMessage = "WICKET! Match Over.";
            matchOver = true;
        } else {
            computerScore += shot;
            userBallRecord[currentBall] = ball;
            computerBallRecord[currentBall] = shot;
            currentBall++;
        }

        if (currentBall >= ballsRemaining) {
            if (computerScore > userScore) {
                screenMessage = "Computer Wins!";
            } else if (computerScore == userScore) {
                screenMessage = "Match Tied!";
            } else {
                screenMessage = "You Win!";
            }
            matchOver = true;
        }
    }

    // Update target after first innings ends
    if (!isFirstInnings) {
        target = isUserBatting ? computerScore : userScore;
    }  
}
int ComputerChoice() {
    // Simple computer choice logic: random shot or ball
    return rand() % 6 + 1; // Returns a random number between 1 and 6
}
void UpdatePlayScreen(GameState *state) {
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                int userChoice = i + 1; // User chose number (1 to 6)
                // Use userChoice for your game logic
                HandleUserChoice(userChoice);
                if(innings >= 2) {
                    matchOver = true; // End the match after two innings
                    screenMessage = "Match Over!!!!!Press Enter to see the summary.";
                    //*state = STATE_SUMMARY; // Transition to summary state
                } else {
                    screenMessage = isUserBatting ? "Your turn ended, now it's computer's turn." : "Computer's turn ended, now it's your turn.";
                    target = isUserBatting ? userScore : computerScore; // Update target based on current batting player
                }
                break;
            }
        }
    }

}
void DrawPlayScreen(){
    for(int i=0;i<NUM_BUTTONS;i++){
    Color btnColor= CheckCollisionPointRec(GetMousePosition(), buttons[i]) ? LIGHTGRAY : DARKGRAY;
    DrawRectangleRec(buttons[i], btnColor);


    char numText[2] = { '1' + i, '\0' };
    int textWidth = MeasureText(numText, 20);
    DrawText(numText, buttons[i].x + (buttons[i].width - textWidth) / 2, buttons[i].y + 10, 20, BLACK);
    }
    for(int i=0;i<NUM_BUTTONS;i++){
        DrawRectangleRec(userChoiceRecord[i], RED);
        int textWidth = MeasureText(TextFormat("%d", userBallRecord[i]), 20);
        DrawText(TextFormat("%d", userBallRecord[i]), userChoiceRecord[i].x + (userChoiceRecord[i].width - textWidth)/2, userChoiceRecord[i].y + 10, 20, WHITE);
    }
    for(int i=0;i<NUM_BUTTONS;i++){
        DrawRectangleRec(computerChoiceRecord[i], RED);
        int textWidth = MeasureText(TextFormat("%d", computerBallRecord[i]), 20);
        DrawText(TextFormat("%d", computerBallRecord[i]), computerChoiceRecord[i].x + (computerChoiceRecord[i].width - textWidth)/2, computerChoiceRecord[i].y + 10, 20, WHITE);
    }
    DrawText("Choose your shot (1-6):", 100, 350, 20, DARKBLUE);
    if (screenMessage != NULL) {
        DrawText(screenMessage, 100, 300, 20, DARKGRAY);
    }
    DrawText(TextFormat("Your Score: %d", userScore), 100, 200, 20, MAROON);
    DrawText(TextFormat("Computer Score: %d", computerScore), 500, 200, 20, BLUE);
    DrawText(TextFormat("Innings: %d", innings + 1), 100, 250, 20, DARKGREEN);
    DrawText(TextFormat("Balls Delivered: %d", currentBall), 500, 250, 20, DARKGRAY);
    DrawText(TextFormat("Balls remaining: %d", ballsRemaining), 500, 300, 20, DARKGRAY);
    if (!isFirstInnings) {
    DrawText(TextFormat("Target: %d", target + 1), 300, 180, 20, ORANGE);
    }
}

