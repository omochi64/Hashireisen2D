
#pragma once

#include "Selene.h"
#include "System/TaskSystem.h"

using namespace Selene;

class CreateBackgroundTexture:public TaskControllBlock
{
	Renderer::ITexture *m_pUnderTex,*m_pCenterTex,*m_pTopTex;
	Renderer::ITexture *m_pBackTex;
	Renderer::Object::IPrimitive2D *m_pUnderPrimitive,*m_pCenterPrimitive,*m_pTopPrimitive;
	bool	m_is_first;

public:
	CreateBackgroundTexture( Renderer::ITexture *pBackTex , Renderer::ITexture *pUnderTexture , Renderer::ITexture *pCenterTexture , 
		Renderer::ITexture *pTopTexture );
	virtual ~CreateBackgroundTexture();
	void run();
};
