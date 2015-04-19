
#pragma once

#include "Selene.h"
#include "System/TaskSystem.h"

using namespace Selene;

#define	BACKGROUND_Z	50.0f

class DrawBackgroundTask:public TaskControllBlock
{
	Renderer::ITexture	*m_texture,*m_render_texture;
	Renderer::Object::IPrimitive2D *m_pPrimitive;
	int	tex_w,tex_h;
	int last_start_x,last_start_y;
	static const int	calibration_x = 10;
	static const int	calibration_y = 30;
//	float	background_z;
	Renderer::ITexture *m_pTopTex,*m_pCenterTex,*m_pUnderTex;

	std::vector<Drawable2DObject *> m_backgrounds;

//	Renderer::Object::IPrimitive3D *m_primitive;
//	Scene::IPrimitiveActor *m_actor[9];

public:
	DrawBackgroundTask( Renderer::ITexture *pTopTex , Renderer::ITexture *pCenterTex , Renderer::ITexture *pUnderTex , Math::Point2DI course_size );
	virtual ~DrawBackgroundTask();

	void run(void);
};
