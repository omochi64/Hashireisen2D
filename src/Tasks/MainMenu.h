
#pragma once

#include "System/TaskSystem.h"
#include "Game/MenuController.h"
#include "Game/BackgroundController.h"
#include "Tasks/Renderer/DrawMenu.h"
#include "GameMain.h"

class MainMenu:public TaskControllBlock
{
	MenuController	*m_pMenu;
	BackgroundController	*m_pBack,*m_pTitle,*m_pWhiteOut;

	std::vector<BackgroundController *>	m_draw_list;
	bool m_is_doing_animation;
	DWORD	m_non_input_frame;
//	GameMain *m_demo_main_task;
	DrawMenu *m_dmm;
	bool m_is_doing_demo;

public:
	MainMenu(bool will_do_title_animation = true, bool will_start_bgm = true , int ini_select_menu =0 );
	virtual ~MainMenu();

	void run();

private:
	bool CreateBackTitleWhiteOut( BackgroundController **pBack , BackgroundController **pTitle , BackgroundController **pWhiteOut ); 
};
