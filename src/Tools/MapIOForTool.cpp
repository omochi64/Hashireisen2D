
#include <iostream>
#include <vector>
#include "IOctreeMapNodeForTool.h"
#include "Game/Map.h"

using namespace Map;

//
// ��������
//


// �������݃��C��
bool AccessToMapFile::WriteMap( IOctreeMapNodeForTool *root , const char *file_name ,
							   std::vector<MODEL_FILE_INFO> &model_file_name_list ,  int depth_x , int depth_y , int depth_z )
{
	File::CFile file( file_name  , FILE_OPEN_TYPE_WRITE );
	if( !file.IsOpened() )
	{
		return false;
	}

	std::vector<IMapObjectDataForTool *> object_list;
	root->GetAllObject( object_list );
	if( !WriteHeader( root , file , model_file_name_list , depth_x , depth_y , depth_z , object_list ) )
	{
		return false;
	}
	if( !WriteBody( root , file , object_list ) )
	{
		return false;
	}

	return true;
}

// �w�b�_��������
bool AccessToMapFile::WriteHeader( IOctreeMapNodeForTool *root , File::CFile &file , std::vector<MODEL_FILE_INFO> &model_file_name_list ,
					int depth_x , int depth_y , int depth_z , std::vector<IMapObjectDataForTool *> &object_list )
{
	HEADER_INFO	header;

	strcpy( header.version ,  MAP_FILE_VERSION_INFO );

	header.divide_x = depth_x;
	header.divide_y = depth_y;
	header.divide_z = depth_z;

	header.minXYZ = root->GetMinXYZ();
	header.maxXYZ = root->GetMaxXYZ();

	
	header.number_of_objects = object_list.size();
	header.number_of_kind_of_objects = model_file_name_list.size();

	// �w�b�_��������
	file.Write( &header , sizeof(HEADER_INFO) );

	// �t�@�C�����X�g
	int i,size = model_file_name_list.size();

	for( i=0 ; i<size ; i++ )
	{
		file.Write( &model_file_name_list.at(i) , sizeof(MODEL_FILE_INFO) );
	}

	// �w�b�_�͂����ŏI��


	return true;
}

// �{����������
bool AccessToMapFile::WriteBody( IOctreeMapNodeForTool *root , File::CFile &file , std::vector<IMapObjectDataForTool *> &object_list )
{
	BODY_INFO info;

	std::vector<IMapObjectDataForTool *>::iterator it = object_list.begin() , end = object_list.end();

	while( it != end )
	{
		(*it)->m_pActor->GetPosition( info.pos );
		info.id = (*it)->m_id;
		file.Write( &info , sizeof(BODY_INFO) );
		it++;
	}

	return true;
}


//
// �ǂݍ���
//



// �{��
bool AccessToMapFile::ReadBodyForTool( Scene::ISceneManager *pSceneMgr , Renderer::Object::IModel **models ,
			  File::CFile &file , IOctreeMapNodeForTool *root , MODEL_FILE_INFO *pModelInfo , int kind_of_model , int num_of_objects )
{
	int i,j;
	BODY_INFO info;
	Scene::IModelActor *pActor = NULL;
	IMapObjectDataForTool *data;
	for( i=0 ; i<num_of_objects ; i++ )
	{
		file.Read( &info , sizeof(BODY_INFO) );

		// �����ID�������f��������
		for( j=0 ; j<kind_of_model ; j++ )
		{
			if( info.id == pModelInfo[j].id )
			{
				// �A�N�^�[����
				pActor = pSceneMgr->CreateActor( models[j] );
				if( !pActor || pActor->IsInvalid() )
				{
					SAFE_RELEASE(pActor);
					return false;
				}
				break;
			}
		}

		pActor->Translation( info.pos );
		data = new IMapObjectDataForTool();
		data->m_id = info.id;
		data->m_pActor = pActor;
		pActor->Collision_Create();
		pActor->TransformUpdateCollisionOnly();

		root->AddObject( data );
	}

	return true;
}

// �ǂݍ��݃��C��
bool AccessToMapFile::ReadMapForToolFromFile( const char *file_name , IOctreeMapNodeForTool **ppRoute , MODEL_FILE_INFO **ppModelInfo , int &model_kind_num , 
							Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , int &divide_x, int& divide_y, int& divide_z )
{
	// Model�Ǘ��n�N���X�ƘA�g�����ق����������H
	//  �����Ɛ݌v���ĂȂ�����Ȃ��Ȃ��炢�����Ⴀ�炢���c
	//  �����A�Ǘ��n�N���X��ǉ������Ƃ��Ă��A�Ăяo�����ł̏����͂��قǕς��Ȃ�
	IOctreeMapNodeForTool *root = NULL;


	File::CFile file( file_name , FILE_OPEN_TYPE_READ);
	
	if( !file.IsOpened() )
	{
		goto FAILED_TO_READ_1;
	}

	root = new IOctreeMapNodeForTool();

	// �f�[�^�ǂݍ���
	HEADER_INFO h_info={0};

	if( !ReadHeader( file , root , ppModelInfo , &h_info ) )
	{
		goto FAILED_TO_READ_1;
	}
	divide_x = h_info.divide_x;
	divide_y = h_info.divide_y;
	divide_z = h_info.divide_z;

	// ���f���̐���
	// ������ӂ�Model�Ǘ��I�u�W�F�N�g�ɂ���Ď���đ���\���݂�
	Renderer::Object::IModel **models = new Renderer::Object::IModel *[h_info.number_of_kind_of_objects];
	memset( models , 0 , sizeof(Renderer::Object::IModel *)*h_info.number_of_kind_of_objects );

	if( !models )
	{
		goto FAILED_TO_READ_1;
	}

	int i;
	for( i=0 ; i<h_info.number_of_kind_of_objects ; i++ )
	{
		models[i] = pRender->CreateModelFromFile( (*ppModelInfo)[i].file_name );
		if( !models[i] || models[i]->IsInvalid() )
		{
			goto FAILED_TO_READ_2;
		}
	}

	// �{�̓ǂݍ���
	if( !ReadBodyForTool( pSceneMgr , models , file , root , *ppModelInfo , h_info.number_of_kind_of_objects , h_info.number_of_objects ) )
	{
		goto FAILED_TO_READ_2;
	}

	*ppRoute = root;

	// ���f�����X�g�j��
	for( i=0 ; i<h_info.number_of_kind_of_objects ; i++ )
	{
		SAFE_RELEASE(models[i]);
	}
	delete [] models;

	model_kind_num = h_info.number_of_kind_of_objects;
	return true;

FAILED_TO_READ_1:
	SAFE_RELEASE(root);
	return false;

FAILED_TO_READ_2:
	for( i=0 ; i<h_info.number_of_kind_of_objects ; i++ )
	{
		SAFE_RELEASE(models[i]);
	}
	delete [] models;
	goto FAILED_TO_READ_1;
}
