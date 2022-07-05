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
scl::lights_storage scl::renderer::SceneLightsStorage {};
scl::shared<scl::constant_buffer> scl::renderer::SceneLightsStorageBuffer {};

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

void scl::renderer::SubmitCamera(const renderer_camera &Camera)
{
    // TODO: Copy frame buffers and matrices from camera to pipeline ar somthing like that.
    SceneCamera = &Camera;
}

void scl::renderer::SubmitPointLight(const vec3 &Position, const vec3 &Color, float Constant, float Linear, float Quadratic)
{
    if (SceneLightsStorage.PointLightsCount >= render_context::LIGHTS_MAX_POINT - 1) return;

    SceneLightsStorage.PointLights[SceneLightsStorage.PointLightsCount].Position  = Position;
    SceneLightsStorage.PointLights[SceneLightsStorage.PointLightsCount].Color     = Color;
    SceneLightsStorage.PointLights[SceneLightsStorage.PointLightsCount].Constant  = Constant;
    SceneLightsStorage.PointLights[SceneLightsStorage.PointLightsCount].Linear    = Linear;
    SceneLightsStorage.PointLights[SceneLightsStorage.PointLightsCount].Quadratic = Quadratic;
    SceneLightsStorage.PointLightsCount++;
}

void scl::renderer::SubmitDirectionalLight(const vec3 &Direction, const vec3 &Color)
{
    if (SceneLightsStorage.IsDirectionalLight) return;

    SceneLightsStorage.DirectionalLight.Direction = Direction;
    SceneLightsStorage.DirectionalLight.Color = Color;
    SceneLightsStorage.IsDirectionalLight = true;
}

void scl::renderer::SubmitSpotLight(const vec3 &Position, const vec3 &Direction, const vec3 &Color, float InnerCutoffCos, float OuterCutoffCos, float Epsilon)
{
    if (SceneLightsStorage.SpotLightsCount >= render_context::LIGHTS_MAX_SPOT - 1) return;

    SceneLightsStorage.SpotLights[SceneLightsStorage.SpotLightsCount].Position = Position;
    SceneLightsStorage.SpotLights[SceneLightsStorage.SpotLightsCount].Direction = Direction;
    SceneLightsStorage.SpotLights[SceneLightsStorage.SpotLightsCount].Color = Color;
    SceneLightsStorage.SpotLights[SceneLightsStorage.SpotLightsCount].InnerCutoffCos = InnerCutoffCos;
    SceneLightsStorage.SpotLights[SceneLightsStorage.SpotLightsCount].OuterCutoffCos = OuterCutoffCos;
    SceneLightsStorage.SpotLights[SceneLightsStorage.SpotLightsCount].Epsilon = Epsilon;
    SceneLightsStorage.SpotLightsCount++;
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
    if (!SceneLightsStorageBuffer)
        SceneLightsStorageBuffer = 
        scl::constant_buffer::Create(scl::render_context::BINDING_POINT_LIGHTS_STORAGE, sizeof(scl::lights_storage));
    SceneLightsStorageBuffer->Update(&SceneLightsStorage, sizeof(lights_storage));
    SceneLightsStorageBuffer->Bind();

    FrameBuffer->Bind();
    for (const submission &subm: SubmissionQueue)
        renderer::Draw(subm.Mesh, subm.Material, subm.Transform, subm.Camera);
    FrameBuffer->Unbind();

    SubmissionQueue.clear();
    memset(&SceneLightsStorage, 0, sizeof(lights_storage));
}

void scl::renderer::FlushToDefaultFrameBuffer()
{
    if (!SceneLightsStorageBuffer)
        SceneLightsStorageBuffer =
        scl::constant_buffer::Create(scl::render_context::BINDING_POINT_LIGHTS_STORAGE, sizeof(scl::lights_storage));
    SceneLightsStorageBuffer->Update(&SceneLightsStorage, sizeof(lights_storage));
    SceneLightsStorageBuffer->Bind();

    for (const submission &subm : SubmissionQueue)
        renderer::Draw(subm.Mesh, subm.Material, subm.Transform, subm.Camera);

    SubmissionQueue.clear();
    memset(&SceneLightsStorage, 0, sizeof(lights_storage));
}
