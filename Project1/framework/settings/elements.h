#pragma once
#include <string>
#include "imgui.h"
#include "../headers/songs.h"
class c_elements
{
public:

	struct
	{
		ImVec2 padding{ 16, 16 };
	} window;

	struct
	{
		float log_reg_width{ 400 };
		float img_height{ 436 };
		ImVec2 padding{ 32, 32 };
		float shadow_radius{ 24 };
		float button_padding{ 14 };
		float button_spacing{ 7 };
		float window_height{ 144 };
	} log_reg_page;

	struct
	{
		float height{ 60 };
		ImVec2 padding{ 12, 12 };
		ImVec2 icon_zone_size{ 36, 36 };
	} info_card;

	struct
	{
		float rounding{ 8 };
		ImVec2 spacing{ 8, 8 };
	} widgets;

	struct
	{
		ImVec2 size{ 336, 40 };
		ImVec2 icon_zone_size{ 14, 14 };
		float padding{ 14 };
	} textfield;


	struct
	{
		float line_weight{ 10 };
		float speed{ 1 };

	}glow_settings;

	struct
	{
		float height{ 44 };
		float blur{ 5 };
		ImVec2 padding{ 12, 12 };
		ImVec2 icon_size{ 20, 20 };
		float icon_2_zone_width{ 14 };
		float texts_spacing{ 4 };
	} top_bar;

	struct
	{
		float top_height{ 14 };
		float icon_zone_width{ 12 };
		float text_spacing{ 4 };
		ImVec2 padding{ 12, 12 };
	} child;

	struct
	{
		float height{ 50 };
		float blur{ 5 };
		ImVec2 padding{ 12, 12 };
		float img_height{ 25 };
		float img_rounding{ 4 };
		ImVec2 rect_size{ 34, 34 };
		float text_spacing{ 4 };
		ImVec2 icon_size{ 12, 14 };
	} game_card;
	struct
	{
		float img_rounding{ 8 };
		float hovered_rounding{ 5 };

	} song_card;
	struct
	{
		ImVec2 size{ 462, 260 };
		float control_height{ 29 };
		float logo_height_1{ 19 };
		float logo_height_2{ 16 };
		float zone_padding{ 7 };
		float slider_height{ 6 };
		float line_padding{ 1 };
		float volume_width{ 50 };
		float volume_padding{ 4 };
		ImVec2 buttons_size{ 14, 14 };
	} player;

	struct
	{
		float back_button_height{  14 };
		float back_button_padding{ 4 };
		float game_zone_height{ 19 };
		ImVec2 img_size{ 16, 16 };
	} product_page;

	struct
	{
		float height{ 16 };
		ImVec2 rect_size{ 14, 14 };
		float rounding{ 4 };
	} checkbox;

	struct
	{
		float spacing{ 7 };
		float padding{ 14 };
		float but_spacing{ 4 };
		ImVec2 img_size{ 20, 14 };
	} selection;

	struct
	{
		float radius{ 10 };
		float notify_height{ 65 };
		float button_padding{ 22 };
		float window_alpha{ 0 };
	} loading;

	struct
	{
		std::vector<std::string> symbols{ "0"};
		int count{ 15 };
	}background;
	struct
	{
		float pad{ 8.f };
		float background_rounding{ 12.f };
	}music_player;
	struct
	{
		ImVec2 size{ 90,50 };
		float blur{ 2 };
		ImVec2 padding{ 12, 12 };
		std::vector<std::string> contacts_name{ "Discord","Telegram","Website" };
		std::vector<std::string> contacts_urls{ "Discord.com","Telegram.com","Website.com" };
		
	} redirect;

};

inline std::unique_ptr<c_elements> elements = std::make_unique<c_elements>();
