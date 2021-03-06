// Vulkan Renderer - print_device_info.cpp
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

#include <iostream>

#include "print_device_info.h"
#include "vkdefines.h"

// I don't even understand why I have to disable this abomination, sometimes Visual Studio is just straight up retarded
#pragma warning(disable: 6385)

void printSinglePhysicalDeviceFeatures(const VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);

    // Print features if desired
}

void printSinglePhysicalDeviceMemoryProperties(const VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

    // Print memory properties if desired
}

void printSinglePhysicalDeviceInfo(const VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    std::cout << "Device name                   " << physicalDeviceProperties.deviceName << '\n';
    std::cout << "Vulkan driver version         " << VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion) << '.'
                                                  << VK_VERSION_MINOR(physicalDeviceProperties.apiVersion) << '.'
                                                  << VK_VERSION_PATCH(physicalDeviceProperties.apiVersion) << '\n';
    std::cout << "Vendor ID                     " << physicalDeviceProperties.vendorID << '\n';
    std::cout << "Device ID                     " << physicalDeviceProperties.deviceID << '\n';

    std::cout << "Device Type                   ";
    
    switch (physicalDeviceProperties.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        std::cout << "Unknown" << '\n';
        break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        std::cout << "Integrated GPU" << '\n';
        break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        std::cout << "Discrete GPU" << '\n';
        break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        std::cout << "Virtual GPU" << '\n';
        break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        std::cout << "CPU" << '\n';
        break;
    }

    std::cout << '\n';
}

void printPhysicalDeviceInfo(const VkPhysicalDevice* physicalDevices, const uint32_t physicalDeviceCount)
{
    std::cout << "==================================================" << '\n';
    std::cout << "Physical device properties" << '\n';
    std::cout << "==================================================" << '\n';

    std::cout << '\n';

    std::cout << "Physical devices: " << physicalDeviceCount << '\n';

    std::cout << '\n';

    for (uint32_t i = 0; i < physicalDeviceCount; ++i)
    {
        std::cout << "--------------------------------------------------" << '\n';
        std::cout << "Physical device: " << i << '\n';
        std::cout << "--------------------------------------------------" << '\n';

        printSinglePhysicalDeviceInfo(physicalDevices[i]);
        printSinglePhysicalDeviceFeatures(physicalDevices[i]);
        printSinglePhysicalDeviceMemoryProperties(physicalDevices[i]);
    }

    std::cout << std::endl;
}

void printSinglePhysicalDeviceQueueFamily(const VkQueueFamilyProperties queueFamilyProperties)
{
    std::cout << "Queue count                   " << queueFamilyProperties.queueCount << '\n';
    std::cout << "VK_QUEUE_GRAPHICS_BIT         " << ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 ? "true" : "false") << '\n';
    std::cout << "VK_QUEUE_COMPUTE_BIT          " << ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0 ? "true" : "false") << '\n';
    std::cout << "VK_QUEUE_TRANSFER_BIT         " << ((queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0 ? "true" : "false") << '\n';
    std::cout << "VK_QUEUE_SPARSE_BINDING_BIT   " << ((queueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0 ? "true" : "false") << '\n';
}

void printDeviceQueueFamilyProperties(const VkQueueFamilyProperties* deviceQueueFamilies, const uint32_t queueFamilyCount)
{
    std::cout << "Queue families: " << queueFamilyCount << '\n';

    std::cout << '\n';

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        std::cout << "Queue family: " << i << '\n';
        std::cout << "--------------------------------------------------" << '\n';

        printSinglePhysicalDeviceQueueFamily(deviceQueueFamilies[i]);

        std::cout << '\n';
    }
}

void printSingleLayerProperty(const VkLayerProperties& instanceLayer)
{
    std::cout << "Name                          " << instanceLayer.layerName << '\n';
    std::cout << "Specification version         " << instanceLayer.specVersion << '\n';
    std::cout << "Implementation version        " << instanceLayer.implementationVersion << '\n';

    std::cout << '\n';
}

void printLayerProperties(const VkLayerProperties* instanceLayers, const uint32_t layerPropertyCount)
{
    std::cout << "==================================================" << '\n';
    std::cout << "Vulkan layers" << '\n';
    std::cout << "==================================================" << '\n';

    std::cout << '\n';

    std::cout << "Layers: " << layerPropertyCount << '\n';

    std::cout << '\n';

    for (uint32_t i = 0; i < layerPropertyCount; i++)
    {
        std::cout << "Vulkan layer: " << i << '\n';
        std::cout << "--------------------------------------------------" << '\n';

        printSingleLayerProperty(instanceLayers[i]);
    }

    std::cout << std::endl;
}

void printSingleExtensionProperty(const VkExtensionProperties& extension)
{
    std::cout << "Name                          " << extension.extensionName << '\n';
    std::cout << "Specification version         " << extension.specVersion << '\n';

    std::cout << '\n';
}

void printExtensionProperties(const VkExtensionProperties* extensions, const uint32_t extensionCount)
{
    std::cout << "==================================================" << '\n';
    std::cout << "Vulkan extensions" << '\n';
    std::cout << "==================================================" << '\n';

    std::cout << '\n';

    std::cout << "Extensions: " << extensionCount << '\n';

    std::cout << '\n';

    for (uint32_t i = 0; i < extensionCount; i++)
    {
        std::cout << "Vulkan extension: " << i << '\n';
        std::cout << "--------------------------------------------------" << '\n';

        printSingleExtensionProperty(extensions[i]);
    }

    std::cout << std::endl;
}

void printSurfaceCapabilities(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
    std::cout << "==================================================" << '\n';
    std::cout << "Surface capabilities" << '\n';
    std::cout << "==================================================" << '\n';
    
    std::cout << std::endl;

    std::cout << "Min image count               " << surfaceCapabilities.minImageCount << '\n';
    std::cout << "Max image count               " << surfaceCapabilities.maxImageCount << '\n';
    std::cout << "Current width                 " << surfaceCapabilities.currentExtent.width << '\n';
    std::cout << "Current height                " << surfaceCapabilities.currentExtent.height << '\n';
    std::cout << "Min image width               " << surfaceCapabilities.minImageExtent.width << '\n';
    std::cout << "Min image height              " << surfaceCapabilities.minImageExtent.height << '\n';
    std::cout << "Max image width               " << surfaceCapabilities.maxImageExtent.width << '\n';
    std::cout << "Max image height              " << surfaceCapabilities.maxImageExtent.height << '\n';

    std::cout << std::endl;
}
