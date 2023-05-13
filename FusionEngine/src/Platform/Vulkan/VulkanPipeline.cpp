#include "fepch.h"
#include "VulkanPipeline.h"

#include "VulkanRenderApi.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
	vk::PrimitiveTopology FusionDrawModeToVkPrimitiveTopology(const Pipeline::DrawMode mode)
	{
		switch (mode) {
			case Pipeline::DrawMode::Triangles: vk::PrimitiveTopology::eTriangleList;
			case Pipeline::DrawMode::Lines: vk::PrimitiveTopology::eLineList;
			case Pipeline::DrawMode::Points: vk::PrimitiveTopology::ePointList;
		}
		return vk::PrimitiveTopology::eTriangleList;
	}
	
    VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec)
    {
        m_Shader = std::dynamic_pointer_cast<VulkanShader>(spec.Shader);
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.flags = vk::PipelineCreateFlags();

		//Shader stages, to be populated later
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		//Vertex Input
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		pipelineInfo.pVertexInputState = &vertexInputInfo;

		//Input Assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
		inputAssemblyInfo.topology = FusionDrawModeToVkPrimitiveTopology(spec.DrawMode);
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

		//Vertex Shader
		FE_INFO("Create vertex shader module");
		vk::ShaderModule vertexShader = CreateShaderModule(m_Shader->GetVertexShader());
		vk::PipelineShaderStageCreateInfo vertexShaderInfo = {};
		vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
		vertexShaderInfo.module = vertexShader;
		vertexShaderInfo.pName = "main";
		shaderStages.push_back(vertexShaderInfo);

		//Viewport and Scissor
		vk::PipelineViewportStateCreateInfo viewportState = {};
		viewportState.flags = vk::PipelineViewportStateCreateFlags();
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;
		pipelineInfo.pViewportState = &viewportState;

    	// Dynamic state
    	vk::DynamicState dynamicStates[] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    	vk::PipelineDynamicStateCreateInfo dynamicStateInfo = {};
    	dynamicStateInfo.dynamicStateCount = 2;
    	dynamicStateInfo.pDynamicStates = dynamicStates;
		pipelineInfo.pDynamicState = &dynamicStateInfo;
    	
		//Rasterizer
		vk::PipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
		rasterizer.depthClampEnable = VK_FALSE; //discard out of bounds fragments, don't clamp them
		rasterizer.rasterizerDiscardEnable = VK_FALSE; //This flag would disable fragment output
		rasterizer.polygonMode = spec.WireFrame ? vk::PolygonMode::eLine : vk::PolygonMode::eFill;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = vk::CullModeFlagBits::eBack;
		rasterizer.frontFace = vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE; //Depth bias can be useful in shadow maps.
		pipelineInfo.pRasterizationState = &rasterizer;

		//Fragment Shader
		FE_INFO("Create fragment shader module");
		vk::ShaderModule fragmentShader = CreateShaderModule(m_Shader->GetFragmentShader());
		vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
		fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
		fragmentShaderInfo.module = fragmentShader;
		fragmentShaderInfo.pName = "main";
		shaderStages.push_back(fragmentShaderInfo);
		//Now both shaders have been made, we can declare them to the pipeline info
		pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();

		//Multisampling
		vk::PipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
		pipelineInfo.pMultisampleState = &multisampling;

		//Color Blend
		vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		colorBlendAttachment.blendEnable = VK_FALSE;
		vk::PipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = vk::LogicOp::eCopy;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;
		pipelineInfo.pColorBlendState = &colorBlending;

		//Pipeline Layout
		FE_INFO("Create Pipeline Layout");
		m_PipelineLayout = MakePipelineLayout();
		pipelineInfo.layout = m_PipelineLayout;

		//Renderpass
		FE_INFO("Create RenderPass");
		pipelineInfo.renderPass = m_RenderApi->m_RenderPass;
		pipelineInfo.subpass = 0;

		//Extra stuff
		pipelineInfo.basePipelineHandle = nullptr;

		//Make the Pipeline
		FE_INFO("Create Graphics Pipeline");
		try {
			m_Pipeline = (m_RenderApi->m_LogicalDevice.createGraphicsPipeline(nullptr, pipelineInfo)).value;
		}
    	catch (vk::SystemError& err)
    	{
    		FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
    		FE_ASSERT(false, "Creating Pipeline failed");
    	}

		//Clean up by destroying shader modules
		m_RenderApi->m_LogicalDevice.destroyShaderModule(vertexShader);
		m_RenderApi->m_LogicalDevice.destroyShaderModule(fragmentShader);
    }

	void VulkanPipeline::Bind()
    {
        m_RenderApi->m_PipelineLayout = m_PipelineLayout;
    	m_RenderApi->m_Pipeline = m_Pipeline;
    }
	
	vk::ShaderModule VulkanPipeline::CreateShaderModule(const std::vector<char>& spirv) const
	{
        vk::ShaderModuleCreateInfo moduleInfo = {};
        moduleInfo.flags = vk::ShaderModuleCreateFlags();
        moduleInfo.codeSize = spirv.size();
        moduleInfo.pCode = reinterpret_cast<const uint32_t*>(spirv.data());

        try {
            return m_RenderApi->m_LogicalDevice.createShaderModule(moduleInfo);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating Shadermodule failed");
        }
		return nullptr;
    }

    vk::PipelineLayout VulkanPipeline::MakePipelineLayout() const
    {
		vk::PipelineLayoutCreateInfo layoutInfo;
		layoutInfo.flags = vk::PipelineLayoutCreateFlags();
		layoutInfo.setLayoutCount = 0;
		layoutInfo.pushConstantRangeCount = 0;
		try {
			return m_RenderApi->m_LogicalDevice.createPipelineLayout(layoutInfo);
		}
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating Pipeline Layout failed");
        }
		return nullptr;
	}
}
