
#pragma once

#include <vector>
#include "System/IDynamicOctreeMapNode.h"
#include "Tools/IMapObjectDataForTool.h"

// ���̃N���X�ł́A��������f�[�^�͕K��"MapObjectDataForTool"�ł���K�v������
// ���̃N���X�ł�肽�����ʂȂ���
//	�E�����ID�̃I�u�W�F�N�g��S�폜
//	�E�����ID�̃I�u�W�F�N�g��S�擾
//	�E�I�u�W�F�N�g���X�g(ObjectData��Octree�[�x�̃��X�g)���擾���t�@�C���ɓf������
class IOctreeMapNodeForTool:public IDynamicOctreeMapNode
{
	bool GetObjectDepthReflexively( IMapObjectData *data , int *depth );
public:
	IOctreeMapNodeForTool();
	virtual ~IOctreeMapNodeForTool();

	void DeleteObjectById( int del_id );
	void ChangeObjectById( int change_id , Renderer::Object::IModel *new_model ,  Scene::ISceneManager *pSceneMgr );

	void GetAllObject( std::vector<IMapObjectDataForTool *> &object_list );

	bool GetObjectDepth( IMapObjectData *data , int *depth );
};
