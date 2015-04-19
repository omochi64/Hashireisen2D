
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
		MessageBox( hWnd , "���f���̍쐬�Ɏ��s���܂���" , NULL , MB_OK|MB_ICONERROR );
		return;
	}

	OBJECT_DATA &data = objects.at(selected_object);

	SAFE_RELEASE(data.model_master);
	data.model_master = pModel;

	lstrcpy( data.name , new_data.name );
	lstrcpy( data.file , new_data.file );

	// �I�u�W�F�N�g�̑�����ւ�
	ChangeAllObjectById( data.id , pModel , g_pSceneMgr );
	
	// �ݒ肵�Ȃ���
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

// �I�u�W�F�N�g���̏C��
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

	// ���O�̃_�u����`�F�b�N
	// �t�@�C�������������H���ɖ��O���o�^����Ă��Ȃ����H�Ȃǂ̃`�F�b�N
	

	GetWindowText( GetDlgItem(hWnd,IDC_EDIT_ADD_OBJECT) , new_data.file , 648 );
	GetWindowText( GetDlgItem(hWnd,IDC_EDIT_OBJECT_NAME) , new_data.name , 62 );

	if( lstrlen(new_data.file) == 0 )
	{
		MessageBox( hWnd , "�t�@�C��������͂��Ă�������" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	if( lstrlen(new_data.name) == 0 )
	{
		MessageBox( hWnd , "�I�u�W�F�N�g������͂��Ă�������" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	// ����
	int i,size = objects.size();
	for( i=0 ; i<size ; i++ )
	{
		if( i==g_now_selected_object )
		{
			continue;
		}
		if( strcmp( objects.at(i).name , new_data.name ) == 0 )
		{
			MessageBox( hWnd , "���̖��O�͊��Ɏg���Ă��܂�" , NULL , MB_OK|MB_ICONERROR );
			return;
		}
	}

	// �ݒ肵�Ȃ���
	OBJECT_DATA &data = objects.at(g_now_selected_object);
	if( lstrcmp( data.file , new_data.file ) != 0 )
	{
		// �I�u�W�F�N�g���̂��ύX���ꂽ�ꍇ�A�}�b�v��ɔz�u����Ă���I�u�W�F�N�g���S�ĕύX����
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
	
	// �ݒ肵�Ȃ���
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