
#include "IMapObjectData.h"
#include "IDynamicOctreeMapNode.h"
/*
IDynamicOctreeMapNode();
	virtual ~IDynamicOctreeMapNode();

	// 空間エリアサイズを変更
	void ChangeSpaceSize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// 分割数を変更
	void ChangeDivideDepth( int depth );*/

// コンストラクタ、デストラクタ
IDynamicOctreeMapNode::IDynamicOctreeMapNode():IOctreeMapNode()
{
}
IDynamicOctreeMapNode::~IDynamicOctreeMapNode()
{
}

// このクラスをNew
IOctreeMapNode *IDynamicOctreeMapNode::NewThisClass()
{
	return new IDynamicOctreeMapNode();
}

// 空間エリアサイズを変更
void IDynamicOctreeMapNode::ChangeSpaceSize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ )
{
	ChangeOnlySize( minXYZ , maxXYZ );
	std::vector<IMapObjectData *> outOfMapObjects;
	Re_PlaceObjects( outOfMapObjects );
	// あふれた分は削除
	int i,size = outOfMapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(outOfMapObjects.at(i));
	}
}

// サイズ変更(変更するだけ)
void IDynamicOctreeMapNode::ChangeOnlySize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ )
{
	Math::Matrix identity;
	identity.Identity();
	m_spaceArea.CreateBox( minXYZ , maxXYZ , identity );
	Math::Vector3D minPoints[8],maxPoints[8];
	IOctreeMapNode::DivideBoxSpace( minXYZ , maxXYZ , minPoints , maxPoints , m_divided_x , m_divided_y , m_divided_z );

	// 子の変更
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IDynamicOctreeMapNode *)m_childrenNode[i])->ChangeOnlySize( minPoints[i] , maxPoints[i] );
		}
	}

}

// オブジェクトの再配置
void IDynamicOctreeMapNode::Re_PlaceObjects( std::vector<IMapObjectData *> &outOfMapObjects )
{
	int i;

	// 子から先に再配置していく
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IDynamicOctreeMapNode *)m_childrenNode[i])->Re_PlaceObjects( outOfMapObjects );
		}
	}
	
//	int size = m_mapObjects.size();

	std::vector<IMapObjectData *> temp_outOfMapObjects;

	for( i=0 ; i<m_objectNum ; i++ )
	{
		IMapObjectData *object = m_mapObjects.at(i);
		object->m_pActor->TransformUpdateCollisionOnly();

		// この空間内にちゃんと入ってる？
		if( !IsObjectInThisSpacePerfectly( object->m_pActor ) )
		{
			// 入っていない
			temp_outOfMapObjects.push_back( object );
			m_mapObjects.erase( m_mapObjects.begin()+i );
			m_objectNum--;
			i--;
			continue;
		}
		else
		{
			int j;
			for( j=0 ; j<8 ; j++ )
			{
				// 子にAddしてみる
				if( m_childrenNode[j] )
				{
					if( m_childrenNode[j]->AddObject( object ) )
					{
						// Addできた
						// ここから消す
						m_mapObjects.erase( m_mapObjects.begin()+i );
						m_objectNum--;
						i--;
						break;
					}
				}
			}
		}
	}

	// 子からあふれてきたObjectsをここにAddできるか？
	int size = outOfMapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		IMapObjectData *object = outOfMapObjects.at(i);
		if( IsObjectInThisSpacePerfectly( object->m_pActor ) )
		{
			// ここにAdd
			m_mapObjects.push_back( object );
			m_objectNum++;
			outOfMapObjects.erase( outOfMapObjects.begin()+i );
			i--,size--;
			continue;
		}
	}

	// tempを追加
	size = temp_outOfMapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		outOfMapObjects.push_back( temp_outOfMapObjects.at(i) );
	}
}

// 空間の再分割
void IDynamicOctreeMapNode::ChangeDivideDepth( int depth_x , int depth_y , int depth_z )
{
	// オブジェクトの収集、ツリーの削除
	Math::Vector3D min = m_spaceArea.vMin , max = m_spaceArea.vMax;
	std::vector<IMapObjectData *> objects;
	CollectObjectAndEraseTree( objects );

	// ツリーの再構築
	CreateTree( depth_x , depth_y , depth_z , min , max );

	// オブジェクトの再配置
	std::vector<IMapObjectData *>::iterator it=objects.begin(),end=objects.end();

	while( it != end )
	{
		(*it)->m_pActor->TransformUpdateCollisionOnly();
		AddObject( *it );
		it++;
	}

}

// オブジェクトの収集
void IDynamicOctreeMapNode::CollectObjectAndEraseTree(std::vector<IMapObjectData*> &collection)
{
	std::vector<IMapObjectData *>::iterator it = m_mapObjects.begin(),end = m_mapObjects.end();

	while( it != end )
	{
		collection.push_back(*it);
		it++;
	}
	m_mapObjects.clear();
	m_objectNum = 0;

	for( int i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IDynamicOctreeMapNode *)m_childrenNode[i])->CollectObjectAndEraseTree( collection );
			SAFE_RELEASE(m_childrenNode[i]);
		}
	}

}
