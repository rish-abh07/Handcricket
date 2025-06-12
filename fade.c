#include "fade.h"

// Global fade effect instance (used across all screens)
FadeEffect fade = { 0 };

/// Initialize the fade effect (recommended at game start or before new transition)
void InitFade(FadeEffect *fade) {
    fade->timer = 0.0f;
    fade->duration = 0.0f;
    fade->fadeIn = true;
    fade->active = false;
    fade->done = false;
}

/// Start a fade effect
/// @param fade Pointer to the fade effect
/// @param duration Duration in seconds
/// @param fadeIn true = fade in (black to screen), false = fade out (screen to black)
void StartFade(FadeEffect *fade, float duration, bool fadeIn) {
    fade->timer = 0.0f;
    fade->duration = duration;
    fade->fadeIn = fadeIn;
    fade->active = true;
    fade->done = false;
}

/// Update fade timing each frame
/// Call this in your `UpdateGame()` function
void UpdateFade(FadeEffect *fade) {
    if (!fade->active) return;

    fade->timer += GetFrameTime();

    if (fade->timer >= fade->duration) {
        fade->timer = fade->duration;
        fade->active = false;
        fade->done = true;
    }
}

/// Draw the black overlay with correct alpha based on fade progress
/// Call this LAST in your `DrawGame()` (after all screens are drawn)
void DrawFade(FadeEffect *fade) {
    if (!fade->active && !fade->done) return;

    float alpha = fade->timer / fade->duration;

    // Reverse alpha for fade-out (i.e., start from 1.0 down to 0.0)
    if (!fade->fadeIn) alpha = 1.0f - alpha;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
}
