#include "headers/Global.h"

bool loadMaze(const std::string& filename);
void drawMaze();
void setCameraAtEntrance();
bool isPlayerNearDoors();

// Przydatne funkcje do kolizji, sprawdzania œcian itp.:
bool isWallAtPosition(float x, float z);
bool isCollision(float x, float z);

// Zwraca losowy plik z labiryntem:
std::string getRandomMazeFile();
