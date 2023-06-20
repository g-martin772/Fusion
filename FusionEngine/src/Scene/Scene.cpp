#include "fepch.h"
#include "Scene.h"

#include "Entity.h"

namespace FusionEngine
{
    Entity Scene::CreateEntity(const std::string_view name, const UUID uuid)
    {
        return Entity(this, uuid, name);
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

    void Scene::RenderScene(const Ref<Camera>& camera)
    {
    }
}
