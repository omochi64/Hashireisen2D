
#pragma once

#include "System/TaskSystem.h"
#include "Tasks/GameMain.h"

class DrawGameOverTask:public TaskControllBlock
{
	DWORD	m_wait_frame;
	GameMain *m_main_task;
	DWORD	m_past_frame;
	Math::Point2DF m_str_size;

public:
	DrawGameOverTask( GameMain *main_task , DWORD wait_frame );
	void run();
};
