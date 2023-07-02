#pragma once

namespace FusionEngine
{
    enum class ImageTextureFilter
    {
        Nearest = 0,
        Linear = 1,
    };
    
    enum class ImageFormat
    {
        None = 0,
        R8G8B8A8,
        R32G32B32A32,
        Depth24Stencil8,
        Depth32F,
        Depth32FStencil8
    };

    enum class ImageUsage
    {
        ColorAttachment,
        DepthAttachment,
        DepthStencilAttachment,
        Texture,
        Storage
    };

    struct ImageSpecification
    {
        ImageTextureFilter Filter = ImageTextureFilter::Linear;
        ImageFormat Format = ImageFormat::R8G8B8A8;
        ImageUsage Usage = ImageUsage::ColorAttachment;
        uint32_t Width = 1, Height = 1, Mips = 1, Layers = 1, Samples = 1;
    };
    
    class Image
    {
    public:
        static Ref<Image> Create(const ImageSpecification& spec);
        virtual ~Image() = default;

        virtual void OnResize(uint32_t width, uint32_t height) = 0;
        [[nodiscard]] virtual void* GetImGuiHandle() = 0;

        [[nodiscard]] ImageFormat GetFormat() const { return m_Spec.Format; }
        [[nodiscard]] ImageTextureFilter GetImageTextureFilter() const { return m_Spec.Filter; }
    protected:
        ImageSpecification m_Spec;
    };
}
