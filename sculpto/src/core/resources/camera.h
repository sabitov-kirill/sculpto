/*****************************************************************//**
 * \file   camera.h
 * \brief  Renderer virtual camer class definiton module.
 * 
 * \author Sabitov Kirill
 * \date   02 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    enum class camera_projection_type
    {
        ORTHOGRAPHIC,
        PERSPECTIVE,
    };

    /* Renderer virtual camer class. */
    class camera
    {
    private: /* Camera data. */
        /* Camer matrices. */
        matr4 Projection {};
        matr4 View {};
        matr4 ViewProjection {};

        /* Camera projection data. */
        camera_projection_type ProjectionType;
        float FieldOfView { 0.1f };
        float ProjectionDistance { 0.1f };
        float FarClip { 1000.0f };
        float ProjectionWidth {};
        float ProjectionHeight {};

        /* Camera view data. */
        vec3 UpDirection {};
        vec3 LookDirection {};
        vec3 RightDirection {};
        vec3 Position {};
        vec3 Focus {};

    public: /* Camer getter/setter functions. */
        /* Camera projection matrix getter function. */
        const matr4 &GetProjection() const { return Projection; }
        /* Camera view matrix getter function. */
        const matr4 &GetView() const { return View; }
        /* Camera view projection matrix getter function. */
        const matr4 &GetViewProjection() const { return ViewProjection; }

        /* Camera projection type getter function. */
        camera_projection_type GetProjectionType() const { return ProjectionType; }
        /* Field of view getter function. */
        float GetFieldOfView() const { return FieldOfView; }
        /* Distance to near clip plane getter function. */
        float GetProjectionDistance() const { return ProjectionDistance; };
        /* Distance to far clip plane getter function. */
        float GetFarClip() const { return FarClip; };
        /* Projection plane width in pixels getter function. */
        float GetProjectionWidth() const { return ProjectionWidth; };
        /* Projection plane height in pixels getter function. */
        float GetProjectionHeight() const { return ProjectionHeight; };

        /* Camera up direction getter function. */
        const vec3 &GetUpDirection() const { return UpDirection; }
        /* Camera forward direction getter function. */
        const vec3 &GetDirection() const { return LookDirection; }
        /* Camera right direction getter function. */
        const vec3 &GetRightDirection() const { return RightDirection; }
        /* Camera postion gertter function. */
        const vec3 &GetPosition() const { return Position; }
        /* Camera focus point getter function. */
        const vec3 &GetFocus() const { return Focus; }

        /* Camera projection type setter function. */
        void SetProjectionType(camera_projection_type ProjectionType) {
            this->ProjectionType = ProjectionType;
            InvalidateProjection();
        }
        /* Field of view getter function. */
        void SetFieldOfView() {
            this->FieldOfView = FieldOfView;
            InvalidateProjection();
        }
        /* Distance to near clip plane getter function. */
        void SetProjectionDistance(float ProjectionDistance) {
            this->ProjectionDistance = ProjectionDistance;
            InvalidateProjection();
        }
        /* Distance to far clip plane getter function. */
        void SetFarClip(float FarClip) {
            this->FarClip = FarClip;
            InvalidateProjection();
        }
        /* Projection plane width in pixels getter function. */
        void SetProjectionWidth(float ProjectionWidth) {
            this->ProjectionWidth = ProjectionWidth;
            InvalidateProjection();
        }
        /* Projection plane height in pixels getter function. */
        void SetProjectionHeight(float ProjectionHeight) {
            this->ProjectionHeight = ProjectionHeight;
            InvalidateProjection();
        }

        /* Camera up direction setter function. */
        void SetUpDirection(const vec3 &UpDirection) {
            this->UpDirection = UpDirection;
            RightDirection = LookDirection.Cross(UpDirection).Normalized();
            InvalidateView();
        }
        /* Camera forward direction setter function. */
        void SetDirection(const vec3 &Direction) {
            this->LookDirection = Direction;
            RightDirection = LookDirection.Cross(UpDirection).Normalized();
            InvalidateView();
        }
        /* Camer position setter function. */
        void SetPosition(const vec3 &Position) {
            this->Position = Position;
            LookDirection = (Focus - Position).Normalized();
            RightDirection = LookDirection.Cross(UpDirection).Normalized();
            InvalidateView();
        }
        /* Camer focus point setter function. */
        void SetFocus(const vec3 &Focus) {
            this->Focus = Focus;
            LookDirection = (Focus - Position).Normalized();
            RightDirection = LookDirection.Cross(UpDirection).Normalized();
            InvalidateView();
        }

        /**
         * Set view matrix by camera translation parametrs.
         * 
         * \param Position - camera position.
         * \param Focus - camera focusing point.
         * \param UpDirection - camera up direction.
         */
        void SetView(const vec3 &Position, const vec3 &Focus, const vec3 &UpDirection)
        {
            this->Position = Position;
            this->UpDirection = UpDirection;
            this->Focus = Focus;
            LookDirection = (Focus - Position).Normalized();
            RightDirection = LookDirection.Cross(UpDirection).Normalized();
            InvalidateView();
        }

    private:
        /**
         * Invalidate camera view projection matrix function.
         * 
         * \param None.
         * \retrun None.
         */
        void InvalidateViewProjection()
        {
            ViewProjection = View * Projection;
        }

        /**
         * Invalidate camera projection matrix function.
         * 
         * \param None.
         * \return None.
         */
        void InvalidateProjection()
        {
            float ratio_x = FieldOfView / 2, ratio_y = FieldOfView / 2;

            if (ProjectionWidth >= ProjectionHeight) ratio_x *= (float)ProjectionWidth / ProjectionHeight;
            else ratio_y *= (float)ProjectionHeight / ProjectionWidth;

            if (ProjectionType == camera_projection_type::ORTHOGRAPHIC)
                Projection = matr4::Ortho(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjectionDistance, FarClip);
            if (ProjectionType == camera_projection_type::PERSPECTIVE)
                Projection = matr4::Frustum(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjectionDistance, FarClip);
            InvalidateViewProjection();
        }

        /**
         * Invalidate camera view matrix function.
         * 
         * \param None.
         * \return None.
         */
        void InvalidateView()
        {
            View = matr4::View(Position, Focus, UpDirection);
            InvalidateViewProjection();
        }

    public:
        /**
         * Render camera controller default constructor.
         * 
         * \param ProjectionType - render camera projection type.
         */
        camera(camera_projection_type ProjectionType)
        {
            this->ProjectionType = ProjectionType;

            Resize(16, 16);
            SetView({ 0, 3, 10 }, vec3 { 0 }, { 0, 1, 0 });
        }

        /**
         * Set camera project pixel size function..
         *
         * \param ProjectionWidth, ProjectionHeight - new projection plane size in pixels.
         * \return - self reference.
         */
        camera &Resize(int ProjectionWidth, int ProjectionHeight)
        {
            this->ProjectionWidth = (float)ProjectionWidth;
            this->ProjectionHeight = (float)ProjectionHeight;

            InvalidateProjection();
            return *this;
        }

        /**
         * Camera rotate function..
         *
         * \param Axis - rotation axis.
         * \param Angle - rotation angle (in degree).
         * \return self reference.
         */
        camera &Rotate(const vec3 &Axis, degrees Angle)
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

        /**
         * Camera movement function.
         *
         * \param MoveVector - movement directions.
         * \return self reference.
         */
        camera &Move(const vec3 &MoveVector)
        {
            Position += MoveVector;
            Focus += MoveVector;

            InvalidateView();
            return *this;
        }
    };
}
