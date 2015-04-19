
#include <windows.h>
#include "Selene.h"
using namespace Selene;

#include "SettingHeaderDialog.h"
#include "resource.h"
#include "CommonForData.h"
#include "SetNowSelectedObject.h"

LRESULT CALLBACK SettingHeaderDialog( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	Math::Vector3D min,max;
	TCHAR str[256];
	switch( msg )
	{
	case WM_INITDIALOG:
		min = g_mapRoute->GetMinXYZ();
		max = g_mapRoute->GetMaxXYZ();
		SetDlgItemInt( hWnd , IDC_EDIT_MAP_MIN_X , min.x , TRUE );
		SetDlgItemInt( hWnd , IDC_EDIT_MAP_MIN_Y , min.y , TRUE );
		SetDlgItemInt( hWnd , IDC_EDIT_MAP_MIN_Z , min.z , TRUE );
		SetDlgItemInt( hWnd , IDC_EDIT_MAP_MAX_X , max.x , TRUE );
		SetDlgItemInt( hWnd , IDC_EDIT_MAP_MAX_Y , max.y , TRUE );
		SetDlgItemInt( hWnd , IDC_EDIT_MAP_MAX_Z , max.z , TRUE );
		return TRUE;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDOK:
			// サイズ取得
			GetDlgItemText( hWnd , IDC_EDIT_MAP_MIN_X , str , 255 );
			min.x = atof( str );
			GetDlgItemText( hWnd , IDC_EDIT_MAP_MIN_Y , str , 255 );
			min.y = atof( str );
			GetDlgItemText( hWnd , IDC_EDIT_MAP_MIN_Z , str , 255 );
			min.z = atof( str );
			GetDlgItemText( hWnd , IDC_EDIT_MAP_MAX_X , str , 255 );
			max.x = atof( str );
			GetDlgItemText( hWnd , IDC_EDIT_MAP_MAX_Y , str , 255 );
			max.y = atof( str );
			GetDlgItemText( hWnd , IDC_EDIT_MAP_MAX_Z , str , 255 );
			max.z = atof( str );

			if( min.x > max.x || min.y > max.y || min.z > max.z )
			{
				MessageBox( hWnd , "サイズが不正です" , NULL , MB_OK|MB_ICONERROR );
				return TRUE;
			}
			// サイズ変更
			SetNowSelectedObject( NULL );
			g_mapRoute->ChangeSpaceSize( min , max );

			EndDialog( hWnd , IDOK );
			return TRUE;

		case IDCANCEL:
			EndDialog( hWnd , IDCANCEL );
			return TRUE;
		}
	}
	return FALSE;
}
