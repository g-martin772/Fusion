#include "fepch.h"
#include "EditorLayer.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

Application* CreateApplication()
{
    auto* app = new Application();
    app->GetPrimaryWindow()->PushLayer(new EditorLayer());
    app->GetTestWindow()->PushLayer(new EditorLayer());
    return app;
}