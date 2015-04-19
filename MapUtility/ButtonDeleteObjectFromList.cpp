
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

	if( MessageBox( hWnd , "�}�b�v��ɔz�u����Ă���A�����ID���������I�u�W�F�N�g���S�ď����܂��B��낵���ł���?" , "�m�F" , MB_OKCANCEL|MB_ICONQUESTION )
		== IDCANCEL )
	{
		return;
	}

	int id = objects.at(g_now_selected_object).id;

	// ���ݑI������Ă��镔�����폜
	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );

	OBJECT_DATA &data = objects.at(g_now_selected_object);
	SAFE_RELEASE(data.model_master);
	objects.erase( objects.begin()+g_now_selected_object );
	ListView_DeleteItem( hList , g_now_selected_object );
	if( objects.size() <= g_now_selected_object )
	{
		g_now_selected_object--;

		// ���̌�I���̂��Ȃ���
	}

	// ���̌�A���ۂɔz�u����Ă���object�ŁA�폜�������̂��g�p���Ă���ꍇ�͑S�폜����
	DeleteAllObjectById( id );
}
