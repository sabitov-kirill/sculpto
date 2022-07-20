/*****************************************************************//**
 * \file   sculpto.h
 * \brief  Sculpto based client applications aimed include module.
 *         Includes all library content.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include <imgui.h>
#include <entt.hpp>
#include <json.h>

/* Core module. */
#include "core/application/application.h"

/* GUI windows. */
#include "core/gui/windows/application_config_window.h"
#include "core/gui/windows/viewport_window.h"
#include "core/gui/windows/profiller_window.h"
#include "core/gui/windows/scene_config_window.h"
#include "core/gui/windows/scene_hierarchy_window.h"
#include "core/gui/windows/scene_object_config_window.h"

/* Render primitives, objects, context. */
#include "core/render/primitives/vertex_array.h"
#include "core/render/primitives/buffer.h"
#include "core/render/primitives/shader.h"
#include "core/render/primitives/texture.h"
#include "core/render/primitives/frame_buffer.h"

#include "core/resources/topology/trimesh.h"
#include "core/resources/topology/cube.h"
#include "core/resources/topology/grid.h"
#include "core/resources/topology/grid.h"
#include "core/resources/topology/plane.h"
#include "core/resources/topology/sphere.h"
#include "core/resources/topology/cone.h"
#include "core/resources/topology/points.h"
#include "core/resources/topology/full_screen_quad.h"
#include "core/resources/materials/material.h"
#include "core/resources/materials/material_phong.h"
#include "core/resources/materials/material_single_color.h"
#include "core/resources/mesh.h"

#include "core/render/render_bridge.h"
#include "core/render/renderer.h"

/* Scene module */
#include "core/scene/scene.h"
#include "core/scene/scene_serializer.h"
#include "core/scene/scene_object.h"
#include "core/scene/scene_object_behaviour.h"
#include "core/components/components.h"

/* Assets manager module. */
#include "utilities/assets_manager/shaders_preprocessor.h"
#include "utilities/assets_manager/shaders_load.h"
#include "utilities/assets_manager/textures_load.h"
#include "utilities/assets_manager/meshes_load.h"
