
#include "AlphaAnimation.h"
#include "Selene.h"
using namespace Selene;

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

	void AttachActor( Scene::IModelActor *pActor , bool release_before_actor = true );
	bool CreateActor( const char *fname , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );
	void SetPosition( const Math::Vector3D &pos );

	virtual bool UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );
	Scene::IModelActor *GetActor();
	AlphaAnimation &GetAlphaAnimation();
	bool UpdateAlphaAnimation();
	void SetVertexColor( CColor color );
};
