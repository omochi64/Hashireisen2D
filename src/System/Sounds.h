
#pragma once

#ifdef _SOUND_GLOBAL
#define	EXTERN_SOUND	/*extern*/
#else
#define EXTERN_SOUND	extern
#endif

#include "System/Manager/SoundManager.h"

namespace GameSound
{
	EXTERN_SOUND SoundManager	*sound_manager;

	EXTERN_SOUND TCHAR	title_bgm[256];
//	EXTERN_SOUND TCHAR	game_bgm[256];
	EXTERN_SOUND TCHAR	clear_bgm[256];
//	EXTERN_SOUND TCHAR	ending_bgm[256];
	EXTERN_SOUND TCHAR	game_over_bgm[256];
	EXTERN_SOUND TCHAR	menu_decide[256];
	EXTERN_SOUND TCHAR	menu_cancel[256];
	EXTERN_SOUND TCHAR	block_crash[256];
	EXTERN_SOUND TCHAR	get_point[256];
	EXTERN_SOUND TCHAR	appear_heart[256];
	EXTERN_SOUND TCHAR	get_heart[256];
	EXTERN_SOUND TCHAR	damage[256];
	EXTERN_SOUND TCHAR	beat_enemy[256];
	EXTERN_SOUND TCHAR	jump[256];
	EXTERN_SOUND TCHAR	one_up[256];
	EXTERN_SOUND TCHAR	game_over[256];
	EXTERN_SOUND TCHAR	menu_select[256];
	EXTERN_SOUND TCHAR	time_to_point[256];
};
