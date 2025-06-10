#ifndef TOSS_SCREEN_H
#define TOSS_SCREEN_H

#include "game.h"
#include "stdlib.h"
#include "raylib.h"
#include <time.h>
extern bool isUserWonToss;
extern bool isComputerTurn;
void InitTossScreen(void);
void UpdateTossScreen(GameState *state);
void DrawTossScreen(int screenWidth, int screenHeight);

#endif // TOSS_SCREEN_H
