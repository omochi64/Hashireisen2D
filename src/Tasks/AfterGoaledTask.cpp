
#include "AfterGoaledTask.h"
#include "System/GlobalNamespace.h"
#include "GoToNextStage.h"
#include "System/IniInfo.h"

#include "Tasks/Renderer/DrawGoalMessages.h"

#include "System/Sounds.h"
#include "Game/UdongeActionConstances.h"

using namespace Udonge;

AfterGoaledTask::AfterGoaledTask( GameMain *main_task , Course *now_course )
{
	this->main_task = main_task;
	m_course = now_course;
	m_did_start_running = false;
	m_is_starting = false;
	m_did_start_reduce_time = false;
	Character::udonge->SetSpeedX(0);
	Character::udonge->SetAccelX(0);
	if( Character::udonge->GetNowAnimation() != Character::udonge->GetAnimations().jump )
	{
		Character::udonge->SetAnimationToStand();
	}
	main_task->StopAllEnemies();
	GameSystem::is_searching_udonge = false;
	GameSystem::is_searching_only_y = true;
	GameSystem::draw_task->AddTask( (m_dgm = new DrawGoalMessages( this )) , 0xfff );

	main_task->StopBGM();
	GameSound::sound_manager->Play( GameSound::clear_bgm );
}

#define GO_TO_NEXT_COURSE_TIME_LIMIT	3000

void AfterGoaledTask::run()
{
	if( main_task->IsStoppedUdonge() )
	{
		m_dgm->Pause();
		return;
	}
	else if( !m_did_start_running && !m_did_start_reduce_time && !m_is_starting )
	{
		m_dgm->Resume();
	}

	if( !m_did_start_running && !m_did_start_reduce_time && !m_is_starting )
	{
		return;
	}

	// 時間→得点の処理
	if( m_did_start_reduce_time )
	{
		main_task->ReduceTime(GameSystem::GAME_FPS);
		Character::udonge->IncreasePoint( GameSystem::game_info.time_to_point );
		if( main_task->GetRestTime() == 0 )
		{
			m_is_starting = true;
			m_did_start_reduce_time = false;
		}
		GameSound::sound_manager->Stop( GameSound::time_to_point );
		GameSound::sound_manager->Play( GameSound::time_to_point );
		return;
	}

	// うどんげを走らせるアニメーション開始
	if( m_is_starting )
	{
		m_did_start_running = true;
		m_did_start_reduce_time = false;
		m_is_starting = false;
		Character::udonge->SetSpeedX(MAX_WALK_SPEED/2);
		Character::udonge->SetAnimationToWalk();
		Character::udonge->SetDirection(true);
		m_start_running_time = GameSystem::now_frame_time;
	}

	// うどんげが視界から消えた？
	if( Character::udonge->GetInvisiblePeriod() > 0 || GameSystem::now_frame_time-m_start_running_time > GO_TO_NEXT_COURSE_TIME_LIMIT )
	{
		GameSystem::all_task->AddTask( new GoToNextStage( main_task , Character::udonge ) , 0xfffff );
		GameSound::sound_manager->Stop( GameSound::clear_bgm );
		return;
	}

	// 邪魔な目の前の物体を消す
	const Math::Vector3D &pos = Character::udonge->GetPos();
	m_course->DeleteObject( Math::Rect2DF( pos.x , pos.y-Character::udonge->GetHeight()/3 , Character::udonge->GetWidth()*2 ,
		Character::udonge->GetHeight() ) );
}

void AfterGoaledTask::startRunningUdonge()
{
	m_did_start_reduce_time = true;
}
