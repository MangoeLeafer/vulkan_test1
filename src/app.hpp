#pragma once

#include "Setup/setup.hpp"

namespace App {
    class Application {
    public:
        SDLHandler sdlHandler;
        VulkanHandler vkHandler;

        void init() {
            sdlHandler.init();
            vkHandler.init();
        }

        void run(bool &keepRunning) {
            sdlHandler.run(keepRunning);
        }

        void cleanUp() {
            vkHandler.cleanUp();
            sdlHandler.cleanUp();
        }

    private:

    };
}