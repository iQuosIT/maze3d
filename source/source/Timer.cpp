#include "headers/Timer.h"
#include "headers/Global.h"
#include "headers/UI.h"

#include <sstream>
#include <iomanip>

void drawTimer()
{
    float xPos = (float)windowWidth - 200.0f;
    float yPos = 30.0f;

    float currentTime = (float)glfwGetTime();
    float elapsed = timerRunning ? (currentTime - timerStart) : timerFinal;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "Czas: " << elapsed << " s";
    drawTextOrtho(oss.str().c_str(), xPos, yPos, 1.0f, 1.0f, 1.0f, 2.0f);
}
