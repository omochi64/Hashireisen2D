

#include "GoToNextStage.h"
#include "InitializeGame.h"
#include "EndingTask.h"

void GoToNextStage::run()
{
	int now_world = m_main_task->GetWorldNum();
	int now_course = m_main_task->GetCourseNum();
	int course_per_world = GameSystem::course_manager->GetCourseNumPerWorld();
	int world_num = GameSystem::course_manager->GetWorldNum();

	int next_world;
	int next_course;

	if( now_course == course_per_world )
	{
		next_world = now_world+1;
		next_course = 1;
	}
	else
	{
		next_world = now_world;
		next_course = now_course+1;
	}

	if( next_world > world_num )
	{
		// 全クリ
		m_main_task->ReleaseCourse();
		GameSystem::draw_task->Release();
		GameSystem::all_task->Release();
		
		GameSystem::all_task->AddTask( new EndingTask() , 0x01 );
		return;
	}
	
	// 次のステージへ
	m_main_task->ReleaseCourse();
	GameSystem::draw_task->Release();
	GameSystem::all_task->Release();
	GameSystem::all_task->AddTask( new InitializeGame(next_world,next_course) , 0x002 );
}
