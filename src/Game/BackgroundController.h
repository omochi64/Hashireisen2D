
#pragma once

#include "Selene.h"

using namespace Selene;

// 2DâÊëúÇÃä«óùÅDIPrimitive2DÇégÇ¡ÇΩï`âÊ
class BackgroundController
{
	Renderer::ITexture *m_pTexture;
	Renderer::Object::IPrimitive2D	*m_pPrimitive;
	Math::Point2DI m_size;

	Math::Vector2D	m_firstPos,m_speedPos;
	Math::Vector2D	m_nowPos;
	Math::Vector4D	m_firstColor,m_speedColor;
	Math::Vector4D	m_nowColor;
	DWORD			m_posAnimationDuration,m_colorAnimationDuration;
//	DWORD			m_posAnimationTime,m_colorAnimationTime;
	DWORD			m_posAnimationPastFrame,m_colorAnimationPastFrame;
//	DWORD			m_posAnimationStopTime,m_colorAnimationStopTime;

	bool			m_did_start_pos_animation;
	bool			m_did_start_color_animation;

	float			m_z;

	Renderer::SPrimitiveVertex2D	m_vertex[2];

public:

	BackgroundController( Renderer::IRender *pRender );
	~BackgroundController();

	void SetTexture( Renderer::ITexture *pTexture ,
		Math::Vector2D uv_leftup = Math::Vector2D(0,0) , Math::Vector2D uv_rightbottom = Math::Vector2D(1.0f,1.0f) );
	Renderer::ITexture *GetTexture();
	void SetSize( Math::Point2DI size );
	const Math::Point2DI &GetSize();
	void SetPosWithoutAnimation( Math::Vector2D pos );
	void SetColorWithoutAnimation( CColor color );

	void SetPosAnimation1( Math::Vector2D firstPos , Math::Vector2D lastPos , DWORD duration );
	void SetPosAnimation2( Math::Vector2D firstPos , Math::Vector2D speed , DWORD duration );

	void SetColorAnimation1( CColor firstColor , CColor lastColor , DWORD duration );
	void SetColorAnimation2( CColor firstColor , Math::Vector4D speed , DWORD duration );

	void SetZ( float z );

	void StartPosAnimation();
	void StartColorAnimation();
	void StopPosAnimation();
	void StopColorAnimation();
	void ResumePosAnimation();
	void ResumeColorAnimation();
	void EndPosAnimationAtOnce();
	void EndColorAnimationAtOnce();

	bool UpdatePosAnimation();
	bool UpdateColorAnimation();

	Renderer::Object::IPrimitive2D *GetPrimitive();

	void Draw();

	void Release();
};
