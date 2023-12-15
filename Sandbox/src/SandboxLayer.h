#pragma once
#include "Core/Base.h"
#include "Core/Layer.h"
#include "Core/Camera/Camera.h"
#include "IO/FileLoaders/ObjModel.h"
#include "Renderer/Pipeline.h"
#include "Scene/Scene.h"

class EditorLayer : public FusionEngine::Layer
{
public:
    void OnAttach() override;
    void OnUpdate(FusionEngine::Shared<FusionEngine::Time> time) override;
    void OnDetach() override;
private:
    FusionEngine::Shared<FusionEngine::Scene> m_Scene;
    
    FusionEngine::Shared<FusionEngine::Camera> m_Camera;
    FusionEngine::Shared<FusionEngine::ObjModel> m_Model;
    FusionEngine::Shared<FusionEngine::ObjModel> m_Dice;
};
