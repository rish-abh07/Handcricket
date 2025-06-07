#ifndef CHOOSE_BAT_BALL_H
#define CHOOSE_BAT_BALL_H

#include "game.h"
#include <stdlib.h>
#include "raylib.h"
#include <time.h>
#include <string.h>

void InitChooseBatBall(bool isComputerTurn);
void UpdateChooseBatBall(GameState *state);
void DrawChooseBatBallScreen(void);
void UnloadChooseBatBall(void);

#endif // CHOOSE_BAT_BALL_H
