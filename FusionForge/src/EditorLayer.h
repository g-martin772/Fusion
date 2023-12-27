// #pragma once
// #include "Core/Base.h"
// #include "Core/Layer.h"
// #include "Core/Camera/Camera.h"
// #include "IO/FileLoaders/ObjModel.h"
// #include "Renderer/Framebuffer.h"
// #include "Scene/Scene.h"
// #include "Scene/Entity.h"
//
// using namespace FusionEngine;
//
// class EditorLayer final : public Layer
// {
// public:
//     void OnAttach() override;
//     void OnUpdate(const Time& time) override;
//     void OnDetach() override;
//
// private:
//     void RenderImGui();
//     void DrawSceneHierarchyPanel();
// private:
//     Shared<Scene> m_Scene;
//     Shared<Camera> m_Camera;
//
//     Shared<Framebuffer> m_ViewportFramebuffer;
//     Shared<Image> m_ViewportImage;
//     glm::uvec2 m_ViewportSize;
//     bool m_IsViewportFocused = false, m_IsViewportHovered = false;
//
//     // SceneHierarchyPanel
//     Entity m_SelectedEntity;
// };
