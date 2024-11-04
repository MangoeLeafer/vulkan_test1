#pragma once

#include "debug.hpp"

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>
#include <cstring>
#include <vector>

class ExtensionsComponent {
public:
    std::vector<const char*> debugExtensions = {
        "VK_EXT_debug_utils"
    };
    std::vector<const char*> extensions = {
        
    };

    void initExtensions() {
        std::vector<const char*> sdlExtensions = getSDLExtensions();

        extensions.insert(extensions.end(), sdlExtensions.begin(), sdlExtensions.end());

        if (IN_DEBUG_ENV && !checkDebugExtensions()) {
            throw std::runtime_error("Debug extensions requested, but not available!");
        }

        if (!checkExtensions()) {
            throw std::runtime_error("Extensions requested, but not available!");
        }
    }

private:
    std::vector<const char*> getSDLExtensions() {
        // Loading SDL3 extensions into the createInfo.
        uint32_t sdl3ExtensionCount = 0;
        const char*const* sdl3Extensions;

        sdl3Extensions = SDL_Vulkan_GetInstanceExtensions(&sdl3ExtensionCount);

        std::vector<const char*> extensions(sdl3Extensions, sdl3Extensions + sdl3ExtensionCount);

        return extensions;
    }

    bool checkExtensions() {
        // Finding what extensions are supported.
        uint32_t suppExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &suppExtensionCount, nullptr);
        std::vector<VkExtensionProperties> suppExtensions(suppExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &suppExtensionCount, suppExtensions.data());

        // Checking requested extensions.
        for (const char* extensionName : extensions) {
            bool extensionFound = false;

            for (VkExtensionProperties extensionsProperties : suppExtensions) {
                if (strcmp(extensionName, extensionsProperties.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }
            }

            if (!extensionFound) {
                std::cout << "Extension: \"" << extensionName << "\" could not be found!" << "\n";

                return false;
            }
        }

        return true;
    }

    bool checkDebugExtensions() {
        // Finding what extensions are supported.
        uint32_t suppExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &suppExtensionCount, nullptr);
        std::vector<VkExtensionProperties> suppExtensions(suppExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &suppExtensionCount, suppExtensions.data());

        // Checking requested debug extensions.
        for (const char* extensionName : debugExtensions) {
            bool extensionFound = false;

            for (VkExtensionProperties extensionsProperties : suppExtensions) {
                if (strcmp(extensionName, extensionsProperties.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }
            }

            if (!extensionFound) {
                std::cout << "Debug extension: \"" << extensionName << "\" could not be found!" << "\n";

                return false;
            }
        }

        return true;
    }
};