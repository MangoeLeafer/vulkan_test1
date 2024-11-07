#pragma once

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>

class SurfaceComponent {
public:
    VkSurfaceKHR getSurface() {return surface;}

    void createSurface(SDL_Window* window, VkInstance instance) {
        if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    void destroySurface(VkInstance instance) {
        SDL_Vulkan_DestroySurface(instance, surface, nullptr);
    }

private:
    VkSurfaceKHR surface;
};