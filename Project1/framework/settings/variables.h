#pragma once
#include <string>
#include <vector>
#include "imgui.h"
#include "../headers/flags.h"
#include <wtypes.h>

#include <d3d11.h>
#include <dxgi.h>

#include "imgui_internal.h"

#include <atomic>
#include <mutex>
#include <thread>
#include <deque>
#include <functional>
#include <memory>
#include "../../thirdparty/portaudio/inc/portaudio.h"       
#include "../headers/songs.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
}

struct video_state
{
	AVFormatContext* format_ctx = nullptr;
	AVCodecContext* video_codec_ctx = nullptr;
	int video_stream = -1;
	std::atomic<bool> playing{ false };
	std::atomic<bool> seeking{ false };
	std::atomic<double> current_pts{ 0 };
	std::atomic<double> seek_target{ 0 };
	double duration = 0;
	double frame_rate = 0;
	int width = 0;
	int height = 0;

	std::mutex frame_mutex;
	std::mutex decode_mutex;
	std::deque<AVFrame*> frame_queue;
	std::atomic<bool> quit{ false };
	std::atomic<bool> eof{ false };

	SwsContext* sws_ctx = nullptr;
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* texture_srv = nullptr;

	AVCodecContext* audio_codec_ctx = nullptr;
	int audio_stream = -1;
	SwrContext* swr_ctx = nullptr;
	std::deque<AVFrame*> audio_frame_queue;
	std::mutex audio_frame_mutex;
	std::atomic<double> audio_clock{ 0 };
	PaStream* audio_stream_pa = nullptr;

	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = 44100;
	int out_channels = 2;
	AVChannelLayout out_ch_layout{};
	std::atomic<float> volume{ 0 };

	std::atomic<int> seek_completed{ 0 };
};

class c_video_player
{
public:
	void video_decode_thread();
	bool init_video(const char* filename);
	void cleanup_video();
	void seek_video(double pos);
	void render(std::string_view id, const ImVec2& size, std::string_view name);
	static int audio_callback(const void* input, void* output, unsigned long frame_count, const PaStreamCallbackTimeInfo* time_info, PaStreamCallbackFlags status_flags, void* user_data);

	video_state state;
	bool texture_initialized = false;
	std::thread decode_thread;

	ID3D11Device* g_pd3dDevice{ nullptr };
	ID3D11DeviceContext* g_pd3dDeviceContext{ nullptr };
};


struct ID3D11ShaderResourceView;
struct ID3D11Device;
struct ID3D11DeviceContext;

class c_variables
{
public:

	struct
	{
		HWND hwnd{};
		RECT rc{};

		bool is_fullscreen{ false };
		RECT restore_rect{};

		ID3D11Device* device_dx11{ nullptr };
		ID3D11DeviceContext* device_context{ nullptr };
		IDXGISwapChain* swap_chain{ nullptr };
	} winapi;

	struct
	{
		ImVec2 size{ 1000, 700 };
		float rounding{ 15 };
		float border_size{ 1 };
		float scroll_bar_width{ 4 };
		float scroll_bar_rounding{ 100 };
		window_flags flags{ window_flags_no_saved_settings | window_flags_no_nav | window_flags_no_decoration | window_flags_no_scrollbar | window_flags_no_scroll_with_mouse | window_flags_no_background };
	} window;

	struct
	{
		bool registration{ false };
		SongsManager manager;
		bool registered{ true };
		std::string username{ "hornymory" };
		std::string password{ "123" };
		int lang_count{ 0 };
		bool lang_changing{ false };

		float stage_alpha{ 0.f };
		int active_stage{ 1 };
		int stage_count{ 1 };

		float content_alpha{ 0.f };
		int active_section{ 0 };
		int section_count{ 0 };

		float dpi = 1.f;
		int stored_dpi = 100;
		bool dpi_changed = true;
		bool update_size{ false };
		bool dragging{ false };

		bool loading = false;

		c_video_player cs_player;
		c_video_player apex_player;
		c_video_player fortnite_player;

		ID3D11ShaderResourceView* menu_background{ nullptr };
		ID3D11ShaderResourceView* decoration[2]{};
		ID3D11ShaderResourceView* img_for_versions[5]{};
		ID3D11ShaderResourceView* games[6]{};
		ID3D11ShaderResourceView* flags[2]{};
		bool resizing{ false };
		float resize_fade{ 0.30f };

	} gui;

	gui_style style;

};

inline std::unique_ptr<c_variables> var = std::make_unique<c_variables>();
