#pragma once

#include "instance.hpp"
#include "debug.hpp"

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

class VulkanHandler {
public:
    InstanceComponent cInstance;
    DebugComponent cDebug;

    void init() {
        cInstance.create();
        
        if (IN_DEBUG_ENV) {
            cDebug.setupDebugMessenger(cInstance.instance);
        }
    }

    void cleanUp() {
        if (IN_DEBUG_ENV) {
            cDebug.destroyDebug(cInstance.instance);
        }

        cInstance.destroy();
    }

private:
};