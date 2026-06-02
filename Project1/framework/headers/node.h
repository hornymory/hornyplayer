#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <iomanip>

#include "../settings/colors.h"
#include "../settings/elements.h"

#include <iostream>

extern void draw_nodes_background(ImDrawList* drawList, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
extern void release_node_resources();
