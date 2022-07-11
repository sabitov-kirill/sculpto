/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class implementation module.
 *         Implements objects rendering functions via render_pass_submission queue.
 *
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#include "sclpch.h"

#include "renderer.h"
#include "render_bridge.h"
#include "primitives/frame_buffer.h"
#include "primitives/buffer.h"
#include "primitives/texture.h"
#include "core/application/timer.h"
#include "core/resources/mesh.h"
#include "core/resources/materials/material.h"

std::vector<scl::render_pass_submission> scl::renderer::SubmissionQueue {};

scl::shared<scl::constant_buffer> scl::renderer::PipelineDataBuffer {};
scl::render_pass_data             scl::renderer::PipelineData {};
scl::matr4                        scl::renderer::PipelineViewProjection {};
scl::shared<scl::constant_buffer> scl::renderer::PipelineLightsStorageBuffer {};
scl::render_pass_lights_storage   scl::renderer::PipelineLightsStorage {};
scl::shared<scl::material>        scl::renderer::PipelineShadowCasterMaterial {};
scl::shared<scl::frame_buffer>    scl::renderer::PipelineShadowCasterFrameBuffer {};

void scl::renderer::Draw(const shared<mesh> &Mesh, const matr4 &Transform)
{
    if (!Mesh->IsDrawing) return;

    for (auto &submesh : Mesh->SubMeshes)
    {
        matr4 world = Transform;

        submesh.Material->Bind();
        submesh.Material->Shader->SetMatr3("u_MatrN", matr3(world.Inverse().Transpose()));
        submesh.Material->Shader->SetMatr4("u_MatrW", world);
        submesh.Material->Shader->SetMatr4("u_MatrWVP", world * PipelineViewProjection);
        render_bridge::DrawIndices(submesh.VertexArray);
    }
}

void scl::renderer::DrawShadowPass(const shared<mesh> &Mesh, const matr4 &Transform)
{
    if (!Mesh->IsCastingShadow) return;

    PipelineShadowCasterMaterial->Bind();
    for (auto &submesh : Mesh->SubMeshes)
    {
        matr4 world = submesh.LocalTransform * Transform;
        
        PipelineShadowCasterMaterial->Shader->SetMatr4("u_MatrWVP", world * matr4(PipelineLightsStorage.DirectionalLight.ViewProjection));
        render_bridge::DrawIndices(submesh.VertexArray);
    }
}

void scl::renderer::StartPipeline(const matr4 &ViewProjection,
                                  const vec3 &CameraDirection, const vec3 &CameraPosition,
                                  const int ViewportWidth, const int ViewportHeight,
                                  const vec3 &EnviromentAmbient, bool IsBloomActive)
{
    PipelineViewProjection = ViewProjection;
    PipelineData.CameraPosition = CameraPosition;
    PipelineData.CameraDirection = CameraDirection;
    PipelineData.ViewportWidth = ViewportWidth;
    PipelineData.ViewportHeight = ViewportHeight;
    PipelineData.EnviromentAmbient = EnviromentAmbient;
    PipelineData.IsBloomActive = IsBloomActive;
    PipelineData.Time = timer::GetTime();
}

void scl::renderer::StartPipeline() {}

void scl::renderer::EndPipeline(const shared<frame_buffer> &MainColorPassFrameBuffer)
{
    if (PipelineLightsStorage.IsDirectionalLight && PipelineLightsStorage.DirectionalLight.IsShadows && PipelineShadowCasterFrameBuffer)
    {
        if (!PipelineShadowCasterMaterial) PipelineShadowCasterMaterial = material::Create(render_bridge::GetShadowPassShader());

        // Shadow pass. Render just all geometery.
        PipelineShadowCasterFrameBuffer->Clear();
        PipelineShadowCasterFrameBuffer->Bind();
        for (const render_pass_submission &subm : SubmissionQueue)
            renderer::DrawShadowPass(subm.Mesh, subm.Transform);
        PipelineShadowCasterFrameBuffer->Unbind();
    }

    // Updateing data, independent by concrete mesh
    if (!PipelineDataBuffer) PipelineDataBuffer = constant_buffer::Create(sizeof(render_pass_data));
    PipelineDataBuffer->Update(&PipelineData, sizeof(render_pass_data));
    PipelineDataBuffer->Bind(render_context::BINDING_POINT_SCENE_DATA);

    // Updating light storage, which is need in main color pass
    if (!PipelineLightsStorageBuffer) PipelineLightsStorageBuffer = scl::constant_buffer::Create(sizeof(scl::render_pass_lights_storage));
    PipelineLightsStorageBuffer->Update(&PipelineLightsStorage, sizeof(render_pass_lights_storage));
    PipelineLightsStorageBuffer->Bind(render_context::BINDING_POINT_LIGHTS_STORAGE);

    // Main color pass.
    MainColorPassFrameBuffer->Clear();
    MainColorPassFrameBuffer->Bind();
    if (PipelineShadowCasterFrameBuffer) PipelineShadowCasterFrameBuffer->GetDepthAttachment()->Bind(render_context::TEXTURE_SLOT_SHADOW_MAP);
    for (const render_pass_submission &subm : SubmissionQueue)
        renderer::Draw(subm.Mesh, subm.Transform);
    MainColorPassFrameBuffer->Unbind();

    // Clear render pass 
    memset(&PipelineData, 0, sizeof(render_pass_data));
    memset(&PipelineLightsStorage, 0, sizeof(render_pass_lights_storage));
    SubmissionQueue.clear();
}

void scl::renderer::SubmitPointLight(const vec3 &Position, const vec3 &Color, float Constant, float Linear, float Quadratic)
{
    if (PipelineLightsStorage.PointLightsCount >= render_context::LIGHTS_MAX_POINT - 1) return;

    PipelineLightsStorage.PointLights[PipelineLightsStorage.PointLightsCount].Position  = Position;
    PipelineLightsStorage.PointLights[PipelineLightsStorage.PointLightsCount].Color     = Color;
    PipelineLightsStorage.PointLights[PipelineLightsStorage.PointLightsCount].Constant  = Constant;
    PipelineLightsStorage.PointLights[PipelineLightsStorage.PointLightsCount].Linear    = Linear;
    PipelineLightsStorage.PointLights[PipelineLightsStorage.PointLightsCount].Quadratic = Quadratic;
    PipelineLightsStorage.PointLightsCount++;
}

void scl::renderer::SubmitDirectionalLight(const vec3 &Direction, const vec3 &Color, bool IsShadows,
                                           const matr4 &ViewProjection, shared<frame_buffer> FrameBuffer)
{
    if (PipelineLightsStorage.IsDirectionalLight) return;

    PipelineLightsStorage.DirectionalLight.Direction = Direction;
    PipelineLightsStorage.DirectionalLight.Color = Color;
    if (IsShadows)
    {
        PipelineLightsStorage.DirectionalLight.IsShadows = IsShadows;
        PipelineLightsStorage.DirectionalLight.ViewProjection = ViewProjection;
        PipelineShadowCasterFrameBuffer = FrameBuffer;
    }
    PipelineLightsStorage.IsDirectionalLight = true;
}

void scl::renderer::SubmitSpotLight(const vec3 &Position, const vec3 &Direction, const vec3 &Color, float InnerCutoffCos, float OuterCutoffCos, float Epsilon)
{
    if (PipelineLightsStorage.SpotLightsCount >= render_context::LIGHTS_MAX_SPOT - 1) return;

    PipelineLightsStorage.SpotLights[PipelineLightsStorage.SpotLightsCount].Position = Position;
    PipelineLightsStorage.SpotLights[PipelineLightsStorage.SpotLightsCount].Direction = Direction;
    PipelineLightsStorage.SpotLights[PipelineLightsStorage.SpotLightsCount].Color = Color;
    PipelineLightsStorage.SpotLights[PipelineLightsStorage.SpotLightsCount].InnerCutoffCos = InnerCutoffCos;
    PipelineLightsStorage.SpotLights[PipelineLightsStorage.SpotLightsCount].OuterCutoffCos = OuterCutoffCos;
    PipelineLightsStorage.SpotLights[PipelineLightsStorage.SpotLightsCount].Epsilon = Epsilon;
    PipelineLightsStorage.SpotLightsCount++;
}

void scl::renderer::Submit(const shared<mesh> &Mesh, const vec3 &Scale, const vec3 &Angles, const vec3 &Position)
{
    matr4 transform =
        matr4::Scale(Scale) *
        matr4::RotateX(Angles.X) *
        matr4::RotateY(Angles.Y) *
        matr4::RotateZ(Angles.Z) *
        matr4::Translate(Position);
    SubmissionQueue.emplace_back<render_pass_submission>({ Mesh, transform });
}

void scl::renderer::Submit(const shared<mesh> &Mesh, const matr4 &Transform)
{
    SubmissionQueue.emplace_back<render_pass_submission>({ Mesh, Transform });
}
