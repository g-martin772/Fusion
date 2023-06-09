﻿#include "fepch.h"
#include "SandboxLayer.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

Application* CreateApplication()
{
    auto* app = new Application();
    app->PushLayer(new EditorLayer);
    return app;
}