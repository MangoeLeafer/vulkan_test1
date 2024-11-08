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

#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

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
    }

    void cleanUp() {
        cCommandPool.destroyCommandPool(cDevice.getLogicalDevice());
        cFramebuffers.destroyFramebuffers(cDevice.getLogicalDevice());
        cGraphicsPipeline.destroy(cDevice.getLogicalDevice());
        cRenderPass.destroyRenderPass(cDevice.getLogicalDevice());
        cImageViews.destroyImageViews(cDevice.getLogicalDevice());
        cSwapChain.destroySwapChain(cDevice.getLogicalDevice());
        cDevice.destroyDevice();

        if (IN_DEBUG_ENV) {
            cDebug.destroyDebug(cInstance.getInstance());
        }

        cSurface.destroySurface(cInstance.getInstance());
        cInstance.destroy();
    }

private:
};