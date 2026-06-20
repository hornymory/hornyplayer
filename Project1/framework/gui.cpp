#include "headers/includes.h"
#include "headers/widgets.h"
void c_gui::render()
{
	gui->initialize();

	gui->push_var(style_var_window_shadow_size, SCALE(0, 0));
	gui->set_next_window_pos(ImVec2(0, 0));
	gui->set_next_window_size(SCALE(var->window.size));
	gui->move_window(var->winapi.hwnd, var->winapi.rc);
	gui->begin("menu", nullptr, var->window.flags);
	{


		gui->drag_window();
		gui->set_style();
		gui->draw_decorations();
		gui->draw_background();

		if (var->gui.resize_fade > 0.01f)
		{
			draw->rect_filled(
				gui->window_drawlist(),
				gui->window_pos(),
				gui->window_pos() + gui->window_size(),
				//draw->get_clr(clr->window.background, 0.22f * var->gui.resize_fade),
				SCALE(var->window.rounding)
			);
		}




		gui->easing(var->gui.stage_alpha, var->gui.active_stage == var->gui.stage_count ? 1.f : 0.f, 6.f, static_easing);
		if (var->gui.stage_alpha == 0.f)
			var->gui.active_stage = var->gui.stage_count;

		gui->easing(var->gui.content_alpha, var->gui.active_section == var->gui.section_count ? 1.f : 0.f, 6.f, static_easing);

		if (var->gui.content_alpha == 0.f)
			var->gui.active_section = var->gui.section_count;

		//gui->easing(var->window.size.x, var->gui.stage_count > 0 ? var->window.new_width : var->window.default_size.x, 800.f, static_easing);
		//gui->easing(var->window.size.y, var->gui.stage_count > 1 ? (var->gui.section_count == 1 ? var->window.section_1_height : var->window.stage_2_height) : (var->gui.section_count == 1 ? var->window.section_1_height : var->window.default_size.y), 200.f, static_easing);

		gui->set_pos(SCALE(0, 0), pos_all);

		gui->begin_content("content", gui->window_size(), SCALE(elements->window.padding), SCALE(elements->widgets.spacing.x, elements->window.padding.y), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
		{


			//gui->push_var(style_var_alpha, var->gui.stage_alpha);
			//if (var->gui.active_stage > 0)
			//{
			//	widgets->top_bar("https://google.com","something", "17.03.2026");
			//	gui->sameline();
			//	widgets->settings_button();
			//}
			//gui->pop_var();
			gui->push_var(style_var_alpha, var->gui.content_alpha * var->gui.stage_alpha);
			//if (var->gui.active_section == 1)
			//{
			//	widgets->widgets_child("General", gui->language("General", "Общие", true), "O");
			//	{
			//		static bool launch_win{ true };
			//		widgets->checkbox_ex("Launch on Windows startup", gui->language("Launch on Windows startup", "Запуск при старте Windows", true), &launch_win);
			//		static bool start_min{ false };
			//		widgets->checkbox_ex("Start minimized to tray", gui->language("Start minimized to tray", "Запуск сворачивается в трей", true), &start_min);
			//		static bool show_up{ true };
			//		widgets->checkbox_ex("Show Update Notifications", gui->language("Show Update Notifications", "Показать уведомления об обновлениях", true), &show_up);
			//	}
			//	widgets->widgets_end_child();
			//	gui->set_screen_pos(GImGui->LastItemData.Rect.Max.y + SCALE(elements->widgets.spacing.y), pos_y);
			//	widgets->selection_buttons("Language", gui->language("Language", "Язык", true), "English", "Русский", var->gui.lang_count);

			//	widgets->widgets_child("Integration", gui->language("Integration", "Интеграция", true), "P");
			//	{
			//		static bool auto_in{ false };
			//		widgets->checkbox_ex("Auto-Inject on Game Launch", gui->language("Auto-Inject on Game Launch", "Автоинъекция при запуске игры", true), &auto_in);
			//		static bool enable_in{ true };
			//		widgets->checkbox_ex("Enable In-Game Overlay", gui->language("Enable In-Game Overlay", "Включение игрового наложения", true), &enable_in);
			//	}
			//	widgets->widgets_end_child();
			//	static int process_sel{ 1 };
			//	gui->set_screen_pos(GImGui->LastItemData.Rect.Max.y + SCALE(elements->widgets.spacing.y), pos_y);
			//	widgets->selection_buttons("Game Process Priority", gui->language("Game Process Priority", "Приоритет игрового процесса", true), gui->language("Normal", "Нормальный", true), gui->language("High", "Высокий", true), process_sel);

			//	widgets->widgets_child("Security", gui->language("Security", "Безопасность", true), "Q");
			//	{
			//		static bool auto_cl{ true };
			//		widgets->checkbox_ex("Automatically Clear Traces on Exit", gui->language("Automatically Clear Traces on Exit", "Автоматическая очистка следов при выходе", true), &auto_cl);
			//		static bool enable_hwid{ true };
			//		widgets->checkbox_ex("Enable HWID Spoofer", gui->language("Enable HWID Spoofer", "Включить HWID Spoofer", true), &enable_hwid);
			//		static bool self_d{ false };
			//		widgets->checkbox_ex("Self-Destruct on Analysis Detection", gui->language("Self-Destruct on Analysis Detection", "Самоуничтожение при обнаружении анализа", true), &self_d);
			//	}
			//	widgets->widgets_end_child();
			//}

			if(var->gui.active_section == 0)
			{
				//if (var->gui.active_stage == 0)
					//widgets->log_reg_page();
				Song play_song = {};

				gui->begin_content("music_list", ImVec2(gui->content_avail().x * 0.6f, gui->content_avail().y), SCALE(0, 0), SCALE(elements->window.padding), window_flags_no_scrollbar);
				{
					//std::cout << "songs:" << var->gui.manager.songs.size() ;
					if (var->gui.active_stage == 1)
					{


						widgets->background_songs();
						gui->set_pos(ImVec2(elements->music_player.pad, elements->music_player.pad + 81), pos_all);
						gui->begin_def_child("songs_scroll", ImVec2(gui->content_avail().x - elements->music_player.pad, gui->content_avail().y - elements->music_player.pad *2), 0, window_flags_no_scrollbar); // window_flags_none или 0
						{
							for (auto& song : var->gui.manager.songs)
							{
								if (widgets->song_card(song.name, song))
								{
									for (auto& s : var->gui.manager.songs)
										s.play = false;
									song.play = true;
									elements->music_player.current_song = song;
								}
							}
						}
						gui->end_def_child();

					}

				}
				gui->end_content();
				gui->sameline(0.f,SCALE(elements->window.padding.x/2));
				gui->begin_content("music_player", ImVec2(gui->content_avail().x, gui->content_avail().y), SCALE(0, 0), SCALE(elements->window.padding), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
				{
					widgets->player("music_player_panel", elements->music_player.current_song);
				}
				gui->end_content();
			}
			gui->pop_var();
		}
		gui->end_content();
		gui->pop_var();

		bool bg{ false };
		if (gui->is_window_cond(GetCurrentContext()->NavWindow, { "dropdown" }))
			bg = true;
		gui->easing(elements->loading.window_alpha, bg || var->gui.loading ? 1.f : 0.f, 8.f, static_easing);

		gui->set_pos(SCALE(0, 0), pos_all);
		gui->push_var(style_var_alpha, elements->loading.window_alpha);
		gui->begin_content("back_alpha", SCALE(0, 0), SCALE(0, 0), SCALE(0, 0));
		{
			//draw->rect_filled(gui->window_drawlist(), gui->window_pos(), gui->window_pos() + gui->window_size(), draw->get_clr(clr->window.background, 0.6), SCALE(var->window.rounding));
			gui->loading();
		}
		gui->end_content();
		gui->pop_var();

		//gui->move_window(var->winapi.hwnd, var->winapi.rc);
	}
	gui->end();

	if (ImGui::IsKeyPressed(ImGuiKey_Equal) && ImGui::IsKeyDown(ImGuiKey_LeftAlt) && var->gui.stored_dpi < 200)
	{
		var->gui.stored_dpi += 10;
		var->gui.dpi_changed = true;
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Minus) && ImGui::IsKeyDown(ImGuiKey_LeftAlt) && var->gui.stored_dpi > 80)
	{
		var->gui.stored_dpi -= 10;
		var->gui.dpi_changed = true;
	}

	if (var->gui.dpi != var->gui.stored_dpi / 100.f)
	{
		var->gui.dpi_changed = true;
		var->gui.update_size = true;
	}
}
