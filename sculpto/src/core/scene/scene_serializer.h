/*!****************************************************************//*!*
 * \file   scene_serializer.h
 * \brief  Scene serializer (also could perform deserialization) class definition module.
 * 
 * \author Sabitov Kirill
 * \date   17 July 2022
 *********************************************************************/

#pragma once

#include <json.h>
namespace scl { using json = nlohmann::json; };

#include "scene.h"
#include "scene_object.h"
#include "core/components/components.h"

namespace scl
{
    /*!*********************************************************************
     * Common types serialization/deserialization to json format functions.
     **********************************************************************/

    namespace math
    {
        template <typename T> void to_json(json &Json, const vec2<T> &Vector)  { Json = { Vector.X, Vector.Y }; }
        template <typename T> void to_json(json &Json, const vec3<T> &Vector)  { Json = { Vector.X, Vector.Y, Vector.Z }; }
        template <typename T> void to_json(json &Json, const vec4<T> &Vector)  { Json = { Vector.X, Vector.Y, Vector.Z, Vector.W }; }
        template <typename T> void to_json(json &Json, const matr3<T> &Matrix) { Json = { Matrix.A }; }
        template <typename T> void to_json(json &Json, const matr4<T> &Matrix) { Json = { Matrix.A }; }

        template <typename T> void from_json(const json &Json, vec2<T> &Vector)
        {
            if (!Json.is_array()) return;
            for (int i = 0; i < 2 && i < Json.size(); i++) Vector[i] = Json[i];
        }
        template <typename T> void from_json(const json &Json, vec3<T> &Vector)
        {
            if (!Json.is_array()) return;
            for (int i = 0; i < 3 && i < Json.size(); i++) Vector[i] = Json[i];
        }
        template <typename T> void from_json(const json &Json, vec4<T> &Vector)
        {
            if (!Json.is_array()) return;
            for (int i = 0; i < 4 && i < Json.size(); i++) Vector[i] = Json[i];
        }
        template <typename T> void from_json(const json &Json, matr3<T> &Matrix)
        {
            if (!Json.is_array()) return;
            for (int i = 0; i < 9 && i < Json.size(); i++) Matrix[i] = Json[i];
        }
        template <typename T> void from_json(const json &Json, matr4<T> &Matrix)
        {
            if (!Json.is_array()) return;
            for (int i = 0; i < 16 && i < Json.size(); i++) Matrix[i] = Json[i];
        }
    }

    /*!*******************************************************************
     * Components serialization/deserialization to json format functions.
     ********************************************************************/

    void to_json(json &Json, const name_component              &NameComponent);
    void to_json(json &Json, const transform_component         &TransformComponent);
    void to_json(json &Json, const camera_component            &CameraComponent);
    void to_json(json &Json, const point_light_component       &PointLightComponent);
    void to_json(json &Json, const directional_light_component &DirectionalLightComponent);
    void to_json(json &Json, const spot_light_component        &SpotLightComponent);

    bool from_json(const json &Json, name_component &NameComponent);
    bool from_json(const json &Json, transform_component &TransformComponent);
    bool from_json(const json &Json, camera_component &CameraComponent);
    bool from_json(const json &Json, point_light_component &PointLightComponent);
    bool from_json(const json &Json, directional_light_component &DirectionalLightComponent);
    bool from_json(const json &Json, spot_light_component &SpotLightComponent);

    /*! Scene serializer class. */
    class scene_serializer
    {
    private: /*! Scene serializer data. */
        weak<scene> Scene;

    private: /*! Scene serializer methods. */
        /*!*
         * Serialize specific component of scene object function.
         * 
         * \tparam Tcomponent - type of serializeing scene object component.
         * \param Json - json to serialize in.
         * \param SceneObject - scene object - scene object whose component is beeing serialized.
         * \return None.
         */
        template <typename Tcomponent>
        static void SerializeComponent(json &Json, const scene_object &SceneObject)
        {
            if (SceneObject.HasComponent<Tcomponent>())
                Json[typeid(Tcomponent).name()] = SceneObject.GetComponent<Tcomponent>();
        }

        template <typename Tcomponent>
        static void DeserializeComponent(const json &Json, scene_object &SceneObject)
        {
            const auto &component_json = Json.find(typeid(Tcomponent).name());
            if (component_json == Json.end()) return;

            Tcomponent component {};
            if (from_json(*component_json, component))
                SceneObject.AddComponent<Tcomponent>(component);
        }

        /*!*
         * Serialize scene object function.
         * 
         * \param SceneObject - scene object to serialize.
         * \return None.
         */
        static void SerializeObject(json &Json, const scene_object &SceneObject);

        /*!*
         * Deserialize object data from json and put in object function.
         * 
         * \param Json
         * \param Object
         */
        static void DeserializeObject(const json &Json, scene_object &SceneObject);

    public:
        /*!*
         * Serialize scene data to file function.
         * 
         * \param OutFileName - output file name.
         * \return None.
         */
        static void Serialize(const shared<scene> &Scene, const std::filesystem::path &OutFileName);

        /*!*
         * Deserialize scene data from specified file function.
         * 
         * \param Scene - scene to put deserialized date in.
         * \param InFileName
         */
        static void Deserialize(shared<scene> &Scene, const std::filesystem::path &InFileName);
    };
}
