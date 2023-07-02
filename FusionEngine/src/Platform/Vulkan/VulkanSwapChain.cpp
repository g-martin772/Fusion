#include "fepch.h"
#include "VulkanSwapChain.h"

#include "VulkanDevice.h"
#include "VulkanRenderApi.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"
#include "Renderer/RenderCommand.h"
#include "VulkanUtils.h"

namespace FusionEngine
{
    VulkanSwapChain::VulkanSwapChain()
    {
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());

        // Acquire Surface from GLFWwindow
        VkSurfaceKHR cSurface;
        if(const auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());
            glfwCreateWindowSurface(m_RenderApi->m_Instance->GetInstance(), window, nullptr, &cSurface) != VK_SUCCESS)
                FE_ASSERT(false, "Failed to create Vulkan Surface")
            else
                FE_INFO("Sucessfully created Vulkan surface");

        m_Surface = cSurface;
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        CleanUpSwapChain();
        m_RenderApi->m_Instance->GetInstance().destroySurfaceKHR(m_Surface);
    }

    void VulkanSwapChain::AcquireSurfaceCapabilities()
    {
        // Query additional surface Information
        m_Capabilities = m_RenderApi->m_Device->Physical().getSurfaceCapabilitiesKHR(m_Surface);

        FE_TRACE("Surface Capabilities:");
        FE_TRACE("\tMinimum image count: {0}", m_Capabilities.minImageCount);
        FE_TRACE("\tMaximum image count: {0}", m_Capabilities.maxImageCount);
        FE_TRACE("\tCurrent Extent: {0}, {1}", m_Capabilities.currentExtent.width, m_Capabilities.currentExtent.height);
        FE_TRACE("\tMinimum Extent: {0}, {1}", m_Capabilities.minImageExtent.width, m_Capabilities.minImageExtent.height);
        FE_TRACE("\tMaximum Extent: {0}, {1}", m_Capabilities.maxImageExtent.width, m_Capabilities.maxImageExtent.height);
        FE_TRACE("\tMaximum image array layers: {0}", m_Capabilities.maxImageArrayLayers);

        m_Formats = m_RenderApi->m_Device->Physical().getSurfaceFormatsKHR(m_Surface);

        FE_TRACE("Surface Formats:");
        bool pickedFormat = false;
        for(const vk::SurfaceFormatKHR format : m_Formats)
        {
            FE_TRACE("\tPixelFormat: {0}", vk::to_string(format.format));
            FE_TRACE("\tColorSpace: {0}", vk::to_string(format.colorSpace));
            if (format.format == vk::Format::eB8G8R8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                pickedFormat = true;
                m_SurfaceFormat = format;
            }
        }
        
        if (!pickedFormat)
            m_SurfaceFormat = m_Formats[0];

        const auto presentModes = m_RenderApi->m_Device->Physical().getSurfacePresentModesKHR(m_Surface);

        FE_TRACE("Surface Present Modes:");
        for(const vk::PresentModeKHR presentMode : presentModes)
        {
            FE_TRACE("\t{0}", vk::to_string(presentMode));
            if (presentMode == vk::PresentModeKHR::eMailbox)
                m_PresentMode = presentMode;
        }

        m_SwapchainExtent = m_Capabilities.currentExtent;
    }

    void VulkanSwapChain::InitializeSwapchain()
    {
        if (!m_Surface)
            AcquireSurfaceCapabilities();
        
        CreateSwapChain();
        CreateFrameBuffers();
    }

    void VulkanSwapChain::CreateSwapChain()
    {
        // https://vulkan.lunarg.com/doc/view/latest/windows/tutorial/html/05-init_swapchain.html

        #undef min // who broke the std lib again ... ohhr
        const uint32_t imageCount = std::min(
            m_Capabilities.maxImageCount,
            m_Capabilities.minImageCount + 1
        );

        FE_INFO("Creating Swapchain with {0} images", imageCount);
        vk::SwapchainCreateInfoKHR createInfo(
            vk::SwapchainCreateFlagsKHR(),
            m_Surface,
            imageCount,
            m_SurfaceFormat.format,
            m_SurfaceFormat.colorSpace,
            m_SwapchainExtent,
            m_Capabilities.maxImageArrayLayers,
            vk::ImageUsageFlagBits::eColorAttachment);

        if(m_RenderApi->m_Device->GraphicsFamily().value() != m_RenderApi->m_Device->PresentFamily().value())
        {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            const uint32_t queueFamilyIndices[] = { m_RenderApi->m_Device->GraphicsFamily().value(), m_RenderApi->m_Device->PresentFamily().value() };
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = m_Capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = m_PresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

        try
        {
            m_SwapChain = m_RenderApi->m_Device->Logical().createSwapchainKHR(createInfo);
            const std::vector<vk::Image> images = m_RenderApi->m_Device->Logical().getSwapchainImagesKHR(m_SwapChain);
        	m_RenderApi->m_Frames = std::vector<VulkanRenderApi::Frame>(images.size());
        	
            for (size_t i = 0; i < images.size(); i++)
            {
                vk::ImageViewCreateInfo createInfo;
                createInfo.image = images[i];
                createInfo.viewType = vk::ImageViewType::e2D;
                createInfo.components.r = vk::ComponentSwizzle::eIdentity;
                createInfo.components.g = vk::ComponentSwizzle::eIdentity;
                createInfo.components.b = vk::ComponentSwizzle::eIdentity;
                createInfo.components.a = vk::ComponentSwizzle::eIdentity;
                createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;
                createInfo.format = m_SurfaceFormat.format;

            	m_RenderApi->m_Frames[i].Image = images[i];
                m_RenderApi->m_Frames[i].ImageView = m_RenderApi->m_Device->Logical().createImageView(createInfo);
            }
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating swapchain failed");
        }
        FE_INFO("Success");
    }

    void VulkanSwapChain::RecreateSwapChain()
    {
        int width = 0, heith = 0;

        while(width <= 0 || heith <= 0)
        {
            glfwGetFramebufferSize(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow()), &width, &heith);
            glfwWaitEvents();
        }

        m_Capabilities = m_RenderApi->m_Device->Physical().getSurfaceCapabilitiesKHR(m_Surface);
        m_SwapchainExtent = m_Capabilities.currentExtent;
    	
        CleanUpSwapChain();
        CreateSwapChain();
        CreateFrameBuffers();

        m_RenderApi->m_ResourceManager->SetFrameCount(static_cast<uint32_t>(m_RenderApi->m_Frames.size()));
        for (auto& frame : m_RenderApi->m_Frames)
        {
            frame.CommandBuffer = m_RenderApi->CreateCommandBuffer();
            frame.InFlightFence = VulkanUtils::CreateFence(m_RenderApi->m_Device->Logical());
            frame.ImageAvailable = VulkanUtils::CreateSemaphoreW(m_RenderApi->m_Device->Logical());
            frame.RenderFinished = VulkanUtils::CreateSemaphoreW(m_RenderApi->m_Device->Logical());
        }
    }

    void VulkanSwapChain::CleanUpSwapChain()
    {
        m_RenderApi->m_Device->Logical().waitIdle();
    	
        for (const auto& frame : m_RenderApi->m_Frames)
        {
            m_RenderApi->m_Device->Logical().destroyFence(frame.InFlightFence);
            m_RenderApi->m_Device->Logical().destroySemaphore(frame.ImageAvailable);
            m_RenderApi->m_Device->Logical().destroySemaphore(frame.RenderFinished);
            m_RenderApi->m_Device->Logical().destroyImageView(frame.ImageView);
            m_RenderApi->m_Device->Logical().destroyFramebuffer(frame.FrameBuffer);
        }

        m_RenderApi->m_Device->Logical().destroySwapchainKHR(m_SwapChain);
    }

    void VulkanSwapChain::CreateFrameBuffers()
    {
        for (auto& frame : m_RenderApi->m_Frames)
        {
            std::vector<vk::ImageView> attachments = {
                frame.ImageView
            };

            vk::FramebufferCreateInfo framebufferInfo;
            framebufferInfo.flags = vk::FramebufferCreateFlags();
            framebufferInfo.renderPass = m_RenderApi->m_MainRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_SwapchainExtent.width;
            framebufferInfo.height = m_SwapchainExtent.height;
            framebufferInfo.layers = 1;

            try {
                frame.FrameBuffer = m_RenderApi->m_Device->Logical().createFramebuffer(framebufferInfo);
            }
            catch (vk::SystemError& err)
            {
                FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
                FE_ASSERT(false, "Creating Framebuffer failed");
            }

        }
    }
}
