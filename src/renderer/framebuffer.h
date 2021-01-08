#pragma once

#include <cstdint>
#include <glad/glad.h>

namespace kubarem {
    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };


    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        virtual ~Framebuffer();

        void Invalidate();

        virtual void Bind();
        virtual void Unbind();

        virtual void Resize(uint32_t width, uint32_t height);

        virtual uint32_t GetColorAttachmentRendererID() const { return color_attachment; }

        virtual const FramebufferSpecification& GetSpecification() const { return specification; }
    private:
        uint32_t renderer_id = 0;
        GLuint color_attachment;
        GLuint depth_attachment;
        FramebufferSpecification specification;
    };
}
