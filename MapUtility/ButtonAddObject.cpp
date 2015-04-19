
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "common.h"
#include "ButtonFunctions.h"
#include "CreateModelFromFile.h"
#include "CreateModelActor.h"
#include "resource.h"

static bool finished_load = true;
static OBJECT_DATA new_data;

// �R�s�[�p�A�N�^�[�̃��[�h���I������Ƃ��̌Ăяo�����
void FinishedCreateActorForCopy( HWND hWnd , Scene::IModelActor *pActor , int )
{
	finished_load = true;

	// ���s�����I�H
	// ���s���Ă���S����
	if( !pActor || pActor->IsInvalid() )
	{
		MessageBox( hWnd , "���f���̍쐬�Ɏ��s���܂���" , NULL , MB_OK|MB_ICONERROR );
		SAFE_RELEASE(pActor);
		SAFE_RELEASE(new_data.model_master);
		return;
	}
	IMapObjectDataForTool *copy_actor = new IMapObjectDataForTool();
	pActor->Collision_Create();
	new_data.actor_for_copy = copy_actor;
	new_data.id = g_next_id++;

	copy_actor->m_pActor = pActor;
	copy_actor->m_id = new_data.id;

	// �ǉ�
	LV_ITEM item;
	memset( &item , 0 , sizeof(LV_ITEM) );
	TCHAR str[256];
	wsprintf( str , "%d" , new_data.id );
	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );

	int size = objects.size();
	item.mask = LVIF_TEXT;
	item.pszText = str;
    item.iItem = size;
    item.iSubItem = 0;
    ListView_InsertItem(hList, &item);

	item.pszText = new_data.name;
	item.iItem = size;
	item.iSubItem = 1;
	ListView_SetItem(hList,&item);

	item.pszText = new_data.file;
	item.iItem = size;
	item.iSubItem = 2;
	ListView_SetItem(hList,&item);

	objects.push_back( new_data );
}

// �I�u�W�F�N�g�̃��[�h���I������Ƃ��ɌĂяo�����
void FinishedLoadModel( HWND hWnd , Renderer::Object::IModel *pModel , int )
{
	// IModel�̍쐬
	new_data.model_master = pModel;
	if( !new_data.model_master || new_data.model_master->IsInvalid() )
	{
		MessageBox( hWnd , "���f���̍쐬�Ɏ��s���܂���" , NULL , MB_OK|MB_ICONERROR );
		SAFE_RELEASE(pModel);
		return;
	}

	// �R�s�[�p�A�N�^�[�̐����v��
	RFC_ACTOR request;
	request.call_func_when_finish = FinishedCreateActorForCopy;
	request.hWnd = hWnd;
	request.pModel = pModel;
	CreateModelActor( request );


	// ���X�g��ŐV�K�I�u�W�F�N�g�������I�ɑI��
	// �ł��Ȃ��Ȃ��c�Ȃ�ł���
/*	item.iItem = size;
	item.iSubItem = 0;
	item.mask = 0;
	ListView_SetItem( hList , &item );*/
	
}

// �I�u�W�F�N�g�̒ǉ�
void ButtonAddObject( HWND hWnd )
{
	if( !finished_load )
	{
		return;
	}
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
		if( strcmp( objects.at(i).name , new_data.name ) == 0 )
		{
			MessageBox( hWnd , "���̖��O�͊��Ɏg���Ă��܂�" , NULL , MB_OK|MB_ICONERROR );
			return;
		}
	}

	RFC_MODEL request;
	request.call_func_when_finish = FinishedLoadModel;
	request.hWnd = hWnd;
	strcpy( request.fname , new_data.file );

	// Create���I��������̊֐����Ă΂��B����܂Œǉ��͂ł��Ȃ�
	CreateModelFromFile( request );
	finished_load = false;
	
}