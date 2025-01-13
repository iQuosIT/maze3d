#include "headers/Rendering.h"
#include "headers/stb_image.h"
#include "headers/Global.h"

// Definicja zmiennych globalnych na tekstury
GLuint wallTexture = 0;
GLuint floorTexture = 0;
GLuint doorTexture = 0;
GLuint openDoorTexture = 0;
GLuint skyTexture = 0;
GLuint skyUpTexture = 0;

void setupProjection() {
    float aspectRatio = (float)windowWidth / (float)windowHeight;
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        aspectRatio,
        0.2f,
        100.0f
    );
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));
}

void setupCamera() {
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
}

GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 1) ? GL_RED :
            ((nrChannels == 3) ? GL_RGB : GL_RGBA);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format,
            width, height, 0,
            format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        std::cerr << "Nie uda³o siê za³adowaæ tekstury: " << path << std::endl;
    }

    stbi_image_free(data);
    return textureID;
}

void drawCube(float size) {
    float halfSize = size / 2.0f;
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, halfSize);
    // back
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, -halfSize);
    // left
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
    // right
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, -halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, halfSize, -halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, -halfSize, halfSize);
    // top
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, halfSize, halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, halfSize, halfSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
    // bottom
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, -halfSize, -halfSize);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, halfSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
    glEnd();
}

void drawFloor() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    float y = 0.0f;
    float minX = 0.0f;
    float maxX = (float)/*szerokoœæ labiryntu*/ 50; // mo¿na poprawiæ, np. mazeWidth
    float minZ = 0.0f;
    float maxZ = (float)/*wysokoœæ labiryntu*/ 50; // mo¿na poprawiæ, np. mazeHeight

    // Mo¿esz to dynamicznie pobraæ z Maze, jeœli chcesz.
    // ¯eby siê nie powtarzaæ, mo¿na tam daæ getMazeWidth(), getMazeHeight() itp.

    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f);                 glVertex3f(minX, y, minZ);
    glTexCoord2f(maxX, 0.f);                glVertex3f(maxX, y, minZ);
    glTexCoord2f(maxX, maxZ);               glVertex3f(maxX, y, maxZ);
    glTexCoord2f(0.f, maxZ);                glVertex3f(minX, y, maxZ);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

// Rysowanie skyboxa (boki i dó³)
void drawSkybox(GLuint skyTex) {
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyTex);

    glPushMatrix();
    // Przeniesienie szeœcianu w pozycjê kamery
    glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);

    float size = 50.f;
    float half = size;

    glBegin(GL_QUADS);
    // FRONT
    glTexCoord2f(0.f, 1.f); glVertex3f(half, -half, -half);
    glTexCoord2f(1.f, 1.f); glVertex3f(-half, -half, -half);
    glTexCoord2f(1.f, 0.f); glVertex3f(-half, half, -half);
    glTexCoord2f(0.f, 0.f); glVertex3f(half, half, -half);
    // BACK
    glTexCoord2f(1.f, 1.f); glVertex3f(-half, -half, half);
    glTexCoord2f(0.f, 1.f); glVertex3f(half, -half, half);
    glTexCoord2f(0.f, 0.f); glVertex3f(half, half, half);
    glTexCoord2f(1.f, 0.f); glVertex3f(-half, half, half);
    // LEFT
    glTexCoord2f(0.f, 1.f); glVertex3f(-half, -half, half);
    glTexCoord2f(1.f, 1.f); glVertex3f(-half, -half, -half);
    glTexCoord2f(1.f, 0.f); glVertex3f(-half, half, -half);
    glTexCoord2f(0.f, 0.f); glVertex3f(-half, half, half);
    // RIGHT
    glTexCoord2f(1.f, 1.f); glVertex3f(half, -half, -half);
    glTexCoord2f(0.f, 1.f); glVertex3f(half, -half, half);
    glTexCoord2f(0.f, 0.f); glVertex3f(half, half, half);
    glTexCoord2f(1.f, 0.f); glVertex3f(half, half, -half);

    // BOTTOM
    glTexCoord2f(1.f, 0.f); glVertex3f(-half, -half, half);
    glTexCoord2f(1.f, 1.f); glVertex3f(-half, -half, -half);
    glTexCoord2f(0.f, 1.f); glVertex3f(half, -half, -half);
    glTexCoord2f(0.f, 0.f); glVertex3f(half, -half, half);
    glEnd();
    
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);
}

// Rysowanie sufitu (TOP) osobn¹ tekstur¹
void drawSkyboxTop() {
    glDepthMask(GL_FALSE);

    // Propozycja np. (70, 86, 105) – bardzo zbli¿ony do oryginalnego (68,86,107),
    // ale minimalnie jaœniejszy i delikatnie zmodyfikowany, ¿eby ³adniej "zlewaæ siê" z do³em.
    glColor3f(61.0f / 255.0f, 78.0f / 255.0f, 100.0f / 255.0f);

    glPushMatrix();
    glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);

    float size = 50.f;
    float half = size;

    glBegin(GL_QUADS);
    // GÓRA
    glVertex3f(-half, half, -half);
    glVertex3f(-half, half, half);
    glVertex3f(half, half, half);
    glVertex3f(half, half, -half);
    glEnd();

    glPopMatrix();

    // Przywrócenie domyœlnego koloru (bia³y), by nie wp³ywaæ na dalsze elementy
    glColor3f(1.0f, 1.0f, 1.0f);

    glDepthMask(GL_TRUE);
}


