#include <filesystem>
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