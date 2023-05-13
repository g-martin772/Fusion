#pragma once

namespace FusionEngine
{
    enum class DrawMode;
    
    class VertexBuffer
    {
    public:
        enum class Attribute : uint32_t
        {
            Bool, Char,
            UShort, Short,
            Int, UInt, Float,
            Vec2, Vec3, Vec4
        };
    public:
        static Ref<VertexBuffer> Create(const std::vector<Attribute>& attributes, DrawMode drawmode);
        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;
        virtual void SetData(void* data) = 0;

        [[nodiscard]] const std::vector<Attribute>& GetVertexAttributes() const { return m_VertexAttributes; }
    protected:
        std::vector<Attribute> m_VertexAttributes;
    };
}
