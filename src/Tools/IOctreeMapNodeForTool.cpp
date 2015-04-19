
#include "IOctreeMapNodeForTool.h"

IOctreeMapNodeForTool::IOctreeMapNodeForTool()
:IDynamicOctreeMapNode()
{
}

IOctreeMapNodeForTool::~IOctreeMapNodeForTool()
{
}

// ID�ɂ���č폜
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

	// ���̃m�[�h���̂��̂��폜
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

// ID�ɂ���ăI�u�W�F�N�g�̑�����ւ�
void IOctreeMapNodeForTool::ChangeObjectById( int change_id , Renderer::Object::IModel *new_model ,  Scene::ISceneManager *pSceneMgr )
{
	int i;
	
	// �q�̓���ւ�
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

	// ���̃m�[�h���̂��̂����ւ�
	for( i=0 ; i<m_objectNum ; i++ )
	{
		data = (IMapObjectDataForTool *)m_mapObjects.at(i);
		if( data->m_id == change_id )
		{
			// �V�����I�u�W�F�N�g�𐶐�
			pActor = pSceneMgr->CreateActor( new_model );
			data->m_pActor->GetPosition( pos );
			pActor->Translation( pos );
			new_data = new IMapObjectDataForTool();
			new_data->m_id = change_id;
			new_data->m_pActor = pActor;
			new_objects.push_back( new_data );

			// �Â��I�u�W�F�N�g���폜
			m_mapObjects.erase(m_mapObjects.begin()+i);
			i--,m_objectNum--;
		}
	}

	// ����
	int size = new_objects.size();
	for( i=0 ; i<size ; i++ )
	{
		m_mapObjects.push_back( new_objects.at(i) );
		m_objectNum++;
	}
}

// �S�I�u�W�F�N�g�̎擾
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

// �I�u�W�F�N�g�̐[���擾
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

// �I�u�W�F�N�g�̐[���擾(�ċA�����p)
bool IOctreeMapNodeForTool::GetObjectDepthReflexively( IMapObjectData *data , int *depth )
{
	int i;
	// �����ɃI�u�W�F�N�g�����݂���H
	for( i=0 ; i<m_objectNum ; i++ )
	{
		if( m_mapObjects.at(i)->Compare( data ) )
		{
			*depth = -1;
			return true;
		}
	}

	// �q��T��
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
