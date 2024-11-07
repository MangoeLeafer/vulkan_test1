#pragma once

#include "debug.hpp"
#include "extensions.hpp"
#include "layers.hpp"
#include "instance.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "image_views.hpp"

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
    }

    void cleanUp() {
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