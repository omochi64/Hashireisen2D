
#include "RecordAndCheckDemoTask.h"
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"
#include "MainMenu.h"
#include "System/Sounds.h"
#include "System/InputData.h"


RecordAndCheckDemoTask::RecordAndCheckDemoTask( GameMain *main_task )
{
	m_save_replay = new ReplayManager();
//	m_save_replay->LoadFromFile( GameSystem::game_info.demo_file );
	m_main_task = main_task;
}

RecordAndCheckDemoTask::~RecordAndCheckDemoTask()
{
	SAFE_RELEASE(m_save_replay);
}

void RecordAndCheckDemoTask::run()
{
	// PAUSEボタンが押されたら終了
	if( InputData::input_keys & KEY_PAUSE )
	{
		m_save_replay->SaveToFile( GameSystem::game_info.demo_file );
		SAFE_RELEASE(m_save_replay);
		GameSound::sound_manager->Stop( GameSound::game_over );
		m_main_task->StopBGM();
		GameSound::sound_manager->Stop( GameSound::clear_bgm );

		m_main_task->ReleaseCourse();
		GameSystem::draw_task->Release();
		GameSystem::all_task->Release();

		GameSystem::all_task->AddTask( new MainMenu() , 0x01 );
		return;
	}

	// このフレームのキーをpush
	m_save_replay->AddKey( InputData::input_keys );
}
