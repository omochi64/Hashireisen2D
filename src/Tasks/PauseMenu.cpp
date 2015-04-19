
#include "PauseMenu.h"
#include "PauseChecker.h"
#include "MainMenu.h"
#include "System/Sounds.h"
#include "System/IniInfo.h"
#include "System/InputData.h"
#include "System/GameSystem.h"

#define PAUSE_MENU_NUM 2

#define MENU_RETURN_TO_GAME	0
#define MENU_RETURN_TO_TITLE 1

static char *menu_str[PAUSE_MENU_NUM] = {
	"Return to Game" , "Return to Title"
};

PauseMenu::PauseMenu( GameMain *main_task , CameraController *camera )
{
	m_main_task = main_task;
	m_camera = camera;

	// �\�����j���[
	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_MENU];
	m_pMenu = new MenuController( font );
	int i;
	Math::Point2DF size;
	for( i=0 ; i<PAUSE_MENU_NUM ; i++ )
	{
		size = font->GetStringSize( menu_str[i] );
		m_pMenu->AddString( menu_str[i] , 	Math::Point2DF( (GameSystem::SCREEN_WIDTH-size.x)/2 ,GameSystem::SCREEN_HEIGHT/2.0f+48*(i-1) ) );
	}
	m_pMenu->SetConnectTopAndBottom( true );

	// �w�i(������ƈÂ�����)
	m_pBlackBG = new BackgroundController( GameSystem::pRender );
	m_pBlackBG->SetSize( Math::Point2DI( GameSystem::SCREEN_WIDTH , GameSystem::SCREEN_HEIGHT ) );
	m_pBlackBG->SetPosWithoutAnimation( Math::Vector2D(0,0) );
	m_pBlackBG->SetColorWithoutAnimation( CColor(0x00,0x00,0x00,0xff/2) );
	m_pBlackBG->SetZ( -1.0f );
	m_draw_list.push_back( m_pBlackBG );

	m_draw_menu = new DrawMenu( &m_draw_list );
	m_draw_menu->SetMenu( m_pMenu );

	GameSystem::draw_task->AddTask( m_draw_menu , 0xffffff );

	GameSound::sound_manager->Stop( GameSound::menu_cancel );
	GameSound::sound_manager->Play( GameSound::menu_cancel );

	if( main_task->IsBeaten() )
	{
		GameSound::sound_manager->Pause( GameSound::game_over );
	}
	if( main_task->IsGoaled() )
	{
		GameSound::sound_manager->Pause( GameSound::clear_bgm );
	}
}

PauseMenu::~PauseMenu()
{
	SAFE_RELEASE(m_pMenu);
	SAFE_RELEASE(m_pBlackBG);
}

void PauseMenu::run()
{
	// �L�[���͂ɂ���đI��ݒ�
	if( InputData::input_key_strokes[KEY_UP] == 1 )
	{
		m_pMenu->SelectPreviousMenu();
		GameSound::sound_manager->Stop( GameSound::menu_select );
		GameSound::sound_manager->Play( GameSound::menu_select );
	}
	if( InputData::input_key_strokes[KEY_DOWN] == 1 )
	{
		m_pMenu->SelectNextMenu();
		GameSound::sound_manager->Stop( GameSound::menu_select );
		GameSound::sound_manager->Play( GameSound::menu_select );
	}
	int selected = m_pMenu->GetSelectedMenuNumber();
	if( InputData::input_key_strokes[KEY_JUMP] == 1 && selected == MENU_RETURN_TO_TITLE )
	{
		// �^�C�g���ɖ߂�
		m_main_task->StopBGM();
//		GameSound::sound_manager->Stop( GameSound::game_bgm );
		GameSound::sound_manager->Stop( GameSound::menu_decide );
		GameSound::sound_manager->Play( GameSound::menu_decide );
		if( m_main_task->IsBeaten() )
		{
			GameSound::sound_manager->Stop( GameSound::game_over );
		}
		if( m_main_task->IsGoaled() )
		{
			GameSound::sound_manager->Stop( GameSound::clear_bgm );
		}
		m_main_task->ReleaseCourse();
		GameSystem::draw_task->Release();
		GameSystem::all_task->Release();

		MainMenu *mm;
		mm = new MainMenu();
		GameSystem::all_task->AddTask( mm , 0x01 );
	
		return;
	}
	else if( ( InputData::input_key_strokes[KEY_JUMP] == 1 && selected == MENU_RETURN_TO_GAME ) ||
		( InputData::input_key_strokes[KEY_PAUSE] == 1 ) ||
		( InputData::input_key_strokes[KEY_DASH] == 1 ) )
	{
		// �Q�[���ɖ߂�
		TaskControllBlock *next;
		next = new PauseChecker( m_main_task , m_camera );
		m_main_task->StartUdonge();
		m_camera->Start();
		
		if( ( InputData::input_key_strokes[KEY_JUMP] == 1 && selected == MENU_RETURN_TO_GAME ) )
		{
			GameSound::sound_manager->Stop( GameSound::menu_decide );
			GameSound::sound_manager->Play( GameSound::menu_decide );
		}
		else
		{
			GameSound::sound_manager->Stop( GameSound::menu_cancel );
			GameSound::sound_manager->Play( GameSound::menu_cancel );
		}

		if( m_main_task->IsBeaten() )
		{
			GameSound::sound_manager->Pause( GameSound::game_over );
		}
		if( m_main_task->IsGoaled() )
		{
			GameSound::sound_manager->Pause( GameSound::clear_bgm );
		}
		else
		{
			m_main_task->StartAllEnemies();
			m_main_task->ResumeCountingTime();
		}

		InputData::input_key_strokes[KEY_JUMP]++;
		GameSystem::draw_task->DeleteTask( m_draw_menu->getID() );
		GameSystem::all_task->ChangeTask( m_id , next );
		return;
	}
}
