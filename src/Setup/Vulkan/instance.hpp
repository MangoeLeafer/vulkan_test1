#pragma once

#include "layers.hpp"
#include "extensions.hpp"
#include "debug.hpp"

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

class InstanceComponent {
public:
    VkInstance instance;
    LayersComponent cLayers;
    ExtensionsComponent cExtensions;

    void create() {
        // Defining some properties for the Vulkan driver.
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "vulkan_testing1";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Initializing the creation info of the VkInstance.
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Extensions handling.
        cExtensions.initExtensions();

        std::vector<const char*> extensions = getExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Layers handling.
        cLayers.initLayers();

        std::vector<const char*> layers = getLayers();
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        DebugComponent::populateDebugMessengerCreateInfo(debugCreateInfo);

        // Creation and validation of Vulkan instance.
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (result != VK_SUCCESS) {
            std::cout << "Vulkan instance creation failure code: " << result << "\n";

            throw std::runtime_error("failed to create Vulkan instance!");
        } 
    }

    void destroy() {
        vkDestroyInstance(instance, nullptr);
    }

private:
    std::vector<const char*> getLayers() {
        std::vector<const char*> allLayers(cLayers.layers);

        allLayers.insert(
            allLayers.end(),
            cLayers.validationLayers.begin(),
            cLayers.validationLayers.end()
        );

        return allLayers;
    }

    std::vector<const char*> getExtensions() {
        std::vector<const char*> allExtensions(cExtensions.extensions);

        allExtensions.insert(
            allExtensions.end(),
            cExtensions.debugExtensions.begin(),
            cExtensions.debugExtensions.end()
        );

        return allExtensions;
    }
};