#pragma once

#include "image_views.hpp"
#include "swapchain.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>

class FramebuffersComponent {
public:
    std::vector<VkFramebuffer> getSwapChainFrameBuffers() {return swapChainFramebuffers;}

    void createFramebuffers(SwapchainComponent cSwapChain, ImageViewsComponent cImageViews, VkRenderPass renderPass, VkDevice device) {
        std::vector<VkImageView> swapChainImageViews = cImageViews.getSwapChainImageViews();
        VkExtent2D swapChainExtent = cSwapChain.getSwapChainExtent();

        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void destroyFramebuffers(VkDevice device) {
        for (VkFramebuffer framebuffer : swapChainFramebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
    }
private:
    std::vector<VkFramebuffer> swapChainFramebuffers;
};