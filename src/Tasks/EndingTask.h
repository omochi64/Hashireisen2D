
#pragma once

#include "System/TaskSystem.h"
#include "Game/EndingScript.h"
//#include "DrawEndingTask.h"

class EndingTask:public TaskControllBlock
{
	EndingScript	*m_pScript;
	DWORD			m_frame_after_end_script;

public:
	EndingTask();
	virtual ~EndingTask();

	void run();
};
