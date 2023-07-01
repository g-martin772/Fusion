#pragma once
#include "Camera.h"

namespace FusionEngine
{
    class PerspectiveCameraController : public Camera
    {
    public:
        PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip);
        void OnUpdate(double deltaTime) override;
        void OnResize(uint32_t width, uint32_t height) override;
        glm::mat4 GetViewMatrix() override;
        glm::mat4 GetProjectionMatrix() override;
    private:
        glm::vec3 m_CameraPosition, m_CameraFront, m_CameraUp;
        float m_CameraSpeed, m_FOV, m_Yaw, m_Pitch, m_ZoomSpeed, m_MouseSensitivity;
        float m_AspectRatio, m_Near, m_Far;
    };
}
