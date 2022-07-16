/*****************************************************************//**
 * \file   camera.cpp
 * \brief  Renderer virtual camer class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   12 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "camera.h"
#include "core/render/primitives/frame_buffer.h"

void scl::camera::SetProjectionType(camera_projection_type ProjectionType) {
    this->ProjectionType = ProjectionType;
    InvalidateProjection();
}

void scl::camera::SetFieldOfView(float FieldOfView) {
    this->FieldOfView = FieldOfView;
    InvalidateProjection();
}

void scl::camera::SetProjectionDistance(float ProjectionDistance) {
    this->ProjectionDistance = ProjectionDistance;
    InvalidateProjection();
}

void scl::camera::SetFarClip(float FarClip) {
    this->FarClip = FarClip;
    InvalidateProjection();
}

void scl::camera::SetViewportWidth(int ViewportWidth) {
    this->ViewportWidth = ViewportWidth;
    InvalidateProjection();
}

void scl::camera::SetViewportHeight(int ViewportHeight) {
    this->ViewportHeight = ViewportHeight;
    InvalidateProjection();
}

void scl::camera::SetUpDirection(const vec3 &UpDirection) {
    this->UpDirection = UpDirection;
    RightDirection = LookDirection.Cross(UpDirection).Normalized();
    InvalidateView();
}

void scl::camera::SetDirection(const vec3 &Direction) {
    this->LookDirection = Direction;
    RightDirection = LookDirection.Cross(UpDirection).Normalized();
    InvalidateView();
}

void scl::camera::SetPosition(const vec3 &Position) {
    this->Position = Position;
    LookDirection = (Focus - Position).Normalized();
    RightDirection = LookDirection.Cross(UpDirection).Normalized();
    InvalidateView();
}

void scl::camera::SetFocus(const vec3 &Focus) {
    this->Focus = Focus;
    LookDirection = (Focus - Position).Normalized();
    RightDirection = LookDirection.Cross(UpDirection).Normalized();
    InvalidateView();
}

void scl::camera::SetView(const vec3 &Position, const vec3 &Focus, const vec3 &UpDirection)
{
    this->Position = Position;
    this->UpDirection = UpDirection;
    this->Focus = Focus;
    LookDirection = (Focus - Position).Normalized();
    RightDirection = LookDirection.Cross(UpDirection).Normalized();
    InvalidateView();
}

void scl::camera::SetRenderToSwapChain(bool IsSwapChainTarget)
{
    frame_buffer_props current_props = MainFrameBuffer->GetFrameBufferProps();
    if (current_props.IsSwapChainTarget != IsSwapChainTarget)
    {
        current_props.IsSwapChainTarget = IsSwapChainTarget;
        MainFrameBuffer->SetFrameBufferProps(current_props);
    }
}

void scl::camera::InvalidateViewProjection()
{
    ViewProjection = View * Projection;
}

void scl::camera::InvalidateProjection()
{
    float ratio_x = FieldOfView / 2, ratio_y = FieldOfView / 2;

    if (ViewportWidth >= ViewportHeight) ratio_x *= (float)ViewportWidth / ViewportHeight;
    else ratio_y *= (float)ViewportHeight / ViewportWidth;
    ViewportProjectionWidth = ratio_x * 2;
    ViewportProjectionHeight = ratio_y * 2;

    if (ProjectionType == camera_projection_type::ORTHOGRAPHIC)
        Projection = matr4::Ortho(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjectionDistance, FarClip);
    if (ProjectionType == camera_projection_type::PERSPECTIVE)
        Projection = matr4::Frustum(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjectionDistance, FarClip);
    InvalidateViewProjection();
}

void scl::camera::InvalidateView()
{
    View = matr4::View(Position, Focus, UpDirection);
    InvalidateViewProjection();
}

void scl::camera::InvalidateBuffers()
{
    ResizeMainFrameBuffer();
    ResizeGBuffer();
    ResizeHDRFrameBuffer();
    ResizeBlurFrameBuffers();
}

void scl::camera::ResizeMainFrameBuffer()
{
    frame_buffer_props current_props = MainFrameBuffer->GetFrameBufferProps();
    if (current_props.Width != ViewportWidth || current_props.Height != ViewportHeight)
    {
        current_props.Width = (int)ViewportWidth;
        current_props.Height = (int)ViewportHeight;
        MainFrameBuffer->SetFrameBufferProps(current_props);
    }
}

void scl::camera::ResizeGBuffer()
{
    frame_buffer_props current_props = GBuffer->GetFrameBufferProps();
    if (current_props.Width != ViewportWidth || current_props.Height != ViewportHeight)
    {
        current_props.Width = (int)ViewportWidth;
        current_props.Height = (int)ViewportHeight;
        GBuffer->SetFrameBufferProps(current_props);
    }
}

void scl::camera::ResizeHDRFrameBuffer()
{
    frame_buffer_props current_props = HDRFrameBuffer->GetFrameBufferProps();
    if (current_props.Width != ViewportWidth || current_props.Height != ViewportHeight)
    {
        current_props.Width = (int)ViewportWidth;
        current_props.Height = (int)ViewportHeight;
        HDRFrameBuffer->SetFrameBufferProps(current_props);
    }
}

void scl::camera::ResizeBlurFrameBuffers()
{
    for (int i = 0; i < 2; i++)
    {
        frame_buffer_props current_props = BlurFrameBuffers[i]->GetFrameBufferProps();
        if (current_props.Width != ViewportWidth || current_props.Height != ViewportHeight)
        {
            current_props.Width = (int)ViewportWidth;
            current_props.Height = (int)ViewportHeight;
            BlurFrameBuffers[i]->SetFrameBufferProps(current_props);
        }
    }
}

scl::camera::camera(camera_projection_type ProjectionType, camera_effects Effects) :
    ProjectionType(ProjectionType), Effects(Effects)
{
    MainFrameBuffer = frame_buffer::Create(frame_buffer_props { 16, 16, 1, false, 1, 0, true });
    GBuffer = frame_buffer::Create(frame_buffer_props { 16, 16, 1, false, 6, 1, true });
    HDRFrameBuffer = frame_buffer::Create(frame_buffer_props { 16, 16, 1, false, 2, 0, true });
    for (int i = 0; i < 2; i++) BlurFrameBuffers[i] = frame_buffer::Create(frame_buffer_props { 16, 16, 1, false, 1, 0, true });

    Resize(16, 16);
    SetView({ 0, 3, 10 }, vec3 { 0 }, { 0, 1, 0 });
}

scl::camera &scl::camera::Resize(int ViewportWidth, int ViewportHeight)
{
    this->ViewportWidth = ViewportWidth;
    this->ViewportHeight = ViewportHeight;

    InvalidateProjection();
    InvalidateBuffers();
    return *this;
}

scl::camera &scl::camera::Rotate(const vec3 &Axis, degrees Angle)
{
    matr4 transform =
        matr4::Translate(-Position) *
        matr4::Rotate(Axis, Angle) *
        matr4::Translate(Position);

    Focus = transform.TransformPoint(Focus);
    Position = transform.TransformPoint(Position);
    UpDirection = transform.TransformVector(UpDirection);
    LookDirection = (Focus - Position).Normalized();
    RightDirection = LookDirection.Cross(UpDirection).Normalized();

    InvalidateView();
    return *this;
}

scl::camera &scl::camera::Move(const vec3 &MoveVector)
{
    Position += MoveVector;
    Focus += MoveVector;

    InvalidateView();
    return *this;
}
