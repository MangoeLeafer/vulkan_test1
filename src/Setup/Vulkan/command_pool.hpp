#pragma once

#include "queues.hpp"

#include <vulkan/vulkan.hpp>

class CommandPoolComponent {
public:
    VkCommandPool getCommandPool() {return commandPool;}

    void createCommandPool(QueuesComponent cQueues, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        QueueFamilyIndices queueFamilyIndices = cQueues.findQueueFamilies(physicalDevice, surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool!");
        }
    }

    void destroyCommandPool(VkDevice device) {
        vkDestroyCommandPool(device, commandPool, nullptr);
    }
    
private:
    VkCommandPool commandPool;
};