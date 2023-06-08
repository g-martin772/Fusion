#pragma once

namespace FusionEngine
{
    class IndexBuffer
    {
    public:
        static Ref<IndexBuffer> Create(uint32_t size);
        virtual ~IndexBuffer() = default;

        virtual void Bind() = 0;
        virtual void SetData(void* data) = 0;
    };
}
