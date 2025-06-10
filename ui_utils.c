#include "ui_utils.h"
#include "raylib.h"
#include "math.h"
#include <stdlib.h>
#include "Montserrat_Fonts.h"

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

void btnIconTextCentered(char *btntext, int x, int y, float width, float height,  int fontSize ,Color color, Font fontType, int spacing, Texture2D icon){
    float icongap = 10;
    float iconScale = 0.05f;
    Vector2 textSize = MeasureTextEx(fontType,btntext, fontSize, spacing);
    int textX = x + (width - textSize.x)/2;
    int textY = y + (height - textSize.y )/2;
    int iconHeight = fontSize;
    int iconWidth = iconHeight *((float)icon.width / icon.height);
    float totalWidth = iconWidth + icongap + textSize.x;
    float startX = x+(width-totalWidth)/2;
    float centerY = y +(height-iconHeight)/2;
    
    DrawTexturePro(icon,(Rectangle){ 0, 0, (float)icon.width, (float)icon.height },(Rectangle){ startX, centerY, iconWidth, iconHeight },(Vector2){ 0, 0 }, 0.0f,WHITE);
    DrawTextEx(fontType,btntext,(Vector2){startX+iconWidth+icongap,  y + (height - textSize.y) / 2}, fontSize, spacing,color);

}
void hoverSize(Rectangle button, Color normalColor, Color hoverColor, int fontSize, float hoverScale,char *btnText, Color fontColor, Font fontType, int spacing, Texture2D icon){
 
      bool isHovered = CheckCollisionPointRec(GetMousePosition(), button);

    float scale = isHovered ? hoverScale : 1.0f;
    Color showColor = isHovered ? hoverColor : normalColor;

    float newWidth = button.width * scale;
    float newHeight = button.height * scale;
    float newX = button.x - (newWidth - button.width) / 2;
    float newY = button.y - (newHeight - button.height) / 2;

    if (isHovered) {
        DrawRectangleRounded((Rectangle){ newX, newY, newWidth, newHeight }, 0.3f, 10, showColor);
        btnIconTextCentered(btnText, newX, newY, newWidth, newHeight, fontSize, fontColor, fontType,  spacing, icon);
    } else {
        DrawRectangleRounded(button, 0.3f, 10, showColor);
        btnIconTextCentered(btnText, button.x, button.y, button.width, button.height, fontSize, fontColor, fontType, spacing, icon);
    }
}