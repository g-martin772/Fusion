#pragma once
#include "VulkanShader.h"
#include "Renderer/Pipeline.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi;
}

namespace FusionEngine
{
    class VulkanPipeline : public Pipeline
    {
    public:
        VulkanPipeline(const PipelineSpecification& spec);
        ~VulkanPipeline();
        
        void Bind() override;
    private:
        [[nodiscard]] vk::PipelineLayout MakePipelineLayout() const;
        [[nodiscard]] vk::DescriptorSetLayout MakeDescriptorSetLayout() const;
    private:
        Ref<VulkanShader> m_Shader;

        vk::PipelineLayout m_PipelineLayout;
        vk::Pipeline m_Pipeline;

        vk::DescriptorSetLayout m_DescriptorSetLayout;
        
        Ref<VulkanRenderApi> m_RenderApi;
        PipelineSpecification m_Spec;
    };
    
}
