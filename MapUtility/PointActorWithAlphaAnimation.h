
#pragma once

#include "ActorWithAlphaAnimation.h"

// ポイントアクター用
class PointActorWithAlphaAnimation:public ActorWithAlphaAnimation
{
public:
	PointActorWithAlphaAnimation();
	virtual ~PointActorWithAlphaAnimation();
	virtual bool UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );

	
};
