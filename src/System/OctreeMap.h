/*
#pragma once

#include "Selene.h"

using namespace Selene;

#include "AllObject.h"
#include "OctreeMapNode.h"

// �X�[�p�N���X�����H

// �����؂ɂ��ÓI�}�b�v
// �����K�v���c�HOctreeMapNode�𒼐ڂ����Ă��ǂ��悤�ȋC�����Ă����BRead�����ʂ̊֐������
class OctreeMap:public AllObject
{
private:
protected:
	Math::Vector3D	m_minXYZ,m_maxXYZ;
	int				m_octreeDepth;
	OctreeMapNode	*m_rootNode;

public:
	OctreeMap();
	virtual ~OctreeMap();
	virtual void Release();

	// �}�b�v�f�[�^����̃��[�h
	bool LoadFromFile( char *file_name , Renderer::IRender *pRender , File::IFileManager *pFileManager );

	// �����蔻��
	// �����������Ԃ�out�ɒǉ�����Ă���
	bool HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out );

	// �w��m�[�h�ԍ��̃m�[�h�擾
	OctreeMapNode *GetOctreeMapNode( int *node_number );

	// ���͈͂̕`��
	//	������ɂ��Ă̓I���C���[�̖{������������Əڂ����ǂ�ł���
	void DrawVisibleArea( Scene::ISceneManager *pSceneMgr );
};
*/