#include "headers/Input.h"
#include "headers/Maze.h"       // isWallAtPosition, isCollision, maze
#include "headers/UI.h"         // showCongratulations, showEndPrompt
#include "headers/Timer.h"
#include "headers/Global.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (appState != 1) return;

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float sensitivity = 0.1f;
    float xoffset = ((float)xpos - lastX) * sensitivity;
    float yoffset = (lastY - (float)ypos) * sensitivity;

    lastX = (float)xpos;
    lastY = (float)ypos;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.f)  pitch = 89.f;
    if (pitch < -89.f) pitch = -89.f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window) {
    int escState = glfwGetKey(window, GLFW_KEY_ESCAPE);

    // Wyœwietlono gratulacje => ESC = powrót do menu
    if (showCongratulations && escState == GLFW_PRESS) {
        showCongratulations = false;
        showEndPrompt = false;
        appState = 0;
        glfwSetInputMode(windowGlobal, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

    // Pauza (tylko w grze)
    if (escState == GLFW_PRESS && !escPressedLastFrame) {
        if (appState == 1) {
            if (!isPaused) {
                isPaused = true;
                glfwSetInputMode(windowGlobal, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else {
                isPaused = false;
                glfwSetInputMode(windowGlobal, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;
            }
        }
    }
    escPressedLastFrame = (escState == GLFW_PRESS);

    // Jeœli nie jesteœmy w grze (lub pauza/gratulacje) – nie obs³ugujemy ruchu
    if (appState != 1 || isPaused || showCongratulations) return;

    // Sprawdzamy, czy gracz jest przy drzwiach => prompt
    if (isPlayerNearDoors() && !showCongratulations) {
        showEndPrompt = true;
    }
    else {
        showEndPrompt = false;
    }

    // Jeœli showEndPrompt i gracz nacisn¹³ 'E' => gratulacje
    int eState = glfwGetKey(window, GLFW_KEY_E);
    if (showEndPrompt && eState == GLFW_PRESS) {
        showCongratulations = true;
        showEndPrompt = false;
        timerRunning = false;
        timerFinal = (float)glfwGetTime() - timerStart;
    }

    // Ruch kamery (W,S,A,D)
    float cameraSpeed = 5.0f * deltaTime;
    glm::vec3 frontXZ = glm::normalize(glm::vec3(cameraFront.x, 0.f, cameraFront.z));
    glm::vec3 right = glm::normalize(glm::cross(frontXZ, cameraUp));
    glm::vec3 moveDir(0.f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir += frontXZ;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= frontXZ;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir += right;

    if (glm::length(moveDir) > 0.f) {
        moveDir = glm::normalize(moveDir) * cameraSpeed;
    }

    glm::vec3 newPos = cameraPos + moveDir;
    glm::vec3 testPosX = glm::vec3(newPos.x, cameraPos.y, cameraPos.z);

    // Kolizja w osi X
    if (!isCollision(testPosX.x, testPosX.z)) {
        cameraPos.x = testPosX.x;
    }
    // Kolizja w osi Z
    glm::vec3 testPosZ = glm::vec3(cameraPos.x, cameraPos.y, newPos.z);
    if (!isCollision(testPosZ.x, testPosZ.z)) {
        cameraPos.z = testPosZ.z;
    }

    cameraPos.y = 1.8f;
}
