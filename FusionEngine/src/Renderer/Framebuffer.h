#pragma once
#include "Image.h"

#include <glm/glm.hpp>



namespace FusionEngine
{
    class Framebuffer;
    
    enum class FrameBufferLoadOp
    {
        Load,
        Clear,
        DontCare
    };

    enum class FramebufferStoreOp
    {
        Store,
        DontCare
    };
    
    struct FrameBufferAttachment
    {
        FrameBufferLoadOp LoadOp = FrameBufferLoadOp::Clear;
        FramebufferStoreOp StoreOp = FramebufferStoreOp::Store;
        Shared<Image> Image;
    };
    
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        glm::vec4 ClearColor;
        float DepthClearValue = 0.0f;
        uint32_t Samples = 1;
        std::vector<FrameBufferAttachment> Attachments;
        Shared<Framebuffer> ExistingFramebuffer;
        uint32_t FramesInFlight = 1;
    };
    
    class Framebuffer
    {
    public:
        static Shared<Framebuffer> Create(const FramebufferSpecification& spec);

        virtual ~Framebuffer() = default;

        virtual void OnResize(uint32_t with, uint32_t height) = 0;
        virtual void Begin() = 0;
        virtual void End() = 0;
        [[nodiscard]] virtual Shared<Image> GetCurrentImage() const = 0;

        [[nodiscard]] Shared<Image> GetImage(const uint32_t index = 0) const { return m_Spec.Attachments[index].Image; }
        [[nodiscard]] FramebufferSpecification GetSpecification() const { return  m_Spec; }
        [[nodiscard]] uint32_t GetWidth() const { return m_Spec.Width; }
        [[nodiscard]] uint32_t GetHeight() const { return m_Spec.Height; }
    protected:
        FramebufferSpecification m_Spec = {};
    };
}
