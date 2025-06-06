#include "choose_bat_ball.h"
#include "raylib.h"
#include <stdlib.h>

static Vector2 batCenter = {250, 300};
static Vector2 ballCenter = {450, 300};
static float choiceRadius = 50.0f;

static bool choiceDone = false;
static const char *choiceText = NULL;

static Texture2D batTexture;
static Texture2D ballTexture;


void InitChooseBatBall(void) {
     choiceDone = false;
    choiceText = NULL;

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
    Vector2 mousePos = GetMousePosition();

    if (!choiceDone && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        float dx1 = mousePos.x - batCenter.x;
        float dy1 = mousePos.y - batCenter.y;
        float dx2 = mousePos.x - ballCenter.x;
        float dy2 = mousePos.y - ballCenter.y;

        if ((dx1 * dx1 + dy1 * dy1) <= (choiceRadius * choiceRadius)) {
            choiceText = "You chose Bat";
            choiceDone = true;
        } else if ((dx2 * dx2 + dy2 * dy2) <= (choiceRadius * choiceRadius)) {
            choiceText = "You chose Ball";
            choiceDone = true;
        }
    }

    if (choiceDone && IsKeyPressed(KEY_ENTER)) {
        //*state = STATE_START_MATCH;  // Transition to the play state
    }
}
void DrawChooseBatBallScreen(void) {
    DrawText("Choose Bat or Ball", 220, 100, 24, DARKBLUE);

    if (!choiceDone) {
        DrawCircleV(batCenter, choiceRadius, MAROON);
        DrawTextureV(batTexture, (Vector2){batCenter.x - batTexture.width/2, batCenter.y - batTexture.height/2},  WHITE);
        DrawCircleV(ballCenter, choiceRadius, BLUE);
        DrawTextureV(ballTexture, (Vector2){ballCenter.x - ballTexture.width/2, ballCenter.y - ballTexture.height/2}, WHITE);
    } else {
        DrawText(choiceText, 220, 375, 20, MAROON);
        DrawText("Press ENTER to continue", 220, 425, 20, DARKGRAY);
    }
}