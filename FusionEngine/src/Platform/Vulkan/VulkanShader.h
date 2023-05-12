#pragma once
#include "Renderer/Shader.h"

namespace FusionEngine
{
    class VulkanShader : public Shader
    {
    public:
        explicit VulkanShader(const std::string& name);

        void Load() override;
        void Bind() override;

        [[nodiscard]] std::vector<char> GetVertexShader() const { return m_VertexShader; }
        [[nodiscard]] std::vector<char> GetFragmentShader() const { return m_FragmentShader; }
    private:
        std::vector<char> m_VertexShader, m_FragmentShader;
    };
}
