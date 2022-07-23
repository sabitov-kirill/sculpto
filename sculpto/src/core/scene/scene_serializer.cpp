/*!****************************************************************//*!*
 * \file   scene_serializer.cpp
 * \brief  Scene serialization class implemenation module.
 * 
 * \author Sabitov Kirill
 * \date   17 July 2022
 *********************************************************************/

#include "sclpch.h"

#include "scene_serializer.h"
#include "core/application/application.h"
#include "core/gui/gui.h"
#include "core/render/render_bridge.h"
#include "utilities/assets_manager/files_load.h"
#include "utilities/assets_manager/files_save.h"
#include "utilities/assets_manager/meshes_load.h"

void scl::to_json(json &Json, const name_component &NameComponent)
{
    Json["name"] = NameComponent.Name;
}

void scl::to_json(json &Json, const transform_component &TransformComponent)
{
    Json["scale"] = TransformComponent.Scale;
    Json["angles"] = TransformComponent.Angles;
    Json["position"] = TransformComponent.Position;
}

void scl::to_json(json &Json, const camera_component &CameraComponent)
{
    const auto &camera = CameraComponent.Camera;
    Json["is_primary"] = CameraComponent.IsPrimary;
    Json["projection_type"] = (int)camera.GetProjectionType();
    Json["fov"] = camera.GetFieldOfView();
    Json["position"] = camera.GetPosition();
    Json["focus_point"] = camera.GetFocus();
    Json["up_direction"] = camera.GetUpDirection();
    Json["viewport_width"] = camera.GetViewportWidth();
    Json["viewport_height"] = camera.GetViewportHeight();
    Json["effects"] = {
        { "is_hdr", camera.Effects.HDR },
        { "exposure", camera.Effects.Exposure },
        { "is_bloom", camera.Effects.Bloom },
        { "bloom_amount", camera.Effects.BloomAmount }
    };
}

void scl::to_json(json &Json, const point_light_component &PointLightComponent)
{
    Json["color"] = PointLightComponent.Color;
    Json["strength"] = PointLightComponent.Strength;
    Json["constant_attitution"] = PointLightComponent.Constant;
    Json["linear_attitution"] = PointLightComponent.Linear;
    Json["quafratic_attitution"] = PointLightComponent.Quadratic;
}

void scl::to_json(json &Json, const directional_light_component &DirectionalLightComponent)
{
    Json["color"] = DirectionalLightComponent.Color;
    Json["strength"] = DirectionalLightComponent.Strength;
    Json["is_casting_shadows"] = DirectionalLightComponent.GetIsShadow();
    Json["projection_box_size"] = DirectionalLightComponent.GetBoxSize();
    Json["projection_box_depth"] = DirectionalLightComponent.GetBoxDepth();
    Json["shadow_map_width"] = DirectionalLightComponent.GetShadowMapWidth();
    Json["shadow_map_height"] = DirectionalLightComponent.GetShadowMapHeight();
}

void scl::to_json(json &Json, const spot_light_component &SpotLightComponent)
{
    Json["color"] = SpotLightComponent.Color;
    Json["strength"] = SpotLightComponent.Strength;
    Json["inner_cutoff_angle"] = SpotLightComponent.GetInnerCutoff();
    Json["outer_cutoff_angle"] = SpotLightComponent.GetOuterCutoff();
}

bool scl::from_json(const json &Json, name_component &NameComponent)
{
    const auto &name = Json.find("name");
    if (name == Json.end()) return false;

    NameComponent.Name = name->get<std::string>();
    return true;
}

bool scl::from_json(const json &Json, transform_component &TransformComponent)
{
    const auto &scale    = Json.find("scale");
    const auto &angles   = Json.find("angles");
    const auto &position = Json.find("position");

    if (scale != Json.end())    TransformComponent.SetScale(scale->get<vec3>());
    if (angles != Json.end())   TransformComponent.SetAngles(angles->get<vec3>());
    if (position != Json.end()) TransformComponent.SetPosition(position->get<vec3>());
    return true;
}

bool scl::from_json(const json &Json, camera_component &CameraComponent)
{
    const auto &is_primary      = Json.find("is_primary");
    const auto &projection_type = Json.find("projection_type");
    const auto &fov             = Json.find("fov");
    const auto &position        = Json.find("position");
    const auto &focus_point     = Json.find("focus_point");
    const auto &up_direction    = Json.find("up_direction");
    const auto &viewport_width  = Json.find("viewport_width");
    const auto &viewport_height = Json.find("viewport_height");

    auto &camera = CameraComponent.Camera;
    if (is_primary      != Json.end()) CameraComponent.IsPrimary = is_primary->get<bool>();
    if (projection_type != Json.end()) camera.SetProjectionType((camera_projection_type)projection_type->get<int>());
    if (fov             != Json.end()) camera.SetFieldOfView(fov->get<float>());
    if (position        != Json.end()) camera.SetPosition(position->get<vec3>());
    if (focus_point     != Json.end()) camera.SetFocus(focus_point->get<vec3>());
    if (up_direction    != Json.end()) camera.SetUpDirection(up_direction->get<vec3>());
    if (viewport_width  != Json.end()) camera.SetViewportWidth(viewport_width->get<int>());
    if (viewport_height != Json.end()) camera.SetViewportHeight(viewport_height->get<int>());

    const auto &effects = Json.find("effects");
    if (effects != Json.end())
    {
        const auto &effects_is_hdr       = effects->find("is_hdr");
        const auto &effects_exposure     = effects->find("exposure");
        const auto &effects_is_bloom     = effects->find("is_bloom");
        const auto &effects_bloom_amount = effects->find("bloom_amount");

        if (effects_is_hdr       != effects->end()) camera.Effects.HDR         = effects_is_hdr->get<bool>();
        if (effects_exposure     != effects->end()) camera.Effects.Exposure    = effects_exposure->get<float>();
        if (effects_is_bloom     != effects->end()) camera.Effects.Bloom       = effects_is_bloom->get<bool>();
        if (effects_bloom_amount != effects->end()) camera.Effects.BloomAmount = effects_bloom_amount->get<int>();
    }
    return true;
}

bool scl::from_json(const json &Json, point_light_component &PointLightComponent)
{
    const auto &color                = Json.find("color");
    const auto &strength             = Json.find("strength");
    const auto &constant_attitution  = Json.find("constant_attitution");
    const auto &linear_attitution    = Json.find("linear_attitution");
    const auto &quadratic_attitution = Json.find("quafratic_attitution");

    if (color                != Json.end()) PointLightComponent.Color = color->get<vec3>();
    if (strength             != Json.end()) PointLightComponent.Strength = strength->get<float>();
    if (constant_attitution  != Json.end()) PointLightComponent.Constant = constant_attitution->get<float>();
    if (linear_attitution    != Json.end()) PointLightComponent.Linear = linear_attitution->get<float>();
    if (quadratic_attitution != Json.end()) PointLightComponent.Quadratic = quadratic_attitution->get<float>();
    return true;
}

bool scl::from_json(const json &Json, directional_light_component &DirectionalLightComponent)
{
    const auto &color                = Json.find("color");
    const auto &strength             = Json.find("strength");
    const auto &is_casting_shadows   = Json.find("is_casting_shadows");
    const auto &projection_box_size  = Json.find("projection_box_size");
    const auto &projection_box_depth = Json.find("projection_box_depth");
    const auto &shadow_map_width     = Json.find("shadow_map_width");
    const auto &shadow_map_height    = Json.find("shadow_map_height");

    if (color != Json.end())                DirectionalLightComponent.Color = color->get<vec3>();
    if (strength != Json.end())             DirectionalLightComponent.Strength = strength->get<float>();
    if (is_casting_shadows != Json.end())   DirectionalLightComponent.SetIsShadows(is_casting_shadows->get<bool>());
    if (projection_box_size != Json.end())  DirectionalLightComponent.SetBoxSize(projection_box_size->get<float>());
    if (projection_box_depth != Json.end()) DirectionalLightComponent.SetBoxDepth(projection_box_depth->get<float>());
    if (shadow_map_width != Json.end())     DirectionalLightComponent.SetShadowMapWidth(shadow_map_width->get<int>());
    if (shadow_map_height != Json.end())    DirectionalLightComponent.SetShadowMapHeight(shadow_map_height->get<int>());
    return true;
}

bool scl::from_json(const json &Json, spot_light_component &SpotLightComponent)
{
    const auto &color              = Json.find("color");
    const auto &strength           = Json.find("strength");
    const auto &inner_cutoff_angle = Json.find("inner_cutoff_angle");
    const auto &outer_cutoff_angle = Json.find("outer_cutoff_angle");

    if (color              != Json.end()) SpotLightComponent.Color = color->get<vec3>();
    if (strength           != Json.end()) SpotLightComponent.Strength = strength->get<float>();
    if (inner_cutoff_angle != Json.end()) SpotLightComponent.SetInnerCutoff(inner_cutoff_angle->get<float>());
    if (outer_cutoff_angle != Json.end()) SpotLightComponent.SetOuterCutoff(outer_cutoff_angle->get<float>());
    return true;
}

void scl::scene_serializer::SerializeObject(json &Json, const scene_object &SceneObject)
{
    SerializeComponent<name_component>(Json, SceneObject);
    SerializeComponent<transform_component>(Json, SceneObject);
    SerializeComponent<camera_component>(Json, SceneObject);
    SerializeComponent<point_light_component>(Json, SceneObject);
    SerializeComponent<directional_light_component>(Json, SceneObject);
    SerializeComponent<spot_light_component>(Json, SceneObject);
}

void scl::scene_serializer::DeserializeObject(const json &Json, scene_object &SceneObject)
{
    DeserializeComponent<transform_component>(Json, SceneObject);
    DeserializeComponent<camera_component>(Json, SceneObject);
    DeserializeComponent<point_light_component>(Json, SceneObject);
    DeserializeComponent<directional_light_component>(Json, SceneObject);
    DeserializeComponent<spot_light_component>(Json, SceneObject);
}

void scl::scene_serializer::Serialize(const shared<scene> &Scene, const std::filesystem::path &OutFileName)
{
    if (!Scene) return;

    json objects_json;
    Scene->Registry.each([&](auto Entity)
    {
        scene_object object = Scene->GetSceneObject(Entity);
        if (!object) return;

        json j;
        SerializeObject(j, object);
        objects_json.push_back(j);
    });

    json scene_json = {
        { "gui", {
            { "is_gui_enabled", application::Get().GuiEnabled },
            { "is_dockspace", gui::IsDockspace },
        } },
        { "render", {
            { "clear_color", render_bridge::GetClearColor() },
            { "is_wireframe", render_bridge::GetWireframeMode() },
            { "is_vsync",     render_bridge::GetVSync() },
        } },
        { "scene_data", { 
            { "viewport_id", Scene->GetViewportId(), },
            { "enviroment_ambient", Scene->GetEnviromentAmbient(), }
        } },
        { "objects", objects_json }
    };
    assets_manager::SaveFile(scene_json.dump(4), OutFileName);
    SCL_CORE_INFO("Scene saved to file \"{}\".", OutFileName.string());
}

void scl::scene_serializer::Deserialize(shared<scene> &Scene, const std::filesystem::path &InFileName)
{
    const std::string root_json_string = assets_manager::LoadFile(InFileName);
    const json root_json = json::parse(root_json_string);

    const auto &gui_json = root_json.find("gui");
    const auto &render_json = root_json.find("render");
    const auto &scene_data_json = root_json.find("scene_data");
    const auto &objects_json = root_json.find("objects");

    if (gui_json != root_json.end())
    {
        const auto &is_gui_enabled = gui_json->find("is_gui_enabled");
        const auto &is_dockspace = gui_json->find("is_dockspace");

        if (is_gui_enabled != gui_json->end()) application::Get().GuiEnabled = is_gui_enabled->get<bool>();
        if (is_dockspace != gui_json->end()) gui::IsDockspace = is_dockspace->get<bool>();
    }
    if (render_json != root_json.end())
    {
        const auto &clear_color = render_json->find("clear_color");
        const auto &is_wireframe = render_json->find("is_wireframe");
        const auto &is_vsync = render_json->find("is_vsync");

        if (clear_color != render_json->end()) render_bridge::SetClearColor(clear_color->get<vec4>());
        if (is_wireframe != render_json->end()) render_bridge::SetWireframeMode(is_wireframe->get<bool>());
        if (is_vsync != render_json->end()) render_bridge::SetVSync(is_vsync->get<bool>());
    }
    if (scene_data_json != root_json.end())
    {
        const auto &viewport_id = scene_data_json->find("viewport_id");
        const auto &enviroment_ambient = scene_data_json->find("enviroment_ambient");

        if (viewport_id != scene_data_json->end()) Scene->SetViewportId(viewport_id->get<int>());
        if (enviroment_ambient != scene_data_json->end()) Scene->SetEnviromentAmbient(enviroment_ambient->get<vec3>());
    }
    if (objects_json != root_json.end())
    {
        for (const auto &object_json : *objects_json)
        {
            scene_object object;
            const auto &name_json = object_json.find(typeid(name_component).name());
            if (name_json == object_json.end())
            {
                object = Scene->CreateObject();
            }
            else
            {
                name_component object_name_component { "" };
                from_json(*name_json, object_name_component);
                object = Scene->CreaetOrGetObject(object_name_component.Name);
            }

            DeserializeObject(object_json, object);
        }
    }

}
