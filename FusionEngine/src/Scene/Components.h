#pragma once
#include <glm/glm.hpp>

namespace FusionEngine
{
    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(0.0f))
            : Position(position), Rotation(rotation), Scale(scale) {}
        
        glm::vec3 Position, Rotation, Scale;
    };
    
    template<typename...>
    struct ComponentGroup {};

    using AllComponents = ComponentGroup<TransformComponent>;
}
