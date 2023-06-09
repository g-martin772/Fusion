﻿#pragma once
#include <glm/glm.hpp>

namespace FusionEngine
{
    class Camera
    {
    public:
        Camera();
        virtual ~Camera() = default;

        virtual void OnUpdate(double deltaTime) {}
        virtual void OnResize(uint32_t width, uint32_t height) {}

        virtual glm::mat4 GetViewMatrix();
        virtual glm::mat4 GetProjectionMatrix();
        virtual glm::mat4 GetViewProjectionMatrix();
    protected:
        bool m_InvalidView = true, m_InvalidProj = true;
        glm::mat4 m_View, m_Proj, m_ViewProj;
    };
}
