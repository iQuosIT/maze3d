#pragma once
#include "headers/Global.h"

void drawTextOrtho(const char* text, float x, float y,
    float r = 1.0f, float g = 1.0f, float b = 1.0f,
    float scale = 1.0f);

void drawButton(const char* text, float x, float y, float w, float h);
void drawMainMenu();
void drawPauseUI();
