#pragma once
#include "imgui.h"
#include <memory>

class c_colors
{
public:

	struct
	{
		ImU32 particle_color = IM_COL32(0xFF, 0xFF, 0xFF, 255);//B8235C
		ImVec4 background = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
	} window;

	struct
	{
		ImColor accent{ 82, 82, 82 };
		ImColor text{ 255, 255, 255 };
		ImColor black{ 0, 0, 0 };

		ImColor super{ 49,49,49,255 };
		
		ImColor text_field{ 28,28,28,179 };

	} main;
	
	struct
	{
		ImU32 music_list_selected = IM_COL32(0xFF, 0x2D, 0x7D, 51);
		ImVec4 border_color_rect = ImVec4(1.0f, 0.176f, 0.49f, 0.39f);
		ImVec4 border_color_line = ImVec4(1.0f, 0.176f, 0.49f, 0.39f);
		ImVec4 stroke_color = ImVec4(1.0f, 0.176f, 0.49f, 1.0f);
		ImU32 background = IM_COL32(0x2F, 0x2D, 0x2D, 80);
		ImU32 background_stroke = IM_COL32(0x5E, 0x5E, 0x5E, 200);
		ImU32 song_hovered = IM_COL32(0x3D, 0x3C, 0x3C, 127);

	}music_player;
	

	struct
	{
		ImColor win_bg{ 25, 22, 28 };
		ImColor but_bg{ 27, 24, 30 };
	} loading;
};

inline std::unique_ptr<c_colors> clr = std::make_unique<c_colors>();
