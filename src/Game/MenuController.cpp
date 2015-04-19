
#include "Game/MenuController.h"

/*

// メニューのコントローラ
// 選択されてるメニューの管理、メニューの描画を行う
class MenuController
{
	vector<MENU_SET>	m_menus;
	bool				m_is_loop_menu;	// メニューの上下が繋がっているか
	int					m_selected_menu;

	bool				m_did_start_menu_animation;
	DWORD				m_menu_animation_start_time;

	CColor				m_unselected_color,m_selected_color;

	Renderer::Object::IFontSprite2D	*m_pFont;

public:
	void StartMenuAnimation( DWORD now_time );
	bool UpdateMenuAnimation( DWORD now_time );

	void Release();

	void UpSelectMenu();
	void DownSelectMenu();
	void DecideNowMenu( TaskController *t_controller );
	void SetConnectTopAndBottom( bool is_connecting );
	int GetSelectedMenuNumber( void );

	void SetUnselectedColor( CColor color );
	void SetSelectedColor( CColor color );

	void DrawMenu();
};

*/

MenuController::MenuController(Renderer::Object::IFontSprite2D *pFont)
{
	m_pFont = pFont;
	m_is_loop_menu = false;
	m_did_start_menu_animation = false;
	m_unselected_color = CColor( 0xff,0xff,0xff, 128 );
	m_selected_color = CColor( 0xff , 0xff , 0xff );
	m_menu_animation_past_frame = 0;
	m_selected_menu = 0;
}

MenuController::~MenuController()
{
	Release();
}

// 描画文字追加
int MenuController::AddString( char *show_string , Math::Point2DF show_pos , Math::Point2DF show_pos_speed , DWORD animation_duration )
{
	MENU_SET set;
	strcpy( set.show_string , show_string );
//	set.add_task = add_task;
//	set.priority = priority;
	set.show_pos = show_pos;
	set.first_pos = show_pos;
	set.show_pos_speed = show_pos_speed;
	set.menu_animation_span = animation_duration;

	return AddString( set );
}

int MenuController::AddString(MENU_SET &add_menu)
{
	m_menus.push_back( add_menu );
	return m_menus.size();
}

// アニメーション関連
void MenuController::StartMenuAnimation()
{
	m_did_start_menu_animation = true;
	m_menu_animation_past_frame = 0;
}

void MenuController::UpdateMenuAnimation()
{
	if( !m_did_start_menu_animation )
	{
		return;
	}

	m_menu_animation_past_frame++;

	// アニメーション更新
	int i,size = m_menus.size();
	for( i=0 ; i<size ; i++ )
	{
		MENU_SET &menu = m_menus.at(i);

		if( m_menu_animation_past_frame > menu.menu_animation_span )
		{
			menu.show_pos = menu.first_pos+menu.show_pos_speed*toF(menu.menu_animation_span);
		}
		else
		{
			menu.show_pos = menu.first_pos+menu.show_pos_speed*toF(m_menu_animation_past_frame);
		}
	}
}

// 一気にアニメーションを終わらせる
void MenuController::EndAnimationAtOnce()
{
	m_did_start_menu_animation = false;

	int i,size = m_menus.size();
	for( i=0 ; i<size ; i++ )
	{
		MENU_SET &menu = m_menus.at(i);

		menu.show_pos = menu.first_pos+menu.show_pos_speed*toF(menu.menu_animation_span);
	}
}

// メニュー操作関連
void MenuController::SelectNextMenu()
{
	m_selected_menu++;
	if( m_selected_menu >= (signed)m_menus.size() )
	{
		if( m_is_loop_menu )
		{
			m_selected_menu = 0;
		}
		else
		{
			m_selected_menu = m_menus.size()-1;
		}
	}
}
void MenuController::SelectPreviousMenu()
{
	m_selected_menu--;
	if( m_selected_menu < 0 )
	{
		if( m_is_loop_menu )
		{
			m_selected_menu = m_menus.size()-1;
		}
		else
		{
			m_selected_menu = 0;
		}
	}
}

bool MenuController::SetSelectCurSel( int cur_sel )
{
	if( cur_sel < 0 || cur_sel >= (signed)m_menus.size() )
	{
		return false;
	}
	m_selected_menu = cur_sel;

	return true;
}
	

// 現在選択しているメニューで決定
/*void MenuController::DecideNowMenu( TaskController *t_controller )
{
	if( m_selected_menu >= (signed)m_menus.size() )
	{
		return;
	}
	MENU_SET &menu = m_menus[m_selected_menu];
	t_controller->AddTask( menu.add_task , menu.priority );
}*/

int MenuController::GetSelectedMenuNumber( void )
{
	return m_selected_menu;
}

// 各種設定
void MenuController::SetConnectTopAndBottom( bool is_connecting )
{
	m_is_loop_menu = is_connecting;
}

void MenuController::SetUnselectedColor( CColor color )
{
	m_unselected_color = color;
}

void MenuController::SetSelectedColor( CColor color )
{
	m_selected_color = color;
}

// 描画
void MenuController::DrawMenu()
{
	int i,size = m_menus.size();

//	m_pFont->Begin();

	for( i=0 ; i<size ; i++ )
	{
		MENU_SET &menu = m_menus.at(i);

		m_pFont->DrawString( menu.show_string , menu.show_pos , i==m_selected_menu?m_selected_color:m_unselected_color );
	}

//	m_pFont->End();
}

// 資源解放
void MenuController::Release()
{
	// taskの解放はまずいよなー。
	m_menus.clear();
}
