
#include "LoadDataBeforeGame.h"
#include "Tasks/Renderer/Draw.h"
#include "CameraController.h"
#include "Tasks/Renderer/DrawStatus.h"
#include "Game/SMFObject.h"
#include "Tasks/Renderer/DrawBackgroundTask.h"
#include "CreateBackgroundTexture.h"
#include "PauseChecker.h"
#include "System/Sounds.h"
#include "RecordAndCheckDemoTask.h"
#include "ReplayDemoTask.h"
#include "System/GameSystem.h"

#include <process.h>
#include <windows.h>


LoadDataBeforeGame::LoadDataBeforeGame(int world, int course, DWORD minimum_wait_time,
									   TaskControllBlock *draw_info_task , bool do_change_bgm )
{
	m_world = world;
	m_course = course;
	m_start_time = 0;
	m_mini_wait_time = minimum_wait_time;
	m_draw_info_task = draw_info_task;
	m_is_record_demo = false;
	m_is_replay_demo = false;
	did_end_load = false;
	m_do_change_bgm = do_change_bgm;
	m_main_menu_controller = NULL;
}

void LoadDataBeforeGame::StartLoadAndWait()
{
	COURSE_INFO ci;
	GameSystem::course_manager->GetCourseInfo( m_world , m_course , ci );

	m_next_task = new GameMain();
	m_start_time = GameSystem::now_frame_time;
	did_end_load = false;
}


// タスク実行
// 時間チェック＆読み込み完了チェック
void LoadDataBeforeGame::run( void )
{
	if( m_start_time == 0 )
	{
		return;
	}

	// めちゃくちゃ強引だけど…
	DWORD now_time = GameSystem::now_frame_time;
	if( now_time - m_start_time <= 100 )
	{
		return;
	}

	COURSE_INFO ci;
	

	if( !did_end_load )
	{
		GameSystem::course_manager->GetCourseInfo( m_world , m_course , ci );
		m_next_task->CreateCourse( m_world , m_course );
		Renderer::ITexture *top_tex = GameSystem::tex_manager->GetAt( ci.background_top_file_name );
		if( !top_tex )
		{
			top_tex = GameSystem::tex_manager->AddTexture( ci.background_top_file_name , GameSystem::pRender );
			if( !top_tex )
			{
				MessageBox( NULL , "Read Texture Error Happened." , NULL , NULL );
				exit(0);
			}
		}
		
		Renderer::ITexture *center_tex = GameSystem::tex_manager->GetAt( ci.background_center_file_name );
		if( !center_tex )
		{
			center_tex = GameSystem::tex_manager->AddTexture( ci.background_center_file_name , GameSystem::pRender );
			if( !center_tex )
			{
				MessageBox( NULL , "Read Texture Error Happened." , NULL , NULL );
				exit(0);
			}
		}
		Renderer::ITexture *under_tex = GameSystem::tex_manager->GetAt( ci.background_under_file_name );
		if( !under_tex )
		{
			under_tex = GameSystem::tex_manager->AddTexture( ci.background_under_file_name , GameSystem::pRender );
			if( !under_tex )
			{
				MessageBox( NULL , "Read Texture Error Happened." , NULL , NULL );
				exit(0);
			}
		}

//		Math::Point2DI size = tex->GetSize();
//		m_pBackCenter = GameSystem::pRender->CreateTextureRenderTarget( size.x/2 , size.y/2 , FORMAT_TARGET_32BIT );
//		m_pBackTop = GameSystem::pRender->CreateTextureRenderTarget( size.x/2 , size.y/2 , FORMAT_TARGET_32BIT );
//		m_pBackUnder = GameSystem::pRender->CreateTextureRenderTarget( size.x/2 , size.y/2 , FORMAT_TARGET_32BIT );

//		GameSystem::draw_task->AddTask( new CreateBackgroundTexture( tex , m_pBackUnder , m_pBackCenter , m_pBackTop ) , 0xffff );
		m_dgt = new DrawBackgroundTask( top_tex , center_tex , under_tex , m_next_task->GetCourseSize() );

		// サウンド読み込み(読み込み済みなら関係ない)
		// BGM
		GameSystem::pFileManager->SetCurrentPath( "Sound" );
		if( !LoadSound() )
		{
			MessageBox( NULL , "Read sound error" , NULL , MB_OK|MB_ICONERROR );
			exit(0);
		}
		GameSystem::pFileManager->SetCurrentPath( "Texture" );


		did_end_load = true;

	}

	
	if( now_time - m_start_time < m_mini_wait_time )
	{
		return;
	}

	// udongeの設定
	Character::udonge->SetAnimationToStand();
	Character::udonge->UpdateVertex();
	Character::udonge->Start();

	Character::udonge->SetDirection(true);
	Character::udonge->SetSpeed(Math::Vector3D(0,0,0));
	Character::udonge->SetAccel(Math::Vector3D(0,0,0));
	Character::udonge->SetRotationSpeed(Math::Vector3D(0,0,0));
	Character::udonge->SetRotation(Math::Vector3D(0,0,0));
	Character::udonge->Start();
	GameSystem::all_object->SetAction( Character::udonge , true );

	// タスクの生成
	// システム処理系
	TaskControllBlock *next_change_block;
	CameraController *camera = new CameraController(m_next_task);
	camera->Start();
	m_next_task->SetCamera( camera );
	if( m_is_record_demo )
	{
		// デモ録画モード
		next_change_block = new RecordAndCheckDemoTask( m_next_task );
	}
	else if( m_is_replay_demo )
	{
		// デモプレイモード
		next_change_block = new ReplayDemoTask( m_next_task , m_main_menu_controller );
	}
	else
	{
		next_change_block = new PauseChecker( m_next_task , camera );
	}

	
	GameSystem::is_searching_udonge = true;
	GameSystem::is_searching_only_y = false;
	GameSystem::all_task->AddTask( camera , 0xfff );
	GameSystem::all_task->AddTask( new ActTask() , m_priority+1 );

	// ドロー系
	if( m_draw_info_task )
	{
		GameSystem::draw_task->DeleteTask( m_draw_info_task->getID() );
	}
	GameSystem::draw_task->AddTask( new DrawTask() , 0x11 );
	GameSystem::draw_task->AddTask( new DrawStatus(m_world,m_course,m_next_task) , 0xffff );
	GameSystem::draw_task->AddTask( m_dgt , 0x10 );
	

	GameSystem::course_manager->GetCourseInfo( m_world , m_course , ci );
	m_next_task->StartCountingTime( ci.time );

	// BGM開始
	if( m_do_change_bgm )
	{
		GameSound::sound_manager->Stop( GameSound::title_bgm );
		m_next_task->StopBGM();
		m_next_task->PlayBGM();
	}

	GameSystem::all_task->AddTask( m_next_task , m_priority+2 );
	GameSystem::all_task->ChangeTask( m_id , next_change_block );
	
}

// 音声読み込み
bool LoadDataBeforeGame::LoadSound()
{
	
/*	if( !GameSound::sound_manager->GetSound( GameSound::game_bgm ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::game_bgm , "OggVorbis" ) )
		{
			return false;
		}
	}
*/
	if( !GameSound::sound_manager->GetSound( GameSound::clear_bgm ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::clear_bgm , "OggVorbis" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::game_over_bgm ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::game_over_bgm , "OggVorbis" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::appear_heart ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::appear_heart , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::beat_enemy ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::beat_enemy , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::block_crash ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::block_crash , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::damage ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::damage , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::get_heart ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::get_heart , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::get_point ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::get_point , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::jump ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::jump , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::one_up ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::one_up , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::game_over ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::game_over , "Wave" ) )
		{
			return false;
		}
	}

	if( !GameSound::sound_manager->GetSound( GameSound::time_to_point ) )
	{
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::time_to_point , "Wave" ) )
		{
			return false;
		}
	}

	return true;
}
