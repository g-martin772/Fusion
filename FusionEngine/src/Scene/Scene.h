#pragma once

#include "Core/UUID.h"
#include "Core/Camera/Camera.h"
#include "entt/entt.hpp"

namespace FusionEngine
{
    class Entity;
    
    class Scene
    {
    public:
        Entity CreateEntity(const std::string_view name = "Entity", const UUID uuid = UUID());

        Entity GetEntityByUUID(const UUID uuid);
        Entity GetEntityByName(const std::string_view name);

        template<typename... Components>
        auto View() { return m_Registry.view<Components...>(); }

        void StartRuntime();
        void StopRuntime();

        void StartSimulation();
        void StopSimulation();

        void OnUpdateRuntime();
        void OnUpdateSimulation();

        void RenderScene(const Ref<Camera>& camera);
    private:
        entt::registry m_Registry;

        friend class Entity;
    };
    
}
