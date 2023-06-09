﻿#pragma once
#include <entt/entt.hpp>

#include "Scene.h"
#include "Core/UUID.h"

namespace FusionEngine
{
    class Entity
    {
    public:
        Entity(){}
        Entity(Scene* scene, UUID uuid = UUID(), std::string_view name = "Entity");

        void Destroy();

        [[nodiscard]] uint32_t GetEntityHandle() const { return static_cast<uint32_t>(m_EntityHandle); }

        [[nodiscard]] UUID GetUUID() const;
        [[nodiscard]] std::string_view GetName() const;

        template<typename T>
        [[nodiscard]] bool HasComponent() const{
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        [[nodiscard]] T& GetComponent() const {
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        void AddComponent(const T& component) const {
            m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, component);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
    private:
        Entity(Scene* scene, const entt::entity entityHandle)
            : m_EntityHandle(entityHandle), m_Scene(scene) {}
    private:
        entt::entity m_EntityHandle = entt::null ;
        Scene* m_Scene = nullptr;

        friend class Scene;
    };
}
