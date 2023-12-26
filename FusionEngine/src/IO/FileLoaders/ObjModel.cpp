#include "fepch.h"
#include "ObjModel.h"

#include <filesystem>

#include "IO/File.h"
#include "IO/StringUitls.h"

namespace FusionEngine
{
    ObjModel::ObjModel(const std::string& path)
    {
        std::vector<std::string> lines = File::ReadLines(path);
        for (std::string& line : lines)
        {
            if(line.empty())
                continue;
            if(line[0] == '#')
                continue;
            
            std::vector<std::string> parts = SplitString(line, " ");
            
            if(parts[0] == "v")
            {
                m_VertexPositions.push_back(glm::vec3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
            }

            if(parts[0] == "f")
            {
                std::vector<std::string> parts = SplitString(line, " ");
                size_t triangleCount = parts.size() - 3;
                for (size_t i = 0; i < triangleCount; i++)
                {
                    m_Indices.push_back(ReadCorner(parts[1]));
                    m_Indices.push_back(ReadCorner(parts[2 + i]));
                    m_Indices.push_back(ReadCorner(parts[3 + i]));
                }
            }
        }
        
        uint32_t vertexDataSize = m_VertexPositions.size() * (sizeof(glm::vec3) + sizeof(glm::vec4));
        void* vertexData = malloc(vertexDataSize);
        uint8_t* vertexDataPtr = static_cast<uint8_t*>(vertexData);

        glm::vec4 color(1.0f);
        
        for (auto& vertex : m_VertexPositions)
        {
            memcpy(vertexDataPtr, &vertex, sizeof(glm::vec3));
            vertexDataPtr += sizeof(glm::vec3);
            memcpy(vertexDataPtr, &color, sizeof(glm::vec4));
            vertexDataPtr += sizeof(glm::vec4);
        }

        m_VertexBuffer = VertexBuffer::Create({ VertexBuffer::Attribute::Vec3, VertexBuffer::Attribute::Vec4 }, vertexDataSize);
        m_VertexBuffer->SetData(vertexData, vertexDataSize);

        m_IndexBuffer = IndexBuffer::Create(m_IndexCount * sizeof(uint32_t));
        m_IndexBuffer->SetData(m_Indices.data(), m_IndexCount * sizeof(uint32_t));
    }

    ObjModel::~ObjModel()
    {
    }

    uint32_t ObjModel::ReadCorner(const std::string& corner)
    {
        std::vector<std::string> values = SplitString(corner, "/");
        m_IndexCount++;
        return std::stoul(values[0]) - 1;
    }
}

