
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <d3d9.h>
//#include <d3dx9.h>
#include <dinput.h>
//#include <dmusici.h>
#include <mmsystem.h>
#include "JoyStick.h"

#include <math.h>
#include <map>

#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib,""
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

//// 頂点バッファの構造体
//typedef struct MY_VERTEX{		
//	D3DXVECTOR3 p;		// 位置(変換済み)
//	float		rhw;
//	DWORD		color;	// 色
//	D3DXVECTOR2	t;		// テクスチャ座標
//}MY_VERTEX;




// キー入力情報セット
typedef struct _key_set{
	int	v_key;		// 仮想キー
	int	g_key;		// ゲーム上のキー
}KEY_SET;

// ジョイパッド入力情報セット
typedef struct _joy_set{
	int joy_button;
	int g_key;
}JOY_SET;

// 頂点バッファーのフォーマットの定義
#define MY_VERTEX_FVF  ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#define CLIENT_WIDTH	640	// クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT	480	// クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度


#define		FPS				60

#define	GAME_KEY_KIND_NUM	13

// キーマクロ
#define		KEY_ALL			(-1)
#define		KEY_CAM_ZOOM_OUT	0x00000001
#define		KEY_JUMP		0x00000002
#define		KEY_DASH		0x00000004
#define		KEY_RIGHT		0x00000008
#define		KEY_LEFT		0x00000010
#define		KEY_UP			0x00000020
#define		KEY_DOWN		0x00000040
//#define		KEY_CAM_TRIG	0x00000080
#define		KEY_PAUSE		0x00000080
#define		KEY_CAM_UP		0x00000100
#define		KEY_CAM_DOWN	0x00000200
#define		KEY_CAM_LEFT	0x00000400
#define		KEY_CAM_RIGHT	0x00000800
#define		KEY_CAM_ZOOM	0x00001000


#define		GAME_BG_NUM		5

#ifdef _DIRECTX_GLOBAL
#define	EXTERN_DX		/*extern*/
#else
#define EXTERN_DX		extern
#endif

// std::map(texture)のキー
typedef enum _tex_key{
	TEX_BLOCK , MAIN_MENU_BG , STRING_CHIPSET , FIELD_BG
}TEX_KEYS;

// soundのキー
typedef enum _sound_key{
	MAIN_MENU_BGM , GAME_BGM , SELECTION_SE , MOVE_BLOCK_SE , DEL_LINE_SE , DOWNED_HINDER_SE , DECIDE_BLOCK_SE , FALL_DOWN_SE
}SOUND_KEYS;

typedef struct _float_rect{
	float	left;
	float	top;
	float	right;
	float	bottom;
}FLOAT_RECT;


void SetFloatRect( FLOAT_RECT *lpRect , float left , float top , float right , float bottom );
/*
typedef struct _sound_controll{
	LPDIRECTSOUNDBUFFER	p_sound;
	int before_ptr;
	int play_size;
}SOUND_CONTROLL;
*/

EXTERN_DX	LPDIRECT3DDEVICE9	g_pD3DDevice;
//EXTERN_DX	LPDIRECTSOUND8		m_pDSound;
EXTERN_DX	std::map<int,LPDIRECT3DTEXTURE9>	g_texture;
EXTERN_DX	JoyStick			g_joyStick;
//EXTERN_DX	LPD3DXFONT			g_font;
EXTERN_DX	KEY_SET				*g_key_sets[2];
EXTERN_DX	JOY_SET				*g_joy_sets[2];
EXTERN_DX	int					g_joy_ids[2];
EXTERN_DX	int					frames;
EXTERN_DX	SOUND_KEYS			playing_bgm;
EXTERN_DX	int					game_bg[GAME_BG_NUM];
EXTERN_DX	int					now_game_bg;
EXTERN_DX	float				g_true_fps;
EXTERN_DX	bool				is_draw_skip;

typedef enum _string_uv_number{
	SCORE_MODE_UV , BATTLE_MODE_UV , VS_HUMAN_UV , VS_COM_UV ,
	OPTION_UV , RANKING_UV , LEVEL_UV , QUIT_UV ,  ON_UV , OFF_UV ,
	MIRROR_UV , HIDDEN_UV , SUDDEN_UV , COM_LEVEL_UV , 
	HIDDEN_LEVEL_UV , SUDDEN_LEVEL_UV , WINDOW_UV , FULL_SCREEN_UV , 
	OK_UV , SCORE_UV , MODE_UV , NEXT_UV , RETURN_GAME_UV , RETRY_UV , REPLAY_UV ,
	END_GAME_UV , RETURN_MENU_UV , EASY_UV , HARD_UV , NORMAL_UV ,
	LINES_UV , NAME_UV , NO_UV , RETURN_REPLAY_UV , END_REPLAY_UV ,
	DUERIS_UV , PLAYER1_UV , PLAYER2_UV , COM_UV ,
	LUNATIC_UV , DEMO_UV , DOT_UV , HOLD_UV ,
	LEVEL_UP_UV
}STRING_UV_NUMBER;


#ifdef _DIRECTX_GLOBAL
FLOAT_RECT	number_uv[10] = { 
	{0.0f,0.75f,0.05f,0.8f},
	{0.05f,0.75f,0.1f,0.8f},
	{0.1f,0.75f,0.15f,0.8f},
	{0.15f,0.75f,0.2f,0.8f},
	{0.2f,0.75f,0.25f,0.8f},
	{0.25f,0.75f,0.3f,0.8f},
	{0.3f,0.75f,0.35f,0.8f},
	{0.35f,0.75f,0.4f,0.8f},
	{0.4f,0.75f,0.45f,0.8f},
	{0.45f,0.75f,0.5f,0.8f},
};
FLOAT_RECT	string_uv[]={
	{0.0f,0.0000f,0.250f,0.0625f},{0.250f,0.0000f,0.5000f,0.0625f},{0.5000f,0.0000f,0.7500f,0.0625f},{0.7500f,0.0000f,1.000f,0.0625f},
	{0.0f,0.0625f,0.250f,0.1250f},{0.250f,0.0625f,0.5000f,0.1250f},{0.5000f,0.0625f,0.6250f,0.1250f},{0.6250f,0.0625f,0.750f,0.1250f},{0.7500f,0.0625f,0.8750f,0.1250f},{0.8750f,0.0625f,1.0000f,0.1250f},
	{0.0f,0.1250f,0.250f,0.1875f},{0.250f,0.1250f,0.392578125f,0.1875f},{0.5000f,0.1250f,0.6484375f,0.1875f},{0.7500f,0.1250f,1.000f,0.1875f},
	{0.0f,0.1875f,0.250f,0.2500f},{0.250f,0.1875f,0.5000f,0.2500f},{0.5000f,0.1875f,0.7500f,0.2500f},{0.7500f,0.1875f,1.000f,0.2500f},
	{0.0f,0.2500f,0.125f,0.3125f},{0.125f,0.2500f,0.2500f,0.3125f},{0.2500f,0.2500f,0.3750f,0.3125f},{0.3750f,0.2500f,0.500f,0.3125f},{0.5000f,0.2500f,0.7500f,0.3125f},{0.7500f,0.2500f,0.8750f,0.3125f},{0.8750f,0.2500f,1.0000f,0.3125f},
	{0.0f,0.3125f,0.250f,0.3750f},{0.250f,0.3125f,0.5000f,0.3750f},{0.5000f,0.3125f,0.6250f,0.3750f},{0.6250f,0.3125f,0.750f,0.3750f},{0.7500f,0.3125f,1.0000f,0.3750f},
	{0.0f,0.3750f,0.125f,0.4375f},{0.125f,0.3750f,0.2500f,0.4375f},{0.2500f,0.3750f,0.3125f,0.4375f},{0.5000f,0.3750f,0.750f,0.4375f},{0.7500f,0.3750f,0.970703125f,0.4375f},
	{0.0f,0.4375f,0.19921875f,0.5000f},{0.250f,0.4375f,0.46875f,0.5000f},{0.5f,0.4375f,0.7373046875f,0.5000f},{0.75f,0.4375f,0.8935546875f,0.5000f},
	{0.0f,0.5000f,0.146484375f,0.5625f},{0.250f,0.5000f,0.5810546875f,0.5625f},{0.5498046875f,0.7763671875f,0.57421875f,0.8f},{0.75f,0.50f,0.857421875f,0.5625f},
	{0.0f,0.5625f,0.250f,0.6250f}
};


#else
EXTERN_DX	FLOAT_RECT		number_uv[10];
EXTERN_DX	FLOAT_RECT		string_uv[];
#endif