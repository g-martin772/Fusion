#include "fepch.h"
#include "Scene.h"

#include <glm/ext/matrix_transform.hpp>

#include "Components.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"

namespace FusionEngine
{
    Entity Scene::CreateEntity(const std::string_view name, const UUID uuid)
    {
        return Entity(this, uuid, name);
    }

    Entity Scene::GetEntityByUUID(const UUID uuid)
    {
        for(auto [entity, entityId] : m_Registry.view<UUID>().each())
        {
            if(entityId == uuid)
            {
                return Entity(this, entity);
            }
        }
        FE_WARN("Entity with ID {0} not found!", uuid.ToString());
        return Entity();
    }

    Entity Scene::GetEntityByName(const std::string_view name)
    {
        for(auto [entity, entityName] : m_Registry.view<std::string_view>().each())
        {
            if(entityName == name)
            {
                return Entity(this, entity);
            }
        }
        FE_WARN("Entity with name \"{0}\" not found!", name);
        return Entity();
    }

    void Scene::StartRuntime()
    {
    }

    void Scene::StopRuntime()
    {
    }

    void Scene::StartSimulation()
    {
    }

    void Scene::StopSimulation()
    {
    }

    void Scene::OnUpdateRuntime()
    {
    }

    void Scene::OnUpdateSimulation()
    {
    }

    void Scene::RenderScene(const Shared<Camera>& camera)
    {
        Renderer2D::BeginScene(camera);
        for(auto [entity, tc, src] : m_Registry.view<TransformComponent, SpriteRenderComponent>().each())
        {
            Renderer2D::DrawQuad(tc.Position, {tc.Scale.x, tc.Scale.y}, src.Color);
        }
        Renderer2D::EndScene();
    }
}
