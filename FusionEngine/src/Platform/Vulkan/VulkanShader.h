#pragma once
#include "Renderer/Shader.h"

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class VulkanRenderApi;

    class VulkanShader : public Shader
    {
    public:
        explicit VulkanShader(const std::string& name);

        [[nodiscard]] std::vector<char> GetVertexShader() const { return m_VertexShader; }
        [[nodiscard]] std::vector<char> GetFragmentShader() const { return m_FragmentShader; }

        [[nodiscard]] vk::ShaderModule GetVertexShaderModule() const { return m_VertexShaderModule; }
        [[nodiscard]] vk::ShaderModule GetFragmentShaderModule() const { return m_FragmentShaderModule; }
    private:
        vk::ShaderModule CreateShaderModule(const std::vector<char>& spirv) const;
    private:
        Shared<VulkanRenderApi> m_RenderApi;
        
        std::vector<char> m_VertexShader, m_FragmentShader;
        vk::ShaderModule  m_VertexShaderModule, m_FragmentShaderModule;
    };
}
