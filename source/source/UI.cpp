#include "headers/UI.h"
#include "headers/Rendering.h"  // bo u¿ywamy np. windowWidth, windowHeight z Global.h
// i chcemy mieæ dostêp do GL
#define STB_EASY_FONT_IMPLEMENTATION
#include "headers/stb_easy_font.h"
#include "headers/Global.h"

// Rysowanie tekstu 2D (ortho)
void drawTextOrtho(const char* text, float x, float y,
    float r, float g, float b, float scale)
{
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, (GLfloat)windowWidth, (GLfloat)windowHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    static char buffer[9999];
    int num_quads = stb_easy_font_print(0, 0, (char*)text, nullptr, buffer, sizeof(buffer));

    glColor3f(r, g, b);
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawButton(const char* text, float x, float y, float w, float h)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, (GLfloat)windowWidth, (GLfloat)windowHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    // T³o przycisku
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Ramka przycisku
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Napis na przycisku
    int textLen = (int)strlen(text);
    float textWidth = (float)(textLen * 8);
    float textHeight = 13.0f;
    float textScale = 1.5f;
    float scaledTextW = textWidth * textScale;
    float scaledTextH = textHeight * textScale;
    float textX = x + (w - scaledTextW) / 2.0f;
    float textY = y + (h - scaledTextH) / 2.0f;

    static char buffer[9999];
    int num_quads = stb_easy_font_print(0, 0, (char*)text, nullptr, buffer, sizeof(buffer));
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(textX, textY, 0.f);
    glScalef(textScale, textScale, 1.f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

#include "headers/Maze.h" // do getRandomMazeFile, loadMaze, setCameraAtEntrance

void drawMainMenu() {
    glfwGetWindowSize(windowGlobal, &windowWidth, &windowHeight);

    float scale = 6.0f;
    const char* title = "MAZE 3D";
    int titleLen = (int)strlen(title);
    float titleW = (float)(titleLen * 8) * scale;
    float titleH = 13.0f * scale;

    float buttonW = 300.0f;
    float buttonH = 80.0f;

    float topY = (windowHeight - (titleH + 100 + buttonH + buttonH)) / 2.0f;
    float titleY = topY;
    float startY = titleY + titleH + 100.0f;
    float exitY = startY + buttonH + 50.0f;

    float startX = (windowWidth - buttonW) / 2.0f - 25.0f;
    float titleX = (startX + buttonW / 2.0f) - (titleW / 2.0f) + 40.0f;

    // Rysowanie tytu³u
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, (GLfloat)windowWidth, (GLfloat)windowHeight, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        {
            static char buffer[9999];
            int num_quads = stb_easy_font_print(0, 0, (char*)title, nullptr, buffer, sizeof(buffer));
            glColor3f(1, 1, 1);
            glPushMatrix();
            glTranslatef(titleX, titleY, 0);
            glScalef(scale, scale, 2.0f);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 16, buffer);
            glDrawArrays(GL_QUADS, 0, num_quads * 4);
            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        }
        glEnable(GL_DEPTH_TEST);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    // Rysujemy przyciski: START, WYJDZ
    drawButton("START", startX, startY, buttonW, buttonH);
    drawButton("WYJDZ", startX, exitY, buttonW, buttonH);

    // Obs³uga klikniêæ
    if (!isPaused && glfwGetMouseButton(windowGlobal, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double mx, my;
        glfwGetCursorPos(windowGlobal, &mx, &my);

        // START
        if (mx >= startX && mx <= startX + buttonW &&
            my >= startY && my <= startY + buttonH)
        {
            std::string mazeFile = getRandomMazeFile();
            if (!loadMaze(mazeFile)) {
                std::cerr << "Nie uda³o siê wczytaæ labiryntu!" << std::endl;
                glfwSetWindowShouldClose(windowGlobal, GLFW_TRUE);
                return;
            }
            setCameraAtEntrance();

            timerStart = (float)glfwGetTime();
            timerRunning = true;
            timerFinal = 0.0f;

            appState = 1;
            glfwSetInputMode(windowGlobal, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
            showCongratulations = false;
            showEndPrompt = false;
        }

        // WYJDZ
        if (mx >= startX && mx <= startX + buttonW &&
            my >= exitY && my <= exitY + buttonH)
        {
            glfwSetWindowShouldClose(windowGlobal, GLFW_TRUE);
        }
    }
}

void drawPauseUI() {
    glfwGetWindowSize(windowGlobal, &windowWidth, &windowHeight);

    const char* pauseText = "PAUZA";
    float scale = 4.0f;
    float textH = 13.0f * scale;
    float topY = ((windowHeight - (textH + 100.0f + 80.0f)) / 2.0f) - 200.0f;
    float pauseTextY = topY;
    float buttonY = pauseTextY + 110;

    int textLen = (int)strlen(pauseText);
    float textW = textLen * 8.0f * scale;
    float pauseTextX = (windowWidth - textW) / 2.0f;

    // Napis "PAUZA"
    drawTextOrtho(pauseText, pauseTextX, pauseTextY, 1.f, 1.f, 1.f, scale);

    float buttonW = 300.f;
    float buttonH = 80.f;
    float buttonX = (windowWidth - buttonW) / 2.0f;

    drawButton("MENU", buttonX - 20.f, buttonY, buttonW, buttonH);

    // Obs³uga klikniêcia
    if (glfwGetMouseButton(windowGlobal, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double mx, my;
        glfwGetCursorPos(windowGlobal, &mx, &my);

        if (mx >= buttonX && mx <= buttonX + buttonW &&
            my >= buttonY && my <= buttonY + buttonH)
        {
            appState = 0;
            isPaused = false;
            glfwSetInputMode(windowGlobal, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}
