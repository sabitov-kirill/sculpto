/*****************************************************************//**
 * \file   camera.h
 * \brief  Virtual camera implementation module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "math_common.h"

namespace scl::math
{
    template <typename T>
    class camera
    {
    public:
        T        ProjDist { 0.1f };           // Near project plane distance
        T        FarClip { 1000.0f };         // Far project plane distance
        T        Size { 0.1f };               // Inner project plane rectangle size
        int      FrameW { 30 };               // Camera frame size
        int      FrameH { 30 };
        vec3<T>  Loc { 0.0f, 0.0f, 10.0f };  // Camera location
        vec3<T>  Dir { 0.0f, 0.0f, -1.0f };  // Basis camera directions
        vec3<T>  Up { 0.0f, 1.0f, 0.0f };
        vec3<T>  Right { 1.0f, 0.0f, 0.0f };
        vec3<T>  At {};                      // Camera pivot point
        matr4<T> View {};                    // View matrix
        matr4<T> Proj {};                    // Projection matrix
        matr4<T> VP {};                      // View and Proj madtrix production

    private: /* Matrices update methods. */
        /**
         * Update camera project parameters function.
         * 
         * \param None.
         * \return None.
         */
        void UpdateProj()
        {
            T ratio_x = Size / 2, ratio_y = Size / 2;

            if (FrameW >= FrameH) ratio_x *= (T)FrameW / FrameH;
            else ratio_y *= (T)FrameH / FrameW;
            Proj = matr4<T>::Frustum(-ratio_x, ratio_x,
                                     -ratio_y, ratio_y,
                                     ProjDist, FarClip);
        }

        /**
         * Update camera view parameters function.
         * 
         * \param None.
         * \return None.
         */
        void UpdateView()
        {
            View = matr4<T>::View(Loc, At, Up);
        }

    public: /* Camera configuration. */
        /* Camera default construcotr. */
        camera()
        {
            UpdateProj();
            UpdateView();
        }

        /**
         * Set project camera parameters function.
         * 
         * \param NewSize - new near project plane size.
         * \param NewProjDist - new project distance.
         * \param NewFarClip - new far clip plane distance.
         * \return self reference.
         */
        camera & SetProj(T NewSize, T NewProjDist, T NewFarClip)
        {
            // Store new projection parameters
            ProjDist = NewProjDist;
            FarClip = NewFarClip;
            Size = NewSize;

            UpdateProj();
            VP = View * Proj;
            return *this;
        }

        /**
         * Set camera location and orientation function.
         * 
         * \param Loc - new camera location.
         * \param At - new camera pivot point.
         * \param Up - new camera approx up direction.
         * \return self reference.
         */
        camera &SetLocAtUp(const vec3<T> &Loc, const vec3<T> &At, const vec3<T> &Up = vec3<T>(0, 1, 0))
        {
            this->Loc = Loc;
            this->At = At;
            this->Up = Up;

            Dir = (At - Loc).Normalized();
            Right = Dir.Cross(Up).Normalized();

            UpdateView();
            VP = View * Proj;
            return *this;
        }

        /**
         * Set camera project pixel size function..
         *
         * \param NewFrameW, NewFrameH - new frame size in pixels.
         * \return - self reference.
         */
        camera &Resize(int NewFrameW, int NewFrameH)
        {
            FrameW = NewFrameW;
            FrameH = NewFrameH;

            UpdateProj();
            VP = View * Proj;
            return *this;
        }

    public: /* Camera manitulation methods. */
        /**
         * Camera rotate function..
         * 
         * \param Axis - rotation axis.
         * \param Angle - rotation angle (in degree).
         * \return self reference.
         */
        camera & Rotate(const vec3<T> &Axis, degrees<T> Angle)
        {
            matr4<T> m =
                matr4<T>::Translate(-Loc) *
                matr4<T>::Rotate(Axis, Angle) *
                matr4<T>::Translate(Loc);

            At = m.TransformPoint(At);
            Up = m.TransformVector(Up);
            SetLocAtUp(Loc, At, Up);
            return *this;
        }

        /**
         * Camera movement function.
         * 
         * \param Direction - movement directions.
         * \return self reference.
         */
        camera & Move(const vec3<T> &Direction)
        {
            Loc += Direction;
            At += Direction;
            SetLocAtUp(Loc, At, Up);
            return *this;
        }
    };
}
