#include "ui_utils.h"
#include "raylib.h"
#include "math.h"
#include <stdlib.h>
#include "Montserrat_Fonts.h"
#include "rlgl.h"
Font montserratTitle;
Font subHeadLight;
Font buttonFontMedium;

void LoadGameFont(){
     montserratTitle = LoadFontFromMemory(".otf",Montserrat_ExtraBold_otf, Montserrat_ExtraBold_otf_len, 40, NULL, 0);
     SetTextureFilter(montserratTitle.texture, TEXTURE_FILTER_POINT);
    subHeadLight = LoadFontFromMemory(".otf", Montserrat_Light_otf, Montserrat_Light_otf_len , 40,NULL,0);
    SetTextureFilter(subHeadLight.texture, TEXTURE_FILTER_POINT);
     buttonFontMedium = LoadFontFromMemory(".otf", Montserrat_Medium_otf, Montserrat_Medium_otf_len , 20,NULL,0);
}

void UnloadGameFont(){
    UnloadFont(montserratTitle);
    UnloadFont(subHeadLight);
    UnloadFont(buttonFontMedium);
}
void CenterIconInButton(Texture2D icon, Rectangle button, float iconHeight, float offsetX, float offsetY) {
    if (icon.id == 0) return;  // Skip drawing if no texture

    float iconWidth = iconHeight * ((float)icon.width / icon.height);
    float x = button.x + (button.width - iconWidth) / 2.0f + offsetX;
    float y = button.y + (button.height - iconHeight) / 2.0f + offsetY;

    DrawTexturePro(icon,
                   (Rectangle){ 0, 0, (float)icon.width, (float)icon.height },
                   (Rectangle){ x, y, iconWidth, iconHeight },
                   (Vector2){ 0, 0 }, 0.0f, WHITE);
}

void btnIconTextCentered(char *btntext, int x, int y, float width, float height,  int fontSize ,Color color, Font fontType, int spacing, Texture2D icon , float offsetX, float offsetY){
    float icongap = 10;
    float iconScale = 0.05f;
    Vector2 textSize = MeasureTextEx(fontType,btntext, fontSize, spacing);
    int textX = x + (width - textSize.x)/2 + offsetX;
    int textY = y + (height - textSize.y )/2 + offsetY;
    int iconHeight = fontSize;
    int iconWidth = iconHeight *((float)icon.width / icon.height);
    float totalWidth = iconWidth + icongap + textSize.x;
    float startX = x+(width-totalWidth)/2;
    float centerY = y +(height-iconHeight)/2;
    if(icon.id != 0){
         DrawTexturePro(icon,(Rectangle){ offsetX, offsetY, (float)icon.width, (float)icon.height },(Rectangle){ startX, centerY, iconWidth, iconHeight },(Vector2){ 0, 0 }, 0.0f,WHITE);
        DrawTextEx(fontType,btntext,(Vector2){startX+iconWidth+icongap,  y + (height - textSize.y) / 2}, fontSize, spacing,color);
    }else{
        DrawTextEx(fontType,btntext,(Vector2){textX,  textY}, fontSize, spacing,color);
    }

}
void hoverSize(Rectangle button, Color normalColor, Color hoverColor, int fontSize, float hoverScale,char *btnText, Color fontColor, Font fontType, int spacing, Texture2D icon, float offsetX, float offsetY){
 
      bool isHovered = CheckCollisionPointRec(GetMousePosition(), button);

    float scale = isHovered ? hoverScale : 1.0f;
    Color showColor = isHovered ? hoverColor : normalColor;

    float newWidth = button.width * scale;
    float newHeight = button.height * scale;
    float newX = button.x - (newWidth - button.width) / 2;
    float newY = button.y - (newHeight - button.height) / 2;

    if (isHovered) {
        DrawRectangleRounded((Rectangle){ newX, newY, newWidth, newHeight }, 0.3f, 10, showColor);
        btnIconTextCentered(btnText, newX, newY, newWidth, newHeight, fontSize, fontColor, fontType,  spacing, icon, offsetX, offsetY);
    } else {
        DrawRectangleRounded(button, 0.3f, 10, showColor);
        btnIconTextCentered(btnText, button.x, button.y, button.width, button.height, fontSize, fontColor, fontType, spacing, icon, offsetX, offsetY);
    }
}
#include "ui_utils.h"

void DrawGradientBox(Rectangle rect, Color topColor, Color bottomColor, float borderThickness, Color borderColor) {
    for (int y = 0; y < rect.height; y++) {
        float t = (float)y / rect.height;

        Color col = {
            (unsigned char)(topColor.r + t * (bottomColor.r - topColor.r)),
            (unsigned char)(topColor.g + t * (bottomColor.g - topColor.g)),
            (unsigned char)(topColor.b + t * (bottomColor.b - topColor.b)),
            (unsigned char)(topColor.a + t * (bottomColor.a - topColor.a))
        };

        DrawRectangle(rect.x, rect.y + y, rect.width, 1, col);
    }

    if (borderThickness > 0) {
        DrawRectangleLinesEx(rect, borderThickness, borderColor);
    }
}

void DrawHorizontalGradientBox(Rectangle rect, Color leftColor, Color rightColor, float borderThickness, Color borderColor) {
    for (int x = 0; x < rect.width; x++) {
        float t = (float)x / rect.width;

        Color col = {
            (unsigned char)(leftColor.r + t * (rightColor.r - leftColor.r)),
            (unsigned char)(leftColor.g + t * (rightColor.g - leftColor.g)),
            (unsigned char)(leftColor.b + t * (rightColor.b - leftColor.b)),
            (unsigned char)(leftColor.a + t * (rightColor.a - leftColor.a))
        };

        DrawRectangle(rect.x + x, rect.y, 1, rect.height, col);
    }

    if (borderThickness > 0) {
        DrawRectangleLinesEx(rect, borderThickness, borderColor);
    }
}
