#pragma once

#include <vector>
#include <string>
#include"../headers/includes.h"
struct text_snowflake
{
    ImVec2 pos;
    float base_speed;
    float phase;
    float size;
    int text_index;
};

void draw_text_snow(
    ImDrawList* draw_list,
    const ImRect& area,
    const std::vector<std::string>& texts,
    ImU32 color,
    int count,
    float time
);