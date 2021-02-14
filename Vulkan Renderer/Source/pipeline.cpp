// Vulkan Renderer - pipeline.cpp
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
#include "pipeline.h"
#include "vkdefines.h"

Pipeline::Pipeline(const Shader& vertexShader, const Shader& fragmentShader)
{
    VkPipelineShaderStageCreateInfo shaderStages[2];
    
    VkPipelineShaderStageCreateInfo& vertexStateCreateInfo = shaderStages[0];
    vertexStateCreateInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexStateCreateInfo.pNext               = nullptr;
    vertexStateCreateInfo.flags               = 0;
    vertexStateCreateInfo.stage               = VK_SHADER_STAGE_VERTEX_BIT;
    vertexStateCreateInfo.module              = vertexShader.getModule();
    vertexStateCreateInfo.pName               = "main";
    vertexStateCreateInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo& fragmentStateCreateInfo = shaderStages[1];
    fragmentStateCreateInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentStateCreateInfo.pNext               = nullptr;
    fragmentStateCreateInfo.flags               = 0;
    fragmentStateCreateInfo.stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentStateCreateInfo.module              = fragmentShader.getModule();
    fragmentStateCreateInfo.pName               = "main";
    fragmentStateCreateInfo.pSpecializationInfo = nullptr;
    
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo;
    vertexInputCreateInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.pNext                           = nullptr;
    vertexInputCreateInfo.flags                           = 0;
    vertexInputCreateInfo.vertexBindingDescriptionCount   = 0;
    vertexInputCreateInfo.pVertexBindingDescriptions      = nullptr;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
    vertexInputCreateInfo.pVertexAttributeDescriptions    = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo;
    inputAssemblyCreateInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.pNext                  = nullptr;
    inputAssemblyCreateInfo.flags                  = 0;
    inputAssemblyCreateInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = 800.0f;
    viewport.height   = 600.0f;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x      = 0;
    scissor.offset.y      = 0;
    scissor.extent.width  = 800;
    scissor.extent.height = 600;

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
    viewportStateCreateInfo.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.pNext         = nullptr;
    viewportStateCreateInfo.flags         = 0;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports    = &viewport;
    viewportStateCreateInfo.scissorCount  = 1;
    viewportStateCreateInfo.pScissors     = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
    rasterizationStateCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.pNext                   = nullptr;
    rasterizationStateCreateInfo.flags                   = 0;
    rasterizationStateCreateInfo.depthClampEnable        = VK_FALSE;
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationStateCreateInfo.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizationStateCreateInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCreateInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCreateInfo.depthBiasEnable         = VK_FALSE;
    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizationStateCreateInfo.depthBiasClamp          = 0.0f;
    rasterizationStateCreateInfo.depthBiasSlopeFactor    = 0.0f;
    rasterizationStateCreateInfo.lineWidth               = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
    multisampleStateCreateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.pNext                 = nullptr;
    multisampleStateCreateInfo.flags                 = 0;
    multisampleStateCreateInfo.rasterizationSamples  = VK_SAMPLE_COUNT_32_BIT;
    multisampleStateCreateInfo.sampleShadingEnable   = VK_FALSE;
    multisampleStateCreateInfo.minSampleShading      = 1.0f;
    multisampleStateCreateInfo.pSampleMask           = nullptr;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable      = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
    colorBlendAttachmentState.blendEnable         = VK_TRUE;
    colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachmentState.colorBlendOp        = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp        = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
    colorBlendStateCreateInfo.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.pNext             = nullptr;
    colorBlendStateCreateInfo.flags             = 0;
    colorBlendStateCreateInfo.logicOpEnable     = VK_FALSE;
    colorBlendStateCreateInfo.logicOp           = VK_LOGIC_OP_NO_OP;
    colorBlendStateCreateInfo.attachmentCount   = 1;
    colorBlendStateCreateInfo.pAttachments      = &colorBlendAttachmentState;
    colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.pNext                  = nullptr;
    pipelineLayoutCreateInfo.flags                  = 0;
    pipelineLayoutCreateInfo.setLayoutCount         = 0;
    pipelineLayoutCreateInfo.pSetLayouts            = nullptr;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges    = nullptr;

    VkResult result = vkCreatePipelineLayout(Graphics::getVkDevice(), &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout);
    CHECK_VKRESULT(result);

    VkAttachmentDescription attachmentDescription;
    attachmentDescription.flags          = 0;
    attachmentDescription.format         = VK_FORMAT_B8G8R8A8_UNORM;
    attachmentDescription.samples        = VK_SAMPLE_COUNT_32_BIT;
    attachmentDescription.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference;
    attachmentReference.attachment = 0;
    attachmentReference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription;
    subpassDescription.flags                   = 0;
    subpassDescription.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.inputAttachmentCount    = 0;
    subpassDescription.pInputAttachments       = nullptr;
    subpassDescription.colorAttachmentCount    = 1;
    subpassDescription.pColorAttachments       = &attachmentReference;
    subpassDescription.pResolveAttachments     = nullptr;
    subpassDescription.pDepthStencilAttachment = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments    = nullptr;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext           = nullptr;
    renderPassCreateInfo.flags           = 0;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments    = &attachmentDescription;
    renderPassCreateInfo.subpassCount    = 1;
    renderPassCreateInfo.pSubpasses      = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies   = nullptr;

    result = vkCreateRenderPass(Graphics::getVkDevice(), &renderPassCreateInfo, nullptr, &_renderPass);
    CHECK_VKRESULT(result);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext               = nullptr;
    pipelineCreateInfo.flags               = 0;
    pipelineCreateInfo.stageCount          = 2;
    pipelineCreateInfo.pStages             = shaderStages;
    pipelineCreateInfo.pVertexInputState   = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineCreateInfo.pTessellationState  = nullptr;
    pipelineCreateInfo.pViewportState      = &viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
    pipelineCreateInfo.pMultisampleState   = &multisampleStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState  = nullptr;
    pipelineCreateInfo.pColorBlendState    = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDynamicState       = nullptr;
    pipelineCreateInfo.layout              = _pipelineLayout;
    pipelineCreateInfo.renderPass          = _renderPass;
    pipelineCreateInfo.subpass             = 0;
    pipelineCreateInfo.basePipelineHandle  = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex   = -1;

    result = vkCreateGraphicsPipelines(Graphics::getVkDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &_pipeline);
    CHECK_VKRESULT(result);
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(Graphics::getVkDevice(), _pipeline, nullptr);
    vkDestroyPipelineLayout(Graphics::getVkDevice(), _pipelineLayout, nullptr);
    vkDestroyRenderPass(Graphics::getVkDevice(), _renderPass, nullptr);
}
