#include "headers/Maze.h"
#include "headers/Rendering.h"  
#include "headers/Global.h"
// Przechowywanie labiryntu
static std::vector<std::vector<int>> maze;
static int mazeWidth = 0;
static int mazeHeight = 0;

std::string getRandomMazeFile() {
    std::vector<std::string> mazeFiles = {
        "source/maps/maze1.txt",
        "source/maps/maze2.txt",
        "source/maps/maze3.txt",
        "source/maps/maze4.txt"
    };
    int index = rand() % (int)mazeFiles.size();
    return mazeFiles[index];
}

bool loadMaze(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku labiryntu: " << filename << std::endl;
        return false;
    }
    maze.clear();
    mazeWidth = 0;
    mazeHeight = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char c : line) {
            if (c == '0') row.push_back(0);
            else if (c == '1') row.push_back(1);
            else if (c == '2') row.push_back(2);
            else if (c == '3') row.push_back(3);
        }
        if (!row.empty()) {
            maze.push_back(row);
            mazeWidth = std::max(mazeWidth, (int)row.size());
        }
    }
    file.close();
    mazeHeight = (int)maze.size();

    if (maze.empty()) {
        std::cerr << "Labirynt jest pusty!" << std::endl;
        return false;
    }
    return true;
}

// Sprawdza, czy (x,z) to œciana/drzwi (1,2,3).
bool isWallAtPosition(float x, float z) {
    int mazeX = (int)std::floor(x);
    int mazeZ = (int)std::floor(z);

    if (mazeZ < 0 || mazeZ >= mazeHeight)            return true;
    if (mazeX < 0 || mazeX >= (int)maze[mazeZ].size()) return true;

    int cell = maze[mazeZ][mazeX];
    return (cell == 1 || cell == 2 || cell == 3);
}

bool isPlayerNearDoors() {
    // Szukamy komórki '2'
    for (int z = 0; z < mazeHeight; z++) {
        for (int x = 0; x < (int)maze[z].size(); x++) {
            if (maze[z][x] == 2) {
                float cellCenterX = x + 0.5f;
                float cellCenterZ = z + 0.5f;
                float dx = cameraPos.x - cellCenterX;
                float dz = cameraPos.z - cellCenterZ;
                float dist = sqrtf(dx * dx + dz * dz);

                // Jeœli jesteœmy wystarczaj¹co blisko drzwi:
                if (dist < 1.0f) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Sprawdza kolizjê (uwzglêdnia promieñ kamery).
bool isCollision(float x, float z) {
    float positions[4][2] = {
        {x - cameraRadius, z - cameraRadius},
        {x + cameraRadius, z - cameraRadius},
        {x - cameraRadius, z + cameraRadius},
        {x + cameraRadius, z + cameraRadius}
    };
    for (int i = 0; i < 4; i++) {
        if (isWallAtPosition(positions[i][0], positions[i][1])) {
            return true;
        }
    }
    return false;
}

void setCameraAtEntrance() {
    for (int z = 0; z < mazeHeight; ++z) {
        for (int x = 0; x < (int)maze[z].size(); ++x) {
            if (maze[z][x] == 0) {
                float posX = x + 0.5f;
                float posZ = z + 0.5f;
                cameraPos = glm::vec3(posX, 1.8f, posZ);
                return;
            }
        }
    }
    // Gdy brak '0'
    cameraPos = glm::vec3(0.f, 1.8f, 0.f);
}

void drawMaze() {
    glEnable(GL_TEXTURE_2D);

    for (int z = 0; z < mazeHeight; ++z) {
        for (int x = 0; x < (int)maze[z].size(); ++x) {
            int cell = maze[z][x];

            if (cell == 1) {
                // Œciana
                glBindTexture(GL_TEXTURE_2D, wallTexture);
                float wallSize = 1.f;
                float wallHeight = 3.f;
                float posX = x * wallSize;
                float posZ = z * wallSize;

                glPushMatrix();
                glTranslatef(posX + 0.5f, wallHeight / 2.f, posZ + 0.5f);
                glScalef(wallSize, wallHeight, wallSize);
                drawCube(1.0f);
                glPopMatrix();
            }
            else if (cell == 2) {
                // Drzwi (zamkniête)
                glBindTexture(GL_TEXTURE_2D, doorTexture);
                float wallSize = 1.f;
                float wallHeight = 3.f;
                float posX = x * wallSize;
                float posZ = z * wallSize;

                glPushMatrix();
                glTranslatef(posX + 0.5f, wallHeight / 2.f, posZ + 0.5f);
                glRotatef(70.f, 0.f, 1.f, 0.f);
                glScalef(wallSize, wallHeight, 0.2f);
                drawCube(1.0f);
                glPopMatrix();
            }
            else if (cell == 3) {
                // Drzwi "otwarte"
                glBindTexture(GL_TEXTURE_2D, openDoorTexture);
                float wallSize = 1.f;
                float wallHeight = 3.f;
                float posX = x * wallSize;
                float posZ = z * wallSize;

                glPushMatrix();
                glTranslatef(posX + 0.5f, wallHeight / 2.f, posZ + 0.5f);
                glRotatef(60.f, 0.f, 1.f, 0.f);
                glScalef(wallSize, wallHeight, 0.2f);
                drawCube(1.0f);
                glPopMatrix();
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

}
