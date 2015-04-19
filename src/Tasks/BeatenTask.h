
#pragma once

#include "Selene.h"
#include "System/TaskSystem.h"
#include "Game/Objects.h"
#include "Game/Course.h"
#include "System/GlobalNamespace.h"
#include "GameMain.h"

using namespace Selene;

class BeatenTask:public TaskControllBlock
{
	int m_world , m_course;
	GameMain *m_main_task;
public:
	BeatenTask( GameMain *main );
	void run();
};
