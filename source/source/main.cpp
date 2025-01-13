#include "headers/Global.h"      // zmienne globalne, includes systemowe
#include "headers/Rendering.h"   // setupProjection, loadTexture
#include "headers/Maze.h"        // loadMaze, drawMaze
#include "headers/UI.h"          // drawMainMenu
#include "headers/Input.h"       // processInput, mouse_callback
#include "headers/Timer.h"       // drawTimer

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

int main(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Nie uda³o siê zainicjowaæ GLFW" << std::endl;
        return -1;
    }

    // Ustawienia okna
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        std::cerr << "Nie uda³o siê uzyskaæ g³ównego monitora" << std::endl;
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    if (!mode) {
        std::cerr << "Nie uda³o siê uzyskaæ trybu wideo monitora" << std::endl;
        glfwTerminate();
        return -1;
    }

    windowWidth = mode->width;
    windowHeight = mode->height;

    // Pe³ny ekran:
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight,
        "3D Maze Game",
        primaryMonitor, nullptr);
    if (!window) {
        std::cerr << "Nie uda³o siê utworzyæ okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    windowGlobal = window;
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Nie uda³o siê zainicjowaæ GLAD" << std::endl;
        return -1;
    }

    // Podstawowe ustawienia OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setupProjection();

    srand((unsigned)time(NULL));

    // £adowanie tekstur
    wallTexture = loadTexture("source/textures/wall.jpg");
    if (wallTexture == 0) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury œcian!" << std::endl;
        return -1;
    }

    floorTexture = loadTexture("source/textures/floor_texture.jpg");
    if (floorTexture == 0) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury pod³ogi!" << std::endl;
        return -1;
    }

    doorTexture = loadTexture("source/textures/door.jpg");
    if (doorTexture == 0) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury drzwi!" << std::endl;
        return -1;
    }

    openDoorTexture = loadTexture("source/textures/open_door.jpg");
    if (openDoorTexture == 0) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury otwartych drzwi!" << std::endl;
        return -1;
    }

    skyTexture = loadTexture("source/textures/sky.jpg");
    if (skyTexture == 0) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury sky.jpg!" << std::endl;
        return -1;
    }

    lastFrame = (float)glfwGetTime();

    // G³ówna pêtla
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Czyszczenie ekranu
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (appState == 0) {
            // MENU
            drawMainMenu();
        }
        else if (appState == 1) {
            // GRA
            if (!isPaused) {
                setupCamera();

                // Rysujemy "zwyk³y" skybox (boki/dó³)
                drawSkybox(skyTexture);

                // Rysujemy sufit osobn¹ tekstur¹
                drawSkyboxTop();

                // Pod³oga, labirynt
                drawFloor();
                drawMaze();

                // Rysowanie timera
                drawTimer();

                // Ewentualne komunikaty 2D ("Nacisnij E..." / "Gratulacje!")
                if (showEndPrompt && !showCongratulations) {
                    const char* promptText = "Nacisnij E aby zakonczyc";
                    float textW = (float)(strlen(promptText) * 8);
                    float centerX = (windowWidth - textW) * 0.5f;
                    float centerY = (float)windowHeight * 0.5f;
                    drawTextOrtho(promptText, centerX, centerY, 1.f, 1.f, 1.f, 2.f);
                }

                if (showCongratulations) {
                    const char* congratsText = "Gratulacje! Ukonczyles labirynt";
                    float textW = (float)(strlen(congratsText) * 8);
                    float centerX = (windowWidth - textW * 2.0f) * 0.5f;
                    float centerY = (float)windowHeight * 0.5f;
                    drawTextOrtho(congratsText, centerX, centerY, 1.f, 1.f, 0.f, 2.f);

                    const char* escText = "Nacisnij ESC aby wrocic do menu";
                    float escW = (float)(strlen(escText) * 8);
                    float escX = (windowWidth - escW * 1.5f) * 0.5f;
                    float escY = centerY + 50.f;
                    drawTextOrtho(escText, escX, escY, 1.f, 1.f, 1.f, 1.5f);
                }
            }
            else {
                // Pauza
                drawPauseUI();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Sprz¹tanie
    glDeleteTextures(1, &wallTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &doorTexture);
    glDeleteTextures(1, &openDoorTexture);
    glDeleteTextures(1, &skyTexture);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
