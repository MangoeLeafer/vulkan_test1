#pragma once

#include "setup/setup.hpp"

namespace App {
    class Application {
    public:
        SDLHandler sdlHandler;
        VulkanHandler vkHandler;

        void init() {
            sdlHandler.init();
            vkHandler.init(sdlHandler.cWindow.get_pWindow());
        }

        void run(bool &keepRunning) {
            sdlHandler.run(keepRunning, vkHandler);
            vkHandler.drawFrame(sdlHandler.cWindow.get_pWindow());
        }

        void cleanUp() {
            vkHandler.cleanUp();
            sdlHandler.cleanUp();
        }

    private:

    };
}