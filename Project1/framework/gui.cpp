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
				SCALE(var->window.rounding)
			);
		}




		gui->easing(var->gui.stage_alpha, var->gui.active_stage == var->gui.stage_count ? 1.f : 0.f, 6.f, static_easing);
		if (var->gui.stage_alpha == 0.f)
			var->gui.active_stage = var->gui.stage_count;

		gui->easing(var->gui.content_alpha, var->gui.active_section == var->gui.section_count ? 1.f : 0.f, 6.f, static_easing);

		if (var->gui.content_alpha == 0.f)
			var->gui.active_section = var->gui.section_count;

		gui->set_pos(SCALE(0, 0), pos_all);
		gui->begin_content("content", ImVec2(gui->window_size().x, gui->window_size().y - (SCALE(elements->window.padding.y) * 2)), SCALE(elements->window.padding), SCALE(elements->widgets.spacing.x, elements->window.padding.y), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
		{

			gui->push_var(style_var_alpha, var->gui.content_alpha * var->gui.stage_alpha);
			//gui->dummy(ImVec2(0,elements->top_bar.padding.y));
			if (var->gui.active_section == 0)
			{
				Song play_song = {};

				gui->begin_content("music_list", ImVec2(gui->content_avail().x * 0.6f, gui->content_avail().y), SCALE(0, 0), SCALE(elements->window.padding), window_flags_no_scrollbar);
				{
					if (var->gui.active_stage == 1)
					{


						widgets->background_songs("back_songs");
						gui->begin_def_child("songs_scroll", ImVec2(gui->content_avail().x - elements->music_player.pad, gui->content_avail().y - elements->music_player.pad * 2), 0, window_flags_no_scrollbar); // window_flags_none или 0
						{
							for (auto& song : var->music_player.manager.songs)
							{
								if (widgets->song_card(song.name, song))
								{
									for (auto& s : var->music_player.manager.songs)
										s.play = false;
									song.play = true;
									var->music_player.current_song = song;
								}
							}
						}
						gui->end_def_child();

					}

				}
				gui->end_content();
				gui->sameline(0.f, SCALE(elements->window.padding.x / 2));
				gui->begin_content("music_player", ImVec2(gui->content_avail().x - elements->music_player.pad * 2, gui->content_avail().y), SCALE(0, 0), SCALE(elements->window.padding), window_flags_no_scroll_with_mouse | window_flags_no_scrollbar);
				{
					update_song_progress(var->music_player.manager, var->music_player.current_song);
					widgets->player("music_player_panel", var->music_player.current_song);

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
			gui->loading();
		}
		gui->end_content();
		gui->pop_var();

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