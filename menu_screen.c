#include "menu_screen.h"
#include "globalsVar.h"
#include "raylib.h"
#include "math.h"
#include <stdlib.h>

float buttonWidth = 300;
float buttonHeight = 40;
int buttonGap = 20;

static Rectangle startButton = { 300, 250, 300, 40};
static Rectangle leaderBoardButton = {300, 300, 300, 40};
static Rectangle settingButton = {300, 350, 300, 40};
static Rectangle howToPlayButton = { 300, 400, 300, 40 };
static Texture2D backgroundTexture;

//headinDesign

Font montserratTitle;
Font subHeadLight;
Font buttonFontMedium;



void InitMenuScreen(){
    backgroundTexture = LoadTexture("asset/menu_background.png");
    startButton.x = (screenWidth - buttonWidth)/2;
    leaderBoardButton.x = (screenWidth - buttonWidth)/2;
    settingButton.x = (screenWidth - buttonWidth)/2;
    howToPlayButton.x = (screenWidth - buttonWidth)/2;
    //toload textures and menu
    //toloadFont
     montserratTitle = LoadFontFromMemory(".otf",Montserrat_ExtraBold_otf, Montserrat_ExtraBold_otf_len, 40, NULL, 0);
     SetTextureFilter(montserratTitle.texture, TEXTURE_FILTER_POINT);
     subHeadLight = LoadFontFromMemory(".otf", MontserratAlternates_Light_otf, MontserratAlternates_Light_otf_len , 20,NULL,0);
     buttonFontMedium = LoadFontFromMemory(".otf", MontserratAlternates_Medium_otf, MontserratAlternates_Medium_otf_len , 20,NULL,0); 
}

void btnTextCentered(char *btntext, int x, int y, float width, float height,  int fontSize ,Color color, Font fontType, int spacing){
    Vector2 textSize = MeasureTextEx(fontType,btntext, fontSize, spacing);
    int textX = x + (width - textSize.x)/2;
    int textY = y + (height - textSize.y )/2;
    DrawTextEx(fontType,btntext,(Vector2){textX, textY}, fontSize, spacing,color);

}
void hoverSize(Rectangle button, Color normalColor, Color hoverColor, int fontSize, char *btnText, Color fontColor, Font fontType, int spacing){
 
      bool isHovered = CheckCollisionPointRec(GetMousePosition(), button);

    float scale = isHovered ? 1.2f : 1.0f;
    Color showColor = isHovered ? hoverColor : normalColor;

    float newWidth = button.width * scale;
    float newHeight = button.height * scale;
    float newX = button.x - (newWidth - button.width) / 2;
    float newY = button.y - (newHeight - button.height) / 2;

    if (isHovered) {
        DrawRectangleRounded((Rectangle){ newX, newY, newWidth, newHeight }, 0.3f, 10, showColor);
        btnTextCentered(btnText, newX, newY, newWidth, newHeight, fontSize, fontColor, fontType,  spacing);
    } else {
        DrawRectangleRounded(button, 0.3f, 10, showColor);
        btnTextCentered(btnText, button.x, button.y, button.width, button.height, fontSize, fontColor, fontType, spacing);
    }
}
void UpdateMenuScreen(GameState *state)
{
    Vector2 mouse = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, startButton)) {

            UnloadFont(montserratTitle);
            *state = STATE_TOSS; // Transition to toss state

        } else if (CheckCollisionPointRec(mouse, howToPlayButton)) {
            CloseWindow(); // Exit the game
        }
    }
}

void DrawMenuScreen(int screenWidth, int screenHeight)
{
     ClearBackground(BLACK); // Optional fallback background

    // Calculate scale to preserve aspect ratio (letterbox/pillarbox logic)
    float scale = fminf((float)screenWidth / backgroundTexture.width,
                        (float)screenHeight / backgroundTexture.height);

    Rectangle dest = {
        (screenWidth - backgroundTexture.width * scale) / 2,
        (screenHeight - backgroundTexture.height * scale) / 2,
        backgroundTexture.width * scale,
        backgroundTexture.height * scale
    };

    DrawTexturePro(backgroundTexture,
                   (Rectangle){ 0, 0, backgroundTexture.width, backgroundTexture.height },
                   dest,
                   (Vector2){ 0, 0 },
                   0.0f,
                   WHITE);

    // hoverColors
    Color hoverStart = (Color){32, 191, 90, 255};
    Color hoverLeader = (Color){ 41, 104, 237,255};
    Color hoverSetting = (Color){149, 55, 235, 255} ; 
    Color hoverExit = (Color){237, 93, 14, 255} ;

     //buttonColor
     Color startButtonColor = (Color){32, 191, 90, 230};
     Color leaderBoardButtonColor = (Color){ 41, 104, 237, 230};
     Color settingButtonColor = (Color){149, 55, 235, 230};
     Color howToPlayButtonColor = (Color){ 237, 93, 14, 230};

     Vector2 textSize = MeasureTextEx(montserratTitle, "HAND CRICKET", 40,1);
     Vector2 titlePosition = {(screenWidth - textSize.x)/2,startButton.y - 100};
     DrawTextEx(montserratTitle,"HAND CRICKET", titlePosition, 40,1, WHITE);
    Vector2 subHeadSize = MeasureTextEx(subHeadLight,"The Ultimate Finger Game", 20,0); // match font size to DrawText
    float subHeadX = titlePosition.x + (textSize.x - subHeadSize.x)/2;
    DrawTextEx(subHeadLight,"The Ultimate Finger Game",(Vector2){ subHeadX, titlePosition.y + 50}, 20,0, YELLOW);


    hoverSize(startButton, startButtonColor,hoverStart, 20, "NEW GAME", WHITE, buttonFontMedium, 0);
    hoverSize(leaderBoardButton, leaderBoardButtonColor,hoverLeader, 20, "LEADERBOARD", WHITE, buttonFontMedium,0);
    hoverSize(settingButton, settingButtonColor,hoverSetting, 20, "SETTING", WHITE, buttonFontMedium,0);
    hoverSize(howToPlayButton, howToPlayButtonColor,hoverExit, 20, "HOW TO PLAY", WHITE, buttonFontMedium,0);
}
void UnloadMenuScreen() {
    UnloadTexture(backgroundTexture); // Unload the background texture
}

