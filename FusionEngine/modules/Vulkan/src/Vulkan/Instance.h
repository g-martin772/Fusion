#pragma once

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    class Instance
    {
    public:
        Instance();
        ~Instance();

        void CreateInstance();
    private:
        vk::Instance m_Instance;
    };
}
