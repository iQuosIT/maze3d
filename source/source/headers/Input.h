#pragma once
#include "headers/Global.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

// Sprawdza, czy gracz stoi w pobli¿u komórki '2' (drzwi).
bool isPlayerNearDoors();
