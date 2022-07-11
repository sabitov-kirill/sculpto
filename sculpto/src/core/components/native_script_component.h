/*****************************************************************//**
 * \file   native_script_component.h
 * \brief  Application scene system object native script component class implementation module.
 * 
 * \author Sabitov Kirill
 * \date   03 July 2022
 *********************************************************************/

#pragma once

namespace scl
{
    /* Scene object behaviour class declaration. */
    class scene_object_behaviour;

    /* Native script */
    struct native_script_component
    {
        /* Native script object behaviour instance. */
        scene_object_behaviour *ObjectBehaviour {};
        /* Native script active status. If false script update calbbacks dont called. */
        bool IsActive { true };
        /* Native script object name. */
        std::string Name {};

        scene_object_behaviour *(*Instanciate)();
        void (*Delete)(native_script_component *NativeScript);

        /**
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
