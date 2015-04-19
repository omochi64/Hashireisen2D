
#include "MainMenu.h"
#include "System/GlobalNamespace.h"
#include "InitializeGame.h"
#include "CleanGame.h"
#include "System/Sounds.h"
#include "LoadDataBeforeGame.h"
#include "System/IniInfo.h"
#include "System/InputData.h"
#include "System/GameSystem.h"

#define	MENU_START_GAME	0
#define	MENU_EXIT		1
#define RECORD_DEMO	2

namespace
{
	int MENU_NUM_WITHOUT_RECORD = 2;
	int MENU_NUM_WITH_RECORD = (MENU_NUM_WITHOUT_RECORD+1);
	char *menu_strings[] = {
		"Start Game" , "Exit" , "Record Demo"
	};

	char *now_loading_string = "NOW LOADING...";

};

MainMenu::MainMenu( bool will_do_title_animation , bool will_start_bgm , int ini_select_menu )
{
	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_MENU];
	m_pMenu = new MenuController( font );
	m_pMenu->SetConnectTopAndBottom( true );
	m_pMenu->SetSelectedColor( CColor(0xff,0x00,0x00) );

//	m_demo_main_task = NULL;
	m_is_doing_demo = false;

	// メニュー文字の設定
	int menu_num;
	if( GameSystem::game_info.add_record_demo_mode )
	{
		menu_num = MENU_NUM_WITH_RECORD;
	}
	else
	{
		menu_num = MENU_NUM_WITHOUT_RECORD;
	}
	Math::Point2DF size_str;
	int i;
	for( i=0 ; i<menu_num ; i++ )
	{
		size_str = font->GetStringSize( menu_strings[i] );
		m_pMenu->AddString( menu_strings[i] , Math::Point2DF((GameSystem::SCREEN_WIDTH-size_str.x)/2.0f,220.0f+(size_str.y+40)*i) );
	}
	m_pMenu->SetSelectCurSel( ini_select_menu );

	
	// 背景作成
	if( !CreateBackTitleWhiteOut( &m_pBack , &m_pTitle , &m_pWhiteOut ) )
	{
		GameSystem::all_task->ChangeTask( m_id , new CleanGame() );
		return;
	}
	
	// タイトルアニメーション設定
	if( will_do_title_animation )
	{
		m_pTitle->StartPosAnimation();
		m_is_doing_animation = true;
	}
	else
	{
		m_pTitle->EndPosAnimationAtOnce();
		m_is_doing_animation = false;
	}


	// メインメニュー用ドローリスト
	m_draw_list.push_back( m_pWhiteOut );
	m_draw_list.push_back( m_pBack );
	m_draw_list.push_back( m_pTitle );

	m_dmm = new DrawMenu( &m_draw_list );
	GameSystem::draw_task->AddTask( m_dmm , 0x0f );

	// タイトルBGMを流し始める
	if( will_start_bgm )
	{
		GameSound::sound_manager->Stop( GameSound::title_bgm );
		GameSound::sound_manager->Play( GameSound::title_bgm , -1 );
	}

	// うどんげ設定
	Character::udonge->SetHPToDefault();
	Character::udonge->SetCoinToDefault();
	Character::udonge->ResetPoint();
	Character::udonge->SetRestLifeToDefault();

	m_non_input_frame = 0;
}
MainMenu::~MainMenu()
{
	SAFE_RELEASE(m_pMenu);
	SAFE_RELEASE(m_pTitle);
	SAFE_RELEASE(m_pBack);
	SAFE_RELEASE(m_pWhiteOut);
//	SAFE_RELEASE(m_pMenu_for_now_loading);
	m_draw_list.clear();
//	m_draw_list_for_demo.clear();
//	if( m_demo_main_task != NULL )
//	{
//		m_demo_main_task->ReleaseCourse();
//		delete m_demo_main_task;
//		m_demo_main_task = NULL;
//	}
}

void MainMenu::run()
{
	if( m_is_doing_animation )
	{
		int i;
		for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
		{
			if( InputData::input_key_strokes[1<<i] == 1 )
			{
				m_pTitle->EndPosAnimationAtOnce();
				m_is_doing_animation = false;
				m_dmm->SetMenu( m_pMenu );
				return;
			}
		}
	}

	if( m_pTitle->UpdatePosAnimation() )
	{
		return;
	}
	else
	{
		m_dmm->SetMenu( m_pMenu );
		m_is_doing_animation = false;
	}

	m_non_input_frame++;

	// キー入力によって選択設定
	if( InputData::input_key_strokes[KEY_UP] == 1 )
	{
		m_pMenu->SelectPreviousMenu();
		GameSound::sound_manager->Stop( GameSound::menu_select );
		GameSound::sound_manager->Play( GameSound::menu_select );
		m_non_input_frame = 0;
	}
	if( InputData::input_key_strokes[KEY_DOWN] == 1 )
	{
		m_pMenu->SelectNextMenu();
		GameSound::sound_manager->Stop( GameSound::menu_select );
		GameSound::sound_manager->Play( GameSound::menu_select );
		m_non_input_frame = 0;
	}
	if( ( InputData::input_key_strokes[KEY_JUMP] == 1 ) )
	{
		TaskControllBlock *next;
		GameSound::sound_manager->Stop( GameSound::menu_decide );
		GameSound::sound_manager->Play( GameSound::menu_decide );
		GameSound::sound_manager->Stop( GameSound::title_bgm );
		m_non_input_frame = 0;
		Character::udonge->ResetAllState();
		switch( m_pMenu->GetSelectedMenuNumber() )
		{
		case MENU_START_GAME:
			// ゲーム開始
			next = new InitializeGame( 1 , 1 );
			break;
/*		case MENU_OPTION:
			// 今はオプションを作っていない
			Character::udonge->SetHPToDefault();
			Character::udonge->SetRestLifeToDefault();
			next = new InitializeGame( 1 , 1 );
			break;
*/
		case MENU_EXIT:
			// ゲーム終了
			next = new CleanGame();
			break;

		case RECORD_DEMO:
			// デモ撮影
			LoadDataBeforeGame *ldb;
			GameSystem::draw_task->Release();
			ldb = new LoadDataBeforeGame( 1 , 1 , 0 , NULL );
			ldb->SetRecordDemoMode();
			ldb->StartLoadAndWait();
			next = ldb;
			break;
		}
		GameSystem::all_task->Release();
		GameSystem::draw_task->Release();
		GameSystem::all_task->AddTask( next , 0x01 );
		
		return;
	}


	// デモ開始条件
	if( m_non_input_frame >= GameSystem::game_info.frame_limit_to_demo && !m_is_doing_demo )
	{
		// デモを現在のような感じではなく、普通のデモに戻す？
		// そのほうがバグをつぶしやすいと思うし、普通のゲームといったらそんなもんじゃないかなぁ

		LoadDataBeforeGame *ldb;
		std::vector<BackgroundController *>* draw_list_for_demo;
		BackgroundController *pWhiteOut,*pTitle,*pBack;

		// まずは背景作成
		if( !CreateBackTitleWhiteOut( &pBack , &pTitle , &pWhiteOut ) )
		{
			return;
		}
		pTitle->EndPosAnimationAtOnce();
		draw_list_for_demo = new std::vector<BackgroundController *>();
	
		// 登録
		draw_list_for_demo->push_back( pWhiteOut );
		draw_list_for_demo->push_back( pBack );
		draw_list_for_demo->push_back( pTitle );

		// メニュー作成
		MenuController *pMenu_for_now_loading = new MenuController( GameSystem::pFonts[GameSystem::FONT_NUMBERS] );
		Math::Point2DF size_str = GameSystem::pFonts[GameSystem::FONT_NUMBERS]->GetStringSize( now_loading_string );
		pMenu_for_now_loading->AddString( now_loading_string ,
			Math::Point2DF(10,GameSystem::SCREEN_HEIGHT-size_str.y-10) );

		DrawMenu *draw_task = new DrawMenu( draw_list_for_demo , true , true );
		draw_task->SetMenu( pMenu_for_now_loading );

		Character::udonge->ResetAllState();
//		m_pTitle->SetColorWithoutAnimation( CColor(0xff,0xff,0xff,0xff/3) );
//		m_pBack->SetColorWithoutAnimation( CColor(0xff,0xff,0xff,0) );
		ldb = new LoadDataBeforeGame(1,1,0,draw_task,true);
		ldb->SetReplayDemoMode(NULL);
		ldb->StartLoadAndWait();

		m_is_doing_demo = true;

		GameSystem::all_task->Release();
		GameSystem::draw_task->Release();
		GameSystem::draw_task->AddTask( draw_task , 0x02 );
		GameSystem::all_task->AddTask( ldb , 0x01 );
		return;
	}

	
}

bool MainMenu::CreateBackTitleWhiteOut( BackgroundController **pBack , BackgroundController **pTitle , BackgroundController **pWhiteOut )
{
	// 背景画像処理
	Renderer::ITexture *tex;
	if( pBack )
	{
		*pBack = new BackgroundController( GameSystem::pRender );
		// 背景画像
		tex = GameSystem::tex_manager->GetAt(GameSystem::game_info.main_menu_bg);
		if( !tex )
		{
			tex = GameSystem::tex_manager->AddTexture( GameSystem::game_info.main_menu_bg , GameSystem::pRender );
			if( !tex )
			{
				return false;
			}
		}
		(*pBack)->SetTexture( tex , GameSystem::game_info.main_menu_uv.leftup , GameSystem::game_info.main_menu_uv.rightbottom );
		(*pBack)->SetPosWithoutAnimation( Math::Vector2D(toF(GameSystem::SCREEN_WIDTH-tex->GetSize().x),toF(GameSystem::SCREEN_HEIGHT-tex->GetSize().y))/2 );
	}
	if( pTitle )
	{
		*pTitle = new BackgroundController( GameSystem::pRender );

		// タイトル
		tex = GameSystem::tex_manager->GetAt(GameSystem::game_info.title);
		if( !tex )
		{
			tex = GameSystem::tex_manager->AddTexture( GameSystem::game_info.title , GameSystem::pRender );
			if( !tex )
			{
				return false;
			}
		}
		(*pTitle)->SetTexture( tex , GameSystem::game_info.title_uv.leftup , GameSystem::game_info.title_uv.rightbottom );
		Math::Point2DI size = tex->GetSize();

		(*pTitle)->SetPosAnimation1( Math::Vector2D( (GameSystem::SCREEN_WIDTH-size.x)/2.0f , -toF(size.y) ) ,
			Math::Vector2D( (GameSystem::SCREEN_WIDTH-size.x)/2.0f , 50 ) , 300 );
	}
	if( pWhiteOut )
	{
		*pWhiteOut = new BackgroundController( GameSystem::pRender );
		// 白背景
		(*pWhiteOut)->SetPosWithoutAnimation( Math::Vector2D(0,0) );
		(*pWhiteOut)->SetSize( Math::Point2DI(GameSystem::SCREEN_WIDTH,GameSystem::SCREEN_HEIGHT) );

	}

	

	return true;
}
