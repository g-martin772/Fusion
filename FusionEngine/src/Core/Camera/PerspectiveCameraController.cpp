﻿#include "fepch.h"
#include "PerspectiveCameraController.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Input.h"

namespace FusionEngine
{
    PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip), m_CameraPosition(glm::vec3(0.0f, 0.0f, 1.0f)),
          m_CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), m_CameraSpeed(1.0f), m_ZoomSpeed(30.0f),
          m_MouseSensitivity(0.1f), m_Yaw(-90.0f), m_Pitch(0.0f)
    {
    }

    void PerspectiveCameraController::OnUpdate(double deltaTime)
    {
        bool viewChanged = false;

        // Camera movement
        if (Input::IsKeyPressed(KeyCode::W))
        {
            m_CameraPosition += m_CameraSpeed * static_cast<float>(deltaTime) * m_CameraFront;
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::S))
        {
            m_CameraPosition -= m_CameraSpeed * static_cast<float>(deltaTime) * m_CameraFront;
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::A))
        {
            m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed * static_cast<float>(deltaTime);
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::D))
        {
            m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed * static_cast<float>(deltaTime);
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::E))
        {
            m_CameraPosition.y -= m_CameraSpeed * static_cast<float>(deltaTime);
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::Q))
        {
            m_CameraPosition.y += m_CameraSpeed * static_cast<float>(deltaTime);
            viewChanged = true;
        }

        // Camera zooming
        if (Input::IsKeyPressed(KeyCode::PageUp))
        {
            m_FOV -= m_ZoomSpeed * deltaTime;
            m_InvalidProj = true;
        }
        if (Input::IsKeyPressed(KeyCode::PageDown))
        {
            m_FOV += m_ZoomSpeed * deltaTime;
            m_InvalidProj = true;
        }

        if (m_FOV > 179.0f)
            m_FOV = 179.0f;
        if (m_FOV < 1.0f)
            m_FOV = 1.0f;

        // Camera orientation control with mouse input
        if (Input::IsMouseButtonPressed(MouseCode::Right))
        {
            const float xOffset = Input::GetMouseDeltaX() * m_MouseSensitivity;
            const float yOffset = Input::GetMouseDeltaY() * m_MouseSensitivity;

            m_Yaw += xOffset;
            m_Pitch += yOffset;

            // Clamp pitch to avoid flipping the camera
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;

            glm::vec3 front;
            front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            front.y = sin(glm::radians(m_Pitch));
            front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            m_CameraFront = glm::normalize(front);

            viewChanged = true;
            Input::CaptureMouse(true);
        }
        else
        {
            Input::CaptureMouse(false);
        }

        if (!m_InvalidView && viewChanged)
            m_InvalidView = true;
    }

    glm::mat4 PerspectiveCameraController::GetViewMatrix()
    {
        if (m_InvalidView)
        {
            m_InvalidView = false;
            m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);
        }

        return m_View;
    }

    glm::mat4 PerspectiveCameraController::GetProjectionMatrix()
    {
        if (m_InvalidProj)
        {
            m_InvalidProj = false;
            FE_INFO("FOEV: {0}", m_FOV);
            m_Proj = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
        }

        return m_Proj;
    }
}
