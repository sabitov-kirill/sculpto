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
    /* Classes declaration. */
    class frame_buffer;

    /* Camera projection type enum. */
    enum class camera_projection_type
    {
        ORTHOGRAPHIC,
        PERSPECTIVE,
    };

    /* Camer rendering effects structure. */
    struct camera_effects
    {
        bool HDR { false };    /* Flag, showing whether use high dynamic range colors, whle rendering. */
        float Exposure { 0 };  /* Exposure level coefficient for exposure tone mapping.
                                  Note: tone mapping applyed only if HDR is on. */
        bool Bloom { false };  /* Flag, showing whether apply bloom effect while rendering or not.
                                  Note: wprks only if HDR is on. */
        int BloomAmount { 0 }; /* Iteration of blur while applying bloom effect. */

        /* Camera effects default constructor. */
        camera_effects(bool HDR = false, float Exposure = 0, bool Bloom = false, int BloomAmount = 0) :
            HDR(HDR), Exposure(Exposure), Bloom(Bloom), BloomAmount(BloomAmount) {}
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
        float ViewportWidth {};
        float ViewportHeight {};

        /* Camera view data. */
        vec3 UpDirection {};
        vec3 LookDirection {};
        vec3 RightDirection {};
        vec3 Position {};
        vec3 Focus {};

        /* Camera frame buffers. */
        shared<frame_buffer> MainFrameBuffer {};     /* Main renderer frame buffer. */
        shared<frame_buffer> GBuffer {};             /* Frame buffer for phong model geometry pass. */
        shared<frame_buffer> HDRFrameBuffer {};      /* HDR Frame buffer, then tone mapped to destination frame buffer. */
        shared<frame_buffer> BlurFrameBuffers[2] {}; /* Frame buffer for apllying gaussian blur effect. */

    public:
        /* Camera rendering effects data. */
        camera_effects Effects {};

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
        float GetViewportWidth() const { return ViewportWidth; };
        /* Projection plane height in pixels getter function. */
        float GetViewportHeight() const { return ViewportHeight; };

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

        /* Main renderer frame buffer. */
        const shared<frame_buffer> &GetMainFrameBuffer() const { return MainFrameBuffer; }
        /* Frame buffer for phong model geometry pass. */
        const shared<frame_buffer> &GetGBuffer() const { return GBuffer; }
        /* HDR Frame buffer, then tone mapped to destination frame buffer. */
        const shared<frame_buffer> &GetHDRFrameBuffer() const { return HDRFrameBuffer; }
        /* Frame buffer for apllying gaussian blur effect getter function. */
        const shared<frame_buffer> &GetBlurFrameBuffers(int Index = 0) const { return BlurFrameBuffers[Index]; }

        /* Camera projection type setter function. */
        void SetProjectionType(camera_projection_type ProjectionType);
        /* Field of view getter function. */
        void SetFieldOfView();
        /* Distance to near clip plane getter function. */
        void SetProjectionDistance(float ProjectionDistance);
        /* Distance to far clip plane getter function. */
        void SetFarClip(float FarClip);
        /* Projection plane width in pixels getter function. */
        void SetViewportWidth(float ViewportWidth);
        /* Projection plane height in pixels getter function. */
        void SetViewportHeight(float ViewportHeight);

        /* Camera up direction setter function. */
        void SetUpDirection(const vec3 &UpDirection);
        /* Camera forward direction setter function. */
        void SetDirection(const vec3 &Direction);
        /* Camer position setter function. */
        void SetPosition(const vec3 &Position);
        /* Camer focus point setter function. */
        void SetFocus(const vec3 &Focus);

        /**
         * Set view matrix by camera translation parametrs.
         * 
         * \param Position - camera position.
         * \param Focus - camera focusing point.
         * \param UpDirection - camera up direction.
         */
        void SetView(const vec3 &Position, const vec3 &Focus, const vec3 &UpDirection);

        /**
         * Set flag, showing wheather camera main buffer is swap chain target or not.
         *
         * \param IsSwapChainTarget - flag, showing wheather camera main buffer is swap chain target or not
         * \return None.
         */
        void SetRenderToSwapChain(bool IsSwapChainTarget);

    private:
        /**
         * Invalidate camera view projection matrix function.
         * 
         * \param None.
         * \retrun None.
         */
        void InvalidateViewProjection();

        /**
         * Invalidate camera projection matrix function.
         * 
         * \param None.
         * \return None.
         */
        void InvalidateProjection();

        /**
         * Invalidate camera view matrix function.
         * 
         * \param None.
         * \return None.
         */
        void InvalidateView();

        /**
         * Invalidate camera frame buffers function.
         * Creates uncreated ones, and resizes if needed.
         * 
         * \param None.
         * \return None.
         */
        void InvalidateBuffers();

        /************************************************************************************
         * Frame buffers resize functions.
         * Call actual resize only if specifed viewport size is diffrent with buffer current.
         ************************************************************************************/

        void ResizeMainFrameBuffer();
        void ResizeGBuffer();
        void ResizeHDRFrameBuffer();
        void ResizeBlurFrameBuffers();

    public:
        /**
         * Render camera controller default constructor.
         * 
         * \param ProjectionType - render camera projection type.
         */
        camera(camera_projection_type ProjectionType, camera_effects Effects = {});

        /**
         * Set camera project pixel size function..
         *
         * \param ViewportWidth, ViewportHeight - new projection plane size in pixels.
         * \return - self reference.
         */
        camera &Resize(int ViewportWidth, int ViewportHeight);

        /**
         * Camera rotate function..
         *
         * \param Axis - rotation axis.
         * \param Angle - rotation angle (in degree).
         * \return self reference.
         */
        camera &Rotate(const vec3 &Axis, degrees Angle);

        /**
         * Camera movement function.
         *
         * \param MoveVector - movement directions.
         * \return self reference.
         */
        camera &Move(const vec3 &MoveVector);
    };
}
