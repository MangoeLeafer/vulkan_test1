#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class WindowComponent {
public:
    SDL_Window* window;

    void init() {
        if (SDL_Init(SDL_INIT_VIDEO) != SDL_APP_SUCCESS) {
            std::cout << "Error: " << SDL_GetError() << "\n";
        }

        SDL_Vulkan_LoadLibrary(nullptr);

        window = SDL_CreateWindow("Vulkan Testing", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);
    }

    void run(bool &keepRunning) {
        SDL_Event windowEvent;

        while(SDL_PollEvent(&windowEvent)) {
            if(windowEvent.type == SDL_EVENT_QUIT) {
                keepRunning = false;
                break;
            }
        }
    }

    void cleanUp() {
        SDL_DestroyWindow(window);
        SDL_Vulkan_UnloadLibrary();
    }

private:

};