
#include "BoxActorWithAlphaAnimation.h"

/*
class BoxActorWithAlphaAnimation
{
protected:
	AlphaAnimation	m_alpha_animation;
	MultipleAnimation3DBox	*m_box;
	CColor	m_vertex_color;

public:

	BoxActorWithAlphaAnimation();
	virtual ~BoxActorWithAlphaAnimation();
	virtual void Release();

	bool CreateActor( float width , float height , float depth , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );
	void SetPosition( const Math::Vector3D &pos );

//	virtual void UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr ) = 0;
	Scene::IModelActor *GetActor();
	AlphaAnimation &GetAlphaAnimation();
	bool UpdateAlphaAnimation();
	void SetVertexColor( CColor color );
};

*/

BoxActorWithAlphaAnimation::BoxActorWithAlphaAnimation()
:ActorWithAlphaAnimation(),m_box()
{
}

BoxActorWithAlphaAnimation::~BoxActorWithAlphaAnimation()
{
	Release();
}


bool BoxActorWithAlphaAnimation::CreateActor( float width , float height , float depth , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	Math::Matrix identity;
	identity.Identity();
	m_box.CreateBox( Math::Vector3D(-width/2,-height/2,-depth/2) , Math::Vector3D(width/2,height/2,depth/2) , identity );

	Renderer::Object::ILine3D *pLine = pRender->CreateLine3D( 24 , false );
	pLine->Begin();
	pLine->PushBox( m_box , m_vertex_color );
	pLine->End();

	Scene::IPrimitiveActor *actor = pSceneMgr->CreateActor( pLine );
	if( !actor || actor->IsInvalid() )
	{
		SAFE_RELEASE(pLine);
		SAFE_RELEASE(actor);
	}
	SAFE_RELEASE(m_pActor);

	m_pActor = actor;
	m_pActor->Translation( m_position );
	SAFE_RELEASE(pLine);

	return true;
}

bool BoxActorWithAlphaAnimation::UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	Renderer::Object::ILine3D *pLine = pRender->CreateLine3D( 24 , false );
	pLine->Begin();
	pLine->PushBox( m_box , m_vertex_color );
	pLine->End();

	Scene::IPrimitiveActor *actor = pSceneMgr->CreateActor( pLine );
	if( !actor || actor->IsInvalid() )
	{
		SAFE_RELEASE(pLine);
		SAFE_RELEASE(actor);
		return false;
	}
	SAFE_RELEASE(m_pActor);

	m_pActor = actor;
	m_pActor->Translation( m_position );
	SAFE_RELEASE(pLine);

	return true;
}

void BoxActorWithAlphaAnimation::SetWidth( float width )
{
	Math::Matrix identity;
	identity.Identity();
	float height = m_box.vMax.y-m_box.vMin.y;
	float depth = m_box.vMax.z-m_box.vMin.z;
	m_box.CreateBox( Math::Vector3D(-width/2,-height/2,-depth/2) , Math::Vector3D(width/2,height/2,depth/2) , identity );
}
void BoxActorWithAlphaAnimation::SetHeight( float height )
{
	Math::Matrix identity;
	identity.Identity();
	float width = m_box.vMax.x-m_box.vMin.x;
	float depth = m_box.vMax.z-m_box.vMin.z;
	m_box.CreateBox( Math::Vector3D(-width/2,-height/2,-depth/2) , Math::Vector3D(width/2,height/2,depth/2) , identity );
}

void BoxActorWithAlphaAnimation::SetDepth( float depth )
{
	Math::Matrix identity;
	identity.Identity();
	float height = m_box.vMax.y-m_box.vMin.y;
	float width = m_box.vMax.x-m_box.vMin.x;
	m_box.CreateBox( Math::Vector3D(-width/2,-height/2,-depth/2) , Math::Vector3D(width/2,height/2,depth/2) , identity );
}