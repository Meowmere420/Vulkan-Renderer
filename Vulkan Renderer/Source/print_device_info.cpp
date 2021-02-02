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
#include "vkassert.h"

// I don't even understand why I have to disable this abomination, sometimes Visual Studio is just straight up retarded
#pragma warning(disable: 6385)

void printSinglePhysicalDeviceInfo(const VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    std::cout << "Device name            " << physicalDeviceProperties.deviceName << '\n';
    std::cout << "Vulkan driver version  " << VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion) << '.'
                                           << VK_VERSION_MINOR(physicalDeviceProperties.apiVersion) << '.'
                                           << VK_VERSION_PATCH(physicalDeviceProperties.apiVersion) << '\n';
    std::cout << "Vendor ID              " << physicalDeviceProperties.vendorID << '\n';
    std::cout << "Device ID              " << physicalDeviceProperties.deviceID << '\n';

    std::cout << "Device Type            ";
    
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
}

void printPhysicalDeviceInfo(const VkInstance instance)
{
    uint32_t physicalDeviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    CHECK_VKRESULT(result);

    VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);
    CHECK_VKRESULT(result);

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

        std::cout << '\n';
    }

    delete[] physicalDevices;
}
