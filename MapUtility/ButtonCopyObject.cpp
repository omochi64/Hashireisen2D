
#include <windows.h>
#include "common.h"
#include "ButtonFunctions.h"
#include "resource.h"
#include "CreateModelActor.h"
#include "Tools/IMapObjectDataForTool.h"
#include "SetNowMovingObject.h"
#include "AddActorToMap.h"
#include "GetModelByID.h"

static bool is_finished_copying = true;

// コピー用のアクターの製作が終わった
static void FinishedCreateModelActorForCopy( HWND hWnd , Scene::IModelActor *pActor , int id )
{
	is_finished_copying = true;

	// 正しく作れた？
	if( !pActor || pActor->IsInvalid() )
	{
		MessageBox( hWnd , TEXT("モデルの生成に失敗しました") , NULL , MB_OK|MB_ICONERROR );
		SAFE_RELEASE(pActor);
		return;
	}

	int i,size=objects.size();
	int index=-1;
	for( i=0 ; i<size ; i++ )
	{
		if( objects.at(i).id == id )
		{
			index = i;
			break;
		}
	}
	if( index == -1 )
	{
		TCHAR str[256];
		wsprintf( str , TEXT("致命的なエラー発生 __FILE__ = %s , __LINE__ = %d") , __FILE__ , __LINE__ );
		MessageBox( hWnd , str , NULL , MB_ICONERROR|MB_OK );
		exit(1);
	}

	// 追加
	IMapObjectDataForTool *add = GetNowMovingObject();
	if( !add || !add->m_pActor )
	{
		TCHAR str[256];
		wsprintf( str , TEXT("致命的なエラー発生 __FILE__ = %s , __LINE__ = %d") , __FILE__ , __LINE__ );
		MessageBox( hWnd , str , NULL , MB_ICONERROR|MB_OK );
		exit(1);
	}
	// 追加できた？
	add->m_pActor->TransformUpdateCollisionOnly();
	if( !AddActorToMap( add ) )
	{
		MessageBox( hWnd , TEXT("マップに追加できませんでした。マップをはみ出している可能性があります。") , NULL , MB_ICONERROR|MB_OK );
		SAFE_RELEASE(pActor);
		return;
	}

	// 移動表示用のアクター変更
	IMapObjectDataForTool *new_moving = new IMapObjectDataForTool();
	pActor->Collision_Create();
	new_moving->m_pActor = pActor;
	new_moving->m_id = id;
	SetNowMovingObject( new_moving );
	Math::Vector3D pos;
	add->m_pActor->GetPosition( pos );
	SetNowMovingObjectPosition( pos );

	objects.at(index).actor_for_copy = new_moving;

}

void ButtonCopyObject( HWND hWnd )
{
	if( !is_finished_copying )
	{
		return;
	}

	// 現在移動中のオブジェクト情報取得
	IMapObjectDataForTool *moving = GetNowMovingObject();
	if( !moving || !moving->m_pActor )
	{
		return;
	}

	// コピー用アクター生成要求
	RFC_ACTOR	request;
	request.call_func_when_finish = FinishedCreateModelActorForCopy;
	request.hWnd = hWnd;
	request.pModel = GetModelByID(moving->m_id);
	request.other_info = moving->m_id;
	

	CreateModelActor( request );

	is_finished_copying = false;
}
