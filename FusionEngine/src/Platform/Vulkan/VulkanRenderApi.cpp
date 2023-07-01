#include "fepch.h"
#include "VulkanRenderApi.h"

#include <GLFW/glfw3.h>

#include "VulkanDevice.h"
#include "VulkanUtils.h"
#include "UI/ImGui.h"

#undef CreateSemaphore

// Give this guy some credit for being a better teacher for free than most of the teaches at my school !
// https://www.youtube.com/@GetIntoGameDev

namespace FusionEngine
{
    void VulkanRenderApi::Init()
    {
        FE_INFO("Initializing Vulkan");
        
		m_Instance = new VulkanInstance();
    	m_SwapChain = new VulkanSwapChain();
		m_Device = new VulkanDevice(m_Instance, m_SwapChain);
    	// Update swapchain capabilities based on the selected device
    	m_SwapChain->AcquireSurfaceCapabilities();
        
    	
    	CreateRenderPass();
    	
		m_SwapChain->InitializeSwapchain();

    	CreateCommandPool();
    	m_MainCommandBuffer = CreateCommandBuffer();

    	m_ResourceManager = new VulkanResourceManager(static_cast<uint32_t>(m_Frames.size()));
    	for (auto& frame : m_Frames)
    	{
    		frame.CommandBuffer = CreateCommandBuffer();
    		frame.InFlightFence = VulkanUtils::CreateFence(m_Device->Logical());
    		frame.ImageAvailable = VulkanUtils::CreateSemaphoreW(m_Device->Logical());
    		frame.RenderFinished = VulkanUtils::CreateSemaphoreW(m_Device->Logical());
    	}

    	// ImGui
    	UI::ImGuiInitVulkan(m_Instance->GetInstance(), m_Device->Logical(), m_Device->Physical(), m_MainRenderPass,
    		m_Device->GraphicsFamily().value(), m_Device->GraphicsQueue(), m_SwapChain->GetImageCount());
    	UI::UploadImGuiFontsVulkan(m_CommandPool, m_MainCommandBuffer, m_Device->Logical(), m_Device->GraphicsQueue());
    }
	
    void VulkanRenderApi::OnWindowResize(uint32_t width, uint32_t height)
    {
    	//m_SwapChain->RecreateSwapChain();
    }

    void VulkanRenderApi::ShutDown()
    {
		m_Device->Logical().waitIdle();

		UI::ImGuiShutdownVulkan(m_Device->Logical());
    	
    	delete m_ResourceManager;
    	delete m_SwapChain;

    	m_Device->Logical().destroyCommandPool(m_CommandPool);
    	
    	m_Device->Logical().destroyRenderPass(m_MainRenderPass);
    	
		delete m_Device;
    	
    	delete m_Instance;
    }

    void VulkanRenderApi::BeginFrame()
    {
		UI::ImGuiNewFrame();
    	
    	auto result1 = m_Device->Logical().waitForFences(1, &m_Frames[m_CurrentFrame].InFlightFence, VK_TRUE, UINT64_MAX);

        try
        {
	        const auto result = m_Device->Logical().acquireNextImageKHR(m_SwapChain->GetSwapChain(), UINT64_MAX, m_Frames[m_CurrentFrame].ImageAvailable, nullptr);
        	if (m_CurrentFrame != result.value)
        	{
        		FE_WARN("Frames got out of sync, resyncing");
        		m_CurrentFrame = result.value;
        	}
        }
        catch (vk::OutOfDateKHRError&)
        {
        	FE_WARN("Swapchain out of date");
        	m_SwapChain->RecreateSwapChain();
        	return;
        }

    	auto result2 = m_Device->Logical().resetFences(1, &m_Frames[m_CurrentFrame].InFlightFence);

    	const vk::CommandBuffer commandBuffer = m_Frames[m_CurrentFrame].CommandBuffer;
    	commandBuffer.reset();

    	try
    	{
    		const vk::CommandBufferBeginInfo beginInfo;
    		commandBuffer.begin(beginInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Beginning Command Buffer failed");
    	}
    }

	void VulkanRenderApi::BeginSwapchainRenderPass()
    {
    	const vk::CommandBuffer commandBuffer = m_Frames[m_CurrentFrame].CommandBuffer;
    	
    	vk::RenderPassBeginInfo renderPassInfo = {};
    	renderPassInfo.renderPass = m_MainRenderPass;
    	renderPassInfo.framebuffer = m_Frames[m_CurrentFrame].FrameBuffer;
    	renderPassInfo.renderArea.offset.x = 0;
    	renderPassInfo.renderArea.offset.y = 0;
    	renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();

    	const vk::ClearValue clearColor = { std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f} };
    	renderPassInfo.clearValueCount = 1;
    	renderPassInfo.pClearValues = &clearColor;

    	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

    	vk::Viewport viewport = {};
    	viewport.x = 0.0f;
    	viewport.y = 0.0f;
    	viewport.width = static_cast<float>(m_SwapChain->GetSwapChainExtent().width);
    	viewport.height = static_cast<float>(m_SwapChain->GetSwapChainExtent().height);
    	viewport.minDepth = 0.0f;
    	viewport.maxDepth = 1.0f;
    	vk::Rect2D scissor = {};
    	scissor.offset.x = 0;
    	scissor.offset.y = 0;
    	scissor.extent = m_SwapChain->GetSwapChainExtent();
    	commandBuffer.setViewport(0, 1, &viewport);
    	commandBuffer.setScissor(0, 1, &scissor);
    }

	void VulkanRenderApi::EndSwapchainRenderPass()
	{
    	const vk::CommandBuffer commandBuffer = m_Frames[m_CurrentFrame].CommandBuffer;

    	// Render ImGui
    	UI::ImGuiRenderVulkan(commandBuffer);

    	// End
    	commandBuffer.endRenderPass();
	}

	void VulkanRenderApi::Draw(uint32_t vertexCount)
    {
    	m_Frames[m_CurrentFrame].CommandBuffer.draw(vertexCount, 1, 0, 0);
    }

    void VulkanRenderApi::DrawIndexed(uint32_t indexCount)
    {
    	m_Frames[m_CurrentFrame].CommandBuffer.drawIndexed(indexCount, 1, 0, 0, 0);
    }

    void VulkanRenderApi::EndFrame()
    {
    	const vk::CommandBuffer commandBuffer = m_Frames[m_CurrentFrame].CommandBuffer;
    	 
    	try {
    		commandBuffer.end();
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Ending Command Buffer failed");
    	}
    	
    	vk::SubmitInfo submitInfo = {};

    	const vk::Semaphore waitSemaphores[] = { m_Frames[m_CurrentFrame].ImageAvailable };
    	constexpr vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    	submitInfo.waitSemaphoreCount = 1;
    	submitInfo.pWaitSemaphores = waitSemaphores;
    	submitInfo.pWaitDstStageMask = waitStages;

    	submitInfo.commandBufferCount = 1;
    	submitInfo.pCommandBuffers = &m_Frames[m_CurrentFrame].CommandBuffer;

    	const vk::Semaphore signalSemaphores[] = { m_Frames[m_CurrentFrame].RenderFinished };
    	submitInfo.signalSemaphoreCount = 1;
    	submitInfo.pSignalSemaphores = signalSemaphores;

    	try {
    		m_Device->GraphicsQueue().submit(submitInfo, m_Frames[m_CurrentFrame].InFlightFence);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Submitting GraphicsQueue failed");
    	}

    	vk::PresentInfoKHR presentInfo = {};
    	presentInfo.waitSemaphoreCount = 1;
    	presentInfo.pWaitSemaphores = signalSemaphores;

    	const vk::SwapchainKHR swapChains[] = { m_SwapChain->GetSwapChain() };
    	presentInfo.swapchainCount = 1;
    	presentInfo.pSwapchains = swapChains;

    	presentInfo.pImageIndices = &m_CurrentFrame;
    	
    	try
    	{
    		const auto result = m_Device->PresentQueue().presentKHR(presentInfo);
    		if(result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
    		{
    			FE_WARN("Swapchain out of date or Suboptimal");
    			m_SwapChain->RecreateSwapChain();
    		}
    	}
    	catch (vk::OutOfDateKHRError&)
    	{
    		FE_WARN("Swapchain out of date");
    		m_SwapChain->RecreateSwapChain();
    	}
    	
    	m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames.size();
    }

    

    vk::CommandBuffer VulkanRenderApi::CreateCommandBuffer()
    {
    	vk::CommandBufferAllocateInfo allocInfo = {};
    	allocInfo.commandPool = m_CommandPool;
    	allocInfo.level = vk::CommandBufferLevel::ePrimary;
    	allocInfo.commandBufferCount = 1;
    	
    	try {
    		FE_INFO("Allocating command buffer");
    		return m_Device->Logical().allocateCommandBuffers(allocInfo)[0];
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
    	colorAttachment.format = m_SwapChain->GetSurfaceFormat().format;
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
    		m_MainRenderPass = m_Device->Logical().createRenderPass(renderpassInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating RenderPass failed");
    	}
    }

    void VulkanRenderApi::CreateCommandPool()
    {
    	vk::CommandPoolCreateInfo poolInfo;
    	poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    	poolInfo.queueFamilyIndex = m_Device->GraphicsFamily().value();

    	try {
    		m_CommandPool = m_Device->Logical().createCommandPool(poolInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating CommandPool failed");
    	}
    }
}
