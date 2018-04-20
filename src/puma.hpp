#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

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

    SDL_Window* window;
    SDL_GLContext context;

    bool running;
};

}
