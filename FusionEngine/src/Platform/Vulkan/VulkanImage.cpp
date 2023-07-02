#include "fepch.h"
#include "VulkanImage.h"

#include "VulkanRenderApi.h"
#include "VulkanDevice.h"
#include "Renderer/RenderCommand.h"
#include "UI/ImGui.h"

namespace FusionEngine
{
    VulkanImage::VulkanImage(const ImageSpecification& spec)
    {
        m_Spec = spec;
        m_RenderApi = std::dynamic_pointer_cast<VulkanRenderApi>(RenderCommand::GetRenderApi());
        m_Width = spec.Width;
        m_Height = spec.Height;

        CreateVulkanObjects();
    }

    VulkanImage::~VulkanImage()
    {
        m_RenderApi->m_Device->Logical().freeMemory(m_Memory);
        m_RenderApi->m_Device->Logical().destroySampler(m_Sampler);
        m_RenderApi->m_Device->Logical().destroyImageView(m_ImageView);
        m_RenderApi->m_Device->Logical().destroyImage(m_Image);
    }

    void VulkanImage::CreateVulkanObjects()
    {
        // Image
        
        vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eSampled;
        if(m_Spec.Usage == ImageUsage::ColorAttachment)
            usage |= vk::ImageUsageFlagBits::eColorAttachment;
        else if(m_Spec.Usage == ImageUsage::DepthAttachment)
            usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        else if(m_Spec.Usage == ImageUsage::DepthStencilAttachment)
            usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
        else if(m_Spec.Usage == ImageUsage::Storage)
            usage |= vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eTransferDst;
        else if(m_Spec.Usage == ImageUsage::Texture)
            usage |= vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst;

        vk::ImageAspectFlags aspectFlags = vk::ImageAspectFlagBits::eColor;
        if(m_Spec.Usage == ImageUsage::DepthAttachment)
            aspectFlags = vk::ImageAspectFlagBits::eDepth;
        else if(m_Spec.Usage == ImageUsage::DepthStencilAttachment)
            aspectFlags = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;

        vk::ImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.imageType = vk::ImageType::e2D;
        imageCreateInfo.format = GetVulkanFormat();
        imageCreateInfo.extent.width = m_Width;
        imageCreateInfo.extent.height = m_Height;
        imageCreateInfo.extent.depth = 1;
        imageCreateInfo.mipLevels = m_Spec.Mips;
        imageCreateInfo.arrayLayers = m_Spec.Layers;
        imageCreateInfo.samples = static_cast<vk::SampleCountFlagBits>(m_Spec.Samples);
        imageCreateInfo.tiling = vk::ImageTiling::eOptimal;
        imageCreateInfo.usage = usage;

        m_Image = m_RenderApi->m_Device->Logical().createImage(imageCreateInfo);

        // Find Image Memory

        vk::MemoryRequirements memoryRequirements = m_RenderApi->m_Device->Logical().getImageMemoryRequirements(m_Image);
        int32_t memoryTypeIndex = -1;

        const vk::PhysicalDeviceMemoryProperties memoryProperties = m_RenderApi->m_Device->Physical().getMemoryProperties();
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
        {
            if(memoryRequirements.memoryTypeBits & (1 << i))
            {
                memoryTypeIndex = i;
                break;
            }
        }
        FE_ASSERT(memoryTypeIndex > -1, "Couldnt find any suatable graphics memory!");

        // Allocate and bind Image memory
        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = memoryTypeIndex;
        
        m_Memory = m_RenderApi->m_Device->Logical().allocateMemory(allocateInfo);
        try {
            m_RenderApi->m_Device->Logical().bindImageMemory(m_Image, m_Memory, 0);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Binding image memory failed");
        }

         // ImageView

        vk::ImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.image = m_Image;
        imageViewCreateInfo.viewType = m_Spec.Layers > 1 ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D;
        imageViewCreateInfo.format = GetVulkanFormat();
        imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = m_Spec.Mips;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = m_Spec.Layers;

        try {
            m_ImageView = m_RenderApi->m_Device->Logical().createImageView(imageViewCreateInfo);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating ImageView failed");
        }

        // Sampler

        vk::SamplerCreateInfo samplerCreateInfo = {};
        samplerCreateInfo.magFilter = m_Spec.Filter == ImageTextureFilter::Linear ? vk::Filter::eLinear : vk::Filter::eNearest;
        samplerCreateInfo.minFilter = m_Spec.Filter == ImageTextureFilter::Linear ? vk::Filter::eLinear : vk::Filter::eNearest;
        samplerCreateInfo.mipmapMode = m_Spec.Filter == ImageTextureFilter::Linear ? vk::SamplerMipmapMode::eLinear : vk::SamplerMipmapMode::eNearest;
        samplerCreateInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.compareOp = vk::CompareOp::eNever;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = 100.0f;
        samplerCreateInfo.maxAnisotropy = 1.0f;
        samplerCreateInfo.anisotropyEnable = VK_FALSE;
        samplerCreateInfo.borderColor = vk::BorderColor::eIntOpaqueWhite;

        try {
            m_Sampler = m_RenderApi->m_Device->Logical().createSampler(samplerCreateInfo);
        }
        catch (vk::SystemError& err)
        {
            FE_ERROR("VulkanException {0}: {1}", err.code(), err.what());
            FE_ASSERT(false, "Creating Sampler failed");
        }

        // ImageDescriptor

        if(m_Spec.Usage == ImageUsage::DepthAttachment || m_Spec.Usage == ImageUsage::DepthStencilAttachment)
            m_ImageInfo.imageLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
        else if (m_Spec.Usage == ImageUsage::ColorAttachment)
            m_ImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        else
            m_ImageInfo.imageLayout = vk::ImageLayout::eGeneral;
        
        m_ImageInfo.imageView = m_ImageView;
        m_ImageInfo.sampler = m_Sampler;
    }

    void VulkanImage::OnResize(uint32_t width, uint32_t height)
    {
        if(m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        m_RenderApi->WaitDeviceIdle();
        if(m_ImGuiHandle != nullptr)
        {
            UI::ImGuiFreeImageHandle(m_ImGuiHandle);
            m_ImGuiHandle = nullptr;
        }
        m_RenderApi->m_Device->Logical().freeMemory(m_Memory);
        m_RenderApi->m_Device->Logical().destroySampler(m_Sampler);
        m_RenderApi->m_Device->Logical().destroyImageView(m_ImageView);
        m_RenderApi->m_Device->Logical().destroyImage(m_Image);

        CreateVulkanObjects();
    }

    void* VulkanImage::GetImGuiHandle()
    {
        if(m_ImGuiHandle == nullptr)
        {
            m_ImGuiHandle = UI::ImGuiCreateImageHandle(m_ImageInfo);
        }

        return m_ImGuiHandle;
    }

    VkImageLayout VulkanImage::GetVulkanImageLayout() const
    {
        if(m_Spec.Usage == ImageUsage::ColorAttachment)
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        if(m_Spec.Usage == ImageUsage::DepthAttachment)
            return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        if(m_Spec.Usage == ImageUsage::DepthStencilAttachment)
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        if(m_Spec.Usage == ImageUsage::Storage)
            return VK_IMAGE_LAYOUT_GENERAL;
        if(m_Spec.Usage == ImageUsage::Texture)
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        return VK_IMAGE_LAYOUT_UNDEFINED;
    }

    vk::Format VulkanImage::GetVulkanFormat() const
    {
        switch (m_Spec.Format) {
            case ImageFormat::None: return vk::Format::eUndefined;
            case ImageFormat::R8G8B8A8: return vk::Format::eB8G8R8A8Unorm;
            case ImageFormat::R32G32B32A32: return vk::Format::eR32G32B32A32Uint;
            case ImageFormat::Depth24Stencil8: return vk::Format::eD24UnormS8Uint;
            case ImageFormat::Depth32F: return vk::Format::eD32Sfloat;
            case ImageFormat::Depth32FStencil8: return vk::Format::eD32SfloatS8Uint;
            default: return vk::Format::eUndefined;
        }
    }
}
