#include <windows.h>
#include <windowsx.h>

#include "ButtonFunctions.h"
#include "resource.h"



// �{�^���������ꂽ�Ƃ��̑���
// �Q��
void ButtonRefFile( HWND hWnd )
{
	OPENFILENAME of;
	memset( &of , 0 , sizeof(OPENFILENAME) );
	TCHAR	path[650]="",title[128]="";
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.lpstrFilter = "SMF�t�@�C��\0*.smf\0\0";
	of.lpstrFile = path;
	of.nMaxFile = 648;
	of.lpstrFileTitle = title;
	of.nMaxFileTitle = 126;
	of.Flags = OFN_FILEMUSTEXIST|OFN_EXPLORER;
	of.lpstrDefExt = "smf";

	if( GetOpenFileName(&of) )
	{
		// �V�K�t�@�C�����ݒ�
		SetWindowText( GetDlgItem(hWnd,IDC_EDIT_ADD_OBJECT) , title );
	}

}
