#ifndef GLOBALS_H
#define GLOBALS_H
#define HIGHSCORE_FILE "highscore.dat"
#define MAX_NAME_LENGTH 32
typedef struct {
    int score;
    char name[MAX_NAME_LENGTH];
} HighScoreEntry;
extern int playerScore;
extern int compScore;
extern char playerName[MAX_NAME_LENGTH + 1]; // +1 for null terminator
extern bool isTypingNam;
extern  int screenWidth ;
extern  int screenHeight;
void SaveHighScore(int score, const char *name);
HighScoreEntry LoadHighScore();
#endif