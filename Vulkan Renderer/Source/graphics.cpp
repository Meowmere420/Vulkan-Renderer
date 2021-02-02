// Vulkan Renderer - graphics.cpp
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

#include "Graphics.h"
#include "vkassert.h"

#pragma comment(lib, "vulkan-1.lib")

Graphics::Graphics()
{
    VkApplicationInfo applicationInfo;
    applicationInfo.sType                      = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext                      = nullptr;
    applicationInfo.pApplicationName           = "Vulkan Application";
    applicationInfo.applicationVersion         = VK_MAKE_VERSION(0, 0, 0);
    applicationInfo.pEngineName                = "Vulkan Renderer";
    applicationInfo.engineVersion              = VK_MAKE_VERSION(0, 0, 0);
    applicationInfo.apiVersion                 = VK_VERSION_1_2;

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                   = nullptr;
    instanceCreateInfo.flags                   = 0;
    instanceCreateInfo.pApplicationInfo        = &applicationInfo;
    instanceCreateInfo.enabledLayerCount       = 0;
    instanceCreateInfo.ppEnabledLayerNames     = nullptr;
    instanceCreateInfo.enabledExtensionCount   = 0;
    instanceCreateInfo.ppEnabledExtensionNames = nullptr;

    VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &_instance);
    CHECK_VKRESULT(result);
}

Graphics::~Graphics()
{
    vkDestroyInstance(_instance, nullptr);
}

VkInstance Graphics::GetVkInstance() const noexcept
{
    return _instance;
}
