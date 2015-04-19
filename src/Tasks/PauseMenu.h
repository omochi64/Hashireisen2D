
#pragma once

#include "System/TaskSystem.h"
#include "GameMain.h"
#include "CameraController.h"
#include "Game/MenuController.h"
#include "Game/BackgroundController.h"
#include "Tasks/Renderer/DrawMenu.h"

class PauseMenu:public TaskControllBlock
{
	GameMain *m_main_task;
	CameraController *m_camera;

	MenuController	*m_pMenu;
	BackgroundController *m_pBlackBG;

	DrawMenu	*m_draw_menu;
	std::vector<BackgroundController *> m_draw_list;

public:
	PauseMenu( GameMain *main_task , CameraController *camera );
	virtual ~PauseMenu();
	void run();
};
