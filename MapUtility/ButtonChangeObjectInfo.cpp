
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "common.h"
#include "ButtonFunctions.h"
#include "resource.h"
#include "CreateModelFromFile.h"
#include "ChangeAllObjectById.h"

static bool finished_change = true;
static OBJECT_DATA new_data;

void FinishedLoadObjectForChange( HWND hWnd , Renderer::Object::IModel *pModel , int selected_object )
{
	finished_change = true;
	if( !pModel || pModel->IsInvalid() )
	{
		MessageBox( hWnd , "モデルの作成に失敗しました" , NULL , MB_OK|MB_ICONERROR );
		return;
	}

	OBJECT_DATA &data = objects.at(selected_object);

	SAFE_RELEASE(data.model_master);
	data.model_master = pModel;

	lstrcpy( data.name , new_data.name );
	lstrcpy( data.file , new_data.file );

	// オブジェクトの総入れ替え
	ChangeAllObjectById( data.id , pModel , g_pSceneMgr );
	
	// 設定しなおし
	LV_ITEM item;
	memset( &item , 0 , sizeof(LV_ITEM) );
	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );

	item.mask = LVIF_TEXT;
	item.pszText = new_data.name;
	item.iItem = selected_object;
	item.iSubItem = 1;
	ListView_SetItem(hList,&item);

	item.pszText = new_data.file;
	item.iItem = selected_object;
	item.iSubItem = 2;
	ListView_SetItem(hList,&item);
}

// オブジェクト情報の修正
void ButtonChangeObjectInfo( HWND hWnd )
{
	if( !finished_change )
	{
		return;
	}

	if( objects.size() == 0 )
	{
		return;
	}

	// 名前のダブりをチェック
	// ファイルが正しいか？既に名前が登録されていないか？などのチェック
	

	GetWindowText( GetDlgItem(hWnd,IDC_EDIT_ADD_OBJECT) , new_data.file , 648 );
	GetWindowText( GetDlgItem(hWnd,IDC_EDIT_OBJECT_NAME) , new_data.name , 62 );

	if( lstrlen(new_data.file) == 0 )
	{
		MessageBox( hWnd , "ファイル名を入力してください" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	if( lstrlen(new_data.name) == 0 )
	{
		MessageBox( hWnd , "オブジェクト名を入力してください" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	// 検索
	int i,size = objects.size();
	for( i=0 ; i<size ; i++ )
	{
		if( i==g_now_selected_object )
		{
			continue;
		}
		if( strcmp( objects.at(i).name , new_data.name ) == 0 )
		{
			MessageBox( hWnd , "その名前は既に使われています" , NULL , MB_OK|MB_ICONERROR );
			return;
		}
	}

	// 設定しなおし
	OBJECT_DATA &data = objects.at(g_now_selected_object);
	if( lstrcmp( data.file , new_data.file ) != 0 )
	{
		// オブジェクト自体が変更された場合、マップ上に配置されているオブジェクトも全て変更する
		RFC_MODEL request;
		strcpy( request.fname , new_data.file );
		request.hWnd = hWnd;
		request.other_info = g_now_selected_object;
		request.call_func_when_finish = FinishedLoadObjectForChange;
		CreateModelFromFile( request );
		finished_change = false;
		return;
	}

	lstrcpy( data.name , new_data.name );
	lstrcpy( data.file , new_data.file );
	
	// 設定しなおし
	LV_ITEM item;
	memset( &item , 0 , sizeof(LV_ITEM) );
	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );

	item.mask = LVIF_TEXT;
	item.pszText = new_data.name;
	item.iItem = g_now_selected_object;
	item.iSubItem = 1;
	ListView_SetItem(hList,&item);

	item.pszText = new_data.file;
	item.iItem = g_now_selected_object;
	item.iSubItem = 2;
	ListView_SetItem(hList,&item);
}