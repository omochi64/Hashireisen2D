
#include "AlphaAnimation.h"
#include "Selene.h"
using namespace Selene;

#include "ActorWithAlphaAnimation.h"

// 4ŠpŒ`ƒ|ƒŠƒSƒ“
class SquarePolygonActorWithAlphaAnimation:public ActorWithAlphaAnimation
{
protected:
	Math::Vector3D	m_vPositions[4];
	bool			m_is_reversible;

public:

	SquarePolygonActorWithAlphaAnimation();
	virtual ~SquarePolygonActorWithAlphaAnimation();

	bool CreateActor( Math::Vector3D vPositions[4] , Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , bool is_reversible = true );

	virtual bool UpdateActor( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr );

	void SetVertexPosition( Math::Vector3D vPositions[4] );
	void SetReversible( bool is_reversible );
};
