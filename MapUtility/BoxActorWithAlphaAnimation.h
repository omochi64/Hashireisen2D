
#include "AlphaAnimation.h"
#include "Selene.h"
using namespace Selene;

#include "ActorWithAlphaAnimation.h"

class BoxActorWithAlphaAnimation:public ActorWithAlphaAnimation
{
protected:
	Collision::CBox	m_box;

public:

	BoxActorWithAlphaAnimation();
	virtual ~BoxActorWithAlphaAnimation();

	bool CreateActor( float width , float height , float depth , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );

	virtual bool UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );

	void SetWidth( float width );
	void SetHeight( float height );
	void SetDepth( float depth );
};
