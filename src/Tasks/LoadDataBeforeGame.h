
#pragma once

#include "System/TaskSystem.h"
#include "GameMain.h"
#include "Tasks/Renderer/DrawBackgroundTask.h"
#include "Game/MenuController.h"

class LoadDataBeforeGame:public TaskControllBlock
{
	int m_world,m_course;
	bool did_end_load;
	DWORD m_mini_wait_time;
	DWORD m_start_time;
	GameMain *m_next_task;
	TaskControllBlock	*m_draw_info_task;
	Renderer::ITexture *m_pBackTop,*m_pBackCenter,*m_pBackUnder;
	DrawBackgroundTask *m_dgt;
	MenuController	*m_main_menu_controller;
	bool	m_is_record_demo;
	bool	m_is_replay_demo;
	bool	m_do_change_bgm;

public:
	LoadDataBeforeGame( int world , int course , DWORD minimum_wait_time ,
		TaskControllBlock *draw_info_task , bool do_change_bgm = true );
	void SetRecordDemoMode(){m_is_record_demo = true,m_is_replay_demo=false;}
	void SetReplayDemoMode(MenuController *mc){m_is_record_demo = false,m_is_replay_demo=true,m_main_menu_controller = mc;}
	void SetNormalMode(){m_is_record_demo = false,m_is_replay_demo=false;}
	void StartLoadAndWait();
	void run();
	GameMain *GetMainTask(){return m_next_task;}

private:
	bool LoadSound();
};
