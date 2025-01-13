#include "headers/Global.h"

// ZMIENNE GLOBALNE (definicje)

int appState = 0;            // 0 = menu, 1 = gra
bool isPaused = false;
bool showEndPrompt = false;
bool showCongratulations = false;
bool escPressedLastFrame = false;

GLFWwindow* windowGlobal = nullptr;

// Kamera
glm::vec3 cameraPos;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;

// Okno
int windowWidth  = 800;
int windowHeight = 600;

// Czas
float deltaTime  = 0.0f;
float lastFrame  = 0.0f;

// Kolizje / wymiary
const float cameraRadius = 0.3f;

// *** TIMER ***
float timerStart    = 0.0f;
bool  timerRunning  = false;
float timerFinal    = 0.0f;
