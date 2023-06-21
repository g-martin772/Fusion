#include "fepch.h"
#include "Entity.h"

#include "Scene.h"
#include "Components.h"

namespace FusionEngine
{
    Entity::Entity(Scene* scene, UUID uuid, std::string_view name)
    {
        m_Scene = scene;
        m_EntityHandle = scene->m_Registry.create();
        m_Scene->m_Registry.emplace<UUID>(m_EntityHandle, uuid);
        m_Scene->m_Registry.emplace<std::string_view>(m_EntityHandle, name);
        m_Scene->m_Registry.emplace<TransformComponent>(m_EntityHandle);
    }   

    void Entity::Destroy()
    {
        m_Scene->m_Registry.destroy(m_EntityHandle);
    }


    UUID Entity::GetUUID() const
    {
        return m_Scene->m_Registry.get<UUID>(m_EntityHandle);
    }

    std::string_view Entity::GetName() const
    {
        return m_Scene->m_Registry.get<std::string_view>(m_EntityHandle);
    }
}
