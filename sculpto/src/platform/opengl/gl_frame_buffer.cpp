#include "sclpch.h"
#include "gl_frame_buffer.h"

void scl::gl_frame_buffer::SetFrameBufferProps(const frame_buffer_props &Props)
{
    this->Props = Props;
    this->Invalidate();
}

const scl::frame_buffer_props &scl::gl_frame_buffer::GetFrameBufferProps() const
{
    return Props;
}

scl::u32 scl::gl_frame_buffer::GetColorAttachmentInnerId() const
{
    return ColorAttachmentId;
}

void scl::gl_frame_buffer::Invalidate()
{
    this->Free();

    if (Props.SwapChainTarget)
    {
        Id = 0;
        ColorAttachmentId = 0;
        DepthAttachmentId = 0;
        return;
    }

    // Create and bind frame buffer
    glCreateFramebuffers(1, &Id);
    glBindFramebuffer(GL_FRAMEBUFFER, Id);

    // Frame buffer color attachment creation
    glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachmentId);
    glBindTexture(GL_TEXTURE_2D, ColorAttachmentId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Props.Width, Props.Height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachmentId, 0);

    // Frame buffer depth attachment creation
    glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachmentId);
    glBindTexture(GL_TEXTURE_2D, DepthAttachmentId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Props.Width, Props.Height,
                 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachmentId, 0);

    // Check frame buffer status, unbinding
    SCL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer creation error!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

scl::gl_frame_buffer::gl_frame_buffer(const frame_buffer_props &Props) :
    Props(Props)
{
    this->Invalidate();
}

scl::gl_frame_buffer::~gl_frame_buffer()
{
    this->Free();
}

void scl::gl_frame_buffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, Id);
    glViewport(0, 0, Props.Width, Props.Height);
}

void scl::gl_frame_buffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void scl::gl_frame_buffer::Free()
{
    if (Id != 0) glDeleteFramebuffers(1, &Id);
    if (ColorAttachmentId != 0) glDeleteTextures(1, &ColorAttachmentId);
    if (DepthAttachmentId != 0)glDeleteTextures(1, &DepthAttachmentId);
}

void scl::gl_frame_buffer::Clear()
{
    this->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->Unbind();
}
