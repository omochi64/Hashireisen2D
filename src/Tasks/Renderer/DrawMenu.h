
#pragma once

#include "System/TaskSystem.h"
#include "Game/MenuController.h"
#include "Game/BackgroundController.h"

class DrawMenu:public TaskControllBlock
{
	MenuController	*m_pMenu;
	std::vector<BackgroundController *> *m_draw_list;
	bool	m_will_release_draw_list;
	bool	m_will_release_menu;
public:
	DrawMenu( std::vector<BackgroundController *> *draw_list , bool will_release_draw_list = false , bool will_release_menu = false );
	virtual ~DrawMenu();
	void SetMenu( MenuController *pMenu );
	void run();
};
