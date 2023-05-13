#pragma once
#include "Core/Base.h"
#include "Core/Layer.h"
#include "Renderer/Pipeline.h"

class SandboxLayer : public FusionEngine::Layer
{
public:
    void OnAttach() override;
    void OnUpdate() override;
    void OnDetach() override;
private:
    FusionEngine::Ref<FusionEngine::Pipeline> m_PipeLine;
    std::shared_ptr<FusionEngine::VertexBuffer> m_VBO;
};
