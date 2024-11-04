#pragma once

#include "window.hpp"

#include <SDL3/SDL.h>

#include <iostream>

class SDLHandler {
public:
    WindowComponent cWindow;

    void init() {
        cWindow.init();
    }

    void run(bool &keepRunning) {
        cWindow.run(keepRunning);
    }

    void cleanUp() {
        cWindow.cleanUp();

        SDL_Quit();
        SDL_Log("Cleaned up with errors: %s", SDL_GetError());
    }

private:

};