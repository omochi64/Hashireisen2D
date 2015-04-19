
#pragma once

#include "Game/Objects.h"
#include "Game/UdongeAnimation.h"
#include "Tasks/GameMain.h"

class DrawStatus:public TaskControllBlock
{
	int m_world,m_course;

	Renderer::ITexture *m_heart_full_tex;
	Renderer::ITexture *m_heart_empty_tex;
	Renderer::ITexture *m_point_item_tex;
	Renderer::Object::IPrimitive2D *m_heart_full;
	Renderer::Object::IPrimitive2D *m_heart_empty;
	Renderer::Object::IPrimitive2D *m_point_item;

	Renderer::SPrimitiveVertex2D m_heart_vertics[3][2];
	Renderer::SPrimitiveVertex2D m_point_vertex[2];
	GameMain *m_main_task;

public:
	DrawStatus( int world , int course , GameMain *main_task );
	virtual ~DrawStatus();
	void run();
};
