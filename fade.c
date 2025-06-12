#include "fade.h"

FadeEffect fade = { 0 };

void StartFade(FadeEffect *fade, float duration, bool fadeIn) {
    fade->timer = 0.0f;
    fade->duration = duration;
    fade->fadeIn = fadeIn;
    fade->active = true;
    fade->done = false;
}

void UpdateFade(FadeEffect *fade) {
    if (!fade->active) return;

    fade->timer += GetFrameTime();
    if (fade->timer >= fade->duration) {
        fade->timer = fade->duration;
        fade->active = false;
        fade->done = true;
    }
}

void DrawFade(FadeEffect *fade) {
    if (!fade->active && !fade->done) return;

    float alpha = fade->timer / fade->duration;
    if (!fade->fadeIn) alpha = 1.0f - alpha;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
}
