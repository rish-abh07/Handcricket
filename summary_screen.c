#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Montserrat_Fonts.h"
#include "game.h"
#include "globalsVar.h"

#define MAX_NAME_LENGTH 32
int playerScore = 0;
int compScore = 0;
// Global for name input
char playerName[MAX_NAME_LENGTH + 1] = "";

// Internal state
static bool isAwaitingNameInput = false;
static const char *smsg = NULL;
static char summaryMessage[128] = "";
// Called once when entering summary screen
void InitSummaryScreen(const char *msg, int uScore, int cScore) {
    smsg = msg;
    strncpy(summaryMessage, msg, sizeof(summaryMessage));
    summaryMessage[sizeof(summaryMessage) - 1] = '\0';  // Ensure null termination
    smsg = summaryMessage;
    // Use global variables so they can be shared across files
    playerScore = uScore;
    compScore = cScore;

    HighScoreEntry saved = LoadHighScore();
    if (playerScore > saved.score) {
        isAwaitingNameInput = true;
        playerName[0] = '\0';  // Reset name input
    } else {
        isAwaitingNameInput = false;
    }
}

// Called every frame
void UpdateSummaryScreen(GameState *state) {
    if (isAwaitingNameInput) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (strlen(playerName) < MAX_NAME_LENGTH)) {
                int len = strlen(playerName);
                playerName[len] = (char)key;
                playerName[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(playerName);
            if (len > 0) playerName[len - 1] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER) && strlen(playerName) > 0) {
            SaveHighScore(playerScore, playerName);
            isAwaitingNameInput = false;
        }

        return; // Skip main menu button check while typing
    }

    Vector2 mouse = GetMousePosition();
    Rectangle btn = { screenWidth / 2 - 75, screenHeight - 100, 150, 40 };

    if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *state = STATE_MENU;
    }
}

// Called every frame
void DrawSummaryScreen(void) {
    DrawTextEx(montserratTitle, "MATCH SUMMARY", 
        (Vector2){ screenWidth / 2 - 150, 50 }, 40, 1, YELLOW);

    if (smsg) {
        DrawTextEx(montserratTitle, smsg,
            (Vector2){ screenWidth / 2 - MeasureTextEx(montserratTitle, smsg, 30, 1).x / 2, 120 },
            30, 1, WHITE);
    }

    DrawTextEx(buttonFontMedium, 
        TextFormat("Your Score: %d runs", playerScore),
        (Vector2){ 100, 200 }, 20, 1, GREEN);

    DrawTextEx(buttonFontMedium, 
        TextFormat("Computer Score: %d runs", compScore),
        (Vector2){ 100, 240 }, 20, 1, RED);

    if (isAwaitingNameInput) {
        DrawText("New High Score! Enter your name:", 100, 300, 20, YELLOW);
        DrawRectangleLines(100, 340, 300, 40, WHITE);
        DrawText(playerName, 110, 350, 20, GREEN);
    } else {
        HighScoreEntry high = LoadHighScore();
        DrawText(TextFormat("High Score: %s - %d", high.name, high.score),
                 100, 300, 30, YELLOW);

        DrawRectangleRounded(
            (Rectangle){ screenWidth / 2 - 75, screenHeight - 100, 150, 40 }, 
            0.2f, 10, ORANGE);

        DrawTextEx(buttonFontMedium, "Main Menu", 
            (Vector2){ screenWidth / 2 - 50, screenHeight - 90 }, 20, 1, WHITE);
    }
}
