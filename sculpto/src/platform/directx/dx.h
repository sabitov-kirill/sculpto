/*****************************************************************//**
 * \file   dx_common.h
 * \brief  DirectX 12 specific common types/macros definition module.
 * 
 * \author Sabitov Kirill
 * \date   22 June 2022
 *********************************************************************/

#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <dxgidebug.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "sclpch.h"
