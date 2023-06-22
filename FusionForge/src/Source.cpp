#include "fepch.h"
#include "EditorLayer.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

Application* CreateApplication()
{
    auto* app = new Application();
    app->PushLayer(new EditorLayer);
    return app;
}