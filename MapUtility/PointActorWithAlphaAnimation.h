
#pragma once

#include "ActorWithAlphaAnimation.h"

// �|�C���g�A�N�^�[�p
class PointActorWithAlphaAnimation:public ActorWithAlphaAnimation
{
public:
	PointActorWithAlphaAnimation();
	virtual ~PointActorWithAlphaAnimation();
	virtual bool UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );

	
};
