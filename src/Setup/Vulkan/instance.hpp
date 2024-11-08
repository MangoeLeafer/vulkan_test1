#pragma once

#include "debug.hpp"
#include "layers.hpp"
#include "extensions.hpp"

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

class InstanceComponent {
public:
    void create(LayersComponent cLayers, ExtensionsComponent cExtensions) {
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
        std::vector<const char*> extensions = cExtensions.getExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Layers handling.
        std::vector<const char*> layers = cLayers.getLayers();
        createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        createInfo.ppEnabledLayerNames = layers.data();

        if (IN_DEBUG_ENV) {
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            DebugComponent::populateDebugMessengerCreateInfo(debugCreateInfo);

            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            createInfo.pNext = nullptr;
        }

        // Creation and validation of Vulkan instance.
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        } 
    }

    void destroy() {
        vkDestroyInstance(instance, nullptr);
    }

    VkInstance getInstance() {return instance;}

private:
    VkInstance instance;
};