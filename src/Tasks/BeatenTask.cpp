
#include "BeatenTask.h"
#include "InitializeGame.h"
#include "Tasks/Renderer/DrawGameOverTask.h"
#include "System/Sounds.h"
#include "System/IniInfo.h"

BeatenTask::BeatenTask( GameMain *main )
{
	m_main_task = main;
	m_world = main->GetWorldNum();
	m_course = main->GetCourseNum();
	udonge->SetAnimationToGameOver();
	GameSystem::is_searching_udonge = false;
	GameSystem::is_searching_only_y = false;

	// 全ての敵の動きを止める
	main->StopAllEnemies();

	main->StopBGM();
	GameSound::sound_manager->Play( GameSound::game_over );
}

void BeatenTask::run()
{
	// とりあえずゲームオーバーアニメーションが完了するまでは続ける
	if( m_main_task->IsStoppedUdonge() )
	{
		Character::udonge->PauseAnimation();
		return;
	}
	else
	{
		Character::udonge->ResumeAnimation();
	}

	if( !Character::udonge->UpdateAnimation() )
	{
		// 残機を1減らしてこのコースをもう一回やる
		if( !Character::udonge->ReduceRestLife() )
		{
			DrawGameOverTask *dgot = new DrawGameOverTask( m_main_task , GameSystem::game_info.game_over_wait_frame );
			GameSound::sound_manager->Stop( GameSound::game_over );
//			GameSound::sound_manager->Stop( GameSound::game_over );
			
			m_main_task->ReleaseCourse();
			GameSystem::draw_task->Release();
			GameSystem::draw_task->AddTask( dgot , 0x02 );
			GameSystem::all_task->Release();
			return;
		}

		Character::udonge->SetHPToDefault();
		Character::udonge->EndNoEnemyMode();
		GameSound::sound_manager->Stop( GameSound::game_over );

		int world = m_world , course = m_course;
		m_main_task->ReleaseCourse();
		GameSystem::all_task->Release();
		GameSystem::draw_task->Release();
		GameSystem::all_task->AddTask( new InitializeGame(world,course) , 0x002 );
	}
}
