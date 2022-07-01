/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class implementation module.
 *         Implements objects rendering functions via submission queue.
 *
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "renderer.h"
#include "render_bridge.h"
#include "mesh.h"
#include "primitives/frame_buffer.h"
#include "primitives/buffer.h"

std::vector<scl::submission> scl::renderer::SubmissionQueue {};
const scl::camera *scl::renderer::SceneCamera {};

void scl::renderer::Draw(const shared<mesh> &Mesh, const camera &Camera)
{
    Mesh->GetMaterial()->Bind();
    Mesh->GetMaterial()->GetShader()->SetMatr4("MatrW", Mesh->GetTransform());
    Mesh->GetMaterial()->GetShader()->SetMatr4("MatrWVP", Mesh->GetTransform() * Camera.VP);
    Mesh->GetVertexArray()->Bind();
    render_bridge::DrawIndices(Mesh->GetVertexArray());
}

void scl::renderer::Submit(const shared<mesh> &Mesh)
{
    SubmissionQueue.emplace_back<submission>({ Mesh, *SceneCamera });
}

void scl::renderer::Flush(const shared<frame_buffer> &FrameBuffer)
{
    FrameBuffer->Bind();
    for (const auto &submission : SubmissionQueue)
        renderer::Draw(submission.Mesh, submission.Camera);
    FrameBuffer->Unbind();

    SubmissionQueue.clear();
}

void scl::renderer::FlushToDefaultFrameBuffer()
{
    for (const auto &submission : SubmissionQueue)
        renderer::Draw(submission.Mesh, submission.Camera);
}

void scl::renderer::BeginScene(const camera &Camera)
{
    SceneCamera = &Camera;
}

void scl::renderer::EndScene(bool ShouldFlush)
{
    if (ShouldFlush) renderer::FlushToDefaultFrameBuffer();
}
