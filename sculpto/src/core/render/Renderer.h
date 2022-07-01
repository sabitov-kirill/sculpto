/*****************************************************************//**
 * \file   renderer.h
 * \brief  Static hich-level renderer class definition module.
 *         Implements objects rendering functions via submission queue.
 * 
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /* Renderer classes declaration. */
    class mesh;
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
        const shared<mesh> &Mesh;                  /* Submitted to render mesh. */
        const camera &Camera;              /* Scene camera, active during submission call. */
     /* cosnt enviroment &Enviroment; */   /* Scene enviroment, active during submission call. */

        /* Submission default constructor. */
        submission(const shared<mesh> &Mesh, const camera &Camera) :
            Mesh(Mesh), Camera(Camera) {}
    };

    /* Renderer class. */
    class renderer
    {
    private: /* renderer data. */
        static std::vector<submission> SubmissionQueue;  /* Render submission queue.
                                                          * Each submission queue contains submitted mesh and data for its rendering.
                                                          * Submission queue could be rendered and cleared via flush function.
                                                          */

        const static camera *SceneCamera;                /* Current scene camera. Added to subbmision object while Submit call. */
     /* static cosnt enviroment &SceneEnviroment; */     /* Current scene enviroment. Added to subbmision object while Submit call. */
     

    private:
        /**
         * Draw mesh function.
         * 
         * \param Mesh - mesh to draw.
         * \return None.
         */
        static void Draw(const shared<mesh> &Mesh, const camera &Camera);

    public:
        /**
         * Submit mesh to submission queue (rendered while flush call) function.
         *
         * \param Mesh - mesh to submit to queue.
         * \return None.
         */
        static void Submit(const shared<mesh> &Mesh);

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
        static void BeginScene(const camera &Camera);

        /**
         * End scene function. By default flushes submission queue to default frame buffer.
         * 
         * \param ShouldFlush - flag, specifing should submission queue be automaticly flushed.
         * \return None.
         */
        static void EndScene(bool ShouldFlush = true);
    };
}
