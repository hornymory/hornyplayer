#pragma once
#include<string>
#include<vector>
#include<d3d11.h>
#include"miniaudio.h"

struct Song
{
	std::wstring path;
	std::string name;
	std::string author;
	std::vector<unsigned char> icon_data;
	

	ID3D11ShaderResourceView* texture{ nullptr };

	bool texture_created{ false };
	bool play{ false };
	float current_time{ 0.f };
	float full_time{ 0.f };
};


struct SongsManager
{	
	std::wstring music_path;
	bool isLoad{ false };
	std::vector<Song> songs;

	ma_engine engine{};
	ma_sound current_sound{};
	bool engine_initialized{ false };
	bool sound_loaded{ false };
	float volume{ 0.5f };


};

void load_songs(SongsManager& manager);
void EnsureSongTextureCreated(Song& song, ID3D11Device* device);

void init_audio(SongsManager& manager);
ma_result play_song(SongsManager& manager, Song& song);
void update_song_progress(SongsManager& manager, Song& song);
void seek_song(SongsManager& manager, Song& song, float percent);
void pause_song(SongsManager& manager);
void set_volume(SongsManager& manager, float volume);
void set_loop(SongsManager& manager, bool loop);

