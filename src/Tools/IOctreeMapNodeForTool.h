
#pragma once

#include <vector>
#include "System/IDynamicOctreeMapNode.h"
#include "Tools/IMapObjectDataForTool.h"

// このクラスでは、所持するデータは必ず"MapObjectDataForTool"である必要がある
// このクラスでやりたい特別なこと
//	・特定のIDのオブジェクトを全削除
//	・特定のIDのオブジェクトを全取得
//	・オブジェクトリスト(ObjectDataとOctree深度のリスト)を取得→ファイルに吐くため
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
