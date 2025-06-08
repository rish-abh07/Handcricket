#include "play_screen.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define NUM_BUTTONS 6
Rectangle buttons[NUM_BUTTONS]; // Array to hold button rectangles

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

void InitPlayScreen(bool userBatFirst) {
   
    isUserBatting = userBatFirst; // Set batting status based on user choice
    InitButtons(); // Initialize buttons for shots
}

int iswicket(int shot, int ball) {
    // Simple wicket logic: if shot equals ball, it's a wicket
    return (shot == ball);
}

void UpdatePlayScreen(GameState *state) {
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < NUM_BUTTONS; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                int userChoice = i + 1; // User chose number (1 to 6)

                // Use userChoice for your game logic
                HandleUserChoice(userChoice);
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
}

void HandleUserChoice(int userChoice){
    if(currentBall >= ballsRemaining) {
        // If all balls are played, end the innings
        turnComplete = true;
        return;
    }
    if(isUserBatting){
        shot = userChoice; // User's shot choice
        ball = ComputerChoice(); // Computer's ball choice
        currentBall++; // Increment the current ball index
    }
    else {
        ball = userChoice; // User's ball choice
        shot = ComputerChoice(); // Computer's shot choice
        currentBall++; // Increment the current ball index
    }    
}
int ComputerChoice() {
    // Simple computer choice logic: random shot or ball
    return rand() % 6 + 1; // Returns a random number between 1 and 6
}