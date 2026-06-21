#include"../headers/includes.h"
#include "../headers/widgets.h"
#include "../headers/songs.h"
void c_widgets::info_card(std::string_view widgets_id, std::string_view icon, std::string_view name, std::string_view desc, float width)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());

    ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(width, SCALE(elements->info_card.height)));
    const ImRect icon_zone(total.Min + SCALE(elements->info_card.padding), total.Min + SCALE(elements->info_card.padding + elements->info_card.icon_zone_size));
    ItemSize(total, style.FramePadding.y);
    if (!ItemAdd(total, id))
        return;

    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.02), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->text_clipped(window->DrawList, font->get(icons_data, (icon == "E" ? 26 : 30)), icon_zone.Min, icon_zone.Max, draw->get_clr(clr->main.accent), icon.data(), NULL, NULL, ImVec2(0.5f, 0.5f));

    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), ImVec2(icon_zone.Max.x + SCALE(elements->widgets.spacing.x), icon_zone.Min.y + SCALE(2)), total.Max, draw->get_clr(clr->main.text), name.data(), gui->text_end(name.data()));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 14), ImVec2(icon_zone.Max.x + SCALE(elements->widgets.spacing.x), icon_zone.Min.y), ImVec2(total.Max.x, icon_zone.Max.y + SCALE(2)), draw->get_clr(clr->main.text, 0.72), desc.data(), gui->text_end(desc.data()), NULL, ImVec2(0.f, 1.f));
};



void c_widgets::version_card(std::string_view widgets_id, std::string_view name, std::string_view update, int img_id)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());

    ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(gui->content_avail().x, SCALE(elements->version_card.rect_size.y)));
    const ImRect img_zone(total.Min, total.Min + SCALE(elements->version_card.rect_size));
    ItemSize(total, style.FramePadding.y);
    if (!ItemAdd(total, id))
        return;

    draw->image_rounded(window->DrawList, var->gui.img_for_versions[img_id], img_zone.Min, img_zone.Max, ImVec2(0, 0), ImVec2(1, 1), draw->get_clr({1.f, 1.f, 1.f, 1.f}), SCALE(elements->version_card.rounding));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_regular_data, 14), ImVec2(img_zone.Max.x + SCALE(elements->version_card.padding), total.Min.y - SCALE(1)), total.Max, draw->get_clr(clr->main.text, 0.48), ("[" + std::string(name) + "]").data(), NULL, NULL, ImVec2(0.f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_regular_data, 14), ImVec2(img_zone.Max.x + SCALE(elements->version_card.padding * 2) + gui->text_size(font->get(suisse_intl_regular_data, 14), ("[" + std::string(name) + "]").data()).x, total.Min.y - SCALE(1)), total.Max, draw->get_clr(clr->main.text, 0.72), update.data(), NULL, NULL, ImVec2(0.f, 0.5f));

};

void c_widgets::top_bar(std::string_view url, std::string_view text, std::string_view date)
{
    struct top_bar_state
    {
        float alpha[3]{ 1.f, 0.f, 0.03f };
        bool clicked{ false };
        ImVec4 icon{ clr->main.accent };
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID("top_bar");

    top_bar_state* state = gui->anim_container<top_bar_state>(id);

    ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(window->WorkRect.Max.x - window->WorkRect.Min.x, SCALE(elements->top_bar.height)));
    ItemSize(total, style.FramePadding.y);
    if (!ItemAdd(total, id))
        return;
    //ImVec2(window->WorkRect.Max.x - window->WorkRect.Min.x, window->WorkRect.Max.y - window->WorkRect.Min.y

    draw->rect(
        window->DrawList,
        total.Min,
        total.Max,
        draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f)),
        0
    );
    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(ImVec4(14.f / 255.f, 14.f / 255.f, 14.f / 255.f, 40.f / 100.f)), 0);

    float line_length = SCALE(20.f);  
    float line_thick = SCALE(4.f);   
    auto corner_clr = draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f), 1.0f); 

    window->DrawList->AddLine(total.Min, ImVec2(total.Min.x + line_length, total.Min.y), corner_clr, line_thick);
    window->DrawList->AddLine(total.Min, ImVec2(total.Min.x, total.Min.y + line_length), corner_clr, line_thick);

    window->DrawList->AddLine(total.Max, ImVec2(total.Max.x - line_length, total.Max.y), corner_clr, line_thick);
    window->DrawList->AddLine(total.Max, ImVec2(total.Max.x, total.Max.y - line_length), corner_clr, line_thick);

    ImVec2 close_pos(total.Max.x - SCALE(30.f), total.Min.y + (total.GetSize().y / 2.f) - SCALE(9.f));
    ImVec2 minimize_pos(total.Max.x - SCALE(60.f), total.Min.y + (total.GetSize().y / 2.f) - SCALE(18.f));

    bool close_hovered = ImGui::IsMouseHoveringRect(close_pos, close_pos + ImVec2(SCALE(18.f), SCALE(18.f)));
    bool minimize_hovered = ImGui::IsMouseHoveringRect(minimize_pos, minimize_pos + ImVec2(SCALE(30.f), SCALE(30.f)));
    //draw->rect(
    //    window->DrawList,
    //    close_pos, close_pos + ImVec2(SCALE(18.f), SCALE(18.f)),
    //    draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f)),
    //    0
    //);
    draw->text_clipped(
        window->DrawList,
        font->get(suisse_intl_semi_bold_data, 18),
        close_pos,
        close_pos + ImVec2(SCALE(18.f), SCALE(18.f)),
        draw->get_clr(clr->main.text, close_hovered ? 1.f : 0.6f),
        "X",
        nullptr,
        nullptr,
        ImVec2(0.5f, 0.5f)
    );

    draw->text_clipped(
        window->DrawList,
        font->get(suisse_intl_semi_bold_data, 30),
        minimize_pos,
        minimize_pos + ImVec2(SCALE(30.f), SCALE(18.f)),
        draw->get_clr(clr->main.text, minimize_hovered ? 1.f : 0.6f),
        "-",
        nullptr,
        nullptr,
        ImVec2(0.5f, 0.5f)
    );


    if (close_hovered && g.IO.MouseClicked[0])
    {
        PostQuitMessage(0);
    }
    if (minimize_hovered && g.IO.MouseClicked[0])
    {
        ShowWindow(var->winapi.hwnd, SW_MINIMIZE);
    }

}
bool c_widgets::progress_row(std::string_view widgets_id, std::string_view name, float procent, float width) 
{
    struct progress_row_state
    {
        float shown{ 0.f };
    };
    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());
    
    const float p = ImClamp(procent, 0.0f, 100.0f);
    const ImVec2 pos = window->DC.CursorPos;

    const float h = SCALE(54.f);
    const ImRect total(pos, pos + ImVec2(width, h));
    const ImRect bar_bg(
        ImVec2(total.Min.x + SCALE(14.f), total.Max.y - SCALE(16.f)),
        ImVec2(total.Max.x - SCALE(14.f), total.Max.y - SCALE(10.f))
    );

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return false;

    progress_row_state* state = gui->anim_container<progress_row_state>(id);
    gui->easing(state->shown,p, 10.f, static_easing);
    
    const float bar_w = ImMax(0.0f, bar_bg.Max.x - bar_bg.Min.x);
    const float fill_w = bar_w * (state->shown / 100.f);
    const ImRect bar_fill(bar_bg.Min, ImVec2(bar_bg.Min.x + fill_w, bar_bg.Max.y));


    const float pulse = (sinf((float)g.Time * 5.0) * 0.5f + 0.5f);
    const float glow_alpha = 0.10 + pulse * 0.18f;

    draw->rect_filled(window->DrawList, bar_bg.Min, bar_bg.Max, draw->get_clr(clr->main.text, 0.08f), SCALE(4.f));

    if (fill_w > 1.0)
    {
        draw->rect_filled(window->DrawList, bar_fill.Min - SCALE(0.f, 4.f), bar_fill.Max + SCALE(0.f, 4.f), draw->get_clr(clr->main.accent, glow_alpha * 0.55), SCALE(7.f));

        draw->rect_filled(window->DrawList, bar_fill.Min - SCALE(0.f, 2.f), bar_fill.Max + SCALE(0.f, 2.f), draw->get_clr(clr->main.accent, glow_alpha), SCALE(6.f));
    }

    draw->rect_filled(window->DrawList, bar_fill.Min, bar_fill.Max, draw->get_clr(clr->main.accent, 1.0f), SCALE(4.f));


    draw->rect_filled(window->DrawList, bar_fill.Min, bar_fill.Max, draw->get_clr(clr->main.text, 0.12f), SCALE(4.f));
    return state->shown >= 99.5f;





}

bool c_widgets::song_card(std::string_view widgets_id, Song& song)
{
    struct song_card_state
    {
        float alpha[3]{ 1.f, 0.f, 0.03f };
        bool clicked{ false };
        bool hovered{ false };
        ImFont* author_font;
        ImFont* song_name_font;
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());

    const ImVec2 pos = window->DC.CursorPos;

    float card_width =
        (window->WorkRect.Max.x  - window->WorkRect.Min.x)
        - SCALE(elements->widgets.spacing.x);

    const ImRect total(
        pos,
        pos + ImVec2(card_width, SCALE(elements->game_card.height))
    );

    const ImRect button = total;

    float img_padding = SCALE(8.f);
    float image_h = total.GetSize().y - img_padding * 2.0f;
    float image_w = image_h;

    const ImRect img_zone(
        ImVec2(total.Min.x + img_padding, total.Min.y + img_padding),
        ImVec2(total.Min.x + img_padding + image_w, total.Min.y + img_padding + image_h)
    );

    gui->item_size(total, style.FramePadding.y);

    if (!gui->item_add(total, id))
        return false;

    bool hovered = gui->item_hoverable(button, id, g.LastItemData.InFlags);
    bool pressed = hovered && g.IO.MouseClicked[0];

    song_card_state* state = gui->anim_container<song_card_state>(id);
    state->hovered = total.Contains(g.IO.MousePos);


    state->song_name_font = (font->is_russian_text(song.name) ? font->get(suisse_intl_medium_data, 16) : font->get(poppins_medium_data, 18));
    state->author_font = (font->is_russian_text(song.author) ? font->get(suisse_intl_medium_data, 13) : font->get(poppins_medium_data, 16));

    if (pressed)
    {
        state->clicked = true;

        // ================================
        // TODO: ЗАПУСК ПЕСНИ ЗДЕСЬ
        // например:
        //
        // play_song(song.path);
        // current_song = &song;
        // audio_player.play(song.path);
        //
        // ================================
        //song.play = !song.play;

        ma_result res = play_song(var->gui.manager, song);
        
    }

    if (state->alpha[0] <= 0.11f)
    {
        state->clicked = false;
    }

    gui->easing(state->alpha[0], state->clicked ? 0.1f : 1.f, 6.f, static_easing);
    gui->easing(state->alpha[1], state->clicked ? 0.6f : 0.f, 6.f, static_easing);
    gui->easing(state->alpha[2], state->hovered || song.play ? 0.1f : 0.0f, 0.3f, static_easing);





    if (hovered || song.play == true)
    {

        ImVec4 stroke_color(1.0f, 0.176f, 0.49f, state->alpha[2]);// Розовый акцент с динамической альфой
        draw->rect(
            window->DrawList,
            total.Min,
            total.Max,
            draw->get_clr(stroke_color),
            0,              // Скругление (0 — острые углы)
            0,              // Число сегментов
            SCALE(1.5f)     // Толщина линии обводки
        );
    }
    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(ImGui::ColorConvertU32ToFloat4(clr->main.music_list_selected),state->alpha[2]), 0);

    if (song.texture)
    {
        draw->image_rounded(
            window->DrawList,
            song.texture,
            img_zone.Min,
            img_zone.Max,
            ImVec2(0, 0),
            ImVec2(1, 1),
            draw->get_clr({ 1.f, 1.f, 1.f, 1.f }),
            //SCALE(elements->game_card.img_rounding)
            0
        );
    }
    else
    {

        draw->text_clipped(
            window->DrawList,
            font->get(icons_data, 18),
            img_zone.Min,
            img_zone.Max,
            draw->get_clr(clr->main.text, 0.35f),
            "K",
            nullptr,
            nullptr,
            ImVec2(0.5f, 0.5f)
        );
    }

    draw->rect(
        window->DrawList,
        img_zone.Min,
        img_zone.Max,
        draw->get_clr(clr->main.text, 0.12f),
        SCALE(0)
    );

    float text_start_x = img_zone.Max.x + SCALE(8.f);

    float name_y = (total.Min.y + total.GetSize().y /2  - state->song_name_font->CalcTextSizeA(font->is_russian_text(song.name) ? 16 : 18, FLT_MAX, 0.f, song.name.c_str()).y / 2 ) - SCALE(10.f);  
    float author_y = (total.Min.y + total.GetSize().y / 2 - state->author_font->CalcTextSizeA(font->is_russian_text(song.name) ? 13 : 16, FLT_MAX, 0.f, song.author.c_str()).y / 2) + SCALE(10.f);

    draw->text_clipped(
        window->DrawList,
        state->song_name_font,
        ImVec2(text_start_x, name_y),
        total.Max,
        draw->get_clr(clr->main.text),
        song.name.c_str(),
        gui->text_end(song.name.c_str())
    );

    draw->text_clipped(
        window->DrawList,
        state->author_font,
        ImVec2(text_start_x, author_y),
        total.Max,
        draw->get_clr(clr->main.text, 0.55f),
        song.author.c_str(),
        gui->text_end(song.author.c_str())
    );

    gui->set_screen_pos(total.Min, pos_all);
    gui->dummy(total.GetSize() + ImVec2(0, SCALE(5.f)));
    if (state->clicked)
    {
        return true;
    }
    else {
        return false;
    }
}

bool c_widgets::player(std::string_view widgets_id, Song& song)
{
    struct song_card_state
    {
        float alpha[3]{ 1.f, 0.f, 0.f };
        bool clicked{ false };
        bool hovered{ false };
        float shown{ 0.f };

        float img_alpha{ 0.f };
        std::string last_song_name{};
        ImFont* author_name_font;
        ImFont* song_name_font;
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());

    const ImVec2 pos = window->DC.CursorPos;
    

    const ImRect total(
        pos,
        pos + ImVec2(window->WorkRect.Max.x - window->WorkRect.Min.x, window->WorkRect.Max.y - window->WorkRect.Min.y )
    );

    float image_size = SCALE(370.f); // размер задаёшь тут

    float center_x = total.Min.x + total.GetSize().x / 2.f - image_size / 2.f;
    float top_y = total.Min.y + SCALE(40.f);

    float progress = (song.current_time / song.full_time) * 100.f;
    const float p = ImClamp(progress, 0.0f, 100.0f);











    const ImRect img_zone(
        ImVec2(center_x, top_y),
        ImVec2(center_x + image_size, top_y + image_size)
    );
    gui->item_size(total, style.FramePadding.y);

    if (!gui->item_add(total, id))
        return false;
    song_card_state* state = gui->anim_container<song_card_state>(id);

    // если сменилась песня — сбрасываем альфу
    if (state->last_song_name != song.name)
    {
        state->img_alpha = 0.f;
        state->last_song_name = song.name;
    }

    // поднимаем альфу если есть текстура
    gui->easing(state->img_alpha, song.texture ? 0.8f : 0.f, 6.f, static_easing);
    gui->easing(state->shown, p, 100.f, static_easing);








    // progress
    float bar_height = SCALE(4.f);
    float bar_y = img_zone.Max.y + SCALE(90.f);

    const ImRect bar_bg(
        ImVec2(img_zone.Min.x, bar_y),
        ImVec2(img_zone.Max.x, bar_y + bar_height)
    );

    const float bar_w = ImMax(0.0f, bar_bg.Max.x - bar_bg.Min.x);
    const float fill_w = bar_w * (state->shown / 100.f);
    const ImRect bar_fill(bar_bg.Min, ImVec2(bar_bg.Min.x + fill_w, bar_bg.Max.y));





    state->song_name_font = (font->is_russian_text(song.name) ? font->get(suisse_intl_medium_data, 24) : font->get(poppins_medium_data, 28));
    state->author_name_font = (font->is_russian_text(song.author) ? font->get(suisse_intl_medium_data, 16) : font->get(poppins_medium_data, 18));
    //window->DrawList->PushClipRect(total.Min, total.Max, true);
    //draw_background_blur(window->DrawList, var->winapi.device_dx11, var->winapi.device_context, SCALE(elements->widgets.rounding), 1);
    //window->DrawList->PopClipRect();

    draw->rect(
        window->DrawList,
        total.Min,
        total.Max,
        draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f)),
        0
    );
    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(ImVec4(14.f / 255.f, 14.f / 255.f, 14.f / 255.f, 40.f / 100.f)), 0);

    float line_length = SCALE(20.f);  // Длина сторон уголка
    float line_thick = SCALE(4.f);   // Толщина линий
    auto corner_clr = draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f), 1.0f); // Твой цвет
        

    draw->image(window->DrawList, song.texture, img_zone.Min, img_zone.Max,
        ImVec2(0, 0), ImVec2(1, 1),
        IM_COL32(255, 255, 255, (int)(255 * state->img_alpha))
    );


    draw->rect(
        window->DrawList,
        img_zone.Min,
        img_zone.Max,
        draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f)),
        0
    );
// name
    draw->text_clipped(
        window->DrawList,
        state->song_name_font,
        ImVec2(total.Min.x + total.GetSize().x / 2 - state->song_name_font->CalcTextSizeA(font->is_russian_text(song.name) ? 26 : 28,FLT_MAX,0.f,song.name.c_str()).x /2, img_zone.Max.y  + SCALE(15.f)),
        ImVec2(img_zone.Max.x,img_zone.Max.y + SCALE(40.f)),
        draw->get_clr(clr->main.text, state->img_alpha),
        song.name.c_str(),
        gui->text_end(song.name.c_str())
    );

// author

    draw->text_clipped(
        window->DrawList,
        state->author_name_font,
        ImVec2(total.Min.x + total.GetSize().x / 2 - state->author_name_font->CalcTextSizeA(font->is_russian_text(song.author) ? 16 : 18, FLT_MAX, 0.f, song.author.c_str()).x / 2, img_zone.Max.y + SCALE(40.f)),
        ImVec2(img_zone.Max.x, img_zone.Max.y + SCALE(70.f)),
        draw->get_clr(clr->main.text, state->img_alpha * 0.5),
        song.author.c_str(),
        gui->text_end(song.author.c_str())
    );


//img

    draw->line(window->DrawList, ImVec2(img_zone.Max.x + line_thick / 2.f, img_zone.Min.y), ImVec2(img_zone.Max.x - line_length, img_zone.Min.y), corner_clr, line_thick);
    draw->line(window->DrawList, ImVec2(img_zone.Max.x, img_zone.Min.y), ImVec2(img_zone.Max.x, img_zone.Min.y + line_length), corner_clr, line_thick);

    draw->line(window->DrawList, ImVec2(img_zone.Min.x - line_thick / 2.f, img_zone.Max.y), ImVec2(img_zone.Min.x + line_length, img_zone.Max.y), corner_clr, line_thick);
    draw->line(window->DrawList, ImVec2(img_zone.Min.x, img_zone.Max.y + line_thick / 2.f), ImVec2(img_zone.Min.x, img_zone.Max.y - line_length), corner_clr, line_thick);


//total

    draw->line(window->DrawList, total.Min, ImVec2(total.Min.x + line_length, total.Min.y), corner_clr, line_thick);
    draw->line(window->DrawList, total.Min, ImVec2(total.Min.x, total.Min.y + line_length), corner_clr, line_thick);

    draw->line(window->DrawList, total.Max, ImVec2(total.Max.x - line_length, total.Max.y), corner_clr, line_thick);
    draw->line(window->DrawList, total.Max, ImVec2(total.Max.x, total.Max.y - line_length), corner_clr, line_thick);

// progress




    const float pulse = (sinf((float)g.Time * 5.0) * 0.5f + 0.5f);
    const float glow_alpha = 0.10 + pulse * 0.18f;

    draw->rect_filled(window->DrawList, bar_bg.Min, bar_bg.Max, draw->get_clr(clr->main.text, 0.08f), SCALE(4.f));

    if (fill_w > 1.0)
    {
        draw->rect_filled(window->DrawList, bar_fill.Min - SCALE(0.f, 4.f), bar_fill.Max + SCALE(0.f, 4.f), draw->get_clr(clr->main.accent, glow_alpha * 0.55), SCALE(7.f));

        draw->rect_filled(window->DrawList, bar_fill.Min - SCALE(0.f, 2.f), bar_fill.Max + SCALE(0.f, 2.f), draw->get_clr(clr->main.accent, glow_alpha), SCALE(6.f));
    }

    draw->rect_filled(window->DrawList, bar_fill.Min, bar_fill.Max, draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f) , 1.0f), SCALE(4.f));


    draw->rect_filled(window->DrawList, bar_fill.Min, bar_fill.Max, draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.20f)), SCALE(4.f));


    bool bar_hovered = bar_bg.Contains(g.IO.MousePos);
    bool bar_clicked = bar_hovered && g.IO.MouseClicked[0];

    if (bar_clicked)
    {
        float click_x = g.IO.MousePos.x - bar_bg.Min.x;
        float bar_width = bar_bg.Max.x - bar_bg.Min.x;
        float click_percent = ImClamp(click_x / bar_width, 0.f, 1.f);

        seek_song(var->gui.manager, song, click_percent);     
    }

    // ============ play/pause button ============

    float btn_size = SCALE(40.f);
    float controls_y = bar_bg.Max.y + SCALE(20.f);

    ImVec2 play_btn_pos(
        total.Min.x + total.GetSize().x / 2.f - btn_size / 2.f,
        controls_y
    );
    ImRect play_btn(play_btn_pos, play_btn_pos + ImVec2(btn_size, btn_size));

    bool play_hovered = play_btn.Contains(g.IO.MousePos);
    bool play_clicked = play_hovered && g.IO.MouseClicked[0];

    bool is_playing = ma_sound_is_playing(&var->gui.manager.current_sound);

    draw->rect_filled(window->DrawList, play_btn.Min, play_btn.Max, draw->get_clr(clr->main.text, play_hovered ? 0.15f : 0.08f), SCALE(8.f));

    draw->text_clipped(
        window->DrawList,
        font->get(icons_data, 20),
        play_btn.Min, play_btn.Max,
        draw->get_clr(clr->main.text),
        is_playing ? "P" : "p", // подставь свои реальные символы паузы/плея
        nullptr, nullptr,
        ImVec2(0.5f, 0.5f)
    );

    if (play_clicked)
        pause_song(var->gui.manager);

    // ============ volume slider ============

    float vol_width = SCALE(150.f);
    float vol_y = play_btn.Max.y + SCALE(20.f);

    ImRect vol_bar(
        ImVec2(total.Min.x + total.GetSize().x / 2.f - vol_width / 2.f, vol_y),
        ImVec2(total.Min.x + total.GetSize().x / 2.f + vol_width / 2.f, vol_y + SCALE(4.f))
    );

    bool vol_hovered = vol_bar.Contains(g.IO.MousePos);

    if (vol_hovered && (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging(0)))
    {
        float x = ImClamp(g.IO.MousePos.x - vol_bar.Min.x, 0.f, vol_width);
        var->gui.manager.volume = x / vol_width;
        set_volume(var->gui.manager, var->gui.manager.volume);
    }

    draw->rect_filled(window->DrawList, vol_bar.Min, vol_bar.Max, draw->get_clr(clr->main.text, 0.1f), SCALE(2.f));

    ImVec2 vol_fill_max(vol_bar.Min.x + vol_width * var->gui.manager.volume, vol_bar.Max.y);
    draw->rect_filled(window->DrawList, vol_bar.Min, vol_fill_max, draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 1.0f)), SCALE(2.f));


    return false;
}
void c_widgets::background_songs()
{
    ImGuiWindow* window = gui->get_window();

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 pos = window->DC.CursorPos;


    const ImRect total(
        pos,
        pos + ImVec2(window->WorkRect.Max.x - window->WorkRect.Min.x, window->WorkRect.Max.y - window->WorkRect.Min.y)
    );
    float plank_h = SCALE(81.f);
    float plank_pad = SCALE(1.f); // чуть уже

    // плашка сверху
    ImRect top_plank(
        ImVec2(total.Min.x + plank_pad, total.Min.y),
        ImVec2(total.Max.x - plank_pad, total.Min.y + plank_h)
    );

    //window->DrawList->PushClipRect(total.Min, total.Max, true);
    //draw_background_blur(window->DrawList, var->winapi.device_dx11, var->winapi.device_context, SCALE(elements->widgets.rounding), 1);
    //window->DrawList->PopClipRect();

    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(ImVec4(14.f / 255.f, 14.f / 255.f, 14.f / 255.f, 40.f / 100.f)), 0);

    float line_length = SCALE(20.f);  // Длина сторон уголка
    float line_thick = SCALE(4.f);   // Толщина линий
    auto corner_clr = draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f), 1.0f); // Твой цвет
    draw->rect_filled(window->DrawList, top_plank.Min, top_plank.Max,
        draw->get_clr(ImVec4(28.f / 255.f, 27.f / 255.f, 27.f / 255.f, 95.f / 100.f)), SCALE(4.f));

    draw->rect(
        window->DrawList,
        total.Min,
        total.Max,
        draw->get_clr(ImVec4(1.0f, 0.176f, 0.49f, 0.39f)),
        0
    );
    //// 1. Уголок: Слева Сверху
    window->DrawList->AddLine(total.Min, ImVec2(total.Min.x + line_length, total.Min.y), corner_clr, line_thick);
    window->DrawList->AddLine(total.Min, ImVec2(total.Min.x, total.Min.y + line_length), corner_clr, line_thick);

    //// 2. Уголок: Справа Снизу
    window->DrawList->AddLine(total.Max, ImVec2(total.Max.x - line_length, total.Max.y), corner_clr, line_thick);
    window->DrawList->AddLine(total.Max, ImVec2(total.Max.x, total.Max.y - line_length), corner_clr, line_thick);
    


    draw->text_clipped(
        window->DrawList,
        font->get(poppins_medium_data, 28),
        ImVec2(top_plank.Min.x + SCALE(10.f), top_plank.Min.y + (top_plank.GetSize().y / 2 ) - (font->get(suisse_intl_semi_bold_data,28)->FontSize/2.f)),
        top_plank.Max,
        draw->get_clr(clr->main.text),
        "Library",
        gui->text_end("Library")
    );
    draw->text_clipped(
        window->DrawList,
        font->get(poppins_medium_data, 30),
        ImVec2(top_plank.Min.x + SCALE(9.f), top_plank.Min.y + (top_plank.GetSize().y / 2) - (font->get(suisse_intl_semi_bold_data, 30)->FontSize / 2.f)),
        top_plank.Max,
        draw->get_clr(clr->main.text,0.2f),
        "Library",
        gui->text_end("Library")
    );
}

