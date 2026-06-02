#include"../headers/includes.h"
#include "../headers/widgets.h"

void c_widgets::log_reg_page()
{
	//float target_pos_x = gui->window_width() - SCALE(elements->log_reg_page.log_reg_width);

	//gui->set_pos(ImVec2(target_pos_x, 0), pos_x);
	gui->begin_content("log_reg_zone", SCALE(elements->log_reg_page.log_reg_width, 0), SCALE(elements->log_reg_page.padding), SCALE(0, elements->window.padding.y), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
	{

		ImVec2 pos = gui->window_pos();
		ImVec2 size = gui->window_size();

		draw_background_blur(gui->window_drawlist(), var->winapi.device_dx11, var->winapi.device_context,SCALE(var->window.rounding),2);
		draw->rect_filled(
			gui->window_drawlist(),
			pos,                          
			pos + size,                   
			draw->get_clr(clr->main.text, 0.03f),
			SCALE(var->window.rounding)
		
		);
		draw->rect(
			gui->window_drawlist(),
			pos,
			pos + size,
			draw->get_clr(clr->main.text, 0.05f),
			SCALE(var->window.rounding),0,1
		);
		draw->circle(gui->window_drawlist(),  gui->window_pos() + SCALE(gui->window_size().x * 0.5f, elements->log_reg_page.padding.y + 25), SCALE(25), draw->get_clr(clr->main.super), 32, 1.0f);

		draw->text_clipped(gui->window_drawlist(), font->get(icons_data, 40),
			gui->window_pos() + SCALE(0, elements->log_reg_page.padding.y + 4),
			gui->window_pos() + gui->window_size(),
			draw->get_clr(clr->main.super), "B", NULL, NULL, ImVec2(0.5f, 0.f));
		


		draw->text_clipped(gui->window_drawlist(), font->get(suisse_intl_medium_data, 16), gui->window_pos() + SCALE(0, elements->log_reg_page.padding.y + elements->window.padding.y + elements->log_reg_page.shadow_radius * 2 - 4), gui->window_pos() + gui->window_size(), draw->get_clr(clr->main.text), "nevercare", NULL, NULL, ImVec2(0.5f, 0.f));
		draw->text_clipped(gui->window_drawlist(), font->get(suisse_intl_regular_data, 14), gui->window_pos() + ImVec2(0, SCALE(elements->log_reg_page.padding.y + elements->window.padding.y + elements->log_reg_page.shadow_radius * 2 + 1) + gui->text_size(font->get(suisse_intl_medium_data, 16), "think again.").y), gui->window_pos() + gui->window_size(), draw->get_clr(clr->main.text, 0.48), "play with comfort.", NULL, NULL, ImVec2(0.5f, 0.f));
		
		gui->easing(elements->log_reg_page.window_height, var->gui.registration ? (elements->textfield.size.y * 4 + elements->window.padding.y + elements->widgets.spacing.y * 2) : (elements->textfield.size.y * 3 + elements->window.padding.y + elements->widgets.spacing.y * 1), 12.f, dynamic_easing);
		gui->set_pos(ImVec2((gui->window_width() - SCALE(elements->textfield.size.x)) / 2, (gui->window_height() - SCALE(elements->log_reg_page.window_height)) / 2), pos_all);
		gui->begin_content("registration_zone", SCALE(elements->textfield.size.x, 0), SCALE(0, 0), SCALE(0, elements->window.padding.y), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
		{
			gui->begin_content("fields_zone", SCALE(elements->textfield.size.x, elements->log_reg_page.window_height - (elements->textfield.size.y + elements->window.padding.y)), SCALE(0, 0), SCALE(0, elements->widgets.spacing.y), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
			{
				static char email[20];
				if(var->gui.registration)
					widgets->text_field("email_field", "Email", "d", email, sizeof(email));
				static char username[20];
				widgets->text_field("username_field", "Username", "B", username, sizeof(username));
				static char password[20];
				widgets->text_field("password_field", "Password", "C", password, sizeof(password));
					


				if (!var->gui.registration && (var->gui.username == username && var->gui.password == password) && var->gui.registered)
					var->gui.stage_count = 1;
				if (var->gui.registration && email[0] != '\0' &&   username[0] != '\0' && password[0] != '\0' && var->gui.registered)
					var->gui.stage_count = 1;
			}
			gui->end_content();

			widgets->reg_log_buttons();
			
		}

		gui->end_content();

	

	}
	gui->end_content();


};

void c_widgets::product_page(c_video_player& player, int img_id, std::string_view name, std::string_view desc, std::string_view launches, std::string_view updated, std::string_view status, std::string_view online)
{
	gui->begin_content("product_desc_content", ImVec2(gui->content_max().x - SCALE(elements->player.size.x + elements->window.padding.x), gui->content_avail().y), SCALE(0, 0), SCALE(elements->widgets.spacing), window_flags_no_scrollbar | window_flags_no_scroll_with_mouse);
	{
		ImGuiContext& g = *GImGui;
		widgets->back_button();

		const ImRect game_rect(ImVec2(g.LastItemData.Rect.Min.x, g.LastItemData.Rect.Max.y + SCALE(elements->widgets.spacing.y)), ImVec2(g.LastItemData.Rect.Min.x + gui->content_avail().x, g.LastItemData.Rect.Max.y + SCALE(elements->widgets.spacing.y + elements->product_page.game_zone_height)));
		const ImRect desc_rect(ImVec2(game_rect.Min.x, game_rect.Max.y + SCALE(elements->product_page.back_button_padding)), gui->window_pos() + gui->window_size() - SCALE(0, elements->widgets.spacing.y * 2 + elements->info_card.height * 2));

		draw->image_rounded(gui->window_drawlist(), var->gui.img_for_versions[img_id], ImVec2(game_rect.Min.x, game_rect.GetCenter().y - SCALE(elements->product_page.img_size.y / 2)), ImVec2(game_rect.Min.x + SCALE(elements->product_page.img_size.x), game_rect.GetCenter().y + SCALE(elements->product_page.img_size.y / 2)), ImVec2(0, 0), ImVec2(1, 1), draw->get_clr({1.f, 1.f, 1.f, 1.f}), SCALE(elements->version_card.rounding));
	
		draw->text_clipped(gui->window_drawlist(), font->get(suisse_intl_medium_data, 19), game_rect.Min + SCALE(elements->product_page.img_size.x + elements->product_page.back_button_padding, 0), game_rect.Max, draw->get_clr(clr->main.text), name.data(), gui->text_end(name.data()), NULL, ImVec2(0.f, 0.5f));
	
		std::vector<std::string> lines = gui->wrap_text(font->get(suisse_intl_medium_data, 13), gui->content_avail().x, std::string(desc));

		ImVec2 desc_pos = desc_rect.Min;
		for (const auto& line : lines)
		{
			draw->text_clipped(gui->window_drawlist(), font->get(suisse_intl_medium_data, 13), desc_pos, desc_rect.Max, draw->get_clr(clr->main.text, 0.48), line.c_str());
			desc_pos.y += gui->text_size(font->get(suisse_intl_medium_data, 13), "A").y;
		}

		gui->set_screen_pos(ImVec2(desc_rect.Min.x, desc_rect.Max.y + SCALE(elements->widgets.spacing.y)), pos_all);
		gui->begin_group();
		{
			widgets->info_card("launches_2_id", "E", gui->language("Launches", "Запущено"), "12.679", (gui->window_size().x - SCALE(elements->widgets.spacing.x)) / 2);
			gui->sameline();
			widgets->info_card("updated_2_id", "W", gui->language("Updated", "Обновлено"), "22.07.25", (gui->window_size().x - SCALE(elements->widgets.spacing.x)) / 2);
		}
		gui->end_group();
		gui->begin_group();
		{
			widgets->info_card("status_2_id", "b", gui->language("Status", "Статус"), gui->language("Undetected", "Незамечен"), (gui->window_size().x - SCALE(elements->widgets.spacing.x)) / 2);
			gui->sameline();
			widgets->info_card("online_2_id", "a", gui->language("Online", "Онлайн"), "146", (gui->window_size().x - SCALE(elements->widgets.spacing.x)) / 2);
		}
		gui->end_group();
	}
	gui->end_content();
	gui->sameline();
	gui->begin_content("player_content", ImVec2(SCALE(elements->player.size.x), gui->content_avail().y), SCALE(0, 0), SCALE(elements->widgets.spacing), window_flags_no_scrollbar | window_flags_no_scroll_with_mouse);
	{
		ImVec2 right_pos = gui->window_pos();
		ImVec2 right_size = gui->window_size();
		ImVec2 right = ImVec2(right_pos.x + right_size.x, right_pos.y + SCALE(260.f));

		draw->image_rounded(gui->window_drawlist(), var->gui.games[img_id], right_pos, right, ImVec2(0, 0), ImVec2(1, 1), draw->get_clr({ 1.f,1.f,1.f,1.f }), SCALE(elements->widgets.rounding));

		draw->rect(gui->window_drawlist(), right_pos, right, draw->get_clr(clr->main.text, 0.06f),SCALE(elements->widgets.rounding));
		gui->set_screen_pos(ImVec2(right_pos.x, right_pos.y + right_size.y - SCALE((elements->textfield.size.y + 8.f) * 2 + elements->widgets.spacing.y)),pos_all);
		if (widgets->launch_button())
			var->gui.loading = true;
		widgets->update_button();

	}
	gui->end_content();

};