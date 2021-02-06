// Vulkan Renderer - swapchain.cpp
//
// Copyright (c) 2020 Meowmere
//
// https://github.com/Meowmere420/Vulkan-Renderer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "graphics.h"
#include "swapchain.h"
#include "vkdefines.h"

Swapchain::Swapchain(VkSurfaceKHR surface)
{
    uint32_t presentModeCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(Graphics::getVkPhysicalDevice(0), surface, &presentModeCount, nullptr);
    CHECK_VKRESULT(result);

    VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(Graphics::getVkPhysicalDevice(0), surface, &presentModeCount, presentModes);
    CHECK_VKRESULT(result);

    VkBool32 surfaceSupport = VK_FALSE;
    result = vkGetPhysicalDeviceSurfaceSupportKHR(Graphics::getVkPhysicalDevice(0), 0, surface, &surfaceSupport);
    CHECK_VKRESULT(result);

    VkExtent2D imageExtent = { 800, 600 };

    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext                 = nullptr;
    swapchainCreateInfo.flags                 = 0;
    swapchainCreateInfo.surface               = surface;
    swapchainCreateInfo.minImageCount         = 2; // To-Do: Find optimal image count
    swapchainCreateInfo.imageFormat           = VK_FORMAT_B8G8R8A8_UNORM; // To-Do: Find optimal image format
    swapchainCreateInfo.imageColorSpace       = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // To-Do: Find optimal color space
    swapchainCreateInfo.imageExtent           = imageExtent;
    swapchainCreateInfo.imageArrayLayers      = 1;
    swapchainCreateInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0;
    swapchainCreateInfo.pQueueFamilyIndices   = nullptr;
    swapchainCreateInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode           = VK_PRESENT_MODE_MAILBOX_KHR;
    swapchainCreateInfo.clipped               = VK_TRUE;
    swapchainCreateInfo.oldSwapchain          = VK_NULL_HANDLE;

    result = vkCreateSwapchainKHR(Graphics::getVkDevice(), &swapchainCreateInfo, nullptr, &_swapchain);
    CHECK_VKRESULT(result);

    result = vkGetSwapchainImagesKHR(Graphics::getVkDevice(), _swapchain, &_imageCount, nullptr);
    CHECK_VKRESULT(result);

    VkImage* images = new VkImage[_imageCount];
    result = vkGetSwapchainImagesKHR(Graphics::getVkDevice(), _swapchain, &_imageCount, images);
    CHECK_VKRESULT(result);

    _imageViews = new VkImageView[_imageCount];

    for (uint32_t i = 0; i < _imageCount; i++)
    {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext                           = nullptr;
        imageViewCreateInfo.flags                           = 0;
        imageViewCreateInfo.image                           = images[i];
        imageViewCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format                          = VK_FORMAT_B8G8R8A8_UNORM; // To-Do: Find optimal image format
        imageViewCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
        imageViewCreateInfo.subresourceRange.levelCount     = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount     = 1;

        result = vkCreateImageView(Graphics::getVkDevice(), &imageViewCreateInfo, nullptr, &_imageViews[i]);
        CHECK_VKRESULT(result);
    }

    delete[] presentModes;
    delete[] images;
}

Swapchain::~Swapchain()
{
    for (uint32_t i = 0; i < _imageCount; i++)
    {
        vkDestroyImageView(Graphics::getVkDevice(), _imageViews[i], nullptr);
    }
    vkDestroySwapchainKHR(Graphics::getVkDevice(), _swapchain, nullptr);
    delete[] _imageViews;
}

void Swapchain::swap() const
{
}
