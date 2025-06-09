#include "choose_bat_ball.h"
#include "toss_screen.h"
#include "play_screen.h"

static const char *computerChoice = NULL;

bool computerTurn = false;
static Vector2 orignalBatCenter = {250, 300};
static Vector2 orignalBallCenter = {450, 300};
static Vector2 batCenter = {250, 300};
static Vector2 ballCenter = {450, 300};
static float choiceRadius = 50.0f;

static bool choiceDone = false;
static const char *choiceText = NULL;
static bool isUserBat = false;
static Texture2D batTexture;
static Texture2D ballTexture;


void InitChooseBatBall(bool isComputerTurn) {
    choiceDone = false;
    choiceText = NULL;

   
    if (isComputerTurn) {
        // Computer randomly chooses Bat or Ball
        if (rand() % 2 == 0) {
            computerChoice = "Bat";
            batCenter.x += 100; // Move bat right for display
            isUserBat = false; // Computer chose Bat
        } else {
            computerChoice = "Ball";
            ballCenter.x -= 100; // Move ball left for display
            isUserBat = true; // Computer chose Ball
        }
        computerTurn = true ;
        choiceDone = true; // Choice made by computer automatically
    }
    else {
        // Reset positions for user choice
        batCenter = orignalBatCenter;
        ballCenter = orignalBallCenter;
        choiceText = "Choose Bat or Ball";
    }
     batTexture = LoadTexture("asset/bat.png");

    ballTexture = LoadTexture("asset/ball.png");

    if (batTexture.id == 0 || ballTexture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load textures!");
    }
}

void UnloadChooseBatBall(void) {
    // Unload textures to free memory
    UnloadTexture(batTexture);
    UnloadTexture(ballTexture);
}

void UpdateChooseBatBall(GameState *state) {
        if(!computerTurn)
    {
            Vector2 mousePos = GetMousePosition();

            if (!choiceDone && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                float dx1 = mousePos.x - batCenter.x;
                float dy1 = mousePos.y - batCenter.y;
                float dx2 = mousePos.x - ballCenter.x;
                float dy2 = mousePos.y - ballCenter.y;

                if ((dx1 * dx1 + dy1 * dy1) <= (choiceRadius * choiceRadius)) {
                    choiceText = "You chose Bat";
                    isUserBat = true; // User chose Bat
                    choiceDone = true;
                } else if ((dx2 * dx2 + dy2 * dy2) <= (choiceRadius * choiceRadius)) {
                    choiceText = "You chose Ball";
                    isUserBat = false; // User chose Ball
                    choiceDone = true;
                }
            }
    }

    if (choiceDone && IsKeyPressed(KEY_ENTER)) {
        *state = STATE_PLAY;  // Transition to the play state
        UnloadChooseBatBall(); // Unload textures before transitioning
        // Initialize play screen with the user's choice
        InitPlayScreen(isUserBat); // Pass the turn based on who chose
    }
}
void DrawChooseBatBallScreen(void) {
   
        if(computerTurn){
            DrawText("Computer Choice:", 220, 100, 24, DARKBLUE);
            if(strcmp(computerChoice, "Bat") == 0) {
                DrawText("Bat", 220, 150, 20, MAROON);
                DrawCircleV(batCenter, choiceRadius, MAROON);
                DrawTextureV(batTexture, (Vector2){batCenter.x - batTexture.width/2, batCenter.y - batTexture.height/2},  WHITE);
            } else if(strcmp(computerChoice, "Ball") == 0) {
                DrawText("Ball", 220, 150, 20, MAROON);
                DrawCircleV(ballCenter, choiceRadius, BLUE);
                DrawTextureV(ballTexture, (Vector2){ballCenter.x - ballTexture.width/2, ballCenter.y - ballTexture.height/2}, WHITE);
            }
            DrawText("Press ENTER to continue", 220, 425, 20, DARKGRAY);
        }
        else{
            DrawText("Choose Bat or Ball", 220, 100, 24, DARKBLUE);
            if (!choiceDone) {
                DrawCircleV(batCenter, choiceRadius, MAROON);
                DrawTextureV(batTexture, (Vector2){batCenter.x - batTexture.width/2, batCenter.y - batTexture.height/2},  WHITE);
                DrawCircleV(ballCenter, choiceRadius, BLUE);
                DrawTextureV(ballTexture, (Vector2){ballCenter.x - ballTexture.width/2, ballCenter.y - ballTexture.height/2}, WHITE);
            } 
            else {
                if(strcmp(choiceText, "You chose Bat") == 0) {
                    DrawCircleV(batCenter, choiceRadius, MAROON);
                    DrawTextureV(batTexture, (Vector2){batCenter.x - batTexture.width/2, batCenter.y - batTexture.height/2}, WHITE);
                } else if(strcmp(choiceText, "You chose Ball") == 0) {
                    DrawCircleV(ballCenter, choiceRadius, BLUE);
                    DrawTextureV(ballTexture, (Vector2){ballCenter.x - ballTexture.width/2, ballCenter.y - ballTexture.height/2}, WHITE);
                }
                DrawText(choiceText, 220, 375, 20, MAROON);
                DrawText("Press ENTER to continue", 220, 425, 20, DARKGRAY);
            }
        }
}