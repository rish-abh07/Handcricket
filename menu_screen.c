#include "menu_screen.h"
#include "globalsVar.h"
#include "raylib.h"
#include "math.h"

float buttonWidth = 200;
float buttonHeight = 40;
int buttonGap = 20;

static Rectangle startButton = { 300, 250, 200, 40};
static Rectangle leaderBoardButton = {300, 300, 200, 40};
static Rectangle settingButton = {300, 350, 200, 40};
static Rectangle howToPlayButton = { 300, 400, 200, 40 };
static Texture2D backgroundTexture;

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


void InitMenuScreen(){
    backgroundTexture = LoadTexture("asset/menu_background.png");
    startButton.x = (screenWidth - buttonWidth)/2;
    leaderBoardButton.x = (screenWidth - buttonWidth)/2;
    settingButton.x = (screenWidth - buttonWidth)/2;
    howToPlayButton.x = (screenWidth - buttonWidth)/2;
    //toload textures and menu
}

void btnTextCentered(char *btntext, int x, int y, float width, float height,  int fontSize ,Color color){
     int textWidth = MeasureText(btntext, fontSize);
    int textX = x + (width - textWidth)/2;
    int textY = y + (height - fontSize )/2;
    DrawText(btntext, textX, textY, fontSize, color);

}
void hoverSize(Rectangle button, Color hoverColor, int fontSize, char *btnText, Color fontColor){
    float scale = CheckCollisionPointRec(GetMousePosition(), button) ? 1.2f : 1.0f;
    
    float newWidth = button.width * scale;
    float newHeight = button.height * scale;
    float newX = button.x - (newWidth - button.width)/2;
    float newY = button.y - (newHeight - button.height)/2;
    
    DrawRectangleRounded((Rectangle){ newX, newY, newWidth, newHeight},0.3f,10, hoverColor);
    btnTextCentered(btnText, newX,newY, newWidth, newHeight, fontSize, fontColor);
   
    
}
void UpdateMenuScreen(GameState *state)
{
    Vector2 mouse = GetMousePosition();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, startButton)) {
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

    DrawText("HAND CRICKET", 270, 150, 40, DARKBLUE);
    hoverSize(startButton, hoverStart, 20, "NEW GAME", WHITE);
    hoverSize(leaderBoardButton, hoverLeader, 20, "LEADERBOARD", WHITE);
    hoverSize(settingButton, hoverSetting, 20, "SETTING", WHITE);
    hoverSize(howToPlayButton, hoverExit, 20, "HOW TO PLAY", WHITE);

    
    // Draw buttons with centered text
    DrawRectangleRounded(startButton,0.3,10, startButtonColor);
    btnTextCentered("NEW GAME", startButton.x, startButton.y, startButton.width, startButton.height, 20, WHITE);

    DrawRectangleRounded(leaderBoardButton,0.3,10, leaderBoardButtonColor);
    btnTextCentered("LEADERBOARD", leaderBoardButton.x, leaderBoardButton.y, leaderBoardButton.width, leaderBoardButton.height, 20, WHITE);

    DrawRectangleRounded(settingButton,0.3,10, settingButtonColor);
    btnTextCentered("SETTING", settingButton.x, settingButton.y, settingButton.width, settingButton.height, 20, WHITE);

    DrawRectangleRounded(howToPlayButton,0.3,10, howToPlayButtonColor);
    btnTextCentered("HOW TO PLAY", howToPlayButton.x, howToPlayButton.y, howToPlayButton.width, howToPlayButton.height, 20, WHITE);
}
void UnloadMenuScreen() {
    UnloadTexture(backgroundTexture); // Unload the background texture
}

