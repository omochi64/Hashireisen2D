
#pragma once

#include "Selene.h"
#include "System/TaskSystem.h"

#include <vector>

using namespace Selene;
using namespace std;

typedef struct _menu_set{
	char	show_string[512];
//	TaskControllBlock	*change_task;		// ���j���[���莞�ɑ}������^�X�N
//	int		id;
	Math::Point2DF	first_pos;
	Math::Point2DF	show_pos;
	Math::Point2DF	show_pos_speed;		// ���j���[�A�j���[�V�����p
	DWORD			menu_animation_span;// ���j���[�A�j���[�V��������
}MENU_SET;

// ���j���[�̃R���g���[��
// �I������Ă郁�j���[�̊Ǘ��A���j���[�̕`����s��
class MenuController
{
	vector<MENU_SET>	m_menus;
	bool				m_is_loop_menu;	// ���j���[�̏㉺���q�����Ă��邩
	int					m_selected_menu;

	bool				m_did_start_menu_animation;
	DWORD				m_menu_animation_past_frame;

	CColor				m_unselected_color,m_selected_color;

	Renderer::Object::IFontSprite2D	*m_pFont;

public:
	MenuController( Renderer::Object::IFontSprite2D	*pFont );
	~MenuController();

	int AddString( MENU_SET & add_menu );
	int AddString( char *show_string , Math::Point2DF show_pos , Math::Point2DF show_pos_speed = Math::Point2DF(0,0) , DWORD animation_duration = 0 );
	void StartMenuAnimation();
	void UpdateMenuAnimation();
	void EndAnimationAtOnce();

	void Release();

	void SelectNextMenu();
	void SelectPreviousMenu();
	bool SetSelectCurSel( int cur_sel );
	void SetConnectTopAndBottom( bool is_connecting );
	int GetSelectedMenuNumber( void );

	void SetUnselectedColor( CColor color );
	void SetSelectedColor( CColor color );

	void DrawMenu();
};
