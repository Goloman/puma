#include "puma.hpp"

#include <stdexcept>
#include <fstream>

void puma::Puma::run() {
    init();
    loop();
    cleanup();
}

void puma::Puma::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        SDL_Log("%s", SDL_GetError());
        throw std::runtime_error("SDL initialization failed");
    }

    window = SDL_CreateWindow("puma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        SDL_Log("%s", SDL_GetError());
        throw std::runtime_error("Window creation failed");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0); // TODO https://wiki.libsdl.org/SDL_GLattr

    context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_Log("%s", SDL_GetError());
        throw std::runtime_error("Context creation failed");
    }

    // NOTE potentially needed?
    //glewExperimental = GL_TRUE
    GLenum glewRet = glewInit();

    createShaderFromFile("resources/phongVS.glsl", GL_VERTEX_SHADER);
    createShaderFromFile("resources/phongFS.glsl", GL_FRAGMENT_SHADER);

    // TODO load shaders and create programs
}

void puma::Puma::loop() {
    running = true;

    while (running) {
        handleEvents();
        update();
        render();
    }
}

void puma::Puma::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            running = false;
            break;
            }
        default:
            break;
        }
    }
}

GLuint puma::Puma::createShaderFromFile(const char* filename, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    std::ifstream f;
    f.open(filename);

    f.seekg(0, std::ios::end);
    int length = f.tellg();
    f.seekg(0, std::ios::beg);

    char buffer[length];
    f.read(buffer, length);
    f.close();

    char *start = buffer;
    glShaderSource(shader, 1, &start, &length);
    glCompileShader(shader);

    int  success;
    char log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 1024, NULL, log);
        SDL_Log("%s:\n%s", filename, log);
        throw std::runtime_error("Shader compilation failed");
    }
    return shader;
}

void puma::Puma::update() {
}

void puma::Puma::render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);



    SDL_GL_SwapWindow(window);
}

void puma::Puma::cleanup() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
