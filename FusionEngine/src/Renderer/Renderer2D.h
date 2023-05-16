#pragma once

#include <glm/glm.hpp>

namespace FusionEngine
{
    class Renderer2D
    {
    public:
        static void Init();
        static void ShutDown();
        
        static void BeginScene();
        static void EndScene();

        static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
    private:
        static void Flush();
    };
}
    
