#ifndef FADE_H
#define FADE_H

#include "raylib.h"

typedef struct {
    float timer;
    float duration;
    bool active;
    bool fadeIn;
    bool done;
} FadeEffect;

extern FadeEffect fade;
void InitFade(FadeEffect *fade);
void StartFade(FadeEffect *fade, float duration, bool fadeIn);
void UpdateFade(FadeEffect *fade);
void DrawFade(FadeEffect *fade);

#endif
