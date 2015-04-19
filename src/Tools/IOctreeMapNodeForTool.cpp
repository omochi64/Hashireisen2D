
#include "IOctreeMapNodeForTool.h"

IOctreeMapNodeForTool::IOctreeMapNodeForTool()
:IDynamicOctreeMapNode()
{
}

IOctreeMapNodeForTool::~IOctreeMapNodeForTool()
{
}

// IDによって削除
void IOctreeMapNodeForTool::DeleteObjectById( int del_id )
{
	int i;

	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			DeleteObjectById(del_id);
		}
	}

	IMapObjectDataForTool *data;

	std::vector<IMapObjectData *>::iterator it;

	// このノード内のものを削除
	for( i=0 ; i<m_objectNum ; i++ )
	{
		data = (IMapObjectDataForTool *)m_mapObjects.at(i);
		if( data->m_id == del_id )
		{
			m_mapObjects.erase(m_mapObjects.begin()+i);
			SAFE_RELEASE(data);
			i--,m_objectNum--;
		}
	}
}

// IDによってオブジェクトの総入れ替え
void IOctreeMapNodeForTool::ChangeObjectById( int change_id , Renderer::Object::IModel *new_model ,  Scene::ISceneManager *pSceneMgr )
{
	int i;
	
	// 子の入れ替え
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			ChangeObjectById(change_id,new_model,pSceneMgr);
		}
	}

	IMapObjectDataForTool *data;
	Math::Vector3D pos;

	std::vector<IMapObjectData *>::iterator it;
	std::vector<IMapObjectDataForTool *> new_objects;
	Scene::IModelActor *pActor;
	IMapObjectDataForTool *new_data;

	// このノード内のものを入れ替え
	for( i=0 ; i<m_objectNum ; i++ )
	{
		data = (IMapObjectDataForTool *)m_mapObjects.at(i);
		if( data->m_id == change_id )
		{
			// 新しいオブジェクトを生成
			pActor = pSceneMgr->CreateActor( new_model );
			data->m_pActor->GetPosition( pos );
			pActor->Translation( pos );
			new_data = new IMapObjectDataForTool();
			new_data->m_id = change_id;
			new_data->m_pActor = pActor;
			new_objects.push_back( new_data );

			// 古いオブジェクトを削除
			m_mapObjects.erase(m_mapObjects.begin()+i);
			i--,m_objectNum--;
		}
	}

	// 結合
	int size = new_objects.size();
	for( i=0 ; i<size ; i++ )
	{
		m_mapObjects.push_back( new_objects.at(i) );
		m_objectNum++;
	}
}

// 全オブジェクトの取得
void IOctreeMapNodeForTool::GetAllObject( std::vector<IMapObjectDataForTool *> &object_list )
{
	int i;
	std::vector<IMapObjectData *>::iterator it,end = m_mapObjects.end();
	it = m_mapObjects.begin();

	while( it != end )
	{
		object_list.push_back( (IMapObjectDataForTool *)(*it) );
		it++;
	}

	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IOctreeMapNodeForTool *)m_childrenNode[i])->GetAllObject(object_list);
		}
	}
}

// オブジェクトの深さ取得
bool IOctreeMapNodeForTool::GetObjectDepth( IMapObjectData *data , int *depth )
{
	*depth = 0;
	if( !GetObjectDepthReflexively( data , depth+1 ) )
	{
		*depth = -1;
		return false;
	}
	else
	{
		return true;
	}
}

// オブジェクトの深さ取得(再帰処理用)
bool IOctreeMapNodeForTool::GetObjectDepthReflexively( IMapObjectData *data , int *depth )
{
	int i;
	// ここにオブジェクトが存在する？
	for( i=0 ; i<m_objectNum ; i++ )
	{
		if( m_mapObjects.at(i)->Compare( data ) )
		{
			*depth = -1;
			return true;
		}
	}

	// 子を探す
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			if( ((IOctreeMapNodeForTool*)m_childrenNode[i])->GetObjectDepthReflexively( data , depth+1 ) )
			{
				*depth = i;
				return true;
			}
		}
	}
	return false;
}
