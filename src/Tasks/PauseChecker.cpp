
#include "PauseChecker.h"
#include "System/GlobalNamespace.h"
#include "PauseMenu.h"
#include "System/InputData.h"

PauseChecker::PauseChecker( GameMain *main_task , CameraController *pCamera )
{
	m_main_task = main_task;
	m_camera = pCamera;
}

void PauseChecker::run()
{
//	static bool is_pause = false;
	if( InputData::input_key_strokes[KEY_PAUSE] == 1 )
	{
		// ポーズメニュー表示タスクへ遷移
/*		if( is_pause )
		{
			m_main_task->StartUdonge();
			m_main_task->StartAllEnemies();
			m_camera->Start();
			m_main_task->ResumeCountingTime();
		}
		else
*/		{
			m_main_task->StopUdonge();
			m_main_task->StopAllEnemies();
			m_camera->Stop();
			m_main_task->StopCountingTime();
		}

		GameSystem::all_task->ChangeTask( m_id , new PauseMenu( m_main_task , m_camera ) );
	}
}
