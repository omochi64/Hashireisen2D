
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "common.h"
#include "ButtonFunctions.h"
#include "DeleteAllObjectById.h"
#include "resource.h"

void ButtonDeleteObjectFromList( HWND hWnd )
{
	if( objects.size() == 0 )
	{
		return;
	}

	if( MessageBox( hWnd , "マップ上に配置されている、同一のIDを持ったオブジェクトも全て消えます。よろしいですか?" , "確認" , MB_OKCANCEL|MB_ICONQUESTION )
		== IDCANCEL )
	{
		return;
	}

	int id = objects.at(g_now_selected_object).id;

	// 現在選択されている部分を削除
	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );

	OBJECT_DATA &data = objects.at(g_now_selected_object);
	SAFE_RELEASE(data.model_master);
	objects.erase( objects.begin()+g_now_selected_object );
	ListView_DeleteItem( hList , g_now_selected_object );
	if( objects.size() <= g_now_selected_object )
	{
		g_now_selected_object--;

		// この後選択のしなおし
	}

	// この後、実際に配置されているobjectで、削除したものを使用している場合は全削除する
	DeleteAllObjectById( id );
}
