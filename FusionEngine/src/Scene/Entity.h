#pragma once
#include <entt/entt.hpp>

#include "Scene.h"
#include "Core/UUID.h"

namespace FusionEngine
{
    class Entity
    {
    public:
        Entity(Scene* scene, UUID uuid = UUID(), std::string_view name = "Entity");
        ~Entity();
        
        UUID GetUuid() const;
        std::string_view GetName() const;

        template<typename T>
        bool HasComponent() const{
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        T& GetComponent() const {
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        void AddComponent(const T& component) const {
            m_Scene->m_Registry.emplace_or_replace<T>(component);
        }
    private:
        entt::entity m_EntityHandle = entt::null ;
        Scene* m_Scene;
    };
}
