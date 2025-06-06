#include "game.h"
#include "raylib.h"
int main(void) {
    InitGame();  // Initialize the game
    while (!WindowShouldClose()) {  // Main game loop
        UpdateGame();  // Update game logic
        DrawGame();    // Draw the current state
    }
   // CloseGame();  // Close the game and clean up resources
    return 0;
}


