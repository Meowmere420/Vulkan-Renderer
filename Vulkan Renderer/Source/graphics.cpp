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

#include <vector>

#include "graphics.h"
#include "print_device_info.h"
#include "vkassert.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

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
    applicationInfo.apiVersion                 = VK_API_VERSION_1_2;

    uint32_t layerPropertyCount = 0;
    VkResult result = vkEnumerateInstanceLayerProperties(&layerPropertyCount, nullptr);
    CHECK_VKRESULT(result);

    VkLayerProperties* instanceLayers = new VkLayerProperties[layerPropertyCount];
    result = vkEnumerateInstanceLayerProperties(&layerPropertyCount, instanceLayers);
    CHECK_VKRESULT(result);

    std::vector<const char*> enabledLayerNames;
    #ifdef _DEBUG
    enabledLayerNames.push_back("VK_LAYER_KHRONOS_validation");
    #endif

    std::vector<const char*> enabledExtensionNames;
    enabledExtensionNames.push_back("VK_KHR_surface");
    enabledExtensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    uint32_t extensionPropertyCount = 0;
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, nullptr);
    CHECK_VKRESULT(result);

    VkExtensionProperties* instanceExtensions = new VkExtensionProperties[extensionPropertyCount];
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, instanceExtensions);
    CHECK_VKRESULT(result);

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                   = nullptr;
    instanceCreateInfo.flags                   = 0;
    instanceCreateInfo.pApplicationInfo        = &applicationInfo;
    instanceCreateInfo.enabledLayerCount       = uint32_t(enabledLayerNames.size());
    instanceCreateInfo.ppEnabledLayerNames     = enabledLayerNames.data();
    instanceCreateInfo.enabledExtensionCount   = uint32_t(enabledExtensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &_instance);
    CHECK_VKRESULT(result);

    GLFWwindow* window = nullptr;

    VkSurfaceKHR surface;
    result = glfwCreateWindowSurface(_instance, window, nullptr, &surface);

    uint32_t physicalDeviceCount = 0;
    result = vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);
    CHECK_VKRESULT(result);

    VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
    result = vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevices);
    CHECK_VKRESULT(result);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, nullptr); // To-Do: Print device queue properties for all devices

    VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, queueFamilyProperties);

    float queuePriorities[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    VkDeviceQueueCreateInfo deviceQueueCreateInfo;
    deviceQueueCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext                = nullptr;
    deviceQueueCreateInfo.flags                = 0;
    deviceQueueCreateInfo.queueFamilyIndex     = 0; // To-Do: Choose optimal queue
    deviceQueueCreateInfo.queueCount           = 4; // To-Do: Verify queue count
    deviceQueueCreateInfo.pQueuePriorities     = queuePriorities;

    VkPhysicalDeviceFeatures enabledDeviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType                     = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext                     = nullptr;
    deviceCreateInfo.flags                     = 0;
    deviceCreateInfo.queueCreateInfoCount      = 1; // To-Do: Enable optimal amount of queues
    deviceCreateInfo.pQueueCreateInfos         = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledLayerCount         = 0;
    deviceCreateInfo.ppEnabledLayerNames       = nullptr;
    deviceCreateInfo.enabledExtensionCount     = 0;
    deviceCreateInfo.ppEnabledExtensionNames   = nullptr;
    deviceCreateInfo.pEnabledFeatures          = &enabledDeviceFeatures;

    result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &_device);
    CHECK_VKRESULT(result);

    VkQueue queue;
    vkGetDeviceQueue(_device, 0, 0, &queue);

    printPhysicalDeviceInfo(physicalDevices, physicalDeviceCount);
    printDeviceQueueFamilyProperties(queueFamilyProperties, queueFamilyCount);
    printLayerProperties(instanceLayers, layerPropertyCount);
    printExtensionProperties(instanceExtensions, extensionPropertyCount);

    delete[] physicalDevices;
    delete[] queueFamilyProperties;
    delete[] instanceLayers;
    delete[] instanceExtensions;
}

Graphics::~Graphics()
{
    vkDeviceWaitIdle(_device);

    vkDestroyDevice(_device, nullptr);
    vkDestroyInstance(_instance, nullptr);
}

VkInstance Graphics::GetVkInstance() const noexcept
{
    return _instance;
}

VkDevice Graphics::GetVkDevice() const noexcept
{
    return _device;
}
