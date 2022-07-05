/*****************************************************************//**
 * \file   sculpto.h
 * \brief  Sculpto based client applications aimed include module.
 *         Includes all library content.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

#pragma once

#include <entt.hpp>

/* Core module. */
#include "core/application/application.h"

/* Render primitives, objects, context. */
#include "core/render/primitives/vertex_array.h"
#include "core/render/primitives/buffer.h"
#include "core/render/primitives/shader.h"
#include "core/render/primitives/texture.h"
#include "core/render/primitives/frame_buffer.h"

#include "core/render/topology/grid.h"
#include "core/render/topology/cube.h"
#include "core/render/topology/sphere.h"
#include "core/render/topology/cone.h"

#include "core/render/material/material.h"
#include "core/render/material/material_phong.h"
#include "core/render/mesh.h"

#include "core/render/render_bridge.h"
#include "core/render/renderer.h"

/* Runtime module */
#include "core/scene/scene.h"
#include "core/scene/scene_object.h"
#include "core/scene/scene_object_behaviour.h"
#include "core/components/components.h"
