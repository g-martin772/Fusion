#pragma once
#include "Renderer/Image.h"

#include <vulkan/vulkan.hpp>

#include "VulkanBuffer.h"

namespace FusionEngine
{
    class VulkanRenderApi;

    class VulkanImage : public Image
    {
    public:
        VulkanImage(const ImageSpecification& spec);
        ~VulkanImage() override;

        void OnResize(uint32_t width, uint32_t height) override;

        [[nodiscard]] void* GetImGuiHandle() override;
        
        [[nodiscard]] VkImageLayout GetVulkanImageLayout() const;
        [[nodiscard]] vk::Format GetVulkanFormat() const;
        
        [[nodiscard]] vk::Image GetImage() const { return m_Image; }
        [[nodiscard]] vk::ImageView GetImageView() const { return m_ImageView; }
        [[nodiscard]] vk::Sampler GetSampler() const { return m_Sampler; }
        [[nodiscard]] vk::DescriptorImageInfo GetDescriptorImageInfo() const { return m_ImageInfo; }
        [[nodiscard]] vk::DeviceMemory GetMemory() const { return m_Memory; }
    private:
        void CreateVulkanObjects();
    private:
        Shared<VulkanRenderApi> m_RenderApi;
        vk::Image m_Image;
        vk::ImageView m_ImageView;
        vk::Sampler m_Sampler;
        uint32_t m_Width, m_Height;
        vk::DescriptorImageInfo m_ImageInfo;
        vk::DeviceMemory m_Memory;
        void* m_ImGuiHandle = nullptr;
    };
}
