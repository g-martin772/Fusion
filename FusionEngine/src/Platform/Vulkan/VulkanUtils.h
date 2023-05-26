#pragma once

#include <vulkan/vulkan.hpp>

#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"

namespace FusionEngine
{
    namespace Utils
	{
		inline vk::PrimitiveTopology FusionDrawModeToVkPrimitiveTopology(const DrawMode mode)
		{
			switch (mode) {
			case DrawMode::Triangles: vk::PrimitiveTopology::eTriangleList;
			case DrawMode::Lines: vk::PrimitiveTopology::eLineList;
			case DrawMode::Points: vk::PrimitiveTopology::ePointList;
			}
			return vk::PrimitiveTopology::eTriangleList;
		}

		inline vk::Format FusionVertexAttributeToVkFormat(const VertexBuffer::Attribute attribute)
		{
			switch (attribute) {
				case VertexBuffer::Attribute::Bool: return vk::Format::eR8Sint;
				case VertexBuffer::Attribute::Char: return vk::Format::eR8Uint;
				case VertexBuffer::Attribute::Short: return vk::Format::eR16Sint;
				case VertexBuffer::Attribute::UShort: return vk::Format::eR16Uint;
				case VertexBuffer::Attribute::Int: return vk::Format::eR32Sint;
				case VertexBuffer::Attribute::UInt: return vk::Format::eR32Uint;
				case VertexBuffer::Attribute::Float: return vk::Format::eR32Sfloat;
				case VertexBuffer::Attribute::Vec2: return vk::Format::eR32G32Sfloat;
				case VertexBuffer::Attribute::Vec3: return vk::Format::eR32G32B32Sfloat;
				case VertexBuffer::Attribute::Vec4: return vk::Format::eR32G32B32A32Sfloat;
			}
			FE_ASSERT(false, "Invalid format");
			return vk::Format::eUndefined;
		}

		inline uint32_t FusionVertexAttributeToByteSize(VertexBuffer::Attribute attribute)
		{
			switch (attribute) {
				case VertexBuffer::Attribute::Bool:	return sizeof(bool);
				case VertexBuffer::Attribute::Char:	return sizeof(char);
				case VertexBuffer::Attribute::UShort: return sizeof(uint16_t);
				case VertexBuffer::Attribute::Short:	return sizeof(int16_t);
				case VertexBuffer::Attribute::Int:	return sizeof(int32_t);
				case VertexBuffer::Attribute::UInt:	return sizeof(uint32_t);
				case VertexBuffer::Attribute::Float:	return sizeof(float);
				case VertexBuffer::Attribute::Vec2:	return 2 * sizeof(float);
				case VertexBuffer::Attribute::Vec3:	return 3 * sizeof(float);
				case VertexBuffer::Attribute::Vec4:	return 4 * sizeof(float);
			}
			FE_ASSERT(false, "Invalid attribute");
			return 0;
		}

		inline vk::ShaderStageFlags FusionShaderTypeToVkShaderType(ShaderType shaderType)
		{
			switch (shaderType) {
				case ShaderType::Vertex: return vk::ShaderStageFlagBits::eVertex;
				case ShaderType::Fragment: return vk::ShaderStageFlagBits::eFragment;
				case ShaderType::Geometry: return vk::ShaderStageFlagBits::eGeometry;
				case ShaderType::Compute: return vk::ShaderStageFlagBits::eCompute;
			}

			FE_ASSERT(false, "Invalid shader stage, defaulting to vertex stage");
			return vk::ShaderStageFlagBits::eVertex;
		}

		inline vk::DescriptorType FusionDescriptorTypeToVkDescriptorType(Pipeline::DescriptorType descriptorType)
		{
			switch (descriptorType)
			{
				case Pipeline::DescriptorType::UniformBuffer: return vk::DescriptorType::eUniformBuffer;
				case Pipeline::DescriptorType::StorageBuffer: return vk::DescriptorType::eStorageBuffer;
			}

			FE_ASSERT(false, "Invalid descriptorType, defaulting to uniform buffer");
			return vk::DescriptorType::eUniformBuffer;
		}
	}
    
    namespace VulkanUtils
    {
        inline vk::Semaphore CreateSemaphore(const vk::Device device)
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

        inline vk::Fence CreateFence(const vk::Device device)
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
