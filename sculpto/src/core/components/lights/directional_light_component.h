/*!****************************************************************//*!*
 * \file   directional_light_component.h
 * \brief  Scene object directional light component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   05 July 2022
 *********************************************************************/

#pragma once

#include "base.h"
#include "core/render/primitives/frame_buffer.h"

namespace scl
{
    /*! Directional light component class. */
    struct directional_light_component
    {
    private: /*! Directional light data. */
        bool IsShadows {};
        shared<frame_buffer> ShadowMap {};
        matr4 Projection {};
        float BoxSize {};
        float BoxDepth {};
        int ShadowMapWidth {}, ShadowMapHeight {};

    public:
        /*! Light color. */
        vec3 Color {};
        float Strength { 1 };

    public: /*! Directiona light data getter/setter functions. */
        /*! Flag, showing wheather directional light is shadow caster or not getter function. */
        bool GetIsShadow() const { return IsShadows; }
        /*! Ortho projection shadow caster box width getter function. */
        float GetBoxSize() const { return BoxSize; }
        /*! Ortho projecction shadow caster box depth getter function. */
        float GetBoxDepth() const { return BoxDepth; }
        /*! Directiona light projection matrix getter function. */
        const matr4 &GetProjection() const { return Projection; }
        /*! Shadow map frame buffer width in pixels getter function. */
        int GetShadowMapWidth() const { return ShadowMapWidth; }
        /*! Shadow map frame buffer height in pixels getter function. */
        int GetShadowMapHeight() const { return ShadowMapHeight; }
        const shared<frame_buffer> &GetShadowMap() const { return ShadowMap; }

        /*!*
         * Enable directiona light shadow casting function.
         * 
         * \param BoxSize - ortho projection shadow caster box width.
         * \param BoxDepth - ortho projecction shadow caster box depth.
         * \param ShadowMapWidth - shadow map frame buffer width in pixels.
         * \param ShadowMapHeight - shadow map frame buffer height in pixels.
         */
        void EnableShadows(float BoxSize = 10, float BoxDepth = 100,
                           int ShadowMapWidth = 1000, int ShadowMapHeight = 1000)
        {
            this->IsShadows = true;
            this->BoxSize = BoxSize;
            this->BoxDepth = BoxDepth;
            this->ShadowMapWidth = ShadowMapWidth, this->ShadowMapHeight = ShadowMapHeight;
            Projection = matr4::Ortho(-BoxSize, BoxSize, -BoxSize, BoxSize, 1, BoxDepth);
            ShadowMap = frame_buffer::Create(frame_buffer_props { ShadowMapWidth, ShadowMapHeight, 1, false, 0, 1 });
        }

        /*!*
         * Disable directional light shadow casting function.
         * 
         * \param None.
         * \return None.
         */
        void DisableShadows()
        {
            this->IsShadows = false;
            if (ShadowMap) ShadowMap->Free();
            ShadowMap.reset();
        }

        /*!*
         * Set flag, showing wheather directiona light casing shadows or not function.
         * 
         * \param IsShadows - flag, showing wheather directiona light casing shadows or not function.
         * \return None.
         */
        void SetIsShadows(bool IsShadows)
        {
            if (IsShadows) EnableShadows();
            else           DisableShadows();
        }

        /*! Ortho projection shadow caster box width getter function. */
        void SetBoxSize(float BoxSize)
        {
            this->BoxSize = BoxSize;
            Projection = matr4::Ortho(-BoxSize, BoxSize, -BoxSize, BoxSize, 1, BoxDepth);
        }

        /*! Ortho projecction shadow caster box depth getter function. */
        void SetBoxDepth(float BoxDepth)
        {
            this->BoxDepth = BoxDepth;
            Projection = matr4::Ortho(-BoxSize, BoxSize, -BoxSize, BoxSize, 1, BoxDepth);
        }

        /*! Shadow map frame buffer width in pixels getter function. */
        void SetShadowMapWidth(int ShadowMapWidth)
        {
            this->ShadowMapWidth = ShadowMapWidth;
            if (!ShadowMap) return;
            ShadowMap->Resize(ShadowMapWidth, ShadowMapHeight);
        }

        /*! Shadow map frame buffer height in pixels getter function. */
        void SetShadowMapHeight(int ShadowMapHeight)
        {
            this->ShadowMapHeight = ShadowMapHeight;
            if (!ShadowMap) return;
            ShadowMap->Resize(ShadowMapWidth, ShadowMapHeight);
        }

    public: /*! Directional light methods. */
        directional_light_component() = default;
        directional_light_component(const directional_light_component &Other) = default;
        directional_light_component(const vec3 &Color, bool IsShadows = false, float BoxSize = 10, float BoxDepth = 0,
                                    int ShadowMapWidth = 1000, int ShadowMapHeight = 1000) : Color(Color)
        {
            if (IsShadows) EnableShadows(BoxSize, BoxDepth, ShadowMapWidth, ShadowMapHeight);
        }
        ~directional_light_component() = default;
    };
}
