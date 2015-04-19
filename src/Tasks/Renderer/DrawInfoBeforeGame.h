
#pragma once

#include "System/TaskSystem.h"
#include "Selene.h"

using namespace Selene;

class DrawInfoBeforeGame:public TaskControllBlock
{
	int m_world,m_course;

	Renderer::ITexture *m_udonge_tex;
	Renderer::Object::IPrimitive2D *m_pUdongePrimitive;
	Renderer::SPrimitiveVertex2D m_udonge_vertex[2];
	GameSystem::PRIMITIVE2D_SET m_set;
	Math::Point2DF	m_worldStringPos,m_restLifeStringPos;

	// 他にもスコア情報とか必要
public:
	DrawInfoBeforeGame( int world , int course );
	virtual ~DrawInfoBeforeGame();

	void run();
};
