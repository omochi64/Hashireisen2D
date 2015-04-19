
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include "Selene.h"
using namespace Selene;
#include "resource.h"
#include "SetNowPosition.h"
#include "common.h"
#include "InitDialog.h"

void UpdateNowPositionInfo( HWND hWnd )
{
	const Math::Vector3D &pos = GetNowPosition();

	g_is_change_programmly = true;
	
	char str[256];
	int v;
	sprintf( str , "%.1f" , pos.x );
	SetDlgItemText( hWnd , IDC_EDIT_POS_X , str );
	v = (int)(atof(str)*10);
	SendMessage(GetDlgItem(hWnd,IDC_SPIN_POS_X), UDM_SETPOS, 0, (LPARAM)MAKELONG((short)(v+SPIN_OFFSET), 0));

	sprintf( str , "%.1f" , pos.y );
	SetDlgItemText( hWnd , IDC_EDIT_POS_Y , str );
	v = (int)(atof(str)*10);
	SendMessage(GetDlgItem(hWnd,IDC_SPIN_POS_Y), UDM_SETPOS, 0, (LPARAM)MAKELONG((short)(v+SPIN_OFFSET), 0));

	sprintf( str , "%.1f" , pos.z );
	SetDlgItemText( hWnd , IDC_EDIT_POS_Z , str );
	v = (int)(atof(str)*10);
	SendMessage(GetDlgItem(hWnd,IDC_SPIN_POS_Z), UDM_SETPOS, 0, (LPARAM)MAKELONG((short)(v+SPIN_OFFSET), 0));

	
	g_is_change_programmly = false;
}
