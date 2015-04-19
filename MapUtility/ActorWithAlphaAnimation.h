
#pragma once

#include "AlphaAnimation.h"
#include "Selene.h"
using namespace Selene;

// αアニメーションを持ったアクター
class ActorWithAlphaAnimation
{
protected:
	AlphaAnimation	m_alpha_animation;
	CColor			m_vertex_color;
	Scene::IPrimitiveActor	*m_pActor;
	Math::Vector3D	m_position;

public:
	ActorWithAlphaAnimation();
	virtual ~ActorWithAlphaAnimation();
	virtual void Release();

	virtual bool UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr ) = 0;
	Scene::IPrimitiveActor *GetActor();
	AlphaAnimation &GetAlphaAnimation();
	bool UpdateAlphaAnimation();
	void SetVertexColor( CColor color );

	void SetPosition( const Math::Vector3D &pos );
};
