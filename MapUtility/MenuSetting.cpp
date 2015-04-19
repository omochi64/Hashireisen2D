
#include <windows.h>
#include <windowsx.h>

#include "common.h"
#include "MenuSetting.h"
#include "resource.h"

#include "SettingDialog.h"

void MenuSetting( HWND hWnd )
{
	// ダイアログの表示
	if( DialogBox( (HINSTANCE)GetWindowLong( hWnd , GWL_HINSTANCE ) ,
		MAKEINTRESOURCE(IDD_DIALOG_SETTING) ,
		hWnd , (DLGPROC)SettingDialog ) == IDOK )
	{
		TCHAR	str[256];
		GetRouteFolder( str );
		g_pFileMgr->SetRootPath( 0 , str );
		GetModelFolder( str );
		g_pFileMgr->SetCurrentPath( str );
	}
}
