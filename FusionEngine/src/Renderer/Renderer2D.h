#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Core/Camera/Camera.h"

namespace FusionEngine
{
    class Renderer2D
    {
    public:
        static void Init();
        static void ShutDown();
        
        static void BeginScene(const Ref<Camera>& camera);
        static void EndScene();

        static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
    private:
        static void Flush();
    };
}
    
