// Vulkan Renderer - surface.cpp
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
#include "print_device_info.h"
#include "surface.h"
#include "vkdefines.h"
#include "window.h"

Surface::Surface(HWND windowHandle, HINSTANCE windowClass)
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext     = nullptr;
    surfaceCreateInfo.flags     = 0;
    surfaceCreateInfo.hinstance = windowClass;
    surfaceCreateInfo.hwnd      = windowHandle;

    VkResult result = vkCreateWin32SurfaceKHR(Graphics::getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
    CHECK_VKRESULT(result);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Graphics::getVkPhysicalDevice(0), _surface, &surfaceCapabilities);
    CHECK_VKRESULT(result);

    printSurfaceCapabilities(surfaceCapabilities);

    uint32_t supportedFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(Graphics::getVkPhysicalDevice(0), _surface, &supportedFormatCount, nullptr);

    VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[supportedFormatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(Graphics::getVkPhysicalDevice(0), _surface, &supportedFormatCount, surfaceFormats);

    delete[] surfaceFormats;
}

Surface::~Surface()
{
    vkDestroySurfaceKHR(Graphics::getVkInstance(), _surface, nullptr);
}

VkSurfaceKHR Surface::getSurface() const noexcept
{
    return _surface;
}
