#include "fepch.h"
#include "ObjModel.h"

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
            
            if(line[0] == 'v')
            {
                std::vector<std::string> parts = SplitString(line, " ");
                m_VertexPositions.push_back(glm::vec3(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])));
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
            m_VertexCount++;
        }

        m_VertexBuffer = VertexBuffer::Create({ VertexBuffer::Attribute::Vec3, VertexBuffer::Attribute::Vec4 }, vertexDataSize);
        m_VertexBuffer->SetData(vertexData, vertexDataSize);
    }

    ObjModel::~ObjModel()
    {
    }
}
