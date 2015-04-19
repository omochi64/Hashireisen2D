
#include <windows.h>
#include <windowsx.h>

#include "common.h"
#include "SettingDialog.h"
#include "resource.h"

static TCHAR route_folder[256];
static TCHAR model_folder[256];


// 設定ダイアログのプロシージャ
LRESULT CALLBACK SettingDialog( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	static HWND hRoute,hModel;
	switch( msg )
	{
	// ダイアログの初期化
	case WM_INITDIALOG:
		SetWindowText( ( hRoute = GetDlgItem( hWnd , IDC_EDIT_ROUTE_FOLDER ) ) , route_folder );
		SetWindowText( ( hModel = GetDlgItem( hWnd , IDC_EDIT_MODEL_DATA_FOLDER ) ) , model_folder );
		return TRUE;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDOK:
			GetWindowText( hRoute , route_folder , 255 );
			GetWindowText( hModel , model_folder , 255 );
			EndDialog( hWnd , IDOK );
			return TRUE;

		case IDCANCEL:
			EndDialog( hWnd , IDCANCEL );
			return TRUE;
		}

		if( HIWORD(wParam) == EN_CHANGE &&
			(lParam == (LPARAM)hRoute || lParam == (LPARAM)hModel) )
		{
			TCHAR str[256],r_str[256],m_str[256];
			GetWindowText( hRoute , r_str , 255 );
			GetWindowText( hModel , m_str , 255 );
			
			wsprintf( str , "%s\\%s" , r_str , m_str );
			SetWindowText( GetDlgItem( hWnd , IDC_STATIC_WHERE_TO_MODEL ) , str );
		}
		break;

	}

	return FALSE;
}

void SetRouteFolder( LPCTSTR str )
{
	lstrcpy( route_folder , str );
}

void SetModelFolder( LPCTSTR str )
{
	lstrcpy( model_folder , str );
}

void GetRouteFolder( PTSTR str )
{
	lstrcpy( str , route_folder );
}

void GetModelFolder( PTSTR str )
{
	lstrcpy( str , model_folder );
}

