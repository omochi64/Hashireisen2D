
#pragma once

#include "Selene.h"
#include <vector>

using namespace Selene;
using namespace std;

#include "System/Manager/TextureManager.h"
#include "System/Manager/SoundManager.h"

// いわゆる中間言語
typedef struct _script_line{
	TCHAR	string[256];
	int		option[5];
}SCRIPT_LINE;

// 中間言語書式
//	string = "*wait"
//	 option[0]:wait_msec
//	 option[1]:did_processed
//	string = "*show"
//	 option[0]:texture_number
//	 option[1]:primitive_number
//	 option[2]:show_left_x
//	 option[3]:width
//	 option[4]:height

#define DEFAULT_SCROLL_SPEED	1.0f

class EndingScript
{
	// エンディングスクリプトが持つ全てのプリミティブ(テクスチャ)と、UpdateScript後に描画が決まったプリミティブ(テクスチャ)
	vector<Renderer::Object::IPrimitive2D *> m_primitives,m_draw_primitives;
	vector<Renderer::ITexture *> m_textures,m_draw_textures;

	Renderer::Object::IFontSprite2D *m_pFont;

	int		m_screen_width;
	int		m_screen_height;
	float	m_left_x;
	int		m_font_size;
	float	m_scroll_speed;
	int		m_scroll_speed_times;
	bool	m_do_stop_when_finished;
	CColor	m_bgColor;
	CColor	m_textColor;
	char	m_endingBGM[256];

	int	m_nextShowLine;		// 一番上に表示する行番号
	float	m_nextShowLineYPos;	// 一番上に表示する行のY座標

	int		m_lineNum;
	SCRIPT_LINE	*m_script;

	bool	m_is_animating;
	bool	m_is_waiting;
//	DWORD	m_start_wait_frame;
	int		m_rest_wait_frame;
//	DWORD	m_lastUpdatedTime;

	DWORD	m_pastFrames;

	Renderer::SPrimitiveVertex2D m_backVertex[2];

public:

	EndingScript( Renderer::Object::IFontSprite2D *pFont , int screen_width , int screen_height );
	EndingScript(  int screen_width , int screen_height );
	~EndingScript();

	void SetFont( Renderer::Object::IFontSprite2D *pFont );

	bool LoadFromFile( char *fname , Renderer::IRender *pRender , CTextureManager *tex_manager ,
		SoundManager *sound_manager , ICore *pCore );
	void StartScript();
	bool UpdateScript();
	bool IsScriptEnd();
	void SpeedUpTimes( int speed_up_times );
	void ResetSpeed( void );
	void Draw();
	void PlayBGM( SoundManager *sound_manager );
	void StopBGM( SoundManager *sound_manager );
	void PauseBGM( SoundManager *sound_manager );

	const char *GetSoundName();

	void ResetStatus();
	void Release();

	vector<Renderer::Object::IPrimitive2D *> &GetDrawPrimitives();
	vector<Renderer::ITexture *> &GetDrawTextures();
};
