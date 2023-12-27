#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// #include "Renderer/IndexBuffer.h"
// #include "Renderer/VertexBuffer.h"
//
// namespace FusionEngine
// {
//     class ObjModel
//     {
//     public:
//         ObjModel(const std::string& path);
//         ~ObjModel();
//
//         Shared<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
//         Shared<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
//         uint32_t GetIndexCount() const { return m_IndexCount; }
//     private:
//         uint32_t ReadCorner(const std::string& corner);
//     private:
//         std::vector<glm::vec3> m_VertexPositions;
//         std::vector<uint32_t> m_Indices;
//         
//         Shared<VertexBuffer> m_VertexBuffer;
//         Shared<IndexBuffer> m_IndexBuffer;
//         uint32_t m_IndexCount = 0;
//     };
//     
// }
