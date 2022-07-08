/*****************************************************************//**
 * \file   gl_frame_buffer.cpp
 * \brief  OpenGL frame buffer class implementation class.
 * 
 * \author Sabitov Kirill
 * \date   06 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "gl_frame_buffer.h"
#include "gl_texture.h"

void scl::gl_frame_buffer::SetFrameBufferProps(const frame_buffer_props &Props)
{
    this->Props = Props;
    this->Invalidate();
}

const scl::frame_buffer_props &scl::gl_frame_buffer::GetFrameBufferProps() const
{
    return Props;
}

const scl::shared<scl::texture_2d> &scl::gl_frame_buffer::GetColorAttachment(int Index) const
{
    return ColorAttachments[Index];
}

const scl::shared<scl::texture_2d> &scl::gl_frame_buffer::GetDepthAttachment(int Index) const
{
    return DepthAttachment;
}

void scl::gl_frame_buffer::Invalidate()
{
    this->Free();

    if (Props.SwapChainTarget) { Id = 0; ClearConfig = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT; return; }

    // Create and bind frame buffer
    glGenFramebuffers(1, &Id);
    glBindFramebuffer(GL_FRAMEBUFFER, Id);

    // Props validation
    SCL_CORE_ASSERT(Props.ColorAttachmentsCount > 0 || Props.DepthAttachmentsCount > 0, "At least one of attachments count must be > 0.");
    SCL_CORE_ASSERT(Props.DepthAttachmentsCount <= 8, "OpenGL support max 8 collor attachments per frame buffer.");
    SCL_CORE_ASSERT(Props.DepthAttachmentsCount <= 1, "OpenGL support max 1 depth attachment per frame buffer.");

    // Frame buffer color and depth attachments creation
    image viewport(Props.Width, Props.Height, 4);
    ColorAttachments.resize(Props.ColorAttachmentsCount);
    for (int i = 0; i < Props.ColorAttachmentsCount; i++) {
        ColorAttachments[i] = texture_2d::Create(viewport, texture_2d_type::COLOR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachments[i]->GetHandle(), 0);
    } if (Props.ColorAttachmentsCount == 0) {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    } if (Props.DepthAttachmentsCount == 1) {
        DepthAttachment = texture_2d::Create(viewport, texture_2d_type::DEPTH);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment->GetHandle(), 0);
    }

    // Set frame buffer clear config depending on attachments count
    ClearConfig = ((Props.ColorAttachmentsCount > 0) * GL_COLOR_BUFFER_BIT) |
                  ((Props.DepthAttachmentsCount == 1) * GL_DEPTH_BUFFER_BIT);

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

void scl::gl_frame_buffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, Id);
    glViewport(0, 0, Props.Width, Props.Height);
}

void scl::gl_frame_buffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void scl::gl_frame_buffer::Free()
{
    if (Id != 0) glDeleteFramebuffers(1, &Id), Id = 0;

    for (auto &color_attachment : ColorAttachments) color_attachment->Free();
    ColorAttachments.clear();

    if (DepthAttachment) DepthAttachment->Free();
}

void scl::gl_frame_buffer::Clear()
{
    this->Bind();
    glClear(ClearConfig);
    this->Unbind();
}
