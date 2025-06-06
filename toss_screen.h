#ifndef TOSS_SCREEN_H
#define TOSS_SCREEN_H

#include "raylib.h"
#include "game.h"

extern bool isUserWonToss;
void InitTossScreen(void);
void UpdateTossScreen(GameState *currentState);
void DrawTossScreen(void);


#endif