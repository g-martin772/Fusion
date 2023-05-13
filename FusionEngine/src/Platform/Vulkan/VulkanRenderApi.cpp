#include "fepch.h"
#include "VulkanRenderApi.h"

#include <GLFW/glfw3.h>

#include "VulkanUtils.h"
#include "Core/Application.h"

#undef CreateSemaphore

// Give this guy some credit for being a better teacher for free than the teaches at my school !
// https://www.youtube.com/@GetIntoGameDev

namespace FusionEngine
{
    void VulkanRenderApi::Init()
    {
        FE_INFO("Initializing Vulkan");
        CreateInstance();

        m_DynamicInstanceDispatcher = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
        CreateDebugMessenger();

        CreateSurface();

        CreatePhysicalDevice();
        CreateLogicalDevice();
        CreateGraphicsQueue();
        CreatePresentQueue();

        CreateSwapChain();

    	CreateRenderPass();

    	CreateFrameBuffers();
    	CreateCommandPool();
    	m_MainCommandBuffer = CreateCommandBuffer();

    	for (auto& frame : m_Frames)
    	{
    		frame.CommandBuffer = CreateCommandBuffer();
    		frame.InFlightFence = VulkanUtils::CreateFence(m_LogicalDevice);
    		frame.ImageAvailable = VulkanUtils::CreateSemaphoreW(m_LogicalDevice);
    		frame.RenderFinished = VulkanUtils::CreateSemaphoreW(m_LogicalDevice);
    	}
    }
	
    void VulkanRenderApi::OnWindowResize(uint32_t width, uint32_t height)
    {
    	m_SwapchainExtent.width = width;
    	m_SwapchainExtent.height = height;
    	RecreateSwapChain();
    }

    void VulkanRenderApi::ShutDown()
    {
		m_LogicalDevice.waitIdle();
    	
    	CleanUpSwapChain();
    	
		m_LogicalDevice.destroyCommandPool(m_CommandPool);
    	
		m_LogicalDevice.destroyPipeline(m_Pipeline);
    	m_LogicalDevice.destroyPipelineLayout(m_PipelineLayout);
    	m_LogicalDevice.destroyRenderPass(m_RenderPass);
        
        m_LogicalDevice.destroySwapchainKHR(m_SwapChain);   
        m_LogicalDevice.destroy();

        m_Instance.destroySurfaceKHR(m_Surface);
        m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DynamicInstanceDispatcher);
        //m_Instance.destroy(); //throws for whatever reason
    }

    void VulkanRenderApi::Render()
    {
    	auto result1 = m_LogicalDevice.waitForFences(1, &m_Frames[m_CurrentFrame].InFlightFence, VK_TRUE, UINT64_MAX);

        try
        {
	        const auto result = m_LogicalDevice.acquireNextImageKHR(m_SwapChain, UINT64_MAX, m_Frames[m_CurrentFrame].ImageAvailable, nullptr);
        	if (m_CurrentFrame != result.value)
        	{
        		FE_WARN("Frames got out of sync, resyncing");
        		m_CurrentFrame = result.value;
        	}
        }
        catch (vk::OutOfDateKHRError&)
        {
        	FE_WARN("Swapchain out of date");
        	RecreateSwapChain();
        	return;
        }

    	auto result2 = m_LogicalDevice.resetFences(1, &m_Frames[m_CurrentFrame].InFlightFence);

        const vk::CommandBuffer commandBuffer = m_Frames[m_CurrentFrame].CommandBuffer;

    	commandBuffer.reset();

    	RecordDrawCommands(commandBuffer, m_CurrentFrame);

    	vk::SubmitInfo submitInfo = {};

        const vk::Semaphore waitSemaphores[] = { m_Frames[m_CurrentFrame].ImageAvailable };
        constexpr vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    	submitInfo.waitSemaphoreCount = 1;
    	submitInfo.pWaitSemaphores = waitSemaphores;
    	submitInfo.pWaitDstStageMask = waitStages;

    	submitInfo.commandBufferCount = 1;
    	submitInfo.pCommandBuffers = &commandBuffer;

        const vk::Semaphore signalSemaphores[] = { m_Frames[m_CurrentFrame].RenderFinished };
    	submitInfo.signalSemaphoreCount = 1;
    	submitInfo.pSignalSemaphores = signalSemaphores;

    	try {
    		m_GraphicsQueue.submit(submitInfo, m_Frames[m_CurrentFrame].InFlightFence);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Submitting GraphicsQueue failed");
    	}

    	vk::PresentInfoKHR presentInfo = {};
    	presentInfo.waitSemaphoreCount = 1;
    	presentInfo.pWaitSemaphores = signalSemaphores;

        const vk::SwapchainKHR swapChains[] = { m_SwapChain };
    	presentInfo.swapchainCount = 1;
    	presentInfo.pSwapchains = swapChains;

    	presentInfo.pImageIndices = &m_CurrentFrame;
    	
        try
        {
	        const auto result = m_PresentQueue.presentKHR(presentInfo);
        	if(result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
        	{
        		FE_WARN("Swapchain out of date or Suboptimal");
        		RecreateSwapChain();
        	}
        }
        catch (vk::OutOfDateKHRError&)
        {
        	FE_WARN("Swapchain out of date");
        	RecreateSwapChain();
        }
    	
    	m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames.size();
    }

    void VulkanRenderApi::CreateInstance()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap4.html#initialization-instances
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap44.html#extendingvulkan-coreversions-versionnumbers
        uint32_t version;
        vkEnumerateInstanceVersion(&version);
        FE_INFO("Vulkan Version: {0}.{1}.{2}", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

        vk::ApplicationInfo appInfo(
            "Fusion Engine",
            version,
            "Fusion Engine",
            version,
            version);

        // Get GLFW Window Extensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        extensions.push_back("VK_EXT_debug_utils");
        CheckSupportedExtensions(extensions);
        CheckSupportedLayers(m_EnabledLayers);
    
        vk::InstanceCreateInfo createInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            static_cast<uint32_t>(m_EnabledLayers.size()),
            m_EnabledLayers.data(),
            static_cast<uint32_t>(extensions.size()),
            extensions.data());

        try
        {
            FE_INFO("Creating Vulkan Instance...");
            m_Instance = vk::createInstance(createInfo);
            FE_INFO("Success");
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Failed to create Vulkan Instance");
        }
    }

    void VulkanRenderApi::CheckSupportedExtensions(const std::vector<const char*>& extensions) const
    {
        const std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

        FE_TRACE("Device can support following Vulkan extensions:");
        for (vk::ExtensionProperties extension : supportedExtensions)
            FE_TRACE("\t{0}", extension.extensionName);
        
        FE_TRACE("Requesting following Vulkan extensions:");
        for (const char* extension : extensions)
            FE_TRACE("\t{0}", extension);

        for (const char* extension : extensions)
        {
            bool found = false;
            for (vk::ExtensionProperties supportedExtension : supportedExtensions)
            {
                if(strcmp(extension, supportedExtension.extensionName) == 0)
                    found = true;
            }
            FE_ASSERT(found, "Vulkan Extension '{0}' is not supported by this device", extension);
        }
    }

    void VulkanRenderApi::CheckSupportedLayers(const std::vector<const char*>& layers) const
    {
        const std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

        FE_TRACE("Device can support following Vulkan layers:");
        for (vk::LayerProperties layer : supportedLayers)
            FE_TRACE("\t{0}", layer.layerName);
        
        FE_TRACE("Requesting following Vulkan layers:");
        for (const char* layer : layers)
            FE_TRACE("\t{0}", layer);

        for (const char* layer : layers)
        {
            bool found = false;
            for (vk::LayerProperties supportedLayer : supportedLayers)
            {
                if(strcmp(layer, supportedLayer.layerName) == 0)
                    found = true;
            }
            FE_ASSERT(found, "Requested Vulkan Layer is not supported by this device");
        }
    }

    // callback for https://github.com/KhronosGroup/Vulkan-Hpp/blob/main/README.md 
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        switch (messageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: FE_TRACE("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: FE_INFO("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: FE_WARN("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: FE_ERROR("[Vulkan]: {0}", pCallbackData->pMessage); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT: FE_FATAL("[Vulkan]: {0}", pCallbackData->pMessage); break;
            default: FE_ASSERT(false, "What error to hell is this?"); break;
        }
        return VK_FALSE;
    }
    
    void VulkanRenderApi::CreateDebugMessenger()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap49.html#debugging-debug-messengers
        vk::DebugUtilsMessengerCreateInfoEXT createInfo(
            vk::DebugUtilsMessengerCreateFlagsEXT(),
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
            debugCallback,
            nullptr);

        m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(createInfo, nullptr, m_DynamicInstanceDispatcher);
    }

    void VulkanRenderApi::CreateSurface()
    {
        VkSurfaceKHR cSurface;
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());
        if(glfwCreateWindowSurface(m_Instance, window, nullptr, &cSurface) != VK_SUCCESS)
            FE_ASSERT(false, "Failed to create Vulkan Surface")
        else
            FE_INFO("Sucessfully created Vulkan surface");

        m_Surface = cSurface;
    }

    void VulkanRenderApi::CreatePhysicalDevice()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#devsandqueues-devices

        const std::vector<vk::PhysicalDevice> availableDevices = m_Instance.enumeratePhysicalDevices();
        FE_INFO("There were {0} physical GPUs found by the engine", availableDevices.size());
        for (vk::PhysicalDevice device : availableDevices)
            FE_INFO("\t{0}", device.getProperties().deviceName);

        std::vector<const char*> requiredExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        FE_TRACE("Requesting following Vulkan Device Extensions:");
        for (const char* extension : requiredExtensions)
            FE_TRACE("\t{0}", extension);

        uint32_t currentScore = 0;
        for (vk::PhysicalDevice device : availableDevices)
        {
            std::set<std::string> requiredExtensionsSet(requiredExtensions.begin(), requiredExtensions.end());

            for (vk::ExtensionProperties extension : device.enumerateDeviceExtensionProperties())
            {
                requiredExtensionsSet.erase(extension.extensionName);
            }
            
            uint32_t score = 0;
            if (!requiredExtensionsSet.empty())
               continue;

            if (device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                score += 5000;

            score += device.getProperties().limits.maxImageDimension2D; // around 1000 - 10000
            
            if(score > currentScore)
            {
                currentScore = score;
                m_PhysicalDevice = device;
            }
        }

        FE_ASSERT(m_PhysicalDevice, "No suitable GPU found!");
        FE_INFO("Picked {0} as rendering GPU", m_PhysicalDevice.getProperties().deviceName);
    }

    void VulkanRenderApi::CreateLogicalDevice()
    {
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#VkQueueFamilyProperties
        // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap5.html#VkQueueFlagBits
        std::vector<vk::QueueFamilyProperties> queueFamilies = m_PhysicalDevice.getQueueFamilyProperties();

        FE_INFO("Device can support {0} Queue families", queueFamilies.size());
        
        for (int i = 0; i < queueFamilies.size(); i++)
        {
            vk::QueueFamilyProperties queueFamily = queueFamilies[i];
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                m_GraphicsFamily = i;
                FE_INFO("Queue Family {0} is suitable for praphics", i);
            }

            if (m_PhysicalDevice.getSurfaceSupportKHR(i, m_Surface))
            {
                m_PresentFamily = i;
                FE_INFO("Queue Family {0} is suitable for presenting", i);
            }

            if (m_GraphicsFamily.has_value() && m_PresentFamily.has_value())
                break;
        }

        FE_ASSERT(m_GraphicsFamily.has_value(), "Device does not support a praphics queue");
        FE_ASSERT(m_PresentFamily.has_value(), "Device does not support a present queue");

        FE_INFO("Creating Logical Device...");
        float queuePriority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queueInfos;
        
        queueInfos.push_back(vk::DeviceQueueCreateInfo(
            vk::DeviceQueueCreateFlags(),
            m_GraphicsFamily.value(),
            1,
            &queuePriority));

        if (m_GraphicsFamily.value() != m_PresentFamily.value())
            queueInfos.push_back(vk::DeviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                m_PresentFamily.value(),
                1,
                &queuePriority));

        std::vector<const char*> deviceExtensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        
        vk::PhysicalDeviceFeatures deviceFeatures {
            //request device features here
        };

        vk::DeviceCreateInfo deviceInfo(
            vk::DeviceCreateFlags(),
            static_cast<uint32_t>(queueInfos.size()),
            queueInfos.data(),
            static_cast<uint32_t>(m_EnabledLayers.size()),
            m_EnabledLayers.data(),
            static_cast<uint32_t>(deviceExtensions.size()),
            deviceExtensions.data(),
            &deviceFeatures);

        try
        {
            m_LogicalDevice = m_PhysicalDevice.createDevice(deviceInfo);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating logical device failed");
        }
        FE_INFO("Success");
    }

    void VulkanRenderApi::CreateGraphicsQueue()
    {
        FE_INFO("Creating Graphics Queue...");
        m_GraphicsQueue = m_LogicalDevice.getQueue(m_GraphicsFamily.value(), 0);
        FE_INFO("Success");
    }

    void VulkanRenderApi::CreatePresentQueue()
    {
        FE_INFO("Creating Present Queue...");
        m_PresentQueue = m_LogicalDevice.getQueue(m_PresentFamily.value(), 0);
        FE_INFO("Success");
    }

    void VulkanRenderApi::QuerySwapchainSupport()
    {
        m_SwapChainCapabilities.Capabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(m_Surface);

        FE_TRACE("Surface Capabilities:");
        FE_TRACE("\tMinimum image count: {0}", m_SwapChainCapabilities.Capabilities.minImageCount);
        FE_TRACE("\tMaximum image count: {0}", m_SwapChainCapabilities.Capabilities.maxImageCount);
        FE_TRACE("\tCurrent Extent: {0}, {1}", m_SwapChainCapabilities.Capabilities.currentExtent.width, m_SwapChainCapabilities.Capabilities.currentExtent.height);
        FE_TRACE("\tMinimum Extent: {0}, {1}", m_SwapChainCapabilities.Capabilities.minImageExtent.width, m_SwapChainCapabilities.Capabilities.minImageExtent.height);
        FE_TRACE("\tMaximum Extent: {0}, {1}", m_SwapChainCapabilities.Capabilities.maxImageExtent.width, m_SwapChainCapabilities.Capabilities.maxImageExtent.height);
        FE_TRACE("\tMaximum image array layers: {0}", m_SwapChainCapabilities.Capabilities.maxImageArrayLayers);

        m_SwapChainCapabilities.Formats = m_PhysicalDevice.getSurfaceFormatsKHR(m_Surface);

        FE_TRACE("Surface Formats:");
        bool pickedFormat = false;
        for(vk::SurfaceFormatKHR format : m_SwapChainCapabilities.Formats)
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
            m_SurfaceFormat = m_SwapChainCapabilities.Formats[0];

        m_SwapChainCapabilities.PresentModes = m_PhysicalDevice.getSurfacePresentModesKHR(m_Surface);

        FE_TRACE("Surface Present Modes:");
        for(vk::PresentModeKHR presentMode : m_SwapChainCapabilities.PresentModes)
        {
            FE_TRACE("\t{0}", vk::to_string(presentMode));
            if (presentMode == vk::PresentModeKHR::eMailbox)
                m_PresentMode = presentMode;
        }

    	if(m_SwapchainExtent.width == 0 || m_SwapchainExtent.height == 0)
			m_SwapchainExtent = m_SwapChainCapabilities.Capabilities.currentExtent;
    }

    void VulkanRenderApi::CreateSwapChain()
    {
        QuerySwapchainSupport();
        // https://vulkan.lunarg.com/doc/view/latest/windows/tutorial/html/05-init_swapchain.html

        #undef min // who broke the std lib again ... ohhr
        uint32_t imageCount = std::min(
            m_SwapChainCapabilities.Capabilities.maxImageCount,
            m_SwapChainCapabilities.Capabilities.minImageCount + 1
        );

        FE_INFO("Creating Swapchain...");
        vk::SwapchainCreateInfoKHR createInfo(
            vk::SwapchainCreateFlagsKHR(),
            m_Surface,
            imageCount,
            m_SurfaceFormat.format,
            m_SurfaceFormat.colorSpace,
            m_SwapchainExtent,
            m_SwapChainCapabilities.Capabilities.maxImageArrayLayers,
            vk::ImageUsageFlagBits::eColorAttachment);

        if(m_GraphicsFamily.value() != m_PresentFamily.value())
        {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            const uint32_t queueFamilyIndices[] = { m_GraphicsFamily.value(), m_PresentFamily.value() };
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = m_SwapChainCapabilities.Capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = m_PresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

        try
        {
            m_SwapChain = m_LogicalDevice.createSwapchainKHR(createInfo);
            const std::vector<vk::Image> images = m_LogicalDevice.getSwapchainImagesKHR(m_SwapChain);
        	m_Frames = std::vector<Frame>(images.size());
        	
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

            	m_Frames[i].Image = images[i];
                m_Frames[i].ImageView = m_LogicalDevice.createImageView(createInfo);
            }
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating swapchain failed");
        }
        FE_INFO("Success");
    }

    void VulkanRenderApi::RecreateSwapChain()
    {
		int width = 0, heith = 0;

		if(width <= 0 || heith <= 0)
		{
    		glfwGetFramebufferSize(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow()), &width, &heith);
			glfwWaitEvents();
		}
    	
    	FE_INFO("Recreating Swapchain");
    	CleanUpSwapChain();
    	CreateSwapChain();
    	CreateFrameBuffers();
    	for (auto& frame : m_Frames)
    	{
    		frame.CommandBuffer = CreateCommandBuffer();
    		frame.InFlightFence = VulkanUtils::CreateFence(m_LogicalDevice);
    		frame.ImageAvailable = VulkanUtils::CreateSemaphoreW(m_LogicalDevice);
    		frame.RenderFinished = VulkanUtils::CreateSemaphoreW(m_LogicalDevice);
    	}
    }

    void VulkanRenderApi::CleanUpSwapChain()
    {
    	m_LogicalDevice.waitIdle();
    	
    	for (auto& frame : m_Frames)
    	{
    		m_LogicalDevice.destroyFence(frame.InFlightFence);
    		m_LogicalDevice.destroySemaphore(frame.ImageAvailable);
    		m_LogicalDevice.destroySemaphore(frame.RenderFinished);
    		m_LogicalDevice.destroyImageView(frame.ImageView);
    		m_LogicalDevice.destroyFramebuffer(frame.FrameBuffer);
    	}
    }

    vk::CommandBuffer VulkanRenderApi::CreateCommandBuffer()
    {
    	vk::CommandBufferAllocateInfo allocInfo = {};
    	allocInfo.commandPool = m_CommandPool;
    	allocInfo.level = vk::CommandBufferLevel::ePrimary;
    	allocInfo.commandBufferCount = 1;
    	
    	try {
    		FE_INFO("Allocating command buffer");
    		return m_LogicalDevice.allocateCommandBuffers(allocInfo)[0];
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating Command Buffer failed");
    	}

    	return nullptr;
    }

	void VulkanRenderApi::CreateRenderPass()
    {
    	//Define a general attachment, with its load/store operations
    	vk::AttachmentDescription colorAttachment = {};
    	colorAttachment.flags = vk::AttachmentDescriptionFlags();
    	colorAttachment.format = m_SurfaceFormat.format;
    	colorAttachment.samples = vk::SampleCountFlagBits::e1;
    	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    	//Declare that attachment to be color buffer 0 of the framebuffer
    	vk::AttachmentReference colorAttachmentRef = {};
    	colorAttachmentRef.attachment = 0;
    	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    	//Renderpasses are broken down into subpasses, there's always at least one.
    	vk::SubpassDescription subpass = {};
    	subpass.flags = vk::SubpassDescriptionFlags();
    	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    	subpass.colorAttachmentCount = 1;
    	subpass.pColorAttachments = &colorAttachmentRef;

    	//Now create the renderpass
    	vk::RenderPassCreateInfo renderpassInfo = {};
    	renderpassInfo.flags = vk::RenderPassCreateFlags();
    	renderpassInfo.attachmentCount = 1;
    	renderpassInfo.pAttachments = &colorAttachment;
    	renderpassInfo.subpassCount = 1;
    	renderpassInfo.pSubpasses = &subpass;
    	try {
    		m_RenderPass = m_LogicalDevice.createRenderPass(renderpassInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating RenderPass failed");
    	}
    }

    void VulkanRenderApi::CreateFrameBuffers()
    {
    	for (auto& frame : m_Frames)
        {
			FE_INFO("Creating FrameBuffer");
    		std::vector<vk::ImageView> attachments = {
    			frame.ImageView
            };

    		vk::FramebufferCreateInfo framebufferInfo;
    		framebufferInfo.flags = vk::FramebufferCreateFlags();
    		framebufferInfo.renderPass = m_RenderPass;
    		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    		framebufferInfo.pAttachments = attachments.data();
    		framebufferInfo.width = m_SwapchainExtent.width;
    		framebufferInfo.height = m_SwapchainExtent.height;
    		framebufferInfo.layers = 1;

    		try {
    			frame.FrameBuffer = m_LogicalDevice.createFramebuffer(framebufferInfo);
    		}
    		catch (vk::SystemError& err)
    		{
    			FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    			FE_ASSERT(false, "Creating Framebuffer failed");
    		}

    	}
    }

    void VulkanRenderApi::CreateCommandPool()
    {
    	vk::CommandPoolCreateInfo poolInfo;
    	poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    	poolInfo.queueFamilyIndex = m_GraphicsFamily.value();

    	try {
    		m_CommandPool = m_LogicalDevice.createCommandPool(poolInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating CommandPool failed");
    	}
    }

    void VulkanRenderApi::RecordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
    {
    	vk::CommandBufferBeginInfo beginInfo;

    	try {
    		commandBuffer.begin(beginInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Beginning Command Buffer failed");
    	}

    	vk::RenderPassBeginInfo renderPassInfo = {};
    	renderPassInfo.renderPass = m_RenderPass;
    	renderPassInfo.framebuffer = m_Frames[imageIndex].FrameBuffer;
    	renderPassInfo.renderArea.offset.x = 0;
    	renderPassInfo.renderArea.offset.y = 0;
    	renderPassInfo.renderArea.extent = m_SwapchainExtent;

        const vk::ClearValue clearColor = { std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f} };
    	renderPassInfo.clearValueCount = 1;
    	renderPassInfo.pClearValues = &clearColor;

    	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

    	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);

    	vk::Viewport viewport = {};
    	viewport.x = 0.0f;
    	viewport.y = 0.0f;
    	viewport.width = static_cast<float>(m_SwapchainExtent.width);
    	viewport.height = static_cast<float>(m_SwapchainExtent.height);
    	viewport.minDepth = 0.0f;
    	viewport.maxDepth = 1.0f;
    	vk::Rect2D scissor = {};
    	scissor.offset.x = 0.0f;
    	scissor.offset.y = 0.0f;
    	scissor.extent = m_SwapchainExtent;
    	commandBuffer.setViewport(0, 1, &viewport);
    	commandBuffer.setScissor(0, 1, &scissor);

    	// finally
    	commandBuffer.bindVertexBuffers(0, 1, &m_CurrentVertexBuffer, &m_CurrentVertexBufferOffset);
    	commandBuffer.draw(3, 1, 0, 0);

    	commandBuffer.endRenderPass();

    	try {
    		commandBuffer.end();
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Ending Command Buffer failed");
    	}
    }
}
