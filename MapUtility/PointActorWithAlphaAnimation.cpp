
#include "PointActorWithAlphaAnimation.h"

PointActorWithAlphaAnimation::PointActorWithAlphaAnimation()
:ActorWithAlphaAnimation()
{
}

PointActorWithAlphaAnimation::~PointActorWithAlphaAnimation()
{
}

// アクターのアップデート
bool PointActorWithAlphaAnimation::UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	Renderer::Object::IPoint3D *primitive = pRender->CreatePoint3D( 1 , false );
	primitive->Begin();

	Renderer::SVertex3DBase vertex;
	vertex.Pos = Math::Vector3D(0,0,0);
	vertex.Col = m_vertex_color;
	vertex.Col.a = m_alpha_animation.GetNowAlpha();

	primitive->Push( &vertex , 1 );

	primitive->End();

	SAFE_RELEASE(m_pActor);
	m_pActor = pSceneMgr->CreateActor( primitive );
	m_pActor->Translation( m_position );

	SAFE_RELEASE(primitive);

	return true;
}
