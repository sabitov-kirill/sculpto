/*!****************************************************************//*!*
 * \file   native_script_component.h
 * \brief  Native script component (for scene objects behaviour) class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   03 July 2022
 *********************************************************************/

#pragma once

namespace scl
{
    /*! Scene object behaviour class declaration. */
    class scene_object_behaviour;

    /*! Native script component (for scene objects behaviour) class. */
    struct native_script_component
    {
        scene_object_behaviour *ObjectBehaviour {}; /*! Native script object behaviour instance. */
        bool IsActive { true };                     /*! Native script active status. If false script update calbbacks dont called. */
        std::string Name {};                        /*! Native script object name. */

        scene_object_behaviour *(*Instanciate)();
        void (*Delete)(native_script_component *NativeScript);

        /*!*
         * Bind native script component (instanciate object behaviour).
         * 
         * \param None.
         * \return None.
         */
        template <typename Tbehaviour>
        void Bind()
        {
            Name = typeid(Tbehaviour).name();

            Instanciate = []() { return (scene_object_behaviour *)(new Tbehaviour()); };
            Delete = [](native_script_component *NativeScript) { delete NativeScript->ObjectBehaviour; NativeScript->ObjectBehaviour = nullptr; };
        }
    };
}
