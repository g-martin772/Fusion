#pragma once
#include "VulkanInstance.h"

namespace FusionEngine
{
    class VulkanSwapChain;
}

namespace FusionEngine
{
    class VulkanDevice
    {
    public:
        VulkanDevice(VulkanInstance* instance, const VulkanSwapChain* swapChain);
        ~VulkanDevice();

        [[nodiscard]] vk::PhysicalDevice Physical() const {return m_PhysicalDevice;}
        [[nodiscard]] vk::Device Logical() const {return m_LogicalDevice;}
        [[nodiscard]] vk::Queue GraphicsQueue() const {return m_GraphicsQueue;}
        [[nodiscard]] vk::Queue PresentQueue() const {return m_PresentQueue;}
        [[nodiscard]] std::optional<uint32_t> GraphicsFamily() const {return m_GraphicsFamily;}
        [[nodiscard]] std::optional<uint32_t> PresentFamily() const {return m_PresentFamily;}
    private:
        void CreatePhysicalDevice();
        void CreateLogicalDevice(vk::SurfaceKHR surface);
        void CreateGraphicsQueue();
        void CreatePresentQueue();

        VulkanInstance* m_Instance;
        
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_LogicalDevice;

        vk::Queue m_GraphicsQueue;
        vk::Queue m_PresentQueue;
        std::optional<uint32_t> m_GraphicsFamily;
        std::optional<uint32_t> m_PresentFamily;
    };
}
