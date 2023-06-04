#include "fepch.h"
#include "OrthographicCameraController.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Input.h"

namespace FusionEngine
{
    OrthographicCameraController::OrthographicCameraController(const float left, const float right, const float bottom, const float top)
        : Camera(), m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
    {
        m_InvalidProj = true;
    }

    void OrthographicCameraController::OnUpdate(const double deltaTime)
    {
        static constexpr float camera_speed = 2.0f;
        static constexpr float zoom_speed = 0.3f;

        bool viewChanged = false;

        if (Input::IsKeyPressed(KeyCode::W))
        {
            m_CameraPosition.y += camera_speed * deltaTime;
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::S))
        {
            m_CameraPosition.y -= camera_speed * deltaTime;
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::A))
        {
            m_CameraPosition.x += camera_speed * deltaTime;
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::D))
        {
            m_CameraPosition.x -= camera_speed * deltaTime;
            viewChanged = true;
        }

        if (Input::IsKeyPressed(KeyCode::Q))
        {
            m_CameraZoom += zoom_speed * deltaTime;
            viewChanged = true;
        }
        if (Input::IsKeyPressed(KeyCode::E))
        {
            m_CameraZoom -= zoom_speed * deltaTime;
            viewChanged = true;
        }

        if(!m_InvalidView && viewChanged)
            m_InvalidView = true;
    }

    glm::mat4 OrthographicCameraController::GetViewMatrix()
    {
        if (m_InvalidView)
        {
            m_InvalidView = false;
            m_View = glm::translate(glm::mat4(1.0f), m_CameraPosition) *
                     glm::scale(glm::mat4(1.0f), glm::vec3(m_CameraZoom, m_CameraZoom, 1.0f));
        }

        return m_View;
    }

    glm::mat4 OrthographicCameraController::GetProjectionMatrix()
    {
        if (m_InvalidProj)
        {
            m_InvalidProj = false;
            m_Proj = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, -1.0f, 1.0f);
        }

        return m_Proj;
    }
}
