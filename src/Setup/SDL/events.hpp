#pragma once

#include "../Vulkan/handler.hpp"
#include "window.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>

class EventsComponent {
public:
    static void eventResizedWindow(VulkanHandler& cVkHandler) {
        cVkHandler.framebufferResized = true;
    }
private:
};