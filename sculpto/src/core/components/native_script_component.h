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
    class scene_object_behaviour;

    struct native_script_component
    {
        scene_object_behaviour *ObjectBehaviour {};

        scene_object_behaviour *(*Instanciate)();
        void (*Delete)(native_script_component *NativeScript);

        template <typename Tbehaviour>
        void Bind()
        {
            Instanciate = []() { return (scene_object_behaviour *)(new Tbehaviour()); };
            Delete = [](native_script_component *NativeScript) { delete NativeScript->ObjectBehaviour; NativeScript->ObjectBehaviour = nullptr; };
        }
    };
}
