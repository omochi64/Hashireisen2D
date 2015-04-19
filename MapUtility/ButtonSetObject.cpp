
#include <windows.h>

#include "common.h"
#include "ButtonFunctions.h"
#include "resource.h"
#include "CreateModelActor.h"
#include "Tools/IMapObjectDataForTool.h"
#include "AddActorToMap.h"
#include "UpdateNowPositionInfo.h"
#include "UpdateNowSelectedObjectInfo.h"
#include "SetNowMovingObject.h"

#include "CreateModelFromFile.h"

static bool finished_create = true;

void FinishedCreateActor( HWND hWnd , Scene::IModelActor *actor , int selected_object )
{
	finished_create = true;

	if( !actor || actor->IsInvalid() )
	{
		MessageBox( hWnd , "モデルの生成に失敗しました" , NULL , MB_OK|MB_ICONERROR );
		SAFE_RELEASE(actor);
		return;
	}
	
	OBJECT_DATA & data = objects[selected_object];

	// アクターを移動させる
	// 現在位置の取得
	TCHAR str[256];
	Math::Vector3D pos;
	GetDlgItemText( hWnd , IDC_EDIT_POS_X , str , 255 );
	pos.x = atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_POS_Y , str , 255 );
	pos.y = atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_POS_Z , str , 255 );
	pos.z = atof( str );
	// 移動
	actor->Translation( pos );
	actor->Collision_Create();

	// 追加
	IMapObjectDataForTool *add_data = new IMapObjectDataForTool();
	add_data->m_id = data.id;
	add_data->m_pActor = actor;
	if( !AddActorToMap( add_data ) )
	{
		SAFE_RELEASE(add_data);
		MessageBox( hWnd , "マップに追加できませんでした。オブジェクトがマップをはみ出している可能性があります。" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	SetNowMovingObject( data.actor_for_copy );

	UpdateNowPositionInfo( hWnd );
	UpdateNowSelectedObjectInfo( hWnd );
}

void ButtonSetObject( HWND hWnd )
{
	if( !finished_create )
	{
		return;
	}
	// IModelからIModelActorを生成し、g_mapRouteに追加(追加はMainWin側の同期の問題があるから、追加関数を用いる)

	// 選択されているモデルを取得
	if( objects.size() == 0 )
	{
		return;
	}
	
	OBJECT_DATA & data = objects[g_now_selected_object];

	// アクター製作のリクエスト
	RFC_ACTOR request;
	request.call_func_when_finish = FinishedCreateActor;
	request.hWnd = hWnd;
	request.pModel = data.model_master;
	request.other_info = g_now_selected_object;
	CreateModelActor( request );
	finished_create = false;
	
}
