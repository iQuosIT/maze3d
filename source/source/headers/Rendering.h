#pragma once
#include "headers/Global.h"

// Tekstury (ID)
extern GLuint wallTexture;
extern GLuint floorTexture;
extern GLuint doorTexture;
extern GLuint openDoorTexture;
extern GLuint skyTexture;
extern GLuint skyUpTexture;

void setupProjection();
void setupCamera();
GLuint loadTexture(const char* path);

void drawCube(float size);
void drawFloor();
void drawSkybox(GLuint skyTex);
void drawSkyboxTop();
