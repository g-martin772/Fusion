#include "fepch.h"
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace FusionEngine
{
    Camera::Camera()
    {
        
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        if (m_InvalidView)
        {
            m_InvalidView = false;
            m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        }

        return m_View;
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        if (m_InvalidProj)
        {
            m_InvalidProj = false;
            m_Proj = glm::ortho(0, 1, 0, 1, -1, 10);
        }

        return m_Proj;
    }

    glm::mat4 Camera::GetViewProjectionMatrix()
    {
        if (m_InvalidView || m_InvalidProj)
        {
            m_ViewProj = GetProjectionMatrix() * GetViewMatrix();
        }

        return m_ViewProj;
    }
}
