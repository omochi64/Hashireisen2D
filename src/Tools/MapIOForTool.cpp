
#include <iostream>
#include <vector>
#include "IOctreeMapNodeForTool.h"
#include "Game/Map.h"

using namespace Map;

//
// 書き込み
//


// 書き込みメイン
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

// ヘッダ書き込み
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

	// ヘッダ書き込み
	file.Write( &header , sizeof(HEADER_INFO) );

	// ファイルリスト
	int i,size = model_file_name_list.size();

	for( i=0 ; i<size ; i++ )
	{
		file.Write( &model_file_name_list.at(i) , sizeof(MODEL_FILE_INFO) );
	}

	// ヘッダはここで終了


	return true;
}

// 本文書き込み
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
// 読み込み
//



// 本体
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

		// 同一のIDを持つモデルを検索
		for( j=0 ; j<kind_of_model ; j++ )
		{
			if( info.id == pModelInfo[j].id )
			{
				// アクター生成
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

// 読み込みメイン
bool AccessToMapFile::ReadMapForToolFromFile( const char *file_name , IOctreeMapNodeForTool **ppRoute , MODEL_FILE_INFO **ppModelInfo , int &model_kind_num , 
							Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , int &divide_x, int& divide_y, int& divide_z )
{
	// Model管理系クラスと連携したほうがいいか？
	//  ちゃんと設計してないからなかなかつらいっちゃあつらいが…
	//  ただ、管理系クラスを追加したとしても、呼び出し側での処理はさほど変わらない
	IOctreeMapNodeForTool *root = NULL;


	File::CFile file( file_name , FILE_OPEN_TYPE_READ);
	
	if( !file.IsOpened() )
	{
		goto FAILED_TO_READ_1;
	}

	root = new IOctreeMapNodeForTool();

	// データ読み込み
	HEADER_INFO h_info={0};

	if( !ReadHeader( file , root , ppModelInfo , &h_info ) )
	{
		goto FAILED_TO_READ_1;
	}
	divide_x = h_info.divide_x;
	divide_y = h_info.divide_y;
	divide_z = h_info.divide_z;

	// モデルの生成
	// ここら辺はModel管理オブジェクトによって取って代わる可能性在り
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

	// 本体読み込み
	if( !ReadBodyForTool( pSceneMgr , models , file , root , *ppModelInfo , h_info.number_of_kind_of_objects , h_info.number_of_objects ) )
	{
		goto FAILED_TO_READ_2;
	}

	*ppRoute = root;

	// モデルリスト破棄
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
