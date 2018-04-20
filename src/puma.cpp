#include "puma.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <stdexcept>

void puma::Puma::run() {
    init();
    loop();
    cleanup();
}

void puma::Puma::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        SDL_Log("%s", SDL_GetError());
        std::runtime_error("SDL initialization failed");
    }

    window = SDL_CreateWindow("puma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        SDL_Log("%s", SDL_GetError());
        std::runtime_error("Window creation failed");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0); //TODO https://wiki.libsdl.org/SDL_GLattr

    context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_Log("%s", SDL_GetError());
        std::runtime_error("Context creation failed");
    }

    // NOTE potentially needed?
    //glewExperimental = GL_TRUE
    GLenum glewRet = glewInit();
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
