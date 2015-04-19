
#include "Selene.h"

using namespace Selene;

#include "IOctreeMapNode.h"
#include "Game/Map.h"

using namespace Map;

//
// �ǂݍ���
//

// �w�b�_
bool AccessToMapFile::ReadHeader( File::CFile &file , IOctreeMapNode *root , MODEL_FILE_INFO **model_info , HEADER_INFO *header_info )
{
	HEADER_INFO info;

	// �w�b�_�ǂݍ���
	file.Read( &info , sizeof(HEADER_INFO) );
	if( strcmp( info.version , MAP_FILE_VERSION_INFO ) )
	{
		return false;
	}

	root->CreateTree( info.divide_x , info.divide_y , info.divide_z , info.minXYZ , info.maxXYZ );

	*model_info = new MODEL_FILE_INFO[info.number_of_kind_of_objects];

	// ���f���f�[�^���̓ǂݍ���
	int i;
	for( i=0 ; i<info.number_of_kind_of_objects ; i++ )
	{
		file.Read( (*model_info)+i , sizeof(MODEL_FILE_INFO) );
	}

	if( header_info )
	{
		memcpy( header_info , &info , sizeof(HEADER_INFO) );
	}

	return true;
}

// �{��
bool AccessToMapFile::ReadBody( Scene::ISceneManager *pSceneMgr , Renderer::Object::IModel **models ,
							   File::CFile &file , IOctreeMapNode *root , MODEL_FILE_INFO *pModelInfo , int kind_of_model , int num_of_objects )
{
	int i,j;
	BODY_INFO info;
	Scene::IModelActor *pActor;
	IMapObjectData *data;
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
		data = new IMapObjectData();
		data->m_pActor = pActor;
		pActor->Collision_Create();
		pActor->TransformUpdateCollisionOnly();

		root->AddObject( data );
	}

	return true;
}

// �ǂݍ��݃��C��
bool AccessToMapFile::ReadMapFromFile( const char *file_name , IOctreeMapNode **ppRoute , MODEL_FILE_INFO **ppModelInfo , int &model_kind_num , 
					Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	// Model�Ǘ��n�N���X�ƘA�g�����ق����������H
	//  �����Ɛ݌v���ĂȂ�����Ȃ��Ȃ��炢�����Ⴀ�炢���c
	//  �����A�Ǘ��n�N���X��ǉ������Ƃ��Ă��A�Ăяo�����ł̏����͂��قǕς��Ȃ�
	IOctreeMapNode *root = NULL;

	File::CFile file( file_name , FILE_OPEN_TYPE_READ);
	
	if( !file.IsOpened() )
	{
		goto FAILED_TO_READ_1;
	}

	root = new IOctreeMapNode();

	// �f�[�^�ǂݍ���
	HEADER_INFO h_info={0};

	if( !ReadHeader( file , root , ppModelInfo , &h_info ) )
	{
		goto FAILED_TO_READ_1;
	}
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
	if( !ReadBody( pSceneMgr , models , file , root , *ppModelInfo , h_info.number_of_kind_of_objects , h_info.number_of_objects ) )
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
	goto FAILED_TO_READ_2;
}
