
#include "SMFObject.h"

/*

#include "Game/UdongeAnimation.h"

// MultipleAnimation�ŊǗ����Ă��邽�߁CMultipleAnimation���p��������𓾂Ȃ�
class SMFObject:public MultipleAnimation
{
	Renderer::Object::IModel *m_pModel;
	float		m_depth;

public:
	SMFObject(Math::Vector3D center_pos , float width , float height , float depth ,
		Math::Vector2D speed=Math::Vector2D(0,0) , Math::Vector2D accel=Math::Vector2D(0,0));
	SMFObject( void );
	virtual ~SMFObject();

	virtual void UpdateVertex( void );
	virtual DrawableObject* Clone( void );
	virtual bool UpdateAnimation( DWORD now_time );

	virtual void Release( void );
};

*/

/*
MultipleAnimation3DBox::MultipleAnimation3DBox(Math::Vector3D center_pos,
											   float width, float height, float depth,
											   Math::Vector2D speed,Math::Vector2D accel)
*/
SMFObject::SMFObject(CModelManager *manager , Math::Vector3D center_pos, float width, float height, float depth,
					 char *model_file , Math::Vector3D speed, Math::Vector3D accel)
					 :MultipleAnimation3DBox(center_pos,width,height,depth,speed,accel)
{
	m_pos = center_pos;
	m_width = width;
	m_height = height;
	m_depth = depth;

	m_modelFile[0] = '\0';

	m_speed = speed;
	m_accel = accel;

	m_rotate = Math::Vector3D(0,0,0);
	m_rotation_speed = Math::Vector3D(0,0,0);

	m_texture = NULL;
	m_is_right_direction = true;

	m_primitive = NULL;
	m_actor = NULL;
	m_pModelActor = NULL;

	m_model_file_atom = 0;
	m_pManager = manager;


	if( LoadModel( model_file ) )
	{
		UpdateVertex();

		m_is_valiable = true;
	}
	else
	{
		m_is_valiable = false;
	}
}

SMFObject::SMFObject(CModelManager *manager):MultipleAnimation3DBox()
{
	m_modelFile[0] = '\0';
	m_pManager = manager;
}

SMFObject::~SMFObject()
{
	Release();
}

// �ǂݍ���
bool SMFObject::LoadModel( char *model_file )
{
/*	Renderer::Object::IModel *pModel = g_pRender->CreateModelFromFile( model_file );
	if( pModel == NULL )
	{
		m_is_valiable = false;
		return false;
	}
	Scene::IModelActor *actor = g_pSceneMgr->CreateActor( pModel );
	if( actor == NULL )
	{
		pModel->Release();
		delete pModel;
		m_is_valiable = false;
		return false;
	}
*/
	SAFE_RELEASE(m_pModelActor);
	m_is_valiable = false;

//	m_pModel = pModel;
//	m_pModelActor = actor;
	if( !m_pManager->GetOriginalModel( model_file ) )
	{
		if( !m_pManager->LoadModel( model_file ) )
		{
			return false;
		}
	}

	m_pModelActor = m_pManager->GetCopyModel( model_file );
	m_is_valiable = true;
	strcpy( m_modelFile , model_file );
	m_model_file_atom = AddAtom(m_modelFile);

	return true;
}

// ���_�A�b�v�f�[�g
void SMFObject::UpdateVertex()
{
	// ��{�������Ȃ��BLoadModel�őS�Ă��������Ă���
}

// �A�j���[�V�����A�b�v�f�[�g
bool SMFObject::UpdateAnimation()
{
	// ��{�������Ȃ��B�A�j���[�V�������s��Ȃ�����
	return true;
}

// ���
void SMFObject::Release()
{
	SAFE_RELEASE(m_pModelActor);
	SAFE_RELEASE(m_primitive);
	SAFE_RELEASE(m_actor);

	m_is_valiable = false;

	MultipleAnimation3DBox::Release();
}

// �`��
void SMFObject::Draw()
{
	if( !m_pModelActor )
	{
		return;
	}
	m_pModelActor->TransformReset();
	m_pModelActor->RotationX( DEG_TO_ANGLE(m_rotate.x) );
	m_pModelActor->RotationY( DEG_TO_ANGLE(m_rotate.y) );
	m_pModelActor->RotationZ( DEG_TO_ANGLE(m_rotate.z) );
	m_pModelActor->Translation( m_pos );
	m_pModelActor->TransformUpdate();

	m_pModelActor->RenderingRequest();
}

char *SMFObject::GetModelFileName()
{
	return m_modelFile;
}

// �N���[��
DrawableObject *SMFObject::Clone()
{
	SMFObject *clone = new SMFObject( m_pManager , m_pos , m_width , m_height , m_depth , m_modelFile , 
		m_speed , m_accel );

	int an = clone->CreateNewAnimation();

	clone->Add( an , NULL , 0 , Math::Vector2D(0,0) , Math::Vector2D(0,0) ,
		m_collision_rects[0]->at(0) , m_speed , m_accel , m_rotation_speed );
	
	return clone;
}
