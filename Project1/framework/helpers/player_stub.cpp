#include "../headers/includes.h"

void c_video_player::video_decode_thread()
{
}

bool c_video_player::init_video(const char* filename)
{
    (void)filename;
    return false;
}

void c_video_player::cleanup_video()
{
}

void c_video_player::seek_video(double pos)
{
    (void)pos;
}

void c_video_player::render(std::string_view id, const ImVec2& size, std::string_view name)
{
    (void)id;
    (void)size;
    (void)name;
}

int c_video_player::audio_callback(const void* input, void* output, unsigned long frame_count, const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags status_flags, void* user_data)
{
    (void)input;
    (void)output;
    (void)frame_count;
    (void)time_info;
    (void)status_flags;
    (void)user_data;
    return paContinue;
}