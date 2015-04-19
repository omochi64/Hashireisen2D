
#include "Selene.h"

using namespace Selene;

#include "IOctreeMapNode.h"
#include "Game/Map.h"

using namespace Map;

//
// 読み込み
//

// ヘッダ
bool AccessToMapFile::ReadHeader( File::CFile &file , IOctreeMapNode *root , MODEL_FILE_INFO **model_info , HEADER_INFO *header_info )
{
	HEADER_INFO info;

	// ヘッダ読み込み
	file.Read( &info , sizeof(HEADER_INFO) );
	if( strcmp( info.version , MAP_FILE_VERSION_INFO ) )
	{
		return false;
	}

	root->CreateTree( info.divide_x , info.divide_y , info.divide_z , info.minXYZ , info.maxXYZ );

	*model_info = new MODEL_FILE_INFO[info.number_of_kind_of_objects];

	// モデルデータ情報の読み込み
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

// 本体
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
		data = new IMapObjectData();
		data->m_pActor = pActor;
		pActor->Collision_Create();
		pActor->TransformUpdateCollisionOnly();

		root->AddObject( data );
	}

	return true;
}

// 読み込みメイン
bool AccessToMapFile::ReadMapFromFile( const char *file_name , IOctreeMapNode **ppRoute , MODEL_FILE_INFO **ppModelInfo , int &model_kind_num , 
					Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	// Model管理系クラスと連携したほうがいいか？
	//  ちゃんと設計してないからなかなかつらいっちゃあつらいが…
	//  ただ、管理系クラスを追加したとしても、呼び出し側での処理はさほど変わらない
	IOctreeMapNode *root = NULL;

	File::CFile file( file_name , FILE_OPEN_TYPE_READ);
	
	if( !file.IsOpened() )
	{
		goto FAILED_TO_READ_1;
	}

	root = new IOctreeMapNode();

	// データ読み込み
	HEADER_INFO h_info={0};

	if( !ReadHeader( file , root , ppModelInfo , &h_info ) )
	{
		goto FAILED_TO_READ_1;
	}
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
	if( !ReadBody( pSceneMgr , models , file , root , *ppModelInfo , h_info.number_of_kind_of_objects , h_info.number_of_objects ) )
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
	goto FAILED_TO_READ_2;
}
