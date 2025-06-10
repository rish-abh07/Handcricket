#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "raylib.h"
#include "game.h"
#include "globalsVar.h"
#include "Montserrat_Fonts.h"

// Macro for loading icons from the asset directory
#define ICON_PATH "asset/"
#define LOAD_ICON(name) LoadTexture(ICON_PATH name)

// Struct for storing menu button icons
typedef struct {
    Texture2D play;
    Texture2D leaderboard;
    Texture2D settings;
    Texture2D howToPlay;
} MenuIcons;
extern MenuIcons menuIcon;
// Function declarations for menu screen lifecycle
void InitMenuScreen();
void UpdateMenuScreen(GameState *state);
void DrawMenuScreen(int screenWidth, int screenHeight);
void UnloadMenuScreen();

#endif // MENU_SCREEN_H
