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
        static Shared<Shader> Create(const std::string& name);

        Shader(const std::string& name);
        virtual ~Shader() = default;

        const std::string& GetName();
    private:
        std::string m_Name;
    };
}
