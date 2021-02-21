// Vulkan Renderer - main.cpp
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

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "print_device_info.h"
#include "utility.h"
#include "vkdefines.h"
#include "windefines.h"

#pragma comment(lib, "vulkan-1.lib")

constexpr uint32_t windowWidth = 800;
constexpr uint32_t windowHeight = 600;

VkInstance        instance;
uint32_t          physicalDeviceCount;
VkPhysicalDevice* physicalDevices;
VkDevice          device;

HINSTANCE         windowClass;
HWND              windowHandle;
const TCHAR*      windowClassName = TEXT("Vulkan Renderer Window");
const TCHAR*      windowTitle = TEXT("Vulkan Renderer Window");

VkSurfaceKHR      surface;
VkSwapchainKHR    swapchain;
uint32_t          imageViewCount;
VkImageView*      imageViews;
VkFramebuffer*    framebuffers;

VkShaderModule    vertexShader;
VkShaderModule    fragmentShader;

VkPipelineLayout  pipelineLayout;
VkPipeline        pipeline;
VkRenderPass      renderPass;

VkCommandPool     commandPool;
VkCommandBuffer*  commandBuffers;

VkSemaphore       imageAvailable;
VkSemaphore       renderingComplete;

void createGraphics()
{
    VkApplicationInfo applicationInfo;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = "Vulkan Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    applicationInfo.pEngineName = "Vulkan Renderer";
    applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_2;

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
    enabledExtensionNames.push_back("VK_KHR_win32_surface");

    uint32_t extensionPropertyCount = 0;
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, nullptr);
    CHECK_VKRESULT(result);

    VkExtensionProperties* instanceExtensions = new VkExtensionProperties[extensionPropertyCount];
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, instanceExtensions);
    CHECK_VKRESULT(result);

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = uint32_t(enabledLayerNames.size());
    instanceCreateInfo.ppEnabledLayerNames = enabledLayerNames.data();
    instanceCreateInfo.enabledExtensionCount = uint32_t(enabledExtensionNames.size());
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames.data();

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
    CHECK_VKRESULT(result);

    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    CHECK_VKRESULT(result);

    physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);
    CHECK_VKRESULT(result);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, nullptr); // To-Do: Print device queue properties for all devices

    VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, queueFamilyProperties);

    float queuePriorities[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    VkDeviceQueueCreateInfo deviceQueueCreateInfo;
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = nullptr;
    deviceQueueCreateInfo.flags = 0;
    deviceQueueCreateInfo.queueFamilyIndex = 0; // To-Do: Choose optimal queue
    deviceQueueCreateInfo.queueCount = 4; // To-Do: Verify queue count
    deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

    VkPhysicalDeviceFeatures enabledDeviceFeatures = {};

    std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };

    VkDeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1; // To-Do: Enable optimal amount of queues
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;
    deviceCreateInfo.enabledExtensionCount = uint32_t(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &enabledDeviceFeatures;

    result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &device);
    CHECK_VKRESULT(result);

    VkQueue queue;
    vkGetDeviceQueue(device, 0, 0, &queue);

    printPhysicalDeviceInfo(physicalDevices, physicalDeviceCount);
    printDeviceQueueFamilyProperties(queueFamilyProperties, queueFamilyCount);
    printLayerProperties(instanceLayers, layerPropertyCount);
    printExtensionProperties(instanceExtensions, extensionPropertyCount);

    delete[] queueFamilyProperties;
    delete[] instanceLayers;
    delete[] instanceExtensions;
}

LRESULT WINAPI processMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CLOSE:
    {
        if (MessageBox(hwnd, L"Close application?", L"Vulkan Renderer Application", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
        {
            PostQuitMessage(0);
        }
        return 0;
    }
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

void createWindow()
{
    windowClass = GetModuleHandle(nullptr);

    WNDCLASSEX wndClass;
    ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_OWNDC;
    wndClass.lpfnWndProc = processMessage;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = windowClass;
    wndClass.hIcon = nullptr;
    wndClass.hCursor = nullptr;
    wndClass.lpszMenuName = nullptr;
    wndClass.lpszClassName = windowClassName;
    wndClass.hIconSm = nullptr;

    RegisterClassEx(&wndClass);

    LONG windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
    RECT clientSize = { 0, 0, LONG(windowWidth), LONG(windowHeight) };
    AdjustWindowRect(&clientSize, windowStyle, FALSE);

    windowHandle = CreateWindow(
        windowClassName,
        windowTitle,
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        clientSize.right - clientSize.left,
        clientSize.bottom - clientSize.top,
        NULL,
        NULL,
        windowClass,
        nullptr
    );

    HRESULT hresult = GetLastError();
    if (hresult != S_OK)
    {
        __debugbreak();
    }

    ShowWindow(windowHandle, SW_SHOWDEFAULT);
}

void createSurface()
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = windowClass;
    surfaceCreateInfo.hwnd = windowHandle;

    VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
    CHECK_VKRESULT(result);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[0], surface, &surfaceCapabilities);
    CHECK_VKRESULT(result);

    printSurfaceCapabilities(surfaceCapabilities);

    uint32_t supportedFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[0], surface, &supportedFormatCount, nullptr);

    VkSurfaceFormatKHR* surfaceFormats = new VkSurfaceFormatKHR[supportedFormatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevices[0], surface, &supportedFormatCount, surfaceFormats);

    delete[] surfaceFormats;
}

void createSwapchain()
{
    uint32_t presentModeCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[0], surface, &presentModeCount, nullptr);
    CHECK_VKRESULT(result);

    VkPresentModeKHR* presentModes = new VkPresentModeKHR[presentModeCount];
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevices[0], surface, &presentModeCount, presentModes);
    CHECK_VKRESULT(result);

    VkBool32 surfaceSupport = VK_FALSE;
    result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[0], 0, surface, &surfaceSupport);
    CHECK_VKRESULT(result);

    VkExtent2D imageExtent = { windowWidth, windowHeight };

    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = 2; // To-Do: Find optimal image count
    swapchainCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM; // To-Do: Find optimal image format
    swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; // To-Do: Find optimal color space
    swapchainCreateInfo.imageExtent = imageExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0;
    swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);
    CHECK_VKRESULT(result);

    result = vkGetSwapchainImagesKHR(device, swapchain, &imageViewCount, nullptr);
    CHECK_VKRESULT(result);

    VkImage* images = new VkImage[imageViewCount];
    result = vkGetSwapchainImagesKHR(device, swapchain, &imageViewCount, images);
    CHECK_VKRESULT(result);

    imageViews = new VkImageView[imageViewCount];

    for (uint32_t i = 0; i < imageViewCount; i++)
    {
        VkImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = images[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = VK_FORMAT_B8G8R8A8_UNORM; // To-Do: Find optimal image format
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        result = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageViews[i]);
        CHECK_VKRESULT(result);
    }

    delete[] presentModes;
    delete[] images;
}

void createShaders()
{
    std::vector<char> vertexShaderCode = loadFile("Source\\Shaders\\vertex_shader.spv");

    VkShaderModuleCreateInfo vertexShaderModuleCreateInfo;
    vertexShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexShaderModuleCreateInfo.pNext = nullptr;
    vertexShaderModuleCreateInfo.flags = 0;
    vertexShaderModuleCreateInfo.codeSize = vertexShaderCode.size();
    vertexShaderModuleCreateInfo.pCode = (uint32_t*)vertexShaderCode.data();

    vkCreateShaderModule(device, &vertexShaderModuleCreateInfo, nullptr, &vertexShader);

    std::vector<char> fragmentShaderCode = loadFile("Source\\Shaders\\fragment_shader.spv");

    VkShaderModuleCreateInfo fragmentShaderModuleCreateInfo;
    fragmentShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragmentShaderModuleCreateInfo.pNext = nullptr;
    fragmentShaderModuleCreateInfo.flags = 0;
    fragmentShaderModuleCreateInfo.codeSize = fragmentShaderCode.size();
    fragmentShaderModuleCreateInfo.pCode = (uint32_t*)fragmentShaderCode.data();

    vkCreateShaderModule(device, &fragmentShaderModuleCreateInfo, nullptr, &fragmentShader);
}

void createPipeline()
{
    VkPipelineShaderStageCreateInfo shaderStages[2];

    VkPipelineShaderStageCreateInfo& vertexStateCreateInfo = shaderStages[0];
    vertexStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexStateCreateInfo.pNext = nullptr;
    vertexStateCreateInfo.flags = 0;
    vertexStateCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexStateCreateInfo.module = vertexShader;
    vertexStateCreateInfo.pName = "main";
    vertexStateCreateInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo& fragmentStateCreateInfo = shaderStages[1];
    fragmentStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentStateCreateInfo.pNext = nullptr;
    fragmentStateCreateInfo.flags = 0;
    fragmentStateCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentStateCreateInfo.module = fragmentShader;
    fragmentStateCreateInfo.pName = "main";
    fragmentStateCreateInfo.pSpecializationInfo = nullptr;

    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.pNext = nullptr;
    vertexInputCreateInfo.flags = 0;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
    vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
    vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.pNext = nullptr;
    inputAssemblyCreateInfo.flags = 0;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(windowWidth);
    viewport.height = static_cast<float>(windowHeight);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.width = windowWidth;
    scissor.extent.height = windowHeight;

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.pNext = nullptr;
    viewportStateCreateInfo.flags = 0;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
    rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.pNext = nullptr;
    rasterizationStateCreateInfo.flags = 0;
    rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
    rasterizationStateCreateInfo.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.pNext = nullptr;
    multisampleStateCreateInfo.flags = 0;
    multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.minSampleShading = 1.0f;
    multisampleStateCreateInfo.pSampleMask = nullptr;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
    colorBlendAttachmentState.blendEnable = VK_TRUE;
    colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.pNext = nullptr;
    colorBlendStateCreateInfo.flags = 0;
    colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_NO_OP;
    colorBlendStateCreateInfo.attachmentCount = 1;
    colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
    colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext = nullptr;
    pipelineLayoutCreateInfo.flags = 0;
    pipelineLayoutCreateInfo.setLayoutCount = 0;
    pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    VkResult result = vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
    CHECK_VKRESULT(result);

    VkAttachmentDescription attachmentDescription;
    attachmentDescription.flags = 0;
    attachmentDescription.format = VK_FORMAT_B8G8R8A8_UNORM;
    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference;
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription;
    subpassDescription.flags = 0;
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &attachmentReference;
    subpassDescription.pResolveAttachments = nullptr;
    subpassDescription.pDepthStencilAttachment = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = nullptr;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = nullptr;

    result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);
    CHECK_VKRESULT(result);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineCreateInfo.pTessellationState = nullptr;
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState = nullptr;
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.layout = pipelineLayout;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline);
    CHECK_VKRESULT(result);
}

void createFramebuffers()
{
    framebuffers = new VkFramebuffer[imageViewCount];

    for (uint32_t i = 0; i < imageViewCount; i++)
    {
        VkFramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = nullptr;
        framebufferCreateInfo.flags = 0;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = imageViews;
        framebufferCreateInfo.width = windowWidth;
        framebufferCreateInfo.height = windowHeight;
        framebufferCreateInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffers[i]);
        CHECK_VKRESULT(result);
    }
}

void createCommandPool()
{
    VkCommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = nullptr;
    commandPoolCreateInfo.flags = 0;
    commandPoolCreateInfo.queueFamilyIndex = 0; // To-Do: Choose optimal queue index

    VkResult result = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool);
    CHECK_VKRESULT(result);
}

void createCommandBuffers()
{
    commandBuffers = new VkCommandBuffer[imageViewCount];

    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = imageViewCount;

    vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers);
}

void recordCommandBuffers()
{
    VkCommandBufferBeginInfo commandBufferBeginInfo;
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    commandBufferBeginInfo.pInheritanceInfo = nullptr;

    for (uint32_t i = 0; i < imageViewCount; i++)
    {
        VkResult result = vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);
        CHECK_VKRESULT(result);

        VkClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };

        VkRenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.framebuffer = framebuffers[i];
        renderPassBeginInfo.renderArea = { 0, 0, windowWidth, windowHeight };
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        result = vkEndCommandBuffer(commandBuffers[i]);
        CHECK_VKRESULT(result);
    }
}

void createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    VkResult result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailable);
    CHECK_VKRESULT(result);
    
    result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderingComplete);
    CHECK_VKRESULT(result);
}

void destroyGraphics()
{
    vkDeviceWaitIdle(device);

    vkDestroySemaphore(device, imageAvailable, nullptr);
    vkDestroySemaphore(device, renderingComplete, nullptr);
 
    vkFreeCommandBuffers(device, commandPool, imageViewCount, commandBuffers);
    vkDestroyCommandPool(device, commandPool, nullptr);

    for (uint32_t i = 0; i < imageViewCount; i++)
    {
        vkDestroyFramebuffer(device, framebuffers[i], nullptr);
    }

    vkDestroyPipeline(device, pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    vkDestroyShaderModule(device, vertexShader, nullptr);
    vkDestroyShaderModule(device, fragmentShader, nullptr);

    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    
    for (uint32_t i = 0; i < imageViewCount; i++)
    {
        vkDestroyImageView(device, imageViews[i], nullptr);
    }

    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);

    DestroyWindow(windowHandle);
    UnregisterClass(windowClassName, windowClass);

    delete[] commandBuffers;
    delete[] framebuffers;
    delete[] imageViews;
    delete[] physicalDevices;
}

int main()
{
    createGraphics();
    createWindow();
    createSurface();
    createSwapchain();
    createShaders();
    createPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    recordCommandBuffers();
    createSemaphores();

    while (bool running = true)
    {
        static MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                destroyGraphics();
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    std::cin.get();
}
