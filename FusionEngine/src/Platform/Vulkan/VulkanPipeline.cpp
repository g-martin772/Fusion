#include "fepch.h"
#include "VulkanPipeline.h"

#include "VulkanRenderApi.h"
#include "VulkanUtils.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec)
    {
        m_Shader = std::dynamic_pointer_cast<VulkanShader>(spec.Shader);
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());
		m_Spec = spec;

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.flags = vk::PipelineCreateFlags();

		//Shader stages, to be populated later
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		//Vertex Input
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
		uint32_t attributeCount = 0, bindingCount = 0;
		std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions;
		std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;
		for (std::vector<VertexBuffer::Attribute> vertexBufferLayout : spec.VertexBufferLayouts)
		{
			uint32_t location = 0, offset = 0;
			for (VertexBuffer::Attribute attribute : vertexBufferLayout)
			{
				vertexInputAttributeDescriptions.emplace_back(location, bindingCount, Utils::FusionVertexAttributeToVkFormat(attribute), offset);
				offset += Utils::FusionVertexAttributeToByteSize(attribute);
				attributeCount++;
				location++;
			}
			vertexInputBindingDescriptions.emplace_back(bindingCount, offset, vk::VertexInputRate::eVertex);
			bindingCount++;
		}
		vertexInputInfo.vertexBindingDescriptionCount = bindingCount;
		vertexInputInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();
		vertexInputInfo.vertexAttributeDescriptionCount = attributeCount;
		vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;

		//Input Assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
		inputAssemblyInfo.topology = Utils::FusionDrawModeToVkPrimitiveTopology(spec.DrawMode);
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

		//Vertex Shader
		FE_INFO("Create vertex shader module");
		vk::ShaderModule vertexShader = m_Shader->GetVertexShaderModule();
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
		rasterizer.cullMode = vk::CullModeFlagBits::eNone;
		rasterizer.frontFace = vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE; //Depth bias can be useful in shadow maps.
		pipelineInfo.pRasterizationState = &rasterizer;

		//Fragment Shader
		FE_INFO("Create fragment shader module");
		vk::ShaderModule fragmentShader = m_Shader->GetFragmentShaderModule();
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

		// DescriptorSet Layout
		FE_INFO("Create DescriptorSet Layout");
		m_DescriptorSetLayout = MakeDescriptorSetLayout();
		
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

    VulkanPipeline::~VulkanPipeline()
    {
		m_RenderApi->m_LogicalDevice.destroyPipeline(m_Pipeline);
		m_RenderApi->m_LogicalDevice.destroyPipelineLayout(m_PipelineLayout);
		m_RenderApi->m_LogicalDevice.destroyDescriptorSetLayout(m_DescriptorSetLayout);
    }

    void VulkanPipeline::Bind()
    {
    	if(m_RenderApi->m_ResourceManager->HasFrameDescriptorSet(m_RenderApi->m_CurrentFrame))
    		m_RenderApi->m_Frames[m_RenderApi->m_CurrentFrame].CommandBuffer.bindDescriptorSets(
				vk::PipelineBindPoint::eGraphics, m_PipelineLayout,
				0,
				1,
				&m_RenderApi->m_ResourceManager->GetFrameDescriptorSet(m_RenderApi->m_CurrentFrame),
				0,
				nullptr);
    	
        m_RenderApi->m_Frames[m_RenderApi->m_CurrentFrame].CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);
    }

    vk::PipelineLayout VulkanPipeline::MakePipelineLayout() const
    {
		vk::PipelineLayoutCreateInfo layoutInfo;
		layoutInfo.flags = vk::PipelineLayoutCreateFlags();

		// DescriptorSetLayouts
		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = &m_DescriptorSetLayout;

		// Push constants
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

    vk::DescriptorSetLayout VulkanPipeline::MakeDescriptorSetLayout() const
	{
		std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.reserve(m_Spec.DescriptorSetLayouts.size());

		for (const DescriptorSetLayout& binding : m_Spec.DescriptorSetLayouts)
		{
			vk::DescriptorSetLayoutBinding layoutBinding;
			layoutBinding.binding = binding.Index;
			layoutBinding.descriptorType = Utils::FusionDescriptorTypeToVkDescriptorType(binding.Type);
			layoutBinding.descriptorCount = binding.Count;
			layoutBinding.stageFlags = Utils::FusionShaderTypeToVkShaderType(binding.Stage);
			layoutBindings.push_back(layoutBinding);
		}
		
		vk::DescriptorSetLayoutCreateInfo layoutInfo;
		layoutInfo.flags = vk::DescriptorSetLayoutCreateFlagBits();
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();


		try {
			return m_RenderApi->m_LogicalDevice.createDescriptorSetLayout(layoutInfo);
		}
		catch (vk::SystemError& err)
		{
			FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
			FE_ASSERT(false, "Creating Descriptor Set Layout failed");
			return nullptr;
		}
    }
}
