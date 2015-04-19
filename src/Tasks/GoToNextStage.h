
#pragma once

#include "GameMain.h"


// 次のコースへと進むためのタスク
class GoToNextStage:public TaskControllBlock
{
	GameMain *m_main_task;
	MultipleAnimation *m_udonge;
public:
	GoToNextStage( GameMain *main_task , MultipleAnimation *udonge )
	{
		m_main_task = main_task;
		m_udonge = udonge;
	};
	void run(void);
};
