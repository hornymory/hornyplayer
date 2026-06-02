#pragma once
#include "imgui.h"
#include <memory>

class c_colors
{
public:

	struct
	{
		ImColor background{ 14, 13, 15 };
		ImColor node_color{ 255,255,255 };
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
		ImColor win_bg{ 25, 22, 28 };
		ImColor but_bg{ 27, 24, 30 };
	} loading;
};

inline std::unique_ptr<c_colors> clr = std::make_unique<c_colors>();
