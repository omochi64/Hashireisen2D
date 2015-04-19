
#pragma once

#include "Selene.h"
#include "System/TaskSystem.h"

using namespace Selene;


class InitializeGame:public TaskControllBlock
{
	int m_world,m_course;

public:
	InitializeGame(int world,int course);
	void run();
};
