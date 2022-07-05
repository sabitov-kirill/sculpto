/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class definition module.
 *         Implements objects rendering functions via submission queue.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "renderer_camera.h"

namespace scl
{
    /* Renderer classes declaration. */
    class mesh;
    class material;
    class frame_buffer;
    class constant_buffer;

    struct scene_meses_data
    {
        matr4 ViewProjection; // Scene view projection matrix ? Probably better just use uniform
        matr4 World;          // Mesh transorm matrix         ? Probably better just use uniform
    };

    /* Render submission structure. Contains current scene camera, enviroment etc. */
    struct submission
    {
        shared<mesh>              Mesh;             /* Submitted to render mesh. */
        shared<material>          Material;         /* Submitted to render mesh material. */
        matr4                     Transform;        /* Submitted to render mesh tranformation matrix. */
        const renderer_camera    &Camera;           /* Scene camera, active during submission call. */
     /* cosnt enviroment         &Enviroment; */    /* Scene enviroment, active during submission call. */

        /* Submission default constructor. */
        submission(shared<mesh> Mesh, shared<material> Material, matr4 Transform, const renderer_camera &Camera) :
            Mesh(Mesh), Camera(Camera), Material(Material), Transform(Transform) {}
    };

    /* Renderer class. */
    class renderer
    {
    private: /* renderer data. */
        static std::vector<submission> SubmissionQueue;  /* Render submission queue.
                                                          * Each submission queue contains submitted mesh and data for its rendering.
                                                          * Submission queue could be rendered and cleared via flush function.
                                                          */

        const static renderer_camera *SceneCamera;       /* Current scene camera. Added to subbmision object while Submit call. */
     /* static cosnt enviroment &SceneEnviroment; */     /* Current scene enviroment. Added to subbmision object while Submit call. */

    private:
        /**
         * Draw mesh function.
         * 
         * \param Mesh - mesh to draw.
         * \param Material - mesh material.
         * \param Transform - mesh tranformations matrix.
         * \param Camera - camera to take view and projection matrix from.
         * \return None.
         */
        static void Draw(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform, const renderer_camera &Camera);

    public:
        /**
         * Calculate mesh transorm matrix and submit to queue (rendered while flush call) function.
         *
         * \param Mesh - mesh to submit to queue.
         * \param Material - mesh material.
         * \param Scale - mesh scale factor along 3 axies.
         * \param Angles - mesh rotation angles factor along 3 axies.
         * \param Position - mesh world position.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const shared<material> &Material, 
                           const vec3 &Scale, const vec3 &Angles, const vec3 &Position);

        /**
         * Submit mesh to submission queue (rendered while flush call) function.
         * 
         * \param Mesh - mesh to submit to queue.
         * \param Material - mesh material.
         * \param Transform - mesh tranformations matrix.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh, const shared<material> &Material, const matr4 &Transform);

        /**
         * Flush submission quque and draw all meshes to specified frame buffer function.
         *
         * \param FrameBuffer - frame buffer to draw in.
         * \return None.
         */
        static void Flush(const shared<frame_buffer> &FrameBuffer);

        /**
         * Flush submission quque and draw all meshes to default screen frame buffer function.
         * 
         * \param None.
         * \return None.
         */
        static void FlushToDefaultFrameBuffer();

        /**
         * Begin scene function.
         * Runs computations for concrete scene,
         * memoize camera & enviroment data for rendering
         * 
         * \param Camera - scene camera to use while rendering scene.
         */
        static void SubmitCamera(const renderer_camera &Camera);
    };
}
