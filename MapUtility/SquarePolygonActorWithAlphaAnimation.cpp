
#include "SquarePolygonActorWithAlphaAnimation.h"


SquarePolygonActorWithAlphaAnimation::SquarePolygonActorWithAlphaAnimation()
:ActorWithAlphaAnimation()
{
	int i;
	for( i=0 ; i<4 ; i++ )
	{
		m_vPositions[i] = Math::Vector3D(0,0,0);
	}
	m_is_reversible = true;
}

SquarePolygonActorWithAlphaAnimation::~SquarePolygonActorWithAlphaAnimation()
{
}

// 両面の設定
void SquarePolygonActorWithAlphaAnimation::SetReversible(bool is_reversible)
{
	m_is_reversible = is_reversible;
}

// 頂点の設定
void SquarePolygonActorWithAlphaAnimation::SetVertexPosition(Selene::Math::Vector3D vPositions[])
{
	int i;
	for( i=0 ; i<4 ; i++ )
	{
		m_vPositions[i] = vPositions[i];
	}
}

// Actorの作成
bool SquarePolygonActorWithAlphaAnimation::CreateActor( Math::Vector3D vPositions[] , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , bool is_reversible )
{
	SetReversible( is_reversible );
	SetVertexPosition( vPositions );
	return UpdateActor( pRender , pSceneMgr );
}

// アクターのアップデート
bool SquarePolygonActorWithAlphaAnimation::UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	Renderer::Object::IPrimitive3D *pPrimitive = pRender->CreatePrimitive3D( 4 , 12 , VERTEX_ELEMENT_3DBASE , false , false );

	if( !pPrimitive || pPrimitive->IsInvalid() )
	{
		return false;
	}

	m_vertex_color.a = m_alpha_animation.GetNowAlpha();

	pPrimitive->Begin();

	Renderer::SVertex3DBase vertex[4];
	int i;
	for( i=0 ; i<4 ; i++ )
	{
		vertex[i].Pos = m_vPositions[i];
		vertex[i].Col = m_vertex_color;
	}
	pPrimitive->Push( vertex , 4 );

	// reversibleかどうかで頂点描画順序のPushが変わる
	if( m_is_reversible )
	{
		Uint16 Idx[] = {
			0 , 1 , 2 , 0 , 2 , 3 , 0 , 3 , 2 , 0 , 2 , 1
		};
		pPrimitive->Push( Idx , 12 );
	}
	else
	{
		Uint16 Idx[] = {
			0 , 1 , 2 , 0 , 2 , 3
		};
		pPrimitive->Push( Idx , 6 );
	}

	pPrimitive->End();

	Scene::IPrimitiveActor *pActor = pSceneMgr->CreateActor( pPrimitive );
	SAFE_RELEASE(pPrimitive);
	if( !pActor || pActor->IsInvalid() )
	{
		SAFE_RELEASE(pActor);
		return false;
	}
	SAFE_RELEASE(m_pActor);
	m_pActor = pActor;

	return true;
}
