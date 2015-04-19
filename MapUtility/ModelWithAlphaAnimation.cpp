
#include "ModelWithAlphaAnimation.h"
/*
class ModelWithAlphaAnimation
{
protected:
	AlphaAnimation	m_alpha_animation;
	Scene::IModelActor *m_pActor;
	CColor	m_vertex_color;

public:

	ModelWithAlphaAnimation();
	virtual ~ModelWithAlphaAnimation();
	virtual void Release();

	Scene::IModelActor *GetActor();
	AlphaAnimation &GetAlphaAnimation();
	bool UpdateAlphaAnimation();
	void SetVertexColor( CColor color );
};
*/

ModelWithAlphaAnimation::ModelWithAlphaAnimation()
:m_alpha_animation()
{
	m_pActor = NULL;
	m_vertex_color = CColor(0,0,0);
}

ModelWithAlphaAnimation::~ModelWithAlphaAnimation()
{
	Release();
}

void ModelWithAlphaAnimation::Release()
{
	SAFE_RELEASE(m_pActor);
}

// アクターの作成
bool ModelWithAlphaAnimation::CreateActor( const char *fname , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	Renderer::Object::IModel *pModel = pRender->CreateModelFromFile( fname );
	if( !pModel || pModel->IsInvalid() )
	{
		SAFE_RELEASE(pModel);
		return false;
	}

	Scene::IModelActor *pActor = pSceneMgr->CreateActor( pModel );
	SAFE_RELEASE(pModel);
	if( !pActor || pActor->IsInvalid() )
	{
		SAFE_RELEASE(pActor);
		return false;
	}

	Math::Vector3D pos(0,0,0);
	if( m_pActor )
	{
		m_pActor->GetPosition(pos);
	}
	SAFE_RELEASE(m_pActor);
	m_pActor = pActor;
	m_pActor->Translation( pos );


	return true;
}

// アクターのアタッチ
void ModelWithAlphaAnimation::AttachActor(Selene::Scene::IModelActor *pActor , bool release_before_actor )
{
	if( release_before_actor )
	{
		SAFE_RELEASE(m_pActor);
	}
	m_pActor = pActor;
}

bool ModelWithAlphaAnimation::UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	if( !m_pActor )
	{
		return true;
	}
	m_vertex_color.a = m_alpha_animation.GetNowAlpha();

	int i,j;
	int mesh = m_pActor->GetMeshCount();
	int material;

	for( i=0 ; i<mesh ; i++ )
	{
		material = m_pActor->GetMeshMaterialCount(i);
		for( j=0 ; j<material ; j++ )
		{
			m_pActor->MeshMaterial_SetMaterialColor( i , j ,
				Math::Vector4D( m_vertex_color.r/255.0f , m_vertex_color.g/255.0f , m_vertex_color.b/255.0f , m_vertex_color.a/255.0f ) );
		}
	}

	return true;
}

// ポジション設定
void ModelWithAlphaAnimation::SetPosition(const Selene::Math::Vector3D &pos)
{
	if( m_pActor )
	{
		m_pActor->Translation( pos );
	}
}

// アクター取得
Scene::IModelActor *ModelWithAlphaAnimation::GetActor()
{
	return m_pActor;
}

AlphaAnimation &ModelWithAlphaAnimation::GetAlphaAnimation()
{
	return m_alpha_animation;
}

bool ModelWithAlphaAnimation::UpdateAlphaAnimation()
{
	return m_alpha_animation.UpdateAnimation();
}

void ModelWithAlphaAnimation::SetVertexColor( CColor color )
{
	m_vertex_color = color;
}
