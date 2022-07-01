/*****************************************************************//**
 * \file   render_bridge.h
 * \brief  Render bridge class implementation module.
 *         Static class for making calls to low-level render api,
 *         bridge between application high and low level rendering systems.
 *
 * \author Sabitov Kirill
 * \date   01 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "render_bridge.h"

scl::shared<scl::render_context> scl::render_bridge::RenderContext = scl::render_context::Create();
