
#pragma once

#include "System/TaskSystem.h"
#include "System/Manager/ReplayManager.h"
#include "GameMain.h"
#include "Game/MenuController.h"

#include <map>

class ReplayDemoTask:public TaskControllBlock
{
	ReplayManager	*m_play_replay;
	GameMain		*m_main_task;
	bool			m_is_opened;
	std::map<int,int>	m_key_strokes;
	MenuController	*m_main_mc;

public:
	ReplayDemoTask(GameMain *main_task,MenuController *main_mc);
	bool IsAccuratelyOpened();
	virtual ~ReplayDemoTask();
	void run();
};
