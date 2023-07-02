#pragma once
#include "imgui.h"
#include "Core/Base.h"
#include "Core/Layer.h"
#include "Core/Camera/Camera.h"
#include "IO/FileLoaders/ObjModel.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Pipeline.h"
#include "Scene/Scene.h"

using namespace FusionEngine;

class EditorLayer : public Layer
{
public:
    void OnAttach() override;
    void OnUpdate(Ref<Time> time) override;
    void OnDetach() override;

private:
    Ref<Scene> m_Scene;
    Ref<Camera> m_Camera;

    Ref<Framebuffer> m_ViewportFramebuffer;
    Ref<Image> m_ViewportImage;
    glm::uvec2 m_ViewportSize;
};
