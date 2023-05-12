#pragma once

namespace FusionEngine
{
    enum class ShaderType
    {
        None = 0,
        Vertex,
        Fragment,
        Geometry,
        Compute
    };
    
    class Shader
    {
    public:
        static Ref<Shader> Create(std::string name);

        Shader(const std::string& name);
        virtual ~Shader() = default;

        const std::string& GetName();

        // virtual bool HasShaderModule(ShaderType type) = 0;

        virtual void Load() = 0;
        virtual void Bind() = 0; // ? Vulkan how
    private:
        std::string m_Name;
    };
}
