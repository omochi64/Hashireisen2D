
#pragma once

#include "System/TaskSystem.h"
#include "Game/EndingScript.h"

class DrawEndingTask:public TaskControllBlock
{
	EndingScript	*m_pDrawEnding;

public:
	DrawEndingTask( EndingScript *pDrawEnding );
	virtual ~DrawEndingTask();

	void run();
};
