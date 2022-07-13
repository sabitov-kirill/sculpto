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
#include "render_pipeline.h"
#include "render_bridge.h"
#include "primitives/frame_buffer.h"
#include "primitives/buffer.h"
#include "primitives/texture.h"
#include "core/application/timer.h"
#include "core/resources/mesh.h"
#include "core/resources/materials/material.h"
#include "core/resources/topology/full_screen_quad.h"

#include "platform/opengl/gl.h"

scl::render_pipeline scl::renderer::Pipeline {};

void scl::renderer::DrawDepth(const shared<mesh> &Mesh, const matr4 &Transform)
{
    if (!Mesh->IsCastingShadow) return;

    Pipeline.ShadowPassShader->Bind();
    for (auto &submesh : Mesh->SubMeshes)
    {
        // matr4 world = submesh.LocalTransform * Transform;
        Pipeline.ShadowPassShader->SetMatr4("u_MatrWVP", Transform * matr4(Pipeline.LightsStorage.DirectionalLight.ViewProjection));
        render_bridge::DrawIndices(submesh.VertexArray);
    }
}

void scl::renderer::DrawGeometry(const shared<mesh> &Mesh, const matr4 &Transform)
{
    if (!Mesh->IsDrawing) return;

    for (auto &submesh : Mesh->SubMeshes)
    {
        // matr4 world = submesh.LocalTransform * Transform;
        submesh.Material->Bind();
        submesh.Material->Shader->SetMatr3("u_MatrN", matr3(Transform.Inverse().Transpose()));
        submesh.Material->Shader->SetMatr4("u_MatrW", Transform);
        submesh.Material->Shader->SetMatr4("u_MatrWVP", Transform * Pipeline.ViewProjection);
        render_bridge::DrawIndices(submesh.VertexArray);
    }
}

void scl::renderer::DrawFullscreenQuad()
{
    static shared<mesh> quad = mesh::Create(topology::full_screen_quad(), nullptr);

    bool save_is_wireframe = render_bridge::GetWireframeMode();
    render_bridge::SetWireframeMode(false);
    render_bridge::DrawIndices(quad->SubMeshes[0].VertexArray);
    render_bridge::SetWireframeMode(save_is_wireframe);
}

void scl::renderer::ApplyTexture(const shared<frame_buffer> &Destination, const shared<texture_2d> &SourceTexture)
{
    Pipeline.TextureAddShader->Bind();
    Destination->GetColorAttachment()->Bind(render_context::TEXTURE_SLOT_APPLY_SOURCE);
    SourceTexture->Bind(render_context::TEXTURE_SLOT_APPLY_TEXTURE_ADD);

    Destination->Bind();
    DrawFullscreenQuad();
    Destination->Unbind();
}

void scl::renderer::ApplyBluredTexture(const shared<frame_buffer> &Destination, const shared<texture_2d> &Source, int Iterations)
{
    Pipeline.GaussianBlurApplyShader->Bind();
    for (int i = 0; i < Iterations; i++)
    {
        if (i % 2 == 0) Pipeline.BlurFrameBuffers0->Bind();
        else            Pipeline.BlurFrameBuffers1->Bind();

        if (i == 0)          Source->Bind(render_context::TEXTURE_SLOT_BLUR_BUFFER);
        else if (i % 2 == 0) Pipeline.BlurFrameBuffers1->GetColorAttachment()->Bind(render_context::TEXTURE_SLOT_BLUR_BUFFER);
        else                 Pipeline.BlurFrameBuffers0->GetColorAttachment()->Bind(render_context::TEXTURE_SLOT_BLUR_BUFFER);

        Pipeline.GaussianBlurApplyShader->SetBool("u_IsHorizontal", i % 2);
        DrawFullscreenQuad();

        if (i % 2 == 0) Pipeline.BlurFrameBuffers0->Unbind();
        else            Pipeline.BlurFrameBuffers1->Unbind();
    }

    ApplyTexture(Destination, Iterations % 2 == 0 ? Pipeline.BlurFrameBuffers0->GetColorAttachment() : Pipeline.BlurFrameBuffers1->GetColorAttachment());
}

void scl::renderer::ComputeDepth()
{
    glCullFace(GL_FRONT);
    Pipeline.ShadowPassShader->Bind();

    Pipeline.ShadowMap->Clear();
    Pipeline.ShadowMap->Bind();
    for (const submission &subm : Pipeline.SubmissionsList)
        renderer::DrawDepth(subm.Mesh, subm.Transform);
    Pipeline.ShadowMap->Unbind();
    glCullFace(GL_BACK);
}

void scl::renderer::ComputeGeometry()
{
    Pipeline.GBuffer->Clear();
    Pipeline.GBuffer->Bind();
    for (const submission &subm : Pipeline.SubmissionsList)
        renderer::DrawGeometry(subm.Mesh, subm.Transform);
    Pipeline.GBuffer->Unbind();
}

void scl::renderer::ComputateLighting()
{
    if (Pipeline.Data.IsHDR) Pipeline.HDRFrameBuffer->Clear(),  Pipeline.HDRFrameBuffer->Bind();
    else                     Pipeline.MainFrameBuffer->Clear(), Pipeline.MainFrameBuffer->Bind();

    Pipeline.PhongLightingApplyShader->Bind();
    Pipeline.GBuffer->GetColorAttachment(render_context::COLOR_ATTACHMENT_GEOM_PASS_OUT_POSITION       )->Bind(render_context::TEXTURE_SLOT_GEOM_PASS_OUT_POSITION       );
    Pipeline.GBuffer->GetColorAttachment(render_context::COLOR_ATTACHMENT_GEOM_PASS_OUT_NORMAL         )->Bind(render_context::TEXTURE_SLOT_GEOM_PASS_OUT_NORMAL         );
    Pipeline.GBuffer->GetColorAttachment(render_context::COLOR_ATTACHMENT_GEOM_PASS_OUT_COLOR          )->Bind(render_context::TEXTURE_SLOT_GEOM_PASS_OUT_COLOR          );
    Pipeline.GBuffer->GetColorAttachment(render_context::COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_DIFFUSE  )->Bind(render_context::TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_DIFFUSE  );
    Pipeline.GBuffer->GetColorAttachment(render_context::COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SPECULAR )->Bind(render_context::TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_SPECULAR );
    Pipeline.GBuffer->GetColorAttachment(render_context::COLOR_ATTACHMENT_GEOM_PASS_OUT_PHONG_SHININESS)->Bind(render_context::TEXTURE_SLOT_GEOM_PASS_OUT_PHONG_SHININESS);

    Pipeline.DataBuffer->Update(&Pipeline.Data, sizeof(pipeline_data));
    Pipeline.DataBuffer->Bind(render_context::BINDING_POINT_SCENE_DATA);

    Pipeline.LightsStorageBuffer->Update(&Pipeline.LightsStorage, sizeof(lights_storage));
    Pipeline.LightsStorageBuffer->Bind(render_context::BINDING_POINT_LIGHTS_STORAGE);

    if (Pipeline.LightsStorage.IsDirectionalLight &&
        Pipeline.LightsStorage.DirectionalLight.IsShadows)
        Pipeline.ShadowMap->GetDepthAttachment()->Bind(render_context::TEXTURE_SLOT_SHADOW_MAP);

    DrawFullscreenQuad();

    if (Pipeline.Data.IsHDR) Pipeline.HDRFrameBuffer->Unbind();
    else                     Pipeline.MainFrameBuffer->Unbind();
}

void scl::renderer::ComputeBloom()
{
    ApplyBluredTexture(Pipeline.HDRFrameBuffer,
                       Pipeline.HDRFrameBuffer->GetColorAttachment(1),
                       Pipeline.Data.BloomAmount);
}

void scl::renderer::ComputeToneMapping()
{
    Pipeline.MainFrameBuffer->Clear(), Pipeline.MainFrameBuffer->Bind();

    Pipeline.ToneMappingApplyShader->Bind();
    Pipeline.ToneMappingApplyShader->SetFloat("u_Exposure", Pipeline.Data.Exposure);
    Pipeline.HDRFrameBuffer->GetColorAttachment()->Bind(render_context::TEXTURE_SLOT_LIGHTING_PASS_OUT_COLOR);

    DrawFullscreenQuad();

    Pipeline.MainFrameBuffer->Unbind();
}

void scl::renderer::StartPipeline(const camera &Camera, const vec3 &EnviromentAmbient)
{
    if (!Pipeline.IsInitialized) Pipeline.Initalize();

    Pipeline.Data.Time = timer::GetTime();
    Pipeline.Data.EnviromentAmbient = EnviromentAmbient;

    Pipeline.Data.ViewportWidth   = Camera.GetViewportWidth();
    Pipeline.Data.ViewportHeight  = Camera.GetViewportHeight();
    Pipeline.Data.CameraPosition  = Camera.GetPosition();
    Pipeline.Data.CameraDirection = Camera.GetDirection();
    Pipeline.ViewProjection       = Camera.GetViewProjection();
    Pipeline.Data.Exposure = Camera.Effects.Exposure;
    Pipeline.Data.IsHDR = Camera.Effects.HDR;
    Pipeline.Data.IsBloom = Camera.Effects.Bloom;
    Pipeline.Data.BloomAmount = Camera.Effects.BloomAmount;

    Pipeline.MainFrameBuffer   = Camera.GetMainFrameBuffer();
    Pipeline.GBuffer           = Camera.GetGBuffer();
    Pipeline.HDRFrameBuffer    = Camera.GetHDRFrameBuffer();
    Pipeline.BlurFrameBuffers0 = Camera.GetBlurFrameBuffers(0);
    Pipeline.BlurFrameBuffers1 = Camera.GetBlurFrameBuffers(1);
}

void scl::renderer::EndPipeline()
{
    if (Pipeline.LightsStorage.IsDirectionalLight && Pipeline.LightsStorage.DirectionalLight.IsShadows) ComputeDepth(); // Shadow pass
    ComputeGeometry();   // Geometry pass
    ComputateLighting(); // Lighting pass

    if (Pipeline.Data.IsHDR && Pipeline.Data.IsBloom) ComputeBloom(); // Bloom pass
    if (Pipeline.Data.IsHDR) ComputeToneMapping();                    // Tone mapping pass

    Pipeline.Clear();
}

void scl::renderer::SubmitPointLight(const vec3 &Position, const vec3 &Color, float Constant, float Linear, float Quadratic)
{
    if (Pipeline.LightsStorage.PointLightsCount >= render_context::LIGHTS_MAX_POINT - 1) return;

    Pipeline.LightsStorage.PointLights[Pipeline.LightsStorage.PointLightsCount].Position  = Position;
    Pipeline.LightsStorage.PointLights[Pipeline.LightsStorage.PointLightsCount].Color     = Color;
    Pipeline.LightsStorage.PointLights[Pipeline.LightsStorage.PointLightsCount].Constant  = Constant;
    Pipeline.LightsStorage.PointLights[Pipeline.LightsStorage.PointLightsCount].Linear    = Linear;
    Pipeline.LightsStorage.PointLights[Pipeline.LightsStorage.PointLightsCount].Quadratic = Quadratic;
    Pipeline.LightsStorage.PointLightsCount++;
}

void scl::renderer::SubmitDirectionalLight(const vec3 &Direction, const vec3 &Color, bool IsShadows,
                                           const matr4 &ViewProjection, const shared<frame_buffer> &ShadowMap)
{
    if (Pipeline.LightsStorage.IsDirectionalLight) return;

    Pipeline.LightsStorage.DirectionalLight.Direction = Direction;
    Pipeline.LightsStorage.DirectionalLight.Color = Color;
    if (IsShadows)
    {
        Pipeline.LightsStorage.DirectionalLight.IsShadows = IsShadows;
        Pipeline.LightsStorage.DirectionalLight.ViewProjection = ViewProjection;
        Pipeline.ShadowMap = ShadowMap;
    }
    Pipeline.LightsStorage.IsDirectionalLight = true;
}

void scl::renderer::SubmitSpotLight(const vec3 &Position, const vec3 &Direction, const vec3 &Color, float InnerCutoffCos, float OuterCutoffCos, float Epsilon)
{
    if (Pipeline.LightsStorage.SpotLightsCount >= render_context::LIGHTS_MAX_SPOT - 1) return;

    Pipeline.LightsStorage.SpotLights[Pipeline.LightsStorage.SpotLightsCount].Position = Position;
    Pipeline.LightsStorage.SpotLights[Pipeline.LightsStorage.SpotLightsCount].Direction = Direction;
    Pipeline.LightsStorage.SpotLights[Pipeline.LightsStorage.SpotLightsCount].Color = Color;
    Pipeline.LightsStorage.SpotLights[Pipeline.LightsStorage.SpotLightsCount].InnerCutoffCos = InnerCutoffCos;
    Pipeline.LightsStorage.SpotLights[Pipeline.LightsStorage.SpotLightsCount].OuterCutoffCos = OuterCutoffCos;
    Pipeline.LightsStorage.SpotLights[Pipeline.LightsStorage.SpotLightsCount].Epsilon = Epsilon;
    Pipeline.LightsStorage.SpotLightsCount++;
}

void scl::renderer::Submit(const shared<mesh> &Mesh, const vec3 &Scale, const vec3 &Angles, const vec3 &Position)
{
    matr4 transform =
        matr4::Scale(Scale) *
        matr4::RotateX(Angles.X) *
        matr4::RotateY(Angles.Y) *
        matr4::RotateZ(Angles.Z) *
        matr4::Translate(Position);
    Pipeline.SubmissionsList.emplace_back<submission>({ Mesh, transform });
}

void scl::renderer::Submit(const shared<mesh> &Mesh, const matr4 &Transform)
{
    Pipeline.SubmissionsList.emplace_back<submission>({ Mesh, Transform });
}
