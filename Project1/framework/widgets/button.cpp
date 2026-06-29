#include"../headers/includes.h"
#include "../headers/widgets.h"

bool c_widgets::reg_log_buttons()
{
    struct reg_log_buttons_state
    {
        bool clicked[2]{ false, false };
        bool selected[2]{ true, false };
        float width{ SCALE(elements->log_reg_page.button_padding * 2 + elements->log_reg_page.button_spacing + elements->textfield.icon_zone_size.x) + gui->text_size(font->get(suisse_intl_medium_data, 16), "Register").x };
        float rect_alpha[4]{ 1.f, 0.48, 0.04, 0.06 };
        float text_alpha[4]{ 1.f, 1.f, 0.24, 0.48 };
        ImVec4 rect[2]{ clr->main.super, clr->main.text };
        float selection_alpha[2]{ 1.f, 1.f };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID("log_reg_buttons");

    reg_log_buttons_state* state = gui->anim_container<reg_log_buttons_state>(id);

    gui->easing(state->width, state->selected[1] ? gui->content_avail().x - (SCALE(elements->log_reg_page.button_padding * 2 + elements->log_reg_page.button_spacing + elements->textfield.icon_zone_size.x + elements->widgets.spacing.x) + gui->text_size(font->get(suisse_intl_medium_data, 16), "Login").x) : SCALE(elements->log_reg_page.button_padding * 2 + elements->log_reg_page.button_spacing + elements->textfield.icon_zone_size.x) + gui->text_size(font->get(suisse_intl_medium_data, 16), "Register").x, 12.f, dynamic_easing);
    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(gui->content_avail().x, SCALE(elements->textfield.size.y)));
    const ImRect but_1(ImVec2(total.Max.x - state->width, total.Min.y), total.Max);
    const ImRect but_2(total.Min, ImVec2(but_1.Min.x - SCALE(elements->widgets.spacing.x), total.Max.y));
    ImColor rect_color = clr->main.super;

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return false;

    if (but_1.Contains(g.IO.MousePos) && g.IO.MouseClicked[0])
    {
        if (state->selected[1])
        {
            state->clicked[1] = true;
            var->gui.registered = true;
        }
        state->selected[1] = true;
        state->selected[0] = false;
        var->gui.registration = true;
    }
    if (but_2.Contains(g.IO.MousePos) && g.IO.MouseClicked[0])
    {
        if (state->selected[0])
        {
            state->clicked[0] = true;
            var->gui.registered = true;
        }
        state->selected[1] = false;
        state->selected[0] = true;
        var->gui.registration = false;
    }
    if (state->selection_alpha[0] <= 0.11)
    {
        state->clicked[0] = false;
        var->gui.registered = false;
    }
    if (state->selection_alpha[1] <= 0.11)
    {
        state->clicked[1] = false;
        var->gui.registered = false;
    }

    gui->easing(state->rect[0], state->selected[0] ? clr->main.accent.Value : clr->main.text.Value, 12.f, dynamic_easing);
    gui->easing(state->rect[1], state->selected[1] ? clr->main.accent.Value : clr->main.text.Value, 12.f, dynamic_easing);

    //login
    gui->easing(state->rect_alpha[0], state->selected[0] ? 1.f : 0.25f, 6.f, static_easing);
    gui->easing(state->rect_alpha[1], state->selected[0] ? 0.48f : 0.06f, 6.f, static_easing);

    //regiser
    gui->easing(state->rect_alpha[2], state->selected[1] ? 1.f : 0.25f, 6.f, static_easing);
    gui->easing(state->rect_alpha[3], state->selected[1] ? 0.48f : 0.06f, 6.f, static_easing);

    gui->easing(state->text_alpha[0], state->selected[0] ? 1.f : 0.24f, 6.f, static_easing);
    gui->easing(state->text_alpha[1], state->selected[0] ? 1.f : 0.48f, 6.f, static_easing);
    gui->easing(state->text_alpha[2], state->selected[1] ? 1.f : 0.24f, 6.f, static_easing);
    gui->easing(state->text_alpha[3], state->selected[1] ? 1.f : 0.48f, 6.f, static_easing);

    gui->easing(state->selection_alpha[0], state->clicked[0] ? 0.1f : 1.f, 6.f, static_easing);
    gui->easing(state->selection_alpha[1], state->clicked[1] ? 0.1f : 1.f, 6.f, static_easing);

    //login
    draw->rect_filled(window->DrawList, but_2.Min, but_2.Max, draw->get_clr(rect_color.Value,state->rect_alpha[0]*state->selection_alpha[0]), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, but_2.Min, but_2.Max, draw->get_clr(rect_color.Value, state->rect_alpha[1]), SCALE(elements->widgets.rounding));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), but_2.Min + SCALE(elements->textfield.icon_zone_size.x + elements->log_reg_page.button_spacing, -1) / 2, but_2.Max + SCALE(elements->textfield.icon_zone_size.x + elements->log_reg_page.button_spacing, 0) / 2, draw->get_clr(clr->main.text, state->text_alpha[1]), gui->language("Login","Логин",true).data(), NULL, NULL, ImVec2(0.5f, 0.5f));
    
    //register
    draw->rect_filled(window->DrawList, but_1.Min, but_1.Max, draw->get_clr(rect_color.Value, state->rect_alpha[2] * state->selection_alpha[1]), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, but_1.Min, but_1.Max, draw->get_clr(rect_color.Value, state->rect_alpha[3]), SCALE(elements->widgets.rounding));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), but_1.Min + SCALE(elements->textfield.icon_zone_size.x + elements->log_reg_page.button_spacing, -1) / 2, but_1.Max + SCALE(elements->textfield.icon_zone_size.x + elements->log_reg_page.button_spacing, 0) / 2, draw->get_clr(clr->main.text, state->text_alpha[3]), gui->language("Register","Регистрация", true).data(), NULL, NULL, ImVec2(0.5f, 0.5f));

    return state->clicked;
}

bool c_widgets::settings_button()
{
    struct driver_state
    {
        float alpha[3]{ 0.f, 0.24, 0.06f };
        bool clicked{ false };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID("settings_button");

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + SCALE(elements->top_bar.height, elements->top_bar.height));

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return false;

    bool hovered = gui->item_hoverable(total, id, g.LastItemData.InFlags);
    bool pressed = hovered && g.IO.MouseClicked[0];
    driver_state* state = gui->anim_container<driver_state>(id);

    if (pressed)
        state->clicked = !state->clicked;

    if (state->clicked)
        var->gui.section_count = 1;
    else
        var->gui.section_count = 0;

    gui->easing(state->alpha[0], state->clicked ? 1.f : (hovered ? 0.1f : 0.f), 8.f, static_easing);
    gui->easing(state->alpha[1], state->clicked ? 1.f : (hovered ? 0.5f : 0.24f), 8.f, static_easing);
    gui->easing(state->alpha[2], state->clicked ? 0.48f : (hovered ? 0.2f : 0.06f), 8.f, static_easing);
    window->DrawList->PushClipRect(total.Min, total.Max, true);
    draw_background_blur(window->DrawList, var->winapi.device_dx11, var->winapi.device_context, 0, elements->redirect.blur);
    window->DrawList->PopClipRect();
    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->rect_filled(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.super, state->alpha[0]), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, total.Min, total.Max, draw->get_clr(clr->main.super, state->alpha[2]), SCALE(elements->widgets.rounding), 0, SCALE(1));
    draw->text_clipped(window->DrawList, font->get(icons_data, 13), total.Min, total.Max, draw->get_clr(clr->main.text, state->alpha[1]), "J", NULL, NULL, ImVec2(0.5f, 0.5f));

    return state->clicked;
}


bool c_widgets::redirect_button()
{
    struct driver_state
    {
        float alpha[2]{ 0.f, 0.24 };
        bool clicked{ false };
        float offset{ 0.f };
    };

    std::string widgets_id = "redirect_button";

    ImGuiWindow* window = gui->get_window();
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 buttonSize = SCALE(elements->redirect.size);  // Assuming elements->redirect.size is a vector of size (width, height)
    for(auto& name : elements->redirect.contacts_name)
    {

        const ImGuiID window_id = window->GetID((widgets_id + name).data());
        const ImVec2 pos = window->DC.CursorPos;

        gui->item_size(buttonSize, style.FramePadding.y); 

        if (!gui->item_add(ImRect(pos, pos + buttonSize),window_id)) { return false; }

        bool hovered = gui->item_hoverable(ImRect(pos, pos + buttonSize),window_id);
        bool pressed = hovered && g.IO.MouseClicked[0];
        driver_state* state = gui->anim_container<driver_state>(window_id); 
        if (pressed) {
            state->clicked = true;
            gui->open_url("https://google.com");
        }

        if (state->alpha[0] >= 0.99) { state->clicked = false; }

        gui->easing(state->alpha[0], state->clicked ? 1.f : (hovered ? 0.5f : 0.24f), 8.f, static_easing);
        gui->easing(state->alpha[1], state->clicked ? 1.f : (hovered ? 0.7f : 0.48f), 8.f, static_easing);

        draw->rect_filled(window->DrawList, pos, pos + buttonSize, draw->get_clr(clr->main.super, state->alpha[0]), SCALE(elements->widgets.rounding));
        draw->text_clipped(window->DrawList, font->get(suisse_intl_semi_bold_data, 16), pos, pos + buttonSize, draw->get_clr(clr->main.text, state->alpha[1]), name.data(), NULL, NULL, ImVec2(0.5f, 0.5f));

            gui->sameline(0.0f,SCALE(3.f));
    }

    return true;
}


bool c_widgets::back_button()
{
    struct driver_state
    {
        float alpha[2]{ 0.24f, 0.48 };
        bool clicked{ false };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    std::string ids = "back_button";
    ids += var->gui.stage_count;
    const ImGuiID id = window->GetID(ids.data());

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(SCALE(elements->product_page.back_button_height + elements->product_page.back_button_padding) + gui->text_size(font->get(suisse_intl_semi_bold_data, 13), gui->language("BACK", "Назад").data()).x, SCALE(elements->product_page.back_button_height)));

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return false;

    bool hovered = gui->item_hoverable(total, id, g.LastItemData.InFlags);
    bool pressed = hovered && g.IO.MouseClicked[0];
    driver_state* state = gui->anim_container<driver_state>(id);

    if (pressed)
    {
        state->clicked = true;
        var->gui.stage_count = 1;
    }
    if (state->alpha[0] >= 0.99)
        state->clicked = false;

    gui->easing(state->alpha[0], state->clicked ? 1.f : (hovered ? 0.5f : 0.24f), 8.f, static_easing);
    gui->easing(state->alpha[1], state->clicked ? 1.f : (hovered ? 0.7f : 0.48f), 8.f, static_easing);

    draw->text_clipped(window->DrawList, font->get(icons_data, 10), total.Min, ImVec2(total.Min.x + SCALE(elements->product_page.back_button_height), total.Max.y), draw->get_clr(clr->main.text, state->alpha[0]), "O", NULL, NULL, ImVec2(0.5f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_semi_bold_data, 13), total.Min, total.Max, draw->get_clr(clr->main.text, state->alpha[1]), gui->language("BACK", "Назад").data(), NULL, NULL, ImVec2(1.f, 0.5f));
    return state->clicked;
}

bool c_widgets::launch_button()
{
    struct driver_state
    {
        float alpha[2]{ 1.f, 1.f };
        float offset{ 0.f };
        bool clicked{ false };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    std::string ids = "launch_button_button";
    ids += var->gui.stage_count;
    const ImGuiID id = window->GetID(ids.data());

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(gui->content_avail().x, SCALE(elements->textfield.size.y)));

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return false;

    bool hovered = gui->item_hoverable(total, id, g.LastItemData.InFlags);
    bool pressed = hovered && g.IO.MouseClicked[0];
    driver_state* state = gui->anim_container<driver_state>(id);

    if (pressed)
    {
        state->clicked = true;
    }
    if (state->alpha[0] <= 0.01)
        state->clicked = false;


    gui->easing(state->alpha[0], state->clicked ? 0.f : 1.f, 8.f, static_easing);
    gui->easing(state->alpha[1], state->clicked ? 0.5f : 1.f, 8.f, static_easing);
    gui->easing(state->offset, state->clicked ? 3.f : 0.f, 50.f, static_easing);

    draw->rect_filled(window->DrawList, total.Min + SCALE(state->offset, state->offset), total.Max - SCALE(state->offset, state->offset), draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->rect_filled(window->DrawList, total.Min + SCALE(state->offset, state->offset), total.Max - SCALE(state->offset, state->offset), draw->get_clr(clr->main.accent, state->alpha[0]), SCALE(elements->widgets.rounding));
    //draw->rect(window->DrawList, total.Min + SCALE(state->offset, state->offset), total.Max - SCALE(state->offset, state->offset), draw->get_clr(clr->main.text, 0.48), SCALE(elements->widgets.rounding));

    draw->text_clipped(window->DrawList, font->get(icons_data, 10), total.Min - ImVec2(gui->text_size(font->get(suisse_intl_medium_data, 16), gui->language("Launch", "Запуск").data()).x + SCALE(elements->widgets.spacing.x), 0) / 2, total.Max - ImVec2(gui->text_size(font->get(suisse_intl_medium_data, 16), gui->language("Launch", "Запуск").data()).x + SCALE(elements->widgets.spacing.x), 0) / 2, draw->get_clr(clr->main.text, state->alpha[1]), "E", NULL, NULL, ImVec2(0.5f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), total.Min + ImVec2(gui->text_size(font->get(icons_data, 10), "E").x + SCALE(elements->widgets.spacing.x), - SCALE(2)) / 2, total.Max + ImVec2(gui->text_size(font->get(icons_data, 10), "E").x + SCALE(elements->widgets.spacing.x), 0) / 2, draw->get_clr(clr->main.text, state->alpha[1]), gui->language("Launch", "Запуск").data(), NULL, NULL, ImVec2(0.5f, 0.5f));
    
    return pressed;
}

bool c_widgets::update_button()
{
    struct driver_state
    {
        float alpha[4]{ 0.24f, 0.48f, 0.f, 0.06f };
        float offset{ 0.f };
        bool clicked{ false };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    std::string ids = "update_button";
    ids += var->gui.stage_count;
    const ImGuiID id = window->GetID(ids.data());

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(gui->content_avail().x, SCALE(elements->textfield.size.y)));

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return false;

    bool hovered = gui->item_hoverable(total, id, g.LastItemData.InFlags);
    bool pressed = hovered && g.IO.MouseClicked[0];
    driver_state* state = gui->anim_container<driver_state>(id);

    if (pressed)
    {
        state->clicked = true;
    }
    if (state->alpha[2] >= 0.90)
        state->clicked = false;

    gui->easing(state->alpha[0], state->clicked ? 1.f : 0.24f, 8.f, static_easing);
    gui->easing(state->alpha[1], state->clicked ? 1.f : 0.48f, 8.f, static_easing);
    gui->easing(state->alpha[2], state->clicked ? 1.f : 0.f, 8.f, static_easing);
    gui->easing(state->alpha[3], state->clicked ? 0.48f : 0.06f, 8.f, static_easing);
    gui->easing(state->offset, state->clicked ? 3.f : 0.f, 50.f, static_easing);

    draw->rect_filled(window->DrawList, total.Min + SCALE(state->offset, state->offset), total.Max - SCALE(state->offset, state->offset), draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->rect_filled(window->DrawList, total.Min + SCALE(state->offset, state->offset), total.Max - SCALE(state->offset, state->offset), draw->get_clr(clr->main.accent, state->alpha[2]), SCALE(elements->widgets.rounding));
    //draw->rect(window->DrawList, total.Min + SCALE(state->offset, state->offset), total.Max - SCALE(state->offset, state->offset), draw->get_clr(clr->main.text, state->alpha[3]), SCALE(elements->widgets.rounding));

    draw->text_clipped(window->DrawList, font->get(icons_data, 12), total.Min - ImVec2(gui->text_size(font->get(suisse_intl_medium_data, 16), gui->language("Update", "Обновить").data()).x + SCALE(elements->widgets.spacing.x), 0) / 2, total.Max - ImVec2(gui->text_size(font->get(suisse_intl_medium_data, 16), gui->language("Update", "Обновить").data()).x + SCALE(elements->widgets.spacing.x), 0) / 2, draw->get_clr(clr->main.text, state->alpha[0]), "Z", NULL, NULL, ImVec2(0.5f, 0.5f));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), total.Min + ImVec2(gui->text_size(font->get(icons_data, 12), "Z").x + SCALE(elements->widgets.spacing.x), -SCALE(2)) / 2, total.Max + ImVec2(gui->text_size(font->get(icons_data, 12), "Z").x + SCALE(elements->widgets.spacing.x), 0) / 2, draw->get_clr(clr->main.text, state->alpha[1]), gui->language("Update", "Обновить").data(), NULL, NULL, ImVec2(0.5f, 0.5f));

    return state->clicked;
}

void c_widgets::music_buttons(std::string_view widgets_id)
{
    struct music_buttons_state
    {
        bool loop{ false };
        float repeat_alpha{ 0.4f };
        float volume{ 1.f };
        bool is_playing{ false };
        float play_alpha{ 1.f };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems) return;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(widgets_id.data());

    const ImVec2 pos = window->DC.CursorPos;

    float available_w = window->WorkRect.Max.x - pos.x - SCALE(20.f);

    const ImRect total(
        pos,
        pos + ImVec2(available_w, SCALE(60.f))
    );

    gui->item_size(total, g.Style.FramePadding.y);
    if (!gui->item_add(total, id)) return;

    music_buttons_state* state = gui->anim_container<music_buttons_state>(id);

    float btn = SCALE(24.f);
    float gap = SCALE(16.f);
    float side_btn = SCALE(20.f);

    float center_y = total.Min.y + total.GetHeight() * 0.5f;
    float main_btn_y = center_y - btn * 0.5f;
    float side_y = center_y - side_btn * 0.5f; // Убрал лишнее смещение + SCALE(3.f), чтобы было ровно по центру

    float group_w = btn * 3 + gap * 2;
    float start = total.Min.x + (total.GetWidth() - group_w) * 0.5f;

    ImRect prev_btn(
        { start,                         main_btn_y },
        { start + btn,                   main_btn_y + btn }
    );
    ImRect play_btn(
        { start + btn + gap,             main_btn_y },
        { start + btn * 2 + gap,         main_btn_y + btn }
    );
    ImRect next_btn(
        { start + (btn + gap) * 2,       main_btn_y },
        { start + (btn + gap) * 2 + btn, main_btn_y + btn }
    );

    ImRect repeat_btn(
        { total.Min.x + SCALE(12.f),            side_y },
        { total.Min.x + SCALE(12.f) + side_btn, side_y + side_btn }
    );
    ImRect volume_btn(
        { total.Max.x - side_btn - SCALE(12.f), side_y },
        { total.Max.x - SCALE(12.f),            side_y + side_btn }
    );

    // Исправление 2: Изменил -= на +=, чтобы скролл колесиком вверх прибавлял звук
    if (ImGui::IsMouseHoveringRect(volume_btn.Min, volume_btn.Max, false))
    {
        if (ImGui::IsMouseClicked(ImGui::GetIO().MouseDown[0]))
        {
            state->volume = 0;
        }
        state->volume += g.IO.MouseWheel * 0.05f;
        state->volume = ImClamp(state->volume, 0.f, 1.f);
        ma_engine_set_volume(&var->music_player.manager.engine, state->volume);
    }
    

    // volume icon

    if (state->volume <= 0.f)
        draw->text_clipped(window->DrawList, font->get(icons_data, 16), volume_btn.Min, volume_btn.Max, draw->get_clr(clr->main.text), "0", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    else if (state->volume < 0.33f)
        draw->text_clipped(window->DrawList, font->get(icons_data, 18), volume_btn.Min, volume_btn.Max, draw->get_clr(clr->main.text), "1", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    else if (state->volume < 0.66f)
        draw->text_clipped(window->DrawList, font->get(icons_data, 18), volume_btn.Min, volume_btn.Max, draw->get_clr(clr->main.text), "2", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    else
        draw->text_clipped(window->DrawList, font->get(icons_data, 18), volume_btn.Min, volume_btn.Max, draw->get_clr(clr->main.text), "3", nullptr, nullptr, ImVec2(0.8f, 0.5f), nullptr);

    if (ImGui::IsMouseHoveringRect(volume_btn.Min, volume_btn.Max, false))
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (g.IO.MouseClicked[0])
        {
            state->volume = 0;
        }

    }


    gui->easing(state->repeat_alpha, state->loop ? 1.f : 0.4f, 6.f, static_easing);
    gui->easing(state->play_alpha, var->music_player.current_song.repeat ? 0.6f : 1.f, 6.f, static_easing);


        
    if (ImGui::IsMouseHoveringRect(play_btn.Min, play_btn.Max, false))
    {
        if (g.IO.MouseClicked[0])
        {
            var->music_player.current_song.play = !var->music_player.current_song.play;
            pause_song(var->music_player.manager);
        }
    }
    if (var->music_player.current_song.play)
    {
        draw->text_clipped(window->DrawList, font->get(icons_data, 18), play_btn.Min, play_btn.Max, draw->get_clr(ImVec4(1, 1, 1, 1)), "g", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    }
    else
    {
        draw->text_clipped(window->DrawList, font->get(icons_data, 18), play_btn.Min, play_btn.Max, draw->get_clr(ImVec4(1, 1, 1, 1)), "E", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    }

    //prev

    draw->text_clipped(window->DrawList, font->get(icons_data, 18), prev_btn.Min, prev_btn.Max, draw->get_clr(clr->main.text), "<", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    if (ImGui::IsMouseHoveringRect(prev_btn.Min, prev_btn.Max, false))
    {
        if (g.IO.MouseClicked[0]) { /* логика prev */ }
    }

    //next

    draw->text_clipped(window->DrawList, font->get(icons_data, 18), next_btn.Min, next_btn.Max, draw->get_clr(clr->main.text), ">", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    if (ImGui::IsMouseHoveringRect(next_btn.Min, next_btn.Max, false))
    {
        if (g.IO.MouseClicked[0]) { /* логика next */ }
    }

    // repeat
    if (var->music_player.current_song.repeat)
    {
        draw->text_clipped(window->DrawList, font->get(icons_data, 22), repeat_btn.Min, repeat_btn.Max, draw->get_clr(clr->main.text,state->repeat_alpha), "t", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);
    }
    else
    {
        draw->text_clipped(window->DrawList, font->get(icons_data, 22), repeat_btn.Min, repeat_btn.Max, draw->get_clr(clr->main.text,state->repeat_alpha), "r", nullptr, nullptr, ImVec2(0.5f, 0.5f), nullptr);

    }

    if (ImGui::IsMouseHoveringRect(repeat_btn.Min, repeat_btn.Max, false))
    {
        if (g.IO.MouseClicked[0])
        {
            state->loop = !state->loop;
            var->music_player.current_song.repeat = state->loop;
            set_loop(var->music_player.manager, state->loop);
        }
    }

}

void c_widgets::selection_buttons(std::string_view widgets_id, std::string_view name, std::string_view p1, std::string_view p2, int& variable)
{
    struct reg_log_buttons_state
    {
        float rect_alpha[4]{ 1.f, 0.48, 0.f, 0.06 };
        float text_alpha[4]{ 1.f, 1.f, 0.12, 0.48 };
    };

    ImGuiWindow* window = gui->get_window();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(widgets_id.data());

    reg_log_buttons_state* state = gui->anim_container<reg_log_buttons_state>(id);

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total(pos, pos + ImVec2(gui->content_avail().x, SCALE(elements->textfield.size.y)));
    const ImRect but_2(ImVec2(total.Max.x - SCALE(elements->selection.padding * 2 + elements->selection.spacing) - gui->text_size(font->get(suisse_intl_medium_data, 16), p2.data()).x - (widgets_id == "Language" ? SCALE(elements->selection.img_size.x) : gui->text_size(font->get(icons_data, 12), "N").x), total.Min.y), total.Max);
    const ImRect but_1(ImVec2(but_2.Min.x - SCALE(elements->selection.padding * 2 + elements->selection.spacing + elements->selection.but_spacing) - gui->text_size(font->get(suisse_intl_medium_data, 16), p1.data()).x - (widgets_id == "Language" ? SCALE(elements->selection.img_size.x) : gui->text_size(font->get(icons_data, 12), "M").x), total.Min.y), ImVec2(but_2.Min.x - SCALE(elements->selection.but_spacing), but_2.Max.y));

    gui->item_size(total, style.FramePadding.y);
    if (!gui->item_add(total, id))
        return;

    if (but_1.Contains(g.IO.MousePos) && g.IO.MouseClicked[0])
    {
        variable = 0;
    }
    if (but_2.Contains(g.IO.MousePos) && g.IO.MouseClicked[0])
    {
        variable = 1;
    }
    if (widgets_id == "Language" && (0.1f <= state->rect_alpha[0] && state->rect_alpha[0] <= 0.9f))
        var->gui.lang_changing = true;
    if(widgets_id == "Language" && !(0.1f <= state->rect_alpha[0] && state->rect_alpha[0] <= 0.9f))
        var->gui.lang_changing = false;

    gui->easing(state->rect_alpha[0], variable == 0 ? 1.f : 0.f, 8.f, static_easing);
    gui->easing(state->rect_alpha[1], variable == 0 ? 0.48f : 0.06f, 8.f, static_easing);
    gui->easing(state->rect_alpha[2], variable == 1 ? 1.f : 0.f, 8.f, static_easing);
    gui->easing(state->rect_alpha[3], variable == 1 ? 0.48f : 0.06f, 8.f, static_easing);

    gui->easing(state->text_alpha[0], variable == 0 ? 1.f : 0.12f, 8.f, static_easing);
    gui->easing(state->text_alpha[1], variable == 0 ? 1.f : 0.48f, 8.f, static_easing);
    gui->easing(state->text_alpha[2], variable == 1 ? 1.f : 0.12f, 8.f, static_easing);
    gui->easing(state->text_alpha[3], variable == 1 ? 1.f : 0.48f, 8.f, static_easing);

    draw->rect_filled(window->DrawList, total.Min, ImVec2(but_1.Min.x - SCALE(elements->widgets.spacing.x), but_1.Max.y), draw->get_clr(clr->main.text, 0.02), SCALE(elements->widgets.rounding));
    //draw->rect(window->DrawList, total.Min, ImVec2(but_1.Min.x - SCALE(elements->widgets.spacing.x), but_1.Max.y), draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), total.Min + SCALE(elements->child.padding.x, 0), total.Max, draw->get_clr(clr->main.text), name.data(), gui->text_end(name.data()), NULL, ImVec2(0.f, 0.5f));

    draw->rect_filled(window->DrawList, but_1.Min, but_1.Max, draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->rect_filled(window->DrawList, but_1.Min, but_1.Max, draw->get_clr(clr->main.accent, state->rect_alpha[0]), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, but_1.Min, but_1.Max, draw->get_clr(clr->main.text, state->rect_alpha[1]), SCALE(elements->widgets.rounding));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), but_1.Min - SCALE(0, 1), but_1.Max - SCALE(elements->selection.padding, 0), draw->get_clr(clr->main.text, state->text_alpha[1]), p1.data(), gui->text_end(p1.data()), NULL, ImVec2(1.f, 0.5f));
    draw->rect_filled(window->DrawList, but_2.Min, but_2.Max, draw->get_clr(clr->main.text, 0.04), SCALE(elements->widgets.rounding));
    draw->rect_filled(window->DrawList, but_2.Min, but_2.Max, draw->get_clr(clr->main.accent, state->rect_alpha[2]), SCALE(elements->widgets.rounding));
    draw->rect(window->DrawList, but_2.Min, but_2.Max, draw->get_clr(clr->main.text, state->rect_alpha[3]), SCALE(elements->widgets.rounding));
    draw->text_clipped(window->DrawList, font->get(suisse_intl_medium_data, 16), but_2.Min - SCALE(0, 1), but_2.Max - SCALE(elements->selection.padding, 0), draw->get_clr(clr->main.text, state->text_alpha[3]), p2.data(), gui->text_end(p2.data()), NULL, ImVec2(1.f, 0.5f));
    //if (widgets_id == "Language")
    //{
    //    draw->image_rounded(window->DrawList, var->gui.flags[0], ImVec2(but_1.Min.x + SCALE(elements->selection.padding), but_1.GetCenter().y - SCALE(elements->selection.img_size.y / 2)), ImVec2(but_1.Min.x + SCALE(elements->selection.padding + elements->selection.img_size.x), but_1.GetCenter().y + SCALE(elements->selection.img_size.y / 2)), ImVec2(0, 0), ImVec2(1, 1), draw->get_clr({ 1.f, 1.f, 1.f, state->text_alpha[1] }), SCALE(elements->version_card.rounding));
    //    draw->rect(window->DrawList, ImVec2(but_1.Min.x + SCALE(elements->selection.padding), but_1.GetCenter().y - SCALE(elements->selection.img_size.y / 2)), ImVec2(but_1.Min.x + SCALE(elements->selection.padding + elements->selection.img_size.x), but_1.GetCenter().y + SCALE(elements->selection.img_size.y / 2)), draw->get_clr(clr->main.text, 0.12), SCALE(elements->version_card.rounding), 0, SCALE(1));

    //    draw->image_rounded(window->DrawList, var->gui.flags[1], ImVec2(but_2.Min.x + SCALE(elements->selection.padding), but_2.GetCenter().y - SCALE(elements->selection.img_size.y / 2)), ImVec2(but_2.Min.x + SCALE(elements->selection.padding + elements->selection.img_size.x), but_2.GetCenter().y + SCALE(elements->selection.img_size.y / 2)), ImVec2(0, 0), ImVec2(1, 1), draw->get_clr({ 1.f, 1.f, 1.f, state->text_alpha[3] }), SCALE(elements->version_card.rounding));
    //    draw->rect(window->DrawList, ImVec2(but_2.Min.x + SCALE(elements->selection.padding), but_2.GetCenter().y - SCALE(elements->selection.img_size.y / 2)), ImVec2(but_2.Min.x + SCALE(elements->selection.padding + elements->selection.img_size.x), but_2.GetCenter().y + SCALE(elements->selection.img_size.y / 2)), draw->get_clr(clr->main.text, 0.12), SCALE(elements->version_card.rounding), 0, SCALE(1));
    //}
    //else
    //{
    //    draw->text_clipped(window->DrawList, font->get(icons_data, 12), but_1.Min + SCALE(elements->selection.padding, 0), but_1.Max, draw->get_clr(clr->main.text, state->text_alpha[1]), "N", NULL, NULL, ImVec2(0.f, 0.5f));
    //    draw->text_clipped(window->DrawList, font->get(icons_data, 12), but_2.Min + SCALE(elements->selection.padding, 0), but_2.Max, draw->get_clr(clr->main.text, state->text_alpha[3]), "M", NULL, NULL, ImVec2(0.f, 0.5f));
    //}
}
