
#pragma once

#include "IOctreeMapNode.h"

// 空間分割を動的に行うオクトツリー(後から分割数を変更できる)
class IDynamicOctreeMapNode:public IOctreeMapNode
{
private:
	void CollectObjectAndEraseTree( std::vector<IMapObjectData *> &collection );

protected:
	// このクラスをNew
	virtual IOctreeMapNode *NewThisClass();
public:
	IDynamicOctreeMapNode();
	virtual ~IDynamicOctreeMapNode();

	// 空間エリアサイズを変更
	void ChangeSpaceSize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// サイズ変更(変更するだけ)
	void ChangeOnlySize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// オブジェクトの再配置
	void Re_PlaceObjects( std::vector<IMapObjectData *> &outOfMapObjects );

	// 分割数を変更
	void ChangeDivideDepth( int depth_x , int depth_y , int depth_z );

};
