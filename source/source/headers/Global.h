#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>        // do sqrt, floor itd.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ZMIENNE GLOBALNE (deklaracje)

// Stan aplikacji: 0 = MENU, 1 = GRA
extern int appState;
extern bool isPaused;
extern bool showEndPrompt;
extern bool showCongratulations;
extern bool escPressedLastFrame;

extern GLFWwindow* windowGlobal;

// Kamera
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern bool firstMouse;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;

// Okno
extern int windowWidth;
extern int windowHeight;

// Czas
extern float deltaTime;
extern float lastFrame;

// Kolizje / wymiary
extern const float cameraRadius;

// *** TIMER ***
extern float timerStart;
extern bool timerRunning;
extern float timerFinal;
