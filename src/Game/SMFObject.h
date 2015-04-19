
#include "Game/MultipleAnimation3DBox.h"
#include "Game/UdongeAnimation.h"
#include "System/Manager/3DModelManager.h"

// MultipleAnimationで管理しているため，MultipleAnimationを継承せざるを得ない
class SMFObject:public MultipleAnimation3DBox
{
	Scene::IModelActor	*m_pModelActor;
//	Renderer::Object::IModel *m_pModel;
	char		m_modelFile[650];
	ATOM		m_model_file_atom;
	CModelManager		*m_pManager;

public:
	SMFObject(CModelManager *manager ,Math::Vector3D center_pos , float width , float height , float depth , char *model_file = NULL,
		Math::Vector3D speed=Math::Vector3D(0,0,0) , Math::Vector3D accel=Math::Vector3D(0,0,0));
	SMFObject(CModelManager *manager);
	virtual ~SMFObject();

	/*
	virtual void Draw( void )=0;
	virtual void Action( void )=0;
	virtual bool CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time )=0;

	virtual DrawableObject* Clone( void )=0;
	// 戻り値は当たっているか否か。
	// hit_directionは、当たっている場合、this→objectへの方向ベクトル(非正規化)
	virtual bool DetectCollision( DrawableObject &object ) = 0;
	virtual void GetCollisionRect( Math::Rect2D<Float> & rect ) = 0;
	*/

	bool LoadModel( char *model_file );

	virtual void UpdateVertex( void );
	virtual void Draw( void );
	virtual DrawableObject* Clone( void );
	virtual bool UpdateAnimation( void );

	virtual void Release( void );

	char *GetModelFileName( void );
};
