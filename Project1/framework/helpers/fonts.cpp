#include "../headers/includes.h"
#include "../../thirdparty/imgui/imgui_freetype.h"
#include "imgui_impl_dx11.h"

void c_font::update()
{
    if (!var->gui.dpi_changed)
        return;

    var->gui.dpi = var->gui.stored_dpi / 100.f;

    ImFontConfig cfg;
    cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_Bitmap;
    cfg.FontDataOwnedByAtlas = false;

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    if (data.empty())
    {
        io.Fonts->AddFontDefault();
    }
    else
    {
        for (auto& font_t : data)
        {
            font_t.font = io.Fonts->AddFontFromMemoryTTF(font_t.data.data(), (int)font_t.data.size(), SCALE(font_t.size), &cfg, io.Fonts->GetGlyphRangesCyrillic());
        }
    }

    if (io.Fonts->Fonts.Size == 0)
        io.Fonts->AddFontDefault();

    io.Fonts->Build();
    ImGui_ImplDX11_CreateDeviceObjects();

    var->gui.dpi_changed = false;
}

ImFont* c_font::get(std::vector<unsigned char> font_data, float size)
{
    for (auto& font : data)
    {
        if (font.data == font_data && font.size == size)
        {
            if (font.font)
                return font.font;

            ImGuiIO& io = ImGui::GetIO();
            return io.FontDefault ? io.FontDefault : ImGui::GetFont();
        }
    }

    add(font_data, size);
    var->gui.dpi_changed = true;

    ImGuiIO& io = ImGui::GetIO();
    return io.FontDefault ? io.FontDefault : ImGui::GetFont();
}

void c_font::add(std::vector<unsigned char> font_data, float size)
{
    data.push_back({ font_data, size, nullptr });
}