#pragma once

#include "System/GlobalNamespace.h"

class ActTask:public TaskControllBlock
{
public:
	void run()
	{
		GameSystem::all_object->ActAll();
	};
};

class DrawTask:public TaskControllBlock
{
public:
	void run()
	{
		GameSystem::all_object->DrawAll( GameSystem::pScene , GameSystem::SCREEN_WIDTH , GameSystem::SCREEN_HEIGHT );
	}
};
