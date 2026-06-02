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

void c_widgets::top_bar(std::string_view url,std::string_view text, std::string_view date)
{
    struct top_bar_state
    {
        float alpha{ 0.f };
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

    const ImRect total(pos, pos + ImVec2(gui->content_max().x - SCALE(elements->widgets.spacing.x + elements->window.padding.x + elements->top_bar.height), SCALE(elements->top_bar.height)));
    const ImRect icon_zone(total.Min, total.Min + SCALE(elements->top_bar.padding * 2 + elements->top_bar.icon_size));
    window->DrawList->PushClipRect(total.Min, total.Max, true);
    draw_background_blur(window->DrawList, var->winapi.device_dx11, var->winapi.device_context,SCALE(elements->widgets.rounding), elements->top_bar.blur);
    window->DrawList->PopClipRect();
    ItemSize(total, style.FramePadding.y);
    if (!ItemAdd(total, id))
        return;

    if (total.Contains(g.IO.MousePos) && g.IO.MouseClicked[0])
    {
        state->clicked = true;
        gui->open_url(url.data());
    }
    if (state->alpha <= 0.41)
    {
        state->clicked = false;
    }

    gui->easing(state->alpha, state->clicked ? 0.4f : 1.f, 2.f, static_easing);
    gui->easing(state->icon, state->clicked ? clr->main.text.Value : clr->main.super.Value, 12.f, dynamic_easing);

    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.03f), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding), 0, SCALE(1));
    draw->text_clipped(window->DrawList, font->get(icons_data, 17), icon_zone.Min, icon_zone.Max, draw->get_clr(state->icon, state->alpha), "A", NULL, NULL, ImVec2(0.5f, 0.5f));
    
    draw->circle_filled(window->DrawList, ImVec2(icon_zone.Max.x, total.GetCenter().y), SCALE(1), draw->get_clr(clr->main.text, 0.12), 30);
    
    draw->text_clipped(window->DrawList, font->get(icons_data, 12), ImVec2(icon_zone.Max.x + SCALE(elements->top_bar.padding.x), total.Min.y), ImVec2(icon_zone.Max.x + SCALE(elements->top_bar.padding.x + elements->top_bar.icon_2_zone_width), total.Max.y), draw->get_clr(clr->main.super), "B", NULL, NULL, ImVec2(0.5f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), ImVec2(icon_zone.Max.x + SCALE(elements->top_bar.padding.x + elements->top_bar.icon_2_zone_width + elements->top_bar.texts_spacing), total.Min.y - SCALE(1)), total.Max, draw->get_clr(clr->main.text), text.data(), NULL, NULL, ImVec2(0.f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), total.Min - SCALE(0, 1), total.Max - SCALE(elements->top_bar.padding.x, 0), draw->get_clr(clr->main.text, 0.48), date.data(), NULL, NULL, ImVec2(1.f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(icons_data, 12), ImVec2(total.Max.x - SCALE(elements->top_bar.padding.x + elements->top_bar.texts_spacing + elements->top_bar.icon_2_zone_width) - gui->text_size(font->get(suisse_intl_medium_data, 16), date.data()).x, total.Min.y), ImVec2(total.Max.x - SCALE(elements->top_bar.padding.x + elements->top_bar.texts_spacing) - gui->text_size(font->get(suisse_intl_medium_data, 16), date.data()).x, total.Max.y), draw->get_clr(clr->main.text, 0.24), "V", NULL, NULL, ImVec2(0.5f, 0.5f));
};
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
    //char percent_text[16];
    //ImFormatString(percent_text, IM_ARRAYSIZE(percent_text), "%.0f%%", state->shown);

    //draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.02f), SCALE(elements->widgets.rounding));
    //draw->rect(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.04f), SCALE(elements->widgets.rounding));

    //draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), total.Min + SCALE(14.f, 8.f),total.Max, draw->get_clr(clr->main.text), name.data());
    //draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 14), total.Min, total.Max - SCALE(14.f, 0.f), draw->get_clr(clr->main.text, 0.72f), percent_text, NULL, NULL, ImVec2(1.f, 0.35f));

    //draw->rect_filled(window->DrawList, bar_bg.Min, bar_bg.Max, draw->get_clr(clr->main.text, 0.08f), SCALE(4.f));
    //draw->rect_filled(window->DrawList, bar_fill.Min, bar_fill.Max, draw->get_clr(clr->main.accent, 1.f), SCALE(4.f));
    //draw->rect(window->DrawList, bar_bg.Min, bar_bg.Max, draw->get_clr(clr->main.text, 0.12f), SCALE(4.f));


    return state->shown >= 99.5f;


}

bool c_widgets::song_card(std::string_view widgets_id, Song& song)
{
    struct song_card_state
    {
        float alpha[3]{ 1.f, 0.f, 0.03f };
        bool clicked{ false };
        bool hovered{ false };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());

    const ImVec2 pos = window->DC.CursorPos;

    float card_width =
        (window->WorkRect.Max.x - window->WorkRect.Min.x)
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

    if (pressed)
    {
        state->clicked = true;

        // ================================
        // TODO: ÇÀÏÓÑÊ ÏÅÑÍÈ ÇÄÅÑÜ
        // íàïðèìåð:
        //
        // play_song(song.path);
        // current_song = &song;
        // audio_player.play(song.path);
        //
        // ================================
    }

    if (state->alpha[0] <= 0.11f)
    {
        state->clicked = false;
    }

    gui->easing(state->alpha[0], state->clicked ? 0.1f : 1.f, 6.f, static_easing);
    gui->easing(state->alpha[1], state->clicked ? 0.6f : 0.f, 6.f, static_easing);
    gui->easing(state->alpha[2], state->hovered ? 0.05f : 0.03f, 0.1f, static_easing);

    //window->DrawList->PushClipRect(total.Min, total.Max, true);
    //draw_background_blur(
    //    window->DrawList,
    //    var->winapi.device_dx11,
    //    var->winapi.device_context,
    //    elements->game_card.blur
    //);
    //window->DrawList->PopClipRect();

    draw->rect_filled(
        window->DrawList,
        total.Min,
        total.Max,
        draw->get_clr(clr->main.text, state->alpha[2]),
        SCALE(elements->widgets.rounding)
    );

    draw->rect(
        window->DrawList,
        total.Min,
        total.Max,
        draw->get_clr(clr->main.text, 0.04f),
        SCALE(elements->widgets.rounding)
    );

    // Êàðòèíêà ïåñíè
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
            SCALE(elements->game_card.img_rounding)
        );
    }
    else
    {
        draw->rect_filled(
            window->DrawList,
            img_zone.Min,
            img_zone.Max,
            draw->get_clr(clr->main.text, 0.08f),
            SCALE(elements->game_card.img_rounding)
        );

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
        SCALE(elements->game_card.img_rounding)
    );

    float text_start_x = img_zone.Max.x + SCALE(8.f);

    float name_y = total.Min.y + SCALE(15.f);
    float author_y = total.Min.y + SCALE(35.f);

    draw->text_clipped(
        window->DrawList,
        font->get(suisse_intl_medium_data, 14),
        ImVec2(text_start_x, name_y),
        total.Max,
        draw->get_clr(clr->main.text),
        song.name.c_str(),
        gui->text_end(song.name.c_str())
    );

    draw->text_clipped(
        window->DrawList,
        font->get(suisse_intl_medium_data, 12),
        ImVec2(text_start_x, author_y),
        total.Max,
        draw->get_clr(clr->main.text, 0.55f),
        song.author.c_str(),
        gui->text_end(song.author.c_str())
    );

    gui->set_screen_pos(total.Min, pos_all);
    gui->dummy(total.GetSize() + ImVec2(0, SCALE(5.f)));

    return pressed;
}
