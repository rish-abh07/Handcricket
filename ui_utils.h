#ifndef UI_UTILS_H
#define UI_UTILS_H

#include "raylib.h"
#include "game.h"
#include <stdlib.h>



void LoadGameFont();
void UnloadGameFont();
void CenterIconInButton(Texture2D icon, Rectangle button, float iconHeight, float offsetX, float offsetY);
void btnIconTextCentered(char *btntext, int x, int y, float width, float height,  int fontSize ,Color color, Font fontType, int spacing, Texture2D icon, float offsetX, float offsetY);
void hoverSize(Rectangle button, Color normalColor, Color hoverColor, int fontSize,float hoverScale, char *btnText, Color fontColor, Font fontType, int spacing, Texture2D icon, float offsetX, float offsetY);

#endif