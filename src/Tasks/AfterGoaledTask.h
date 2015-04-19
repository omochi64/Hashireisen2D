
#pragma once

#include "System/TaskSystem.h"
#include "Game/Course.h"
#include "GameMain.h"

class DrawGoalMessages;

class AfterGoaledTask:public TaskControllBlock
{
	Course *m_course;
	GameMain *main_task;
	bool	m_did_start_running;
	bool	m_is_starting;
	bool	m_did_start_reduce_time;
	DrawGoalMessages	*m_dgm;
	DWORD				m_start_running_time;
public:
	AfterGoaledTask( GameMain *main_task , Course *now_course );
	void startRunningUdonge();
	void run();
};
