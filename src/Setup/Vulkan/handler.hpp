#pragma once

#include "debug.hpp"
#include "extensions.hpp"
#include "layers.hpp"
#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "image_views.hpp"
#include "graphics_pipeline.hpp"
#include "shader_modules.hpp"
#include "render_pass.hpp"
#include "framebuffers.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "sync_objects.hpp"
#include "settings.hpp"

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <iostream>
#include <cstdint>
#include <vector>

class VulkanHandler {
public:
    DebugComponent cDebug;
    ExtensionsComponent cExtensions;
    LayersComponent cLayers;
    InstanceComponent cInstance;
    DeviceComponent cDevice;
    QueuesComponent cQueues;
    SurfaceComponent cSurface;
    SwapchainComponent cSwapChain;
    ImageViewsComponent cImageViews;
    GraphicsPipelineComponent cGraphicsPipeline;
    ShaderModulesComponent cShaderModules;
    RenderPassComponent cRenderPass;
    FramebuffersComponent cFramebuffers;
    CommandPoolComponent cCommandPool;
    CommandBufferComponent cCommandBuffer;
    SyncObjectsComponent cSyncObjects;
    uint32_t currentFrame = 0;
    bool framebufferResized = false;

    void init(SDL_Window* window) {
        cLayers.initLayers();
        cExtensions.initExtensions();
        cInstance.create(cLayers, cExtensions);
        
        if (IN_DEBUG_ENV) {
            cDebug.setupDebugMessenger(cInstance.getInstance());
        }

        cSurface.createSurface(window, cInstance.getInstance());
        cDevice.pickPhysicalDevice(cQueues, cExtensions, cInstance.getInstance(), cSurface.getSurface());
        cDevice.createLogicalDevice(cQueues, cLayers, cExtensions, cSurface.getSurface());
        cSwapChain.createSwapChain(cQueues, cDevice.getLogicalDevice(), cDevice.getPhysicalDevice(), cSurface.getSurface(), window);
        cImageViews.createImageViews(cDevice.getLogicalDevice(), cSwapChain.getSwapChainImages(), cSwapChain.getSwapChainImageFormat());
        cRenderPass.createRenderPass(cSwapChain, cDevice.getLogicalDevice());
        cGraphicsPipeline.createGraphicsPipeline(cShaderModules, cSwapChain, cDevice.getLogicalDevice(), cRenderPass.getRenderPass());
        cFramebuffers.createFramebuffers(cSwapChain, cImageViews, cRenderPass.getRenderPass(), cDevice.getLogicalDevice());
        cCommandPool.createCommandPool(cQueues, cDevice.getLogicalDevice(), cDevice.getPhysicalDevice(), cSurface.getSurface());
        cCommandBuffer.createCommandBuffer(cDevice.getLogicalDevice(), cCommandPool.getCommandPool());
        cSyncObjects.createSyncObjects(cDevice);
    }

    void drawFrame(SDL_Window* window) {
        vkWaitForFences(cDevice.getLogicalDevice(), 1, &cSyncObjects.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(cDevice.getLogicalDevice(), cSwapChain.getSwapChain(), UINT64_MAX, cSyncObjects.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain(window);
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        vkResetFences(cDevice.getLogicalDevice(), 1, &cSyncObjects.inFlightFences[currentFrame]);

        vkResetCommandBuffer(cCommandBuffer.commandBuffers[currentFrame], 0);
        cCommandBuffer.recordCommandBuffer(cSwapChain, cFramebuffers, imageIndex, cRenderPass.getRenderPass(), cGraphicsPipeline.getGraphicsPipeline(), currentFrame);

        VkSemaphore waitSemaphores[] = {cSyncObjects.imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cCommandBuffer.commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {cSyncObjects.renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(cQueues.graphicsQueue, 1, &submitInfo, cSyncObjects.inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {cSwapChain.getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(cQueues.presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain(window);
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void cleanUp() {
        vkDeviceWaitIdle(cDevice.getLogicalDevice());

        cleanupSwapChain();
        cSyncObjects.destroySyncObjects(cDevice);
        cCommandPool.destroyCommandPool(cDevice.getLogicalDevice());
        cFramebuffers.destroyFramebuffers(cDevice.getLogicalDevice());
        cGraphicsPipeline.destroy(cDevice.getLogicalDevice());
        cRenderPass.destroyRenderPass(cDevice.getLogicalDevice());
        cImageViews.destroyImageViews(cDevice.getLogicalDevice());
        cDevice.destroyDevice();

        if (IN_DEBUG_ENV) {
            cDebug.destroyDebug(cInstance.getInstance());
        }

        cSurface.destroySurface(cInstance.getInstance());
        cInstance.destroy();
    }

private:
    void recreateSwapChain(SDL_Window* window) {
        int width = 0, height = 0;
        SDL_GetWindowSizeInPixels(window, &width, &height);

        while (width == 0 || height == 0) {
            SDL_GetWindowSizeInPixels(window, &width, &height);
            SDL_WaitEvent(nullptr);
        }

        vkDeviceWaitIdle(cDevice.getLogicalDevice());

        cleanupSwapChain();

        cSwapChain.createSwapChain(cQueues, cDevice.getLogicalDevice(), cDevice.getPhysicalDevice(), cSurface.getSurface(), window);
        cImageViews.createImageViews(cDevice.getLogicalDevice(), cSwapChain.getSwapChainImages(), cSwapChain.getSwapChainImageFormat());
        cFramebuffers.createFramebuffers(cSwapChain, cImageViews, cRenderPass.getRenderPass(), cDevice.getLogicalDevice());
    }

    void cleanupSwapChain() {
        VkDevice device = cDevice.getLogicalDevice();
        std::vector<VkFramebuffer> swapChainFramebuffers = cFramebuffers.getSwapChainFrameBuffers();
        std::vector<VkImageView> swapChainImageViews = cImageViews.getSwapChainImageViews();
        VkSwapchainKHR swapChain = cSwapChain.getSwapChain();

        for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
        }

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            vkDestroyImageView(device, swapChainImageViews[i], nullptr);
        }

        vkDestroySwapchainKHR(device, swapChain, nullptr);
        cSwapChain.destroySwapChain(cDevice.getLogicalDevice());
    }
};