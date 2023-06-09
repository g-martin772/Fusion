#pragma once

namespace vk
{
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
    
}}
