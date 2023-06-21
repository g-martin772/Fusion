#pragma once
#include <glm/glm.hpp>

namespace FusionEngine
{
    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
            : Position(position), Rotation(rotation), Scale(scale) {}
        
        glm::vec3 Position = glm::vec3(0.0f), Rotation = glm::vec3(0.0f), Scale = glm::vec3(1.0f);
    };

    struct SpriteRenderComponent
    {
        SpriteRenderComponent() = default;
        SpriteRenderComponent(const SpriteRenderComponent&) = default;
        SpriteRenderComponent(const glm::vec4& color)
            : Color(color) {}

        // Texture, Tiling, etc.
        glm::vec4 Color;
    };
    
    template<typename...>
    struct ComponentGroup {};

    using AllComponents = ComponentGroup<TransformComponent>;
}
