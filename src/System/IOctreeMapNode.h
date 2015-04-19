#pragma once

#include <vector>
#include "Selene.h"

using namespace Selene;

#include "AllObject.h"
#include "IMapObjectData.h"


/*
// マップデータからのロード
OctreeMapNode* LoadFromFile( char *file_name , Renderer::IRender *pRender , File::IFileManager *pFileManager );
このような外部関数を作っておく。そうすればOctreeMapクラスはいらない
*/

// マップ部品データ構造体
//  ここは構造体にせず、マップデータインターフェースクラスを作っておく。そして、…いや、まて、それもなんかへんだ…
//	うーん、IDの保存とかはどうしよう…
//	いや、やっぱり上で書いたようにデータは継承かなあ…最上位の親はModelActorだけを持ち、ReleaseとCopyを最低限のメソッドとして持つ…うーん…
//typedef struct _map_object_data{
//	Scene::IModelActor *pModel;
//	Math::Vector3D	vPosition;
//	Math::Vector3D	vRotation;
//}MAP_OBJECT_DATA;

// 八分木の内部ノード
// コイツはマップ生成ツールにも使う
// となると、動的にツリーを変更する操作も入れたい。コイツを継承して作る？
class IOctreeMapNode:public AllObject
{
protected:
	

	std::vector<IMapObjectData *>	m_mapObjects;
	int				m_objectNum;
	IOctreeMapNode	*m_childrenNode[8];	// 子ノード。必ず8つ以下となる
	Collision::CBox	m_spaceArea;
	//		int				visibleArea;	まだ仕様が未定
	bool			m_divided_x,m_divided_y,m_divided_z;

	// 可視判定
	//	可視判定についてはオライリーの本をもうちょっと詳しく読んでおく
	bool CheckVisible( Scene::ISceneManager *pSceneMgr , int window_width , int window_height );

	// 自身をnew。継承して上書きする用
	virtual IOctreeMapNode *NewThisClass();

	// エリアボックスとの当たり判定
	bool Collision_Area_Model( Scene::IModelActor *pModel );
	bool Collision_Area_Box( const Collision::CBox &box );
	bool Collision_Area_Ray( const Collision::CLine3D &ray );

public:
	IOctreeMapNode();
	virtual ~IOctreeMapNode();
	virtual void Release();

	// 八分岐を作成。再帰処理用
	virtual void CreateTree( int depth_x , int depth_y , int depth_z , const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// オブジェクトを追加
	// add_node_numberは、どのノードにこのデータを追加するかを表す
	//	add_node_number = {1,2,2,0}
	// だったら、3段目(ルートは0段目)の2番目のノードに追加する
	bool AddObject( IMapObjectData *add_data , int *add_node_number );
	// オブジェクトを追加
	//  どこに割り当てられるかは自動的に判別
	bool AddObject( IMapObjectData *add_data );

	// オブジェクトの削除
	//  ノード指定型
	bool DeleteObject( IMapObjectData *delete_data , int *delete_node_number , bool will_delete_data = true );
	//  自動検索型
	//		-親ノード優先
	bool DeleteObjectPrimirilyParent( IMapObjectData *delete_data , bool will_delete_data = true );
	//	自動検索型
	//		-子ノード優先
	bool DeleteObjectPrimirilyChild( IMapObjectData *delete_data , bool will_delete_data = true );

	int GetObjectNum( void ){return m_objectNum;}

	// Octreeをコンパクトにする。オブジェクト数が0なノードは破棄
	void Compact( void );

	// max,minの取得
	const Math::Vector3D &GetMinXYZ();
	const Math::Vector3D &GetMaxXYZ();

	// 当たり判定
	// 当たった順番でoutに追加されていく
	bool HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out , std::vector<IMapObjectData *> &out_object );

	// ↓こいつら(Octree空間に所属する/しないのチェック関数)は、CheckCollisionあたりを使えば作れるかな？

	// 渡されたボックスがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
	bool IsBoxtInThisSpacePerfectly( const Collision::CBox &box );
	// 渡されたボックスがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
	bool IsBoxtInThisSpacePartly( const Collision::CBox &box );
	// 渡されたオブジェクトがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
	bool IsObjectInThisSpacePerfectly( Scene::IModelActor *pModel );
	// 渡されたオブジェクトがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
	bool IsObjectInThisSpacePartly( Scene::IModelActor *pModel );
	// 渡されたレイがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
	bool IsRayInThisSpacePerfectly( const Collision::CLine3D &ray );
	// 渡されたレイがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
	bool IsRayInThisSpacePartly( const Collision::CLine3D &ray );


	// 可視判定領域の描画
	void DrawVisibleArea( Scene::ISceneManager *pSceneMgr , int window_width , int window_height );

	// 領域自体の描画
	void DrawOctreeArea( Renderer::Object::ILine3D *pLine , Scene::ISceneManager *pSceneMgr , int window_width , int window_height );


	// 空間分割関数
	static void DivideBoxSpace( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ ,
		Math::Vector3D minPoints[] , Math::Vector3D maxPoints[] , bool is_divide_x , bool is_divide_y , bool is_divide_z );
};
