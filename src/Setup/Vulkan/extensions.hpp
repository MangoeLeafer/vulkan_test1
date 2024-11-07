#pragma once

#include "debug.hpp"

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>
#include <cstring>
#include <vector>
#include <set>

class ExtensionsComponent {
public:
    std::vector<const char*>* get_pExtensions() {return &extensions;}
    std::vector<const char*> getExtensions() {return extensions;}
    std::vector<const char*> getDeviceExtensions() {return deviceExtensions;}

    void initExtensions() {
        std::vector<const char*> sdlExtensions = getSDLExtensions();

        extensions.insert(extensions.end(), sdlExtensions.begin(), sdlExtensions.end());

        if (IN_DEBUG_ENV && !checkDebugExtensions()) {
            throw std::runtime_error("Debug extensions requested, but not available!");
        }

        if (!checkExtensions()) {
            throw std::runtime_error("Extensions requested, but not available!");
        }

        if (IN_DEBUG_ENV) {
            extensions.insert(extensions.end(), debugExtensions.begin(), debugExtensions.end());
        }
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const VkExtensionProperties& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

private:
    const std::vector<const char*> debugExtensions {
        "VK_EXT_debug_utils"
    };
    const std::vector<const char*> deviceExtensions {
        "VK_KHR_swapchain"
    };
    std::vector<const char*> extensions {
        
    };

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

            for (VkExtensionProperties& extensionsProperties : suppExtensions) {
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

            for (VkExtensionProperties& extensionsProperties : suppExtensions) {
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