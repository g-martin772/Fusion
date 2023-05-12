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
        VulkanPipeline(const Ref<Shader>& shader);
        
        void Bind() override;
    private:
        vk::ShaderModule CreateModule(const std::vector<char>& spirv) const;
        vk::PipelineLayout MakePipelineLayout() const;
    private:
        Ref<VulkanShader> m_Shader;

        vk::PipelineLayout m_PipelineLayout;
        vk::Pipeline m_Pipeline;
        
        Ref<VulkanRenderApi> m_RenderApi;
    };
    
}
