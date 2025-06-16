#ifndef SUMMARY_SCREEN_H
#define SUMMARY_SCREEN_H

#include "game.h"

void InitSummaryScreen(const char *msg, int userScore,  int compScore);
void UpdateSummaryScreen(GameState *state);
void DrawSummaryScreen(void);

#endif
