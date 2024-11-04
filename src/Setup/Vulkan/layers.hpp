#pragma once

#include "debug.hpp"

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <cstring>
#include <vector>

class LayersComponent {
public:
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    const std::vector<const char*> layers = {
        
    };

    void initLayers() {
        if (IN_DEBUG_ENV && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        if (!checkLayerSupport()) {
            throw std::runtime_error("Layers requested, but not available!");
        }
    }

private:
    bool checkLayerSupport() {
        uint32_t layerCount;

        // Retrieving all instance layers.
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Checking requested layers.
        for (const char* layerName : layers) {
            bool layerFound = false;

            for (VkLayerProperties layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                std::cout << "Layer: \"" << layerName << "\" could not be found!" << "\n";

                return false;
            }
        }

        return true;
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;

        // Retrieving all instance layers.
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Checking requested layers.
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (VkLayerProperties layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                std::cout << "Validation Layer: \"" << layerName << "\" could not be found!" << "\n";

                return false;
            }
        }

        return true;
    }
};