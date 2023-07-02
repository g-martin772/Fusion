#pragma once
#include "Renderer/Image.h"

namespace vk
{
    struct DescriptorImageInfo;
    class Instance;
    class PhysicalDevice;
    class RenderPass;
    class Queue;
    class Device;
    class CommandBuffer;
    class CommandPool;
}

namespace FusionEngine { namespace UI {

    void ImGuiInitVulkan(vk::Instance instance, vk::Device device, vk::PhysicalDevice physicalDevice, vk::RenderPass renderPass, uint32_t queueFamilyIndex, vk::Queue queue, uint32_t imageCount);
    void ImGuiShutdownVulkan(vk::Device device);
    void ImGuiNewFrame();
    void ImGuiRenderVulkan(vk::CommandBuffer commandBuffer);
    void UploadImGuiFontsVulkan(vk::CommandPool commandPool, vk::CommandBuffer commandBuffer, vk::Device device, vk::Queue queue);

    void* ImGuiCreateImageHandle(vk::DescriptorImageInfo imageInfo);
    void ImGuiFreeImageHandle(void* handle);
}}
