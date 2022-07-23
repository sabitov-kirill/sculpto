/*!****************************************************************//*!*
 * \file   full_screen_quad.h
 * \brief  Topology full screen quad object class implementation module.
 *
 * \author Sabitov Kirill
 * \date   12 July 2022
 *********************************************************************/

#include "sclpch.h"
#include "full_screen_quad.h"

scl::topology::full_screen_quad::full_screen_quad() :
    grid(1, 1)
{
    Vertices.push_back(vertex({ -1, -1, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 0 }));
    Vertices.push_back(vertex({ -1,  1, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 1 }));
    Vertices.push_back(vertex({  1, -1, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 1, 0 }));
    Vertices.push_back(vertex({  1,  1, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 1, 1 }));
}