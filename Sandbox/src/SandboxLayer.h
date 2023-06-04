#pragma once
#include "Core/Base.h"
#include "Core/Layer.h"
#include "Core/Camera/Camera.h"
#include "Renderer/Pipeline.h"

class SandboxLayer : public FusionEngine::Layer
{
public:
    void OnAttach() override;
    void OnUpdate() override;
    void OnDetach() override;
private:
    FusionEngine::Ref<FusionEngine::Camera> m_Camera;
};
