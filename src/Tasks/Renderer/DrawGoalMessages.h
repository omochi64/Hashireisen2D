
#pragma once

#include "System/TaskSystem.h"
#include "Tasks/AfterGoaledTask.h"

class DrawGoalMessages:public TaskControllBlock
{
	AfterGoaledTask *m_contact_task;
	Math::Point2DI draw_pos1,draw_pos2;
	bool	is_animating;
	char	m_show_string_line1[256];
	char	m_show_string_line2[256];
	int		m_now_line;
	const static int animation_duration = 100;
	DWORD m_start_time,m_pause_time;
public:
	DrawGoalMessages(AfterGoaledTask *agt);
	void Pause();
	void Resume();
	void run();
};
