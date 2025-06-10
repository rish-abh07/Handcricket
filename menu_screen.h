#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H
#include "raylib.h"
#include "game.h"
#include "globalsVar.h"
#include "Montserrat_Fonts.h"


void InitMenuScreen();
void UpdateMenuScreen(GameState *state);
void DrawMenuScreen(int screenWidth, int screenHeight);
void UnloadMenuScreen();

#endif
