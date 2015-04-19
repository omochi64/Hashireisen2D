
#include "EndingTask.h"
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"
#include "System/InputData.h"
#include "System/Sounds.h"
#include "MainMenu.h"
#include "Tasks/Renderer/DrawEndingTask.h"
#include "System/GameSystem.h"

EndingTask::EndingTask()
{
	m_pScript = new EndingScript(GameSystem::pFonts[GameSystem::FONT_ENDING] , GameSystem::SCREEN_WIDTH , GameSystem::SCREEN_HEIGHT );
	if( !m_pScript->LoadFromFile( GameSystem::game_info.ending_script , GameSystem::pRender , GameSystem::tex_manager ,
			GameSound::sound_manager , GameSystem::pCore ) )
	{
		SAFE_RELEASE(m_pScript);
		return;
	}

	m_pScript->StartScript();
	GameSound::sound_manager->Stop( GameSound::clear_bgm );
	m_pScript->PlayBGM( GameSound::sound_manager );

	GameSystem::draw_task->AddTask( new DrawEndingTask( m_pScript ) , 0x02 );

	m_frame_after_end_script = 0;
}

EndingTask::~EndingTask()
{
	SAFE_RELEASE(m_pScript);
}

void EndingTask::run()
{
	if( InputData::input_keys & KEY_JUMP )
	{
		m_pScript->SpeedUpTimes(4);
	}
	else
	{
		m_pScript->ResetSpeed();
	}

	// スクリプトアップデート
	if( !m_pScript->UpdateScript() )
	{
		m_frame_after_end_script++;
		// 何か押されるまで終わらない
		if( !InputData::input_keys && m_frame_after_end_script < GameSystem::game_info.ending_script_time_out )
		{
			return;
		}
		// 終了。メインメニューに戻る
		m_pScript->StopBGM( GameSound::sound_manager );

		SAFE_RELEASE(m_pScript);

		GameSystem::draw_task->Release();
		GameSystem::all_task->Release();

		GameSystem::all_task->AddTask( new MainMenu() , 0x01 );
	}
}
