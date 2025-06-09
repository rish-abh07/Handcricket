#ifndef GAME_H
#define GAME_H

typedef enum {
    STATE_MENU,
    STATE_TOSS,
    STATE_BAT_BOWL,
    STATE_PLAY,
    STATE_SUMMARY,
} GameState;


void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void CLoseGame(void);

#endif