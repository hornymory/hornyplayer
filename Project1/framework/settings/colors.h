#pragma once
#include "imgui.h"
#include <memory>

class c_colors
{
public:

	struct
	{
		ImU32 color_up = IM_COL32(0x1A, 0x0A, 0x14, 255);// 1A0A14;
		ImU32 color_down = IM_COL32(0x08, 0x08, 0x08, 255);// 080808;
		ImU32 particle_color = IM_COL32(0xB8, 0x24, 0x5C, 255);//B8235C
	} window;

	struct
	{
		ImColor accent{ 82, 82, 82 };
		ImColor text{ 255, 255, 255 };
		ImColor black{ 0, 0, 0 };

		ImColor super{ 49,49,49,255 };
		
		ImColor text_field{ 28,28,28,179 };

		ImU32 music_list_selected = IM_COL32(0xFF, 0x2D, 0x7D, 51);
	} main;
	

	struct
	{
		ImColor win_bg{ 25, 22, 28 };
		ImColor but_bg{ 27, 24, 30 };
	} loading;
};

inline std::unique_ptr<c_colors> clr = std::make_unique<c_colors>();
