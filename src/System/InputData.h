
#pragma once

#include "Selene.h"
#include <map>


// キーマクロ
#define		KEY_ALL			(-1)
#define		KEY_CAM_ZOOM_OUT	0x00000001
#define		KEY_JUMP		0x00000002
#define		KEY_DASH		0x00000004
#define		KEY_RIGHT		0x00000008
#define		KEY_LEFT		0x00000010
#define		KEY_UP			0x00000020
#define		KEY_DOWN		0x00000040
#define		KEY_PAUSE		0x00000080
#define		KEY_CAM_UP		0x00000100
#define		KEY_CAM_DOWN	0x00000200
#define		KEY_CAM_LEFT	0x00000400
#define		KEY_CAM_RIGHT	0x00000800
#define		KEY_CAM_ZOOM	0x00001000

namespace InputData
{
	// キー入力情報セット
	typedef struct _key_set{
		Selene::eVirtualKeyCode	v_key;		// 仮想キー
		int	g_key;		// ゲーム上のキー
	}KEY_SET;

	// ジョイパッド入力情報セット
	typedef struct _joy_set{
		int joy_button;
		int g_key;
	}JOY_SET;
	
	extern const int GAME_KEY_KIND_NUM;
	extern int		input_keys;
	extern std::map<int,int> input_key_strokes;

};
