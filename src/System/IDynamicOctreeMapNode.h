
#pragma once

#include "IOctreeMapNode.h"

// ��ԕ����𓮓I�ɍs���I�N�g�c���[(�ォ�番������ύX�ł���)
class IDynamicOctreeMapNode:public IOctreeMapNode
{
private:
	void CollectObjectAndEraseTree( std::vector<IMapObjectData *> &collection );

protected:
	// ���̃N���X��New
	virtual IOctreeMapNode *NewThisClass();
public:
	IDynamicOctreeMapNode();
	virtual ~IDynamicOctreeMapNode();

	// ��ԃG���A�T�C�Y��ύX
	void ChangeSpaceSize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// �T�C�Y�ύX(�ύX���邾��)
	void ChangeOnlySize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// �I�u�W�F�N�g�̍Ĕz�u
	void Re_PlaceObjects( std::vector<IMapObjectData *> &outOfMapObjects );

	// ��������ύX
	void ChangeDivideDepth( int depth_x , int depth_y , int depth_z );

};
