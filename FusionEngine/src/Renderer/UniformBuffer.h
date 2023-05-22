#pragma once


namespace FusionEngine
{
    enum class ShaderType;

    class UniformBuffer
    {
    public:
        static Ref<UniformBuffer> Create(const std::string& name, uint32_t size, uint32_t binding, ShaderType shaderType);
        virtual ~UniformBuffer() = default;

        virtual void Bind() = 0;
        virtual void SetData(void* data) = 0;

        [[nodiscard]] const std::string& GetName() const { return m_Name; }
    protected:
        std::string m_Name;
    
    };
}
