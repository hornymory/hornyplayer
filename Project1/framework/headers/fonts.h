#pragma once
#include "includes.h"

class c_font
{
public:
    void update();

    ImFont* get(std::vector<unsigned char> font_data, float size);

    bool is_russian_text(const std::string& str)
    {
        for (size_t i = 0; i + 1 < str.size(); i++)
        {
            unsigned char c1 = str[i];
            unsigned char c2 = str[i + 1];
            if (c1 >= 0xD0 && c1 <= 0xD3)
                return true;
        }
        return false;
    }

private:
    struct font_data
    {
        std::vector<unsigned char> data;
        float size;
        ImFont* font;

        bool operator==(const font_data& other) const
        {
            return data == other.data && size == other.size;
        }
    };

    void add(std::vector<unsigned char> font_data, float size);

    std::vector<font_data> data;
};

inline std::unique_ptr<c_font> font = std::make_unique<c_font>();