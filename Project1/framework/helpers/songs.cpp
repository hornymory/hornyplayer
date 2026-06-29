#include <filesystem>
#include"../headers/includes.h"

#define MINIAUDIO_IMPLEMENTATION
#include"miniaudio.h"
#include "../headers/songs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define DR_FLAC_IMPLEMENTATION
#include "../headers/dr_flac.h"

void load_songs(SongsManager& manager)
{
    manager.isLoad = true;
    manager.songs.clear();

    if (!std::filesystem::exists(manager.music_path)) {
        manager.isLoad = false;
        return;
    }

    auto on_meta = [](void* pUserData, drflac_metadata* pMetadata) {
        Song* song = static_cast<Song*>(pUserData);

        if (!song || !pMetadata) return;

        if (pMetadata->type == DRFLAC_METADATA_BLOCK_TYPE_PICTURE) {
            const auto& picture = pMetadata->data.picture;

            if (picture.pictureDataSize > 0 && picture.pPictureData != nullptr) {
                song->icon_data.assign(
                    picture.pPictureData,
                    picture.pPictureData + picture.pictureDataSize
                );
            }
        }

        if (pMetadata->type == DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT) {
            const auto& vc = pMetadata->data.vorbis_comment;
            drflac_vorbis_comment_iterator it;
            drflac_init_vorbis_comment_iterator(&it, vc.commentCount, vc.pComments);
            drflac_uint32 len = 0;
            const char* comment = drflac_next_vorbis_comment(&it, &len);
            while (comment) {
                std::string s(comment, len);
                // переводим ключ в верхний регистр для сравнения
                std::string upper = s;
                std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

                if (upper.rfind("TITLE=", 0) == 0)
                    song->name = s.substr(6);
                else if (upper.rfind("ARTIST=", 0) == 0)
                    song->author = s.substr(7);
                comment = drflac_next_vorbis_comment(&it, &len);
            }
        }
        };

    for (const auto& entry : std::filesystem::directory_iterator(manager.music_path)) {
        if (!entry.is_regular_file()) continue;

        if (entry.path().extension() == L".flac") {
            Song newSong;

            newSong.path = entry.path().wstring();
            newSong.name = entry.path().stem().string();
            newSong.author = "Unknown Artist";

            drflac* pFlac = drflac_open_file_with_metadata_w(
                newSong.path.c_str(),
                on_meta,
                &newSong,
                nullptr
            );

            if (pFlac) {
                drflac_close(pFlac);
            }

            manager.songs.push_back(std::move(newSong));
        }
    }

    manager.isLoad = false;
}

void EnsureSongTextureCreated(Song& song, ID3D11Device* device)
{
    if (song.texture_created)
        return;

    song.texture_created = true;

    if (!device)
        return;

    if (song.icon_data.empty())
        return;

    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels = stbi_load_from_memory(
        song.icon_data.data(),
        static_cast<int>(song.icon_data.size()),
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    if (!pixels || width <= 0 || height <= 0) {
        if (pixels)
            stbi_image_free(pixels);

        song.icon_data.clear();
        song.icon_data.shrink_to_fit();
        return;
    }

    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = static_cast<UINT>(width);
    textureDesc.Height = static_cast<UINT>(height);
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = pixels;
    initData.SysMemPitch = static_cast<UINT>(width * 4);
    initData.SysMemSlicePitch = 0;

    ID3D11Texture2D* texture = nullptr;

    HRESULT hr = device->CreateTexture2D(
        &textureDesc,
        &initData,
        &texture
    );

    if (SUCCEEDED(hr) && texture) {
        ID3D11ShaderResourceView* srv = nullptr;

        hr = device->CreateShaderResourceView(
            texture,
            nullptr,
            &srv
        );

        if (SUCCEEDED(hr)) {
            song.texture = srv;
        }

        texture->Release();
    }

    stbi_image_free(pixels);

    song.icon_data.clear();
    song.icon_data.shrink_to_fit();
}








void init_audio(SongsManager& manager)
{
    if (manager.engine_initialized)
        return;
    ma_result result = ma_engine_init(NULL, &manager.engine);
    if (result == MA_SUCCESS)
        manager.engine_initialized = true;
}
ma_result play_song(SongsManager& manager, Song& song)
{
    if (!manager.engine_initialized)
        return (ma_result)-999;

    if (manager.sound_loaded)
    {
        ma_sound_uninit(&manager.current_sound);
        manager.sound_loaded = false;
    }

    ma_result res = ma_sound_init_from_file_w(&manager.engine, song.path.c_str(), 0, NULL, NULL, &manager.current_sound);

    if (res == MA_SUCCESS)
    {
        manager.sound_loaded = true;

        ma_sound_set_volume(&manager.current_sound, manager.volume);
        ma_sound_set_looping(&manager.current_sound, song.repeat); // <-- добавить

        ma_sound_start(&manager.current_sound);

        ma_uint64 length_frames = 0;
        ma_sound_get_length_in_pcm_frames(&manager.current_sound, &length_frames);
        ma_uint32 sample_rate = ma_engine_get_sample_rate(&manager.engine);
        song.full_time = (float)length_frames / sample_rate;
    }

    return res; // обязательно добавь
}
void update_song_progress(SongsManager& manager, Song& song)
{
    if (!manager.sound_loaded)
        return;

    ma_uint64 cursor_frames = 0;
    ma_sound_get_cursor_in_pcm_frames(&manager.current_sound, &cursor_frames);
    ma_uint32 sample_rate = ma_engine_get_sample_rate(&manager.engine);

    song.current_time = (float)cursor_frames / sample_rate;
}

void seek_song(SongsManager& manager, Song& song, float percent)
{
    if (!manager.sound_loaded)
        return;

    percent = ImClamp(percent, 0.f, 1.f);

    ma_uint64 length_frames = 0;
    ma_sound_get_length_in_pcm_frames(&manager.current_sound, &length_frames);

    ma_uint64 target_frame = (ma_uint64)(length_frames * percent);

    ma_sound_seek_to_pcm_frame(&manager.current_sound, target_frame);
}
void pause_song(SongsManager& manager)
{
    if (!manager.sound_loaded)
        return;
    
    if (ma_sound_is_playing(&manager.current_sound))
        ma_sound_stop(&manager.current_sound);
    else
        ma_sound_start(&manager.current_sound);
}
void set_volume(SongsManager& manager, float volume)
{
    if (!manager.sound_loaded)
        return;
    volume = ImClamp(volume, 0.f, 1.f);
    ma_sound_set_volume(&manager.current_sound, volume);
}
void set_loop(SongsManager& manager, bool loop)
{
    if (!manager.sound_loaded)
        return;

    ma_sound_set_looping(&manager.current_sound, loop);
}
