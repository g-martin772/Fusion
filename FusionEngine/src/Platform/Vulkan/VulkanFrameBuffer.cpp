#include "fepch.h"
#include "VulkanFrameBuffer.h"

#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanRenderApi.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    VulkanFrameBuffer::VulkanFrameBuffer(const FramebufferSpecification& spec)
    {
        m_Spec = spec;
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());
    	FE_ASSERT(spec.Width > 0 && spec.Height > 0, "Framebuffer size must be greater than 0");
    	m_Width = spec.Width;
    	m_Height = spec.Height;
    	
		// CreateRenderPass
    	
    	std::vector<vk::AttachmentDescription> attachments;
    	std::vector<vk::AttachmentReference> attachmentRefs;
    	
		for(size_t i = 0; i < m_Spec.Attachments.size(); i++)
		{
			vk::AttachmentDescription attachment = {};
			attachment.flags = vk::AttachmentDescriptionFlags();
			attachment.format = std::dynamic_pointer_cast<VulkanImage>(m_Spec.Attachments[i].AttachedImage)->GetVulkanFormat();
			
			attachment.samples = static_cast<vk::SampleCountFlagBits>(m_Spec.Samples);

			switch (m_Spec.Attachments[i].LoadOp) {
				case FrameBufferLoadOp::Load: attachment.loadOp = vk::AttachmentLoadOp::eLoad; break;
				case FrameBufferLoadOp::Clear: attachment.loadOp = vk::AttachmentLoadOp::eClear; break;
				case FrameBufferLoadOp::DontCare: attachment.loadOp = vk::AttachmentLoadOp::eDontCare; break;
			}

			switch (m_Spec.Attachments[i].StoreOp) {
				case FramebufferStoreOp::Store: attachment.storeOp = vk::AttachmentStoreOp::eStore; break;
				case FramebufferStoreOp::DontCare: attachment.storeOp = vk::AttachmentStoreOp::eDontCare; break;
			}
			
			attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
			attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
			attachment.initialLayout = vk::ImageLayout::eUndefined;
			attachment.finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

			vk::AttachmentReference attachmentRef = {};
			attachmentRef.attachment = i;
			attachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

			attachments.push_back(attachment);
			attachmentRefs.push_back(attachmentRef);
		}
    	
    	vk::SubpassDescription subpass = {};
    	subpass.flags = vk::SubpassDescriptionFlags();
    	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    	subpass.colorAttachmentCount = attachmentRefs.size();
    	subpass.pColorAttachments = attachmentRefs.data();

    	std::vector<vk::SubpassDependency> dependencies;

    	{
    		VkSubpassDependency& depedency = dependencies.emplace_back();
    		depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
    		depedency.dstSubpass = 0;
    		depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    		depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    		depedency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    		depedency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    		depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    	}
    	{
    		VkSubpassDependency& depedency = dependencies.emplace_back();
    		depedency.srcSubpass = 0;
    		depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
    		depedency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    		depedency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    		depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    		depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    		depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    	}

    	vk::RenderPassCreateInfo renderpassInfo = {};
    	renderpassInfo.flags = vk::RenderPassCreateFlags();
    	renderpassInfo.attachmentCount = attachments.size();
    	renderpassInfo.pAttachments = attachments.data();
    	renderpassInfo.subpassCount = 1;
    	renderpassInfo.pSubpasses = &subpass;
    	renderpassInfo.dependencyCount = dependencies.size();
    	renderpassInfo.pDependencies = dependencies.data();
    	
    	try {
    		m_RenderPass = m_RenderApi->m_Device->Logical().createRenderPass(renderpassInfo);
    	}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating RenderPass failed");
    	}

    	CreateFrameBuffers();
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
    	m_RenderApi->WaitDeviceIdle();
    	m_RenderApi->m_Device->Logical().destroyRenderPass(m_RenderPass);
    	for(const vk::Framebuffer framebuffer : m_Framebuffers)
			m_RenderApi->m_Device->Logical().destroyFramebuffer(framebuffer);
    }

	void VulkanFrameBuffer::CreateFrameBuffers()
    {
    	for (int i = 0; i < m_Spec.FramesInFlight; i++)
    	{
    		std::vector<vk::ImageView> attachments;
    		for (auto attachment : m_Spec.Attachments)
    		{
    			attachments.push_back(std::dynamic_pointer_cast<VulkanImage>(attachment.AttachedImage)->GetImageView());
    		}

    		vk::FramebufferCreateInfo framebufferInfo;
    		framebufferInfo.flags = vk::FramebufferCreateFlags();
    		framebufferInfo.renderPass = m_RenderPass;
    		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    		framebufferInfo.pAttachments = attachments.data();
    		framebufferInfo.width = m_Width;
    		framebufferInfo.height = m_Height;
    		framebufferInfo.layers = 1;

    		try {
    			m_Framebuffers.push_back(m_RenderApi->m_Device->Logical().createFramebuffer(framebufferInfo));
    		}
    		catch (vk::SystemError& err)
    		{
    			FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    			FE_ASSERT(false, "Creating Framebuffer failed");
    		}
    	}
    }

    void VulkanFrameBuffer::OnResize(uint32_t width, uint32_t height)
    {
        if(m_Width == width && m_Height == height)
            return;

    	m_Width = width;
    	m_Height = height;

    	for(auto attachment : m_Spec.Attachments)
			attachment.AttachedImage->OnResize(width, height);

    	m_RenderApi->WaitDeviceIdle();
    	for(const vk::Framebuffer framebuffer : m_Framebuffers)
    	{
    		m_RenderApi->m_Device->Logical().destroyFramebuffer(framebuffer);
    	}
    	m_Framebuffers.clear();

    	CreateFrameBuffers();
    }

    void VulkanFrameBuffer::Begin()
    {
    	const vk::CommandBuffer commandBuffer = m_RenderApi->GetCurrentFrame().CommandBuffer;
    	
    	vk::RenderPassBeginInfo renderPassInfo = {};
    	renderPassInfo.renderPass = m_RenderPass;
    	renderPassInfo.framebuffer = m_Framebuffers[m_FrameIndex];
    	renderPassInfo.renderArea.offset.x = 0;
    	renderPassInfo.renderArea.offset.y = 0;
    	renderPassInfo.renderArea.extent = vk::Extent2D(m_Width, m_Height);

    	const vk::ClearValue clearColor = { std::array<float, 4>{m_Spec.ClearColor.r, m_Spec.ClearColor.g, m_Spec.ClearColor.b, m_Spec.ClearColor.a} };
    	renderPassInfo.clearValueCount = 1;
    	renderPassInfo.pClearValues = &clearColor;

    	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

    	vk::Viewport viewport = {};
    	viewport.x = 0.0f;
    	viewport.y = 0.0f;
    	viewport.width = m_Width;
    	viewport.height = m_Height;
    	viewport.minDepth = 0.0f;
    	viewport.maxDepth = 1.0f;
    	vk::Rect2D scissor = {};
    	scissor.offset.x = 0;
    	scissor.offset.y = 0;
    	scissor.extent = vk::Extent2D(m_Width, m_Height);
    	commandBuffer.setViewport(0, 1, &viewport);
    	commandBuffer.setScissor(0, 1, &scissor);

    	if(m_Framebuffers.size() - 1 > m_FrameIndex)
    		m_FrameIndex++;
    	else
    		m_FrameIndex = 0;
    }

    void VulkanFrameBuffer::End()
    {
    	m_RenderApi->GetCurrentFrame().CommandBuffer.endRenderPass();
    }

    Shared<Image> VulkanFrameBuffer::GetCurrentImage() const
    {
    	return  m_Spec.Attachments[m_FrameIndex].AttachedImage;
    }
}
