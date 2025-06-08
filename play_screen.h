#ifndef PLAY_SCREEN_H
#define PLAY_SCREEN_H

#include "raylib.h"
#include "game.h"

void InitPlayScreen(bool userBatFirst);
void UpdatePlayScreen(GameState *state);
void DrawPlayScreen(void);

#endif