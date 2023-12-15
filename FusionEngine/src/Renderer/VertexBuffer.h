#pragma once

#include <vector>

namespace FusionEngine
{
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
        static Shared<VertexBuffer> Create(const std::vector<Attribute>& attributes, uint32_t size);
        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;
        virtual void SetData(void* data, uint32_t size) = 0;

        [[nodiscard]] const std::vector<Attribute>& GetVertexAttributes() const { return m_VertexAttributes; }
    protected:
        std::vector<Attribute> m_VertexAttributes;
    };
}
