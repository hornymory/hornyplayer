#include"../headers/emoj.h"


static std::vector<text_snowflake> g_text_snow;
static bool g_text_snow_inited = false;

static float rand01(int seed)
{
    float x = sinf(seed * 12.9898f) * 43758.5453f;
    return x - floorf(x);
}

void draw_text_snow(
    ImDrawList* draw_list,
    const ImRect& area,
    const std::vector<std::string>& texts,
    ImU32 color,
    int count,
    float time
)
{
    if (texts.empty() || count <= 0)
        return;

    if (!g_text_snow_inited || g_text_snow.size() != count)
    {
        g_text_snow.clear();
        g_text_snow.resize(count);

        for (int i = 0; i < count; i++)
        {
            float rx = rand01(i * 17 + 1);
            float ry = rand01(i * 29 + 2);
            float rs = rand01(i * 41 + 3);
            float rp = rand01(i * 53 + 4);

            g_text_snow[i].pos = ImVec2(
                area.Min.x + rx * area.GetWidth(),
                area.Min.y + ry * area.GetHeight()
            );

            g_text_snow[i].base_speed = 20.0f + rs * 80.0f;
            g_text_snow[i].phase = rp * 6.28318f;
            g_text_snow[i].size = 12.0f + rand01(i * 67 + 5) * 10.0f;
            g_text_snow[i].text_index = i % texts.size();
        }

        g_text_snow_inited = true;
    }

    draw_list->PushClipRect(area.Min, area.Max, true);

    for (int i = 0; i < count; i++)
    {
        auto& p = g_text_snow[i];

        float speed_mod = fabsf(sinf(time * 1.3f + p.phase));
        float speed = p.base_speed * (0.35f + speed_mod);

        p.pos.y += speed * ImGui::GetIO().DeltaTime;

        float sway = sinf(time * 1.7f + p.phase) * 12.0f;

        if (p.pos.y > area.Max.y + p.size)
        {
            p.pos.y = area.Min.y - p.size;
            p.pos.x = area.Min.x + rand01(i * 91 + (int)(time * 100.0f)) * area.GetWidth();
        }

        const std::string& txt = texts[p.text_index];

        ImVec2 text_pos(
            p.pos.x + sway,
            p.pos.y
        );

        ImVec2 text_max(
            text_pos.x + 80.0f,
            text_pos.y + p.size + 8.0f
        );

        draw->text_clipped(
            draw_list,
            font->get(suisse_intl_medium_data, (int)p.size),
            text_pos,
            text_max,
            color,
            txt.c_str(),
            gui->text_end(txt.c_str())
        );
    }

    draw_list->PopClipRect();
}