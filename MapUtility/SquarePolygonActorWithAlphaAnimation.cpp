
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

// ���ʂ̐ݒ�
void SquarePolygonActorWithAlphaAnimation::SetReversible(bool is_reversible)
{
	m_is_reversible = is_reversible;
}

// ���_�̐ݒ�
void SquarePolygonActorWithAlphaAnimation::SetVertexPosition(Selene::Math::Vector3D vPositions[])
{
	int i;
	for( i=0 ; i<4 ; i++ )
	{
		m_vPositions[i] = vPositions[i];
	}
}

// Actor�̍쐬
bool SquarePolygonActorWithAlphaAnimation::CreateActor( Math::Vector3D vPositions[] , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , bool is_reversible )
{
	SetReversible( is_reversible );
	SetVertexPosition( vPositions );
	return UpdateActor( pRender , pSceneMgr );
}

// �A�N�^�[�̃A�b�v�f�[�g
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

	// reversible���ǂ����Œ��_�`�揇����Push���ς��
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
