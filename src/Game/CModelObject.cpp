
#include "Game/CModelObject.h"
#include "System/GameSystem.h"

using namespace Selene;

CModelObject::CModelObject(ATOM object_name_atom)
{
	m_object_name_atom = object_name_atom;
	m_actor_id = -1;
	initialize();
}

CModelObject::CModelObject(LPCTSTR object_name)
{
	m_object_name_atom = AddAtom( object_name );
	initialize();
}

CModelObject::~CModelObject()
{
	Release();
}

// ���ʏ���������
void CModelObject::initialize()
{
	m_now_animation = NULL;
	m_model_name_atom = 0;
	m_is_pausing = false;
	m_actor_id = -1;
	m_animations.clear();
	m_animation_list.clear();
}

// �������
void CModelObject::Release()
{
	GameSystem::SystemGetter::GetModelManager()->ReleaseModel(m_actor_id);
	ClearAnimation();
}

// ���[�h
bool CModelObject::LoadModel( LPCTSTR model_name )
{
	ATOM name_atom = AddAtom( model_name );

	// �}�X�^�̃��[�h
	CModelManager *pModelMgr = GameSystem::SystemGetter::GetModelManager();

	if( !pModelMgr->GetOriginalModel( name_atom ) )
	{
		if( !pModelMgr->LoadModel( model_name ) )
		{
			return false;
		}
	}

	// �A�N�^�[�̎擾
//	Scene::IModelActor *pActor;
//	pActor = pModelMgr->GetCopyModel( name_atom );
//	m_pActor = pActor;
	if( (m_actor_id = pModelMgr->CreateNewModel(name_atom)) < 0 )
	{
		return false;
	}
	m_model_name_atom = name_atom;

	return true;
}

// �A�j���[�V�����ǉ�
void CModelObject::AddAnimation(ATOM animation_name , CAnimation *add_animation)
{
	m_animations[animation_name] = add_animation;
	m_animation_list.push_back(animation_name);
}

// �A�j���[�V�����폜
void CModelObject::ClearAnimation()
{
	std::vector<ATOM>::iterator it = m_animation_list.begin(),end = m_animation_list.end();

	while( it != end )
	{
		delete m_animations[*it];
		it++;
	}
	m_animations.clear();
	m_animation_list.clear();
}

// ���s����A�j���[�V�����ݒ�
bool CModelObject::SetAnimation( ATOM animation_name , bool will_reset )
{
	CAnimation *to = m_animations[animation_name];

	if( !to )
	{
		return false;
	}
	m_now_animation = to;
	if( will_reset )
	{
		to->ResetFrame();
	}
	return true;
}
// �f�t�H���g�A�j���[�V�����ݒ�
bool CModelObject::SetDefaultAnimation( ATOM animation_name )
{
	CAnimation *to = m_animations[animation_name];

	if( !to )
	{
		return false;
	}
	m_default_animation = to;
	if( !m_now_animation )
	{
		m_now_animation = to;
		to->ResetFrame();
	}
	return true;
}

// �f�t�H���g�A�j���[�V�����ɐݒ�
void CModelObject::SetAnimationToDefault( bool will_reset )
{
	if( m_default_animation )
	{
		m_now_animation = m_default_animation;
		if( will_reset )
		{
			m_now_animation->ResetFrame();
		}
	}
}

// �A�j���[�V�����ꎞ��~�A�ĊJ
void CModelObject::PauseAnimation()
{
	m_is_pausing = true;
}
void CModelObject::ResumeAnimation()
{
	m_is_pausing = false;
}

// �A�b�v�f�[�g
bool CModelObject::Update()
{
	if( m_is_pausing )
	{
		return true;
	}

	// Animation�̃A�b�v�f�[�g���悭������Ȃ��c
	// �A�j���[�V�����̎��Ԏ��͂܂��d�l���߂Ă����Ȃ��Ƃ����Ȃ�����A�Ƃ肠�����t���[�����𒼐ړ��Ă�
	int now_tm_animation = 0;
	float now_tm_time = 0.0f;
	if( m_now_animation )
	{
		now_tm_animation = m_now_animation->GetTimelineAnimationNumber();
		now_tm_time = (float)m_now_animation->GetFrameInThisAnimation();
		m_now_animation->Update();
	}

	Scene::IModelActor *pActor = GameSystem::SystemGetter::GetModelManager()->GetModel(m_actor_id);
	if( pActor )
	{
		pActor->UpdateAnimation( now_tm_animation , now_tm_time );
	}

	return true;
}

// �`��
void CModelObject::Draw()
{
	Scene::IModelActor *pActor = GameSystem::SystemGetter::GetModelManager()->GetModel(m_actor_id);
	if( !pActor )
	{
		return;
	}

	Math::Vector3D pos(0,0,0),rot(0,0,0);
	if( m_now_animation )
	{
		m_now_animation->GetPosition( pos );
		m_now_animation->GetRotation( rot );
	}

	// �`��
	pActor->TransformReset();
	pActor->RotationX(rot.x);
	pActor->RotationY(rot.y);
	pActor->RotationZ(rot.z);
	pActor->Translation(pos);
	pActor->TransformUpdate();

	pActor->RenderingRequest();
}

// �I�u�W�F�N�g����
IObject *CModelObject::Clone()
{
	// �A�N�^�[�̃R�s�[�擾
	CModelManager *pModelMgr = GameSystem::SystemGetter::GetModelManager();
	int new_id = pModelMgr->CreateNewModel( m_model_name_atom );
//	Scene::IModelActor *pActor = pModelMgr->GetCopyModel( m_model_name_atom );
	if( new_id < 0 )
	{
		return NULL;
	}

	CModelObject *new_object = new CModelObject(m_object_name_atom);
	new_object->m_actor_id = new_id;
	new_object->m_model_name_atom = m_model_name_atom;	

	std::vector<ATOM>::iterator it=m_animation_list.begin(),end=m_animation_list.end();

	while( it != end )
	{
		CAnimation *animation = new CAnimation( *m_animations[*it] );
		new_object->AddAnimation( *it , animation );
		it++;
	}

	new_object->m_is_pausing = m_is_pausing;

	return new_object;
}
