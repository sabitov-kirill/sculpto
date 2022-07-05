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
#include "material/material.h"

std::vector<scl::submission> scl::renderer::SubmissionQueue {};
const scl::renderer_camera *scl::renderer::SceneCamera {};

void scl::renderer::Draw(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform, const renderer_camera &Camera)
{
    Material->Bind();
    Material->Shader->SetMatr3("u_MatrN", matr3(Transform.Inverse().Transpose()));
    Material->Shader->SetMatr4("u_MatrW", Transform);
    Material->Shader->SetMatr4("u_MatrWVP", Transform * Camera.GetViewProjection());
    Material->Shader->SetFloat3("u_CameraPosition", Camera.GetPosition());
    Mesh->GetVertexArray()->Bind();
    render_bridge::DrawIndices(Mesh->GetVertexArray());
}

void scl::renderer::Submit(const shared<mesh> &Mesh, const shared<material> &Material,
                           const vec3 &Scale, const vec3 &Angles, const vec3 &Position)
{
    if (SceneCamera == nullptr) return;

    matr4 transform =
        matr4::Scale(Scale) *
        matr4::RotateX(Angles.X) *
        matr4::RotateY(Angles.Y) *
        matr4::RotateZ(Angles.Z) *
        matr4::Translate(Position);
    SubmissionQueue.emplace_back<submission>({ Mesh, Material, transform, *SceneCamera });
}

void scl::renderer::Submit(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform)
{
    if (SceneCamera == nullptr) return;

    //Draw(Mesh, Material, Transform, *SceneCamera);
    SubmissionQueue.emplace_back<submission>({ Mesh, Material, Transform, *SceneCamera });
}

void scl::renderer::Flush(const shared<frame_buffer> &FrameBuffer)
{
    FrameBuffer->Bind();
    for (const submission &subm: SubmissionQueue)
        renderer::Draw(subm.Mesh, subm.Material, subm.Transform, subm.Camera);
    FrameBuffer->Unbind();

    SubmissionQueue.clear();
}

void scl::renderer::FlushToDefaultFrameBuffer()
{
    for (const submission &subm : SubmissionQueue)
        renderer::Draw(subm.Mesh, subm.Material, subm.Transform, subm.Camera);

    SubmissionQueue.clear();
}

void scl::renderer::SubmitCamera(const renderer_camera &Camera)
{
    // TODO: Copy frame buffers and matrices from camera to pipeline ar somthing like that.
    SceneCamera = &Camera;
}
