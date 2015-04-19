
#include <windows.h>
#include "common.h"
#include "ButtonFunctions.h"
#include "resource.h"

#include "CommonForData.h"
#include "CommonForSynchronize.h"

void ButtonDivideArea( HWND hWnd )
{
	TCHAR str[256];
	int divide_x,divide_y,divide_z;
	int now_x,now_y,now_z;

	GetDlgItemText( hWnd , IDC_EDIT_DIVIDE_X , str , 256 );
	if( lstrlen( str ) == 0 )
	{
		MessageBox( hWnd , TEXT("•ªŠ„”‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢") , NULL , MB_ICONERROR|MB_OK );
		return;
	}
	divide_x = atoi(str);

	GetDlgItemText( hWnd , IDC_EDIT_DIVIDE_Y , str , 256 );
	if( lstrlen( str ) == 0 )
	{
		MessageBox( hWnd , TEXT("•ªŠ„”‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢") , NULL , MB_ICONERROR|MB_OK );
		return;
	}
	divide_y = atoi(str);

	GetDlgItemText( hWnd , IDC_EDIT_DIVIDE_Z , str , 256 );
	if( lstrlen( str ) == 0 )
	{
		MessageBox( hWnd , TEXT("•ªŠ„”‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢") , NULL , MB_ICONERROR|MB_OK );
		return;
	}
	divide_z = atoi(str);

	BOOL b;
	now_x = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_X , &b , FALSE );
	now_y = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Y , &b , FALSE );
	now_z = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Z , &b , FALSE );
	if( now_x == divide_x &&
		now_y == divide_y &&
		now_z == divide_z )
	{
		return;
	}

	EnterCriticalSection( &g_hMapRouteCS );

	g_mapRoute->ChangeDivideDepth( divide_x , divide_y , divide_z );

	LeaveCriticalSection( &g_hMapRouteCS );

	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_X , divide_x , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Y , divide_y , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Z , divide_z , FALSE );
}
