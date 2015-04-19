
#include "ActorWithAlphaAnimation.h"


ActorWithAlphaAnimation::ActorWithAlphaAnimation():
m_alpha_animation()
{
	m_pActor = NULL;
	m_vertex_color = CColor(0xff,0xff,0xff);
	m_position = Math::Vector3D(0,0,0);
}

ActorWithAlphaAnimation::~ActorWithAlphaAnimation()
{
	Release();
}

void ActorWithAlphaAnimation::Release()
{
	SAFE_RELEASE(m_pActor);
}

// アクター取得
Scene::IPrimitiveActor *ActorWithAlphaAnimation::GetActor()
{
	return m_pActor;
}

// αアニメーション取得
AlphaAnimation &ActorWithAlphaAnimation::GetAlphaAnimation()
{
	return m_alpha_animation;
}

// αアニメーションアップデート
bool ActorWithAlphaAnimation::UpdateAlphaAnimation()
{
	bool res = m_alpha_animation.UpdateAnimation();
	m_vertex_color.a = m_alpha_animation.GetNowAlpha();
	return res;
}

// カラー設定
void ActorWithAlphaAnimation::SetVertexColor( CColor color )
{
	m_vertex_color = color;
}

void ActorWithAlphaAnimation::SetPosition( const Math::Vector3D &pos )
{
	m_position = pos;
	if( m_pActor )
	{
		m_pActor->Translation( pos );
	}
}
