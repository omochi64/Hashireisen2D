
#pragma once

#include "System/GlobalNamespace.h"
#include "System/TaskSystem.h"
#include "GameMain.h"

#define MAX_MOVE_X	3.7f
#define MAX_MOVE_Y	2.0f
#define MAX_MOVE_Z_NEAREST	5.5f
#define MAX_MOVE_Z_FAREST	8.5f
#define CAM_MOVE_SPEED	0.15f
#define	CAM_MOVE_SPEED_Z	0.12f

#define	CHECK_VISIBLE_AREA_WIDTH	37
#define	CHECK_VISIBLE_AREA_HEIGHT	27

#define		DEFAULT_DISTANCE_TO_CAMERA	7.0f

class CameraController:public TaskControllBlock
{
	Math::Vector3D before_pos,before_lookat;
	Math::Vector3D move;
	GameMain *m_main_task;
	bool	m_is_stopped;
public:
	CameraController(GameMain *main_task);
	void Stop(){m_is_stopped = true;};
	void Start(){m_is_stopped = false;};
	void run();
	void GetCheckVisibleArea( Math::Rect2DF &area );
};
