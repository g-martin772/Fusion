#pragma once

#include <vulkan/vulkan.hpp>

namespace FusionEngine
{
    namespace VulkanUtils
    {
        vk::Semaphore CreateSemaphore(const vk::Device device)
        {
            vk::SemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.flags = vk::SemaphoreCreateFlags();

            try
            {
                return device.createSemaphore(semaphoreInfo);
            }
            catch (vk::SystemError& err)
            {
                FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
                FE_ASSERT(false, "Creating semaphore failed");
                return nullptr;
            }
        }

        vk::Fence CreateFence(const vk::Device device)
        {
            vk::FenceCreateInfo fenceInfo = {};
            fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;

            try
            {
                return device.createFence(fenceInfo);
            }
            catch (vk::SystemError& err)
            {
                FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
                FE_ASSERT(false, "Creating fence failed");
                return nullptr;
            }
        }
    }
}
