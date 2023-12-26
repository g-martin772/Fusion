#include "fepch.h"
#include "VulkanResourceManager.h"

#include <map>

#include "VulkanDevice.h"
#include "VulkanRenderApi.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    VulkanResourceManager::VulkanResourceManager(const uint32_t frameCount)
    {
        m_FrameCount = frameCount;
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());

        
    }

    VulkanResourceManager::~VulkanResourceManager()
    {
        m_RenderApi->m_Device->Logical().destroyDescriptorPool(m_DescriptorPool);
        m_RenderApi->m_Device->Logical().destroyDescriptorPool(m_FrameDescriptorPool);
        m_RenderApi->m_Device->Logical().destroyDescriptorSetLayout(m_DescriptorSetLayout);
        m_RenderApi->m_Device->Logical().destroyDescriptorSetLayout(m_FrameDescriptorSetLayout);
    }

    void VulkanResourceManager::SetFrameCount(const uint32_t frameCount)
    {
        if(frameCount != m_FrameCount)
        {
            m_FrameCount = frameCount;
            RecreateFrameResources();
        }
        else
        {
            m_FrameCount = frameCount;
        }
    }

    void VulkanResourceManager::RecreateResources()
    {
        m_DescriptorSetLayout = m_RenderApi->m_Device->Logical().createDescriptorSetLayout(
            vk::DescriptorSetLayoutCreateInfo({},
                static_cast<uint32_t>(m_LayoutBindings.size()),
                m_LayoutBindings.data()));

        std::vector<vk::DescriptorPoolSize> poolSizes;
        std::map<vk::DescriptorType, uint32_t> descriptorCounts;

        for (const auto& binding : m_LayoutBindings) {
            descriptorCounts[binding.descriptorType] += binding.descriptorCount;
        }

        poolSizes.reserve(descriptorCounts.size());
        for (const auto& [fst, snd] : descriptorCounts) {
            poolSizes.emplace_back(fst, snd);
        }

        m_DescriptorPool = m_RenderApi->m_Device->Logical().createDescriptorPool(
            vk::DescriptorPoolCreateInfo({},
                1,
                static_cast<uint32_t>(poolSizes.size()),
                poolSizes.data()));

        m_DescriptorSet = m_RenderApi->m_Device->Logical().allocateDescriptorSets(
            vk::DescriptorSetAllocateInfo(m_DescriptorPool, 1, &m_DescriptorSetLayout))[0];
    }

    void VulkanResourceManager::RecreateFrameResources()
    {
        m_FrameDescriptorSetLayout = m_RenderApi->m_Device->Logical().createDescriptorSetLayout(
            vk::DescriptorSetLayoutCreateInfo({},
                static_cast<uint32_t>(m_FrameLayoutBindings.size()),
                m_FrameLayoutBindings.data()));

        std::vector<vk::DescriptorPoolSize> poolSizes;
        std::map<vk::DescriptorType, uint32_t> descriptorCounts;

        for (const auto& binding : m_FrameLayoutBindings) {
            descriptorCounts[binding.descriptorType] += binding.descriptorCount;
        }

        poolSizes.reserve(descriptorCounts.size());
        for (const auto& [fst, snd] : descriptorCounts) {
            poolSizes.emplace_back(fst, snd);
        }

        m_FrameDescriptorPool = m_RenderApi->m_Device->Logical().createDescriptorPool(
            vk::DescriptorPoolCreateInfo({},
                m_FrameCount,
                static_cast<uint32_t>(poolSizes.size()),
                poolSizes.data()));

        std::vector<vk::DescriptorSetLayout> layouts(m_FrameCount, m_FrameDescriptorSetLayout);

        m_FrameDescriptorSets = m_RenderApi->m_Device->Logical().allocateDescriptorSets(
            vk::DescriptorSetAllocateInfo(m_FrameDescriptorPool, m_FrameCount, layouts.data()));
    }

    void VulkanResourceManager::AddResource(const vk::DescriptorSetLayoutBinding& binding)
    {
        m_LayoutBindings.push_back(binding);
        RecreateResources();
    }

    void VulkanResourceManager::AddFrameResource(const vk::DescriptorSetLayoutBinding& binding)
    {
        m_FrameLayoutBindings.push_back(binding);
        RecreateFrameResources();
    }

    void VulkanResourceManager::BindResource(const uint32_t bindingIndex, const VulkanBuffer* buffer) const
    {
        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = buffer->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = buffer->GetSize();

        vk::WriteDescriptorSet writeDescriptorSet;
        writeDescriptorSet.dstSet = m_DescriptorSet;
        writeDescriptorSet.dstBinding = bindingIndex;
        writeDescriptorSet.dstArrayElement = 0;
        writeDescriptorSet.descriptorType = vk::DescriptorType::eUniformBuffer;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.pBufferInfo = &bufferInfo;

        m_RenderApi->m_Device->Logical().updateDescriptorSets(1, &writeDescriptorSet, 0, nullptr);
    }

    void VulkanResourceManager::BindFrameResource(const uint32_t bindingIndex, const VulkanBuffer* buffer) const
    {
        for (const auto& descriptorSet : m_FrameDescriptorSets) {
            vk::DescriptorBufferInfo bufferInfo;
            bufferInfo.buffer = buffer->GetBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range = buffer->GetSize();

            vk::WriteDescriptorSet writeDescriptorSet;
            writeDescriptorSet.dstSet = descriptorSet;
            writeDescriptorSet.dstBinding = bindingIndex;
            writeDescriptorSet.dstArrayElement = 0;
            writeDescriptorSet.descriptorType = vk::DescriptorType::eUniformBuffer;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.pBufferInfo = &bufferInfo;

            m_RenderApi->m_Device->Logical().updateDescriptorSets(1, &writeDescriptorSet, 0, nullptr);
        }
    }
}
