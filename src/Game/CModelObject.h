
#pragma once

#include "IObject.h"
#include "CAnimation.h"
#include <map>
#include <vector>

class CModelObject:public IObject
{
protected:
	std::map<ATOM,CAnimation *>	m_animations;
	CAnimation	*m_now_animation;
	CAnimation	*m_default_animation;
	std::vector<ATOM>	m_animation_list;
	ATOM		m_object_name_atom;
	ATOM		m_model_name_atom;
//	Selene::Scene::IModelActor *m_pActor;
	int			m_actor_id;
	bool		m_is_pausing;

private:
	void initialize();

public:
	CModelObject(ATOM object_name_atom);
	CModelObject(LPCTSTR object_name);
	virtual ~CModelObject();

	ATOM GetObjectNameAtom(){return m_object_name_atom;}

	// ���[�h�n
	bool LoadModel( LPCTSTR model_name );

	// �A�j���[�V�����̒ǉ�
	void AddAnimation( ATOM animation_name , CAnimation *add_animation );
	// �A�j���[�V�����S�폜
	void ClearAnimation();

	// ���s�A�j���[�V�����ݒ�
	bool SetAnimation( ATOM animation_name , bool will_reset = true );
	bool SetDefaultAnimation( ATOM animation_name );
	void SetAnimationToDefault( bool will_reset = true );
	// �A�j���[�V��������
	void PauseAnimation();
	void ResumeAnimation();
	// �A�j���[�V�����̃Q�b�^�[�͗v��Ȃ����ȁH

	virtual void Draw();
	virtual void Release();
	virtual bool Update();
	virtual IObject *Clone();

	// ���̓R���W��������
	// �ł���΃R���W�������o��̏����̓X�N���v�g����ɔC������
	// ���ǂ񂰂̏����̓v���O�������ɖ��ߍ���ł��������c�G�̎�ނ������邽�тɃv���O����������������͖̂ʓ|
};
