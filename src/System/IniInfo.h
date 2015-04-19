
#pragma once

#include <windows.h>
#include <vector>
#include "BasicStructure.h"

#include "Selene.h"

namespace GameSystem
{
	// 情報格納構造体
	typedef struct _info{
		TCHAR heart_full_tex[256];
		TCHAR heart_enmpy_tex[256];
		TCHAR point_item_tex[256];
		TCHAR udonge_stand_tex[256];
		TCHAR main_menu_bg[256];
		TCHAR title[256];
		TCHAR ending_script[256];
		TCHAR demo_file[256];
		TEXTURE_UV heart_full_uv;
		TEXTURE_UV heart_empty_uv;
		TEXTURE_UV point_item_uv;
		TEXTURE_UV udonge_stand_uv;
		TEXTURE_UV main_menu_uv;
		TEXTURE_UV title_uv;
		int	time_to_point;
		DWORD frame_limit_to_demo;
		DWORD game_over_wait_frame;
		DWORD ending_script_time_out;
		bool add_record_demo_mode;
		// これ以外にも、メニュー用背景等々の情報をここに入れる
	}INFO_SET;

	
	extern INFO_SET	game_info;

	// フォント情報
	typedef enum _font{
		FONT_STAGECLEAR , FONT_NUMBERS , FONT_WORLDTIME , FONT_MENU , FONT_ENDING
	}FONT_IDENTITY;

	extern std::vector<Selene::Renderer::Object::IFontSprite2D *> pFonts;
	extern std::vector<int>	fonts_spaces;
};
