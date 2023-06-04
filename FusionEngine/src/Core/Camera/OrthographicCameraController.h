#pragma once
#include "Camera.h"

namespace FusionEngine
{
    class OrthographicCameraController : public Camera
    {
    public:
        OrthographicCameraController(float left, float right, float bottom, float top);
        
        void OnUpdate(float deltaTime) override;
        glm::mat4 GetViewMatrix() override;
        glm::mat4 GetProjectionMatrix() override;
    private:
        float m_Left, m_Right, m_Bottom, m_Top;
        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, -1.0f };
        float m_CameraZoom = 1.0f;
    };
}
