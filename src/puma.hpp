#pragma once

#include "mesh.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace puma {

class Puma {
public:
    void run();

private:
    static const int SHADER_LOCATION_POSITION = 0;
    static const int SHADER_LOCATION_NORMAL = 1;
    static const int SHADER_LOCATION_TEXTURE_COORDINATE = 2;
    static const int SHADER_LOCATION_VELOCITY = 3;
    static const int SHADER_LOCATION_AGE = 4;

    static const int SHADER_UNIFORM_LOCATION_MODEL = 10;
    static const int SHADER_UNIFORM_LOCATION_VIEW = 11;
    static const int SHADER_UNIFORM_LOCATION_PROJECTION = 12;
    static const int SHADER_UNIFORM_LOCATION_LIGHT_POSITION = 13;

    void init();
    void loop();
    void cleanup();

    void handleEvents();
    void update();
    void render();

    GLuint createShaderFromFile(const char* filename, GLenum shaderType);

    SDL_Window* window;
    SDL_GLContext context;

    glm::mat4 viewMatrix;
    glm::mat4 projectiomMatrix;

    Mesh robotMesh[6];
    glm::mat4 robotMatrix[6];

    GLuint phongProgram;

    bool running;
};

}
