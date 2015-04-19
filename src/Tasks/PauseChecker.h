
#pragma once

#include "System/TaskSystem.h"
#include "GameMain.h"
#include "CameraController.h"

class PauseChecker:public TaskControllBlock
{
	GameMain	*m_main_task;
	CameraController *m_camera;
public:
	PauseChecker( GameMain *main_task , CameraController *camera );
	void run();
};
