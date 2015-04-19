
#include "ReplayDemoTask.h"
#include "System/GlobalNamespace.h"
#include "MainMenu.h"
#include "System/Sounds.h"
#include "System/IniInfo.h"
#include "System/InputData.h"

ReplayDemoTask::ReplayDemoTask(GameMain *main_task,MenuController *main_mc)
{
	m_main_task = main_task;
	m_main_mc = main_mc;

	m_key_strokes.clear();

	m_play_replay = new ReplayManager();
	if( m_play_replay )
	{
		m_is_opened = m_play_replay->LoadFromFile( GameSystem::game_info.demo_file , GameSystem::pFileManager );
	}
	else
	{
		m_is_opened = false;
	}
}

ReplayDemoTask::~ReplayDemoTask()
{
	SAFE_RELEASE(m_play_replay);
}

void ReplayDemoTask::run()
{
	if( !m_is_opened )
	{
		return;
	}

	int input_data = m_play_replay->GetNextKey();

	bool is_inputed_any_key = false;
	// このフレームのキーをInputDataに適用。strokesにも反映させる
	int i;
	InputData::input_keys = input_data;
	int key;
	for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
	{
		key = 1<<i;
		if( ( input_data & key ) )
		{
			m_key_strokes[key]++;
		}
		else
		{
			m_key_strokes[key] = 0;
		}
		if( InputData::input_key_strokes[key] == 1 )
		{
			is_inputed_any_key = true;
			break;
		}
		InputData::input_key_strokes[key] = m_key_strokes[key];
	}

	// リプレイ終了 or 何かキーが押されたらメインメニューに戻る
	if( input_data == -1 || is_inputed_any_key )
	{
		GameSound::sound_manager->Stop( GameSound::game_over );
		m_main_task->StopBGM();
		GameSound::sound_manager->Stop( GameSound::clear_bgm );

		SAFE_RELEASE(m_play_replay);

		GameSystem::draw_task->Release();
		GameSystem::all_task->Release();

		GameSystem::all_task->AddTask( new MainMenu() , 0x01 );
		
		return;
	}


}
