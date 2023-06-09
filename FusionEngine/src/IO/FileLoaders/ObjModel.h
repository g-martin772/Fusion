#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Renderer/VertexBuffer.h"

namespace FusionEngine
{
    class ObjModel
    {
    public:
        ObjModel(const std::string& path);
        ~ObjModel();

        Ref<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
        uint32_t GetVertexCount() const { return m_VertexCount; }
    private:
        std::vector<glm::vec3> m_VertexPositions;
        Ref<VertexBuffer> m_VertexBuffer;
        uint32_t m_VertexCount = 0;
    };
    
}
