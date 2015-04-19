
#include <windows.h>
#include "common.h"
#include "resource.h"
#include "CreateModelActor.h"
#include "AddActorToMap.h"
#include "ButtonFunctions.h"
#include "DeleteObjectFromMap.h"
#include "SetNowSelectedObject.h"
#include "UpdateNowSelectedObjectInfo.h"

static bool finished_create = true;

void FinishedCreateNewActor( HWND hWnd , Scene::IModelActor *actor , int selected_object )
{
	finished_create = true;

	if( !actor || actor->IsInvalid() )
	{
		MessageBox( hWnd , "モデルの生成に失敗しました" , NULL , MB_OK|MB_ICONERROR );
		SAFE_RELEASE(actor);
		return;
	}
	
	OBJECT_DATA & data = objects[selected_object];
	IMapObjectDataForTool *old_data = GetNowSelectedObjectData();

	// 移動
	Math::Vector3D pos;
	old_data->m_pActor->GetPosition( pos );	
	actor->Translation( pos );

	// 追加
	IMapObjectDataForTool *add_data = new IMapObjectDataForTool();
	add_data->m_id = data.id;
	add_data->m_pActor = actor;
	if( !AddActorToMap( add_data ) )
	{
		SAFE_RELEASE(add_data);
		MessageBox( hWnd , "入れ替えができませんでした。オブジェクトがマップをはみ出している可能性があります。" , NULL , MB_OK|MB_ICONERROR );
		return;
	}

	// 古いものの削除
	DeleteObjectFromMap( old_data );
	SetNowSelectedObject( add_data );
	SAFE_RELEASE(old_data);

	// Movingの変更
	// *********未実装***********

	UpdateNowSelectedObjectInfo( hWnd );
}


void ButtonChangeObject( HWND hWnd )
{
	if( !finished_create )
	{
		return;
	}

	// テキストボックスの値を取得
	TCHAR str[256];
	GetDlgItemText( hWnd , IDC_EDIT_SELECTED_OBJECT_ID , str , 255 );
	if( lstrlen( str ) == 0 )
	{
		return;
	}
	int new_id = atoi(str);

	// 現在オブジェクトが選択されている？
	if( !GetNowSelectedObjectData() )
	{
		return;
	}

	// 選択されているモデルを取得
	if( objects.size() == 0 )
	{
		return;
	}
	
	int i,size = objects.size();
	int new_index = -1;
	for( i=0 ; i<size ; i++ )
	{
		if( objects.at(i).id == new_id )
		{
			new_index = i;
			break;
		}
	}

	if( new_id < 0 || new_id >= objects.size() || new_index == -1 )
	{
		MessageBox( hWnd , "そのようなオブジェクトは存在しません" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	
	OBJECT_DATA & data = objects[new_index];

	// アクター製作のリクエスト
	RFC_ACTOR request;
	request.call_func_when_finish = FinishedCreateNewActor;
	request.hWnd = hWnd;
	request.pModel = data.model_master;
	request.other_info = new_index;
	CreateModelActor( request );
	finished_create = false;
}
