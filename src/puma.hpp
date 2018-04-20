#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace puma {

class Puma {
public:
    void run();

private:
    void init();
    void loop();
    void cleanup();

    void handleEvents();
    void update();
    void render();

    GLuint createShaderFromFile(const char* filename, GLenum shaderType);

    SDL_Window* window;
    SDL_GLContext context;

    bool running;
};

}
