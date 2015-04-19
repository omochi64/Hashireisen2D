
#pragma once

#include "System/TaskSystem.h"
#include "System/Manager/ReplayManager.h"
#include "GameMain.h"

class RecordAndCheckDemoTask:public TaskControllBlock
{
	ReplayManager *m_save_replay;
	GameMain *m_main_task;

public:
	RecordAndCheckDemoTask( GameMain *main_task );
	virtual ~RecordAndCheckDemoTask();
	void run();
};
