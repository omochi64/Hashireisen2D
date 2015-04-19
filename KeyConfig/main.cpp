
#include <windows.h>
#include "resource.h"

LRESULT CALLBACK ConfigDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE, LPSTR, int )
{
	hInst = hInstance;
	DialogBox( hInstance , MAKEINTRESOURCE(IDD_DIALOG_CONFIG),NULL,(DLGPROC)ConfigDlgProc );
	return 0;
}
