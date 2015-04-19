/*
#pragma once

#include "Selene.h"

using namespace Selene;

#include "AllObject.h"
#include "OctreeMapNode.h"

// スーパクラスを作る？

// 八分木による静的マップ
// こいつ必要か…？OctreeMapNodeを直接もっても良いような気がしてきた。Readだけ別の関数作って
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

	// マップデータからのロード
	bool LoadFromFile( char *file_name , Renderer::IRender *pRender , File::IFileManager *pFileManager );

	// 当たり判定
	// 当たった順番でoutに追加されていく
	bool HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out );

	// 指定ノード番号のノード取得
	OctreeMapNode *GetOctreeMapNode( int *node_number );

	// 可視範囲の描画
	//	可視判定についてはオライリーの本をもうちょっと詳しく読んでおく
	void DrawVisibleArea( Scene::ISceneManager *pSceneMgr );
};
*/