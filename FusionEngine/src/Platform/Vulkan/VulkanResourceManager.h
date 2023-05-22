#pragma once
#include <vulkan/vulkan.hpp>

#include "VulkanBuffer.h"

namespace FusionEngine
{
    class VulkanRenderApi;

    class VulkanResourceManager
    {
    public:
        VulkanResourceManager(uint32_t frameCount = 1);
        ~VulkanResourceManager();

        void SetFrameCount(uint32_t frameCount);

        void AddResource(const vk::DescriptorSetLayoutBinding& binding);
        void AddFrameResource(const vk::DescriptorSetLayoutBinding& binding);

        void BindResource(uint32_t bindingIndex, const VulkanBuffer* buffer) const;
        void BindFrameResource(uint32_t bindingIndex, const VulkanBuffer* buffer) const;
        // for later: void Bind(uint32_t bindingIndex, vk::ImageView imageView, vk::Sampler sampler);
        
        [[nodiscard]] vk::DescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }
        [[nodiscard]] vk::DescriptorSet GetFrameDescriptorSet(const uint32_t frameIndex) const { return m_FrameDescriptorSets[frameIndex]; }

        [[nodiscard]] bool HasDescriptorSet() const { return m_DescriptorSet != vk::DescriptorSet(nullptr); }
        [[nodiscard]] bool HasFrameDescriptorSet(const uint32_t frameIndex) const { return m_FrameDescriptorSets[frameIndex] != vk::DescriptorSet(nullptr); }
    private:
        void RecreateResources();
        void RecreateFrameResources();
    private:
        Ref<VulkanRenderApi> m_RenderApi;
        uint32_t m_FrameCount;
        
        std::vector<vk::DescriptorSetLayoutBinding> m_LayoutBindings;
        std::vector<vk::DescriptorSetLayoutBinding> m_FrameLayoutBindings;

        vk::DescriptorSetLayout m_DescriptorSetLayout;
        vk::DescriptorSetLayout m_FrameDescriptorSetLayout;
        
        vk::DescriptorPool m_DescriptorPool;
        vk::DescriptorPool m_FrameDescriptorPool;

        vk::DescriptorSet m_DescriptorSet;
        std::vector<vk::DescriptorSet> m_FrameDescriptorSets;
    };
}
