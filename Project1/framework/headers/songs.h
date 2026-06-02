#pragma once
#include<string>
#include<vector>
#include<d3d11.h>
struct Song
{
	std::wstring path;
	std::string name;
	std::string author;
	std::vector<unsigned char> icon_data;
	

	ID3D11ShaderResourceView* texture{ nullptr };

	bool texture_created{ false };
};


struct SongsManager
{	
	std::wstring music_path;
	bool isLoad{ false };
	std::vector<Song> songs;


};

void load_songs(SongsManager& manager);
void EnsureSongTextureCreated(Song& song, ID3D11Device* device);

