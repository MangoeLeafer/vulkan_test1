#pragma once

#include "events.hpp"
#include "../Vulkan/handler.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class WindowComponent {
public:
    SDL_Window* window;

    SDL_Window* get_pWindow() {return window;}

    void init() {
        if (SDL_Init(SDL_INIT_VIDEO) != SDL_APP_SUCCESS) {
            std::cout << "Error: " << SDL_GetError() << "\n";
        }

        SDL_Vulkan_LoadLibrary(nullptr);

        window = SDL_CreateWindow("Vulkan Testing", WIDTH, HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    }

    void run(bool& keepRunning, VulkanHandler& cVkHandler) {
        SDL_Event windowEvent;

        while(SDL_PollEvent(&windowEvent)) {
            if(windowEvent.type == SDL_EVENT_QUIT) {
                keepRunning = false;
                break;
            }

            if(windowEvent.type == SDL_EVENT_WINDOW_RESIZED) {
                EventsComponent::eventResizedWindow(cVkHandler);
            }
        }
    }

    void cleanUp() {
        SDL_DestroyWindow(window);
        SDL_Vulkan_UnloadLibrary();
    }

private:

};