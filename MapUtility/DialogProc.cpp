
// ToDo
//	//�E���X�g���N���b�N���ďC��
//	//�E���X�g����̍폜
//	//�E�t�H���_�K�w�\���̖�����
//	//�E���C���E�B���h�E�Ƃ̃I�u�W�F�N�g�̊֘A�t��(IModel,IModelActor�Ȃǂ��ǂ��ŊǗ����邩���肷��)
//	//�E�_�C�A���O�ォ��̃}�b�v�T�C�Y�̕ύX
//	//�E�ǉ�
//	//�E���C���E�B���h�E��ł̕\�� �� �I�u�W�F�N�g�A�����A���݈ʒu�A����XZ���ʁA���ݑI���I�u�W�F�N�g�̕\�������B�g���Ă����Ďg���ɂ����Ɗ�������ς��Ă���
//	//	�����݈ʒu�ɂ́A���ݑI�����Ă���I�u�W�F�N�g�̃v���r���[��\������H
//	�~�E�I�u�W�F�N�g���̕\��	��	������������Ȃ��c��񂪎擾�ł��Ȃ��c
//	//�E���C���E�B���h�E��ł̑���B�I�u�W�F�N�g�̑I���A���_�ύX���I������B
//	�~�E�אڒǉ�	��	������������Ȃ��c��񂪎擾�ł��Ȃ��c
//	//�E�����I�u�W�F�N�g�̍폜
//	//�E�����I�u�W�F�N�g�̒u������
//	//�E�I�u�W�F�N�g���X�g���ύX�ɂ��}�b�v�̕ύX �� ���Ԃ����邩��(�߂�ǂ������I�ȈӖ���)
//	//�EDivideBoxSpace�̏C��
//	//�EOctree�����A�����̈�̉���
//	///*�E�t�@�C���ւ̕ۑ��A�t�@�C������̓ǂݍ��݁��������͎̂��������B��͌Ăяo���t�����g�G���h����B�f�[�^�̍폜/�쐬�Ɨ���ł邩��߂�ǂ�������*/
//	//	����̓f�o�b�O�B/*ApplyModelListToList�C���ƁA*/�J�����ۑ����V�K�쐬�����Ƃ��̃o�O�C��(test_map2�̂Ƃ������Č����Ȃ��H)������c�������H
//	//	�@�e�X�g�P�[�X(�ȒP�Ȃ���)�F�F�X�����遨�V�K�쐬 or �J���A�J���������遨���O��(ry�A�ۑ��A��
//	//���E�I�u�W�F�N�g�̈ʒu�̕ύX���܂�(���������S�[�X�g�ňړ��������)�B�ǂ���������u�����v�{�^��������H
//	//	���ꉞ�ł������f�o�b�O���s�\���B�����|��
//	//�E�����蔻��o�O�݂�B�{�b�N�X�̓����蔻��ɁA��0����CBox��p���Ă�̂������H
//	//	model->TransformUpdateCollisionOnly�Œ�����
//	//�E���_�̈ړ��������Ƃ��A�I�u�W�F�N�g�̑I���Ƀo�O�݂�(���݈ʒu�̎w��͖��Ȃ�)
//	//�E�}�b�v�f�[�^�Ƀo�O�݂�B
//	//	��f�[�^(�}�b�v�T�C�Y�Ƃ��͎w��݂�)�ۑ����J���A���f�������X�g�ɒǉ��A�}�b�v�ɔz�u���ۑ��A�I�����J���A���f�������X�g�ɒǉ��A�I�����J��
//	//	����ɂ��A��ɒǉ��������f����ID��0�ƂȂ��Ă��܂��Ă���
//	//�E�I�u�W�F�N�g�̕ύX��(1�̃I�u�W�F�N�g�ɑ΂���ID�ύX��)�AMoving���ύX����Ă��Ȃ�(ButtonChangeObject��)
//	//�E�}�b�v�O�ɂ��ړ��ł��Ă��܂�(�����͖���)
//	�E�S�̓I�ɓ����蔻�肪���������H
//		SetObject��MoveObject�ł́A�[�߂��ł͂ݏo���Ă��Ȃ��̂ɂ͂ݏo�����Ɣ��肳���ꍇ������B
//		CopyObject�ł́A�[�߂��ŏ����͂ݏo���Ă��Ă����v�Ɣ��肳���B
//			�ꍇ�ɂ�邩���B�ꍇ�ɂ���Ă͈ړ���NoHit�Ɣ��f����A������Hit�Ɣ��f�����B�����������Ȃ񂾂���́c
//		����ς蓖���蔻�肨�������c�BDepth�����炩�ɕ�
//	���ǂ����f�[�^���������������悤�BSelene�̂������A�����̍�����}�b�v�f�[�^�̂������悭�킩��Ȃ��B�Ƃ肠�����A���G�Ȓn�`�`�b�v�͍��Ȃ��ق�������
//	�EOctreeMapNode��CheckVisible���s�\���B������̂Ɍ����Ȃ��Ɣ��肳��邱�Ƃ����遨���w�n�̖{��ǂ񂾂ق��������HGems�Ƃ�

// �ȉ��K�{�ł͂Ȃ���������
//	//�E�I���I�u�W�F�N�g�̐[��(Octree��̈ʒu)���\���ł����ق������� �� ����ƃf�o�b�O�����֗�
//	//�E�ړ��p�I�u�W�F�N�g���I���I�u�W�F�N�g���O�ɕ\������āA�I���I�u�W�F�N�g�Əd�Ȃ��Ă�Ƃ��ɐԐF�̃I�u�W�F�N�g�������Ȃ��Ȃ��Ă��܂��B
//	//	�����Ȃ��Ǝ��p�ł��Ȃ��قǂ̃o�O�ł͂Ȃ����A�C�ɂȂ�
//	//�E���Ԃ�����΁AF1���Ŏ��_���f�t�H���g�ɖ߂�

// �t�@�C���I�[�v���A�ۑ����͑������v���Ǝv�����A��������Ƃ�����ƕۑ�����Ȃ��\�������邱�Ƃ�������ƌ����Ă������ق�������
// �N�����ҏW��(���O������)�ۑ��A�N�����J����(���O������)�ۑ��A���x�̂��Ƃɂ����ق��������B
// �V�K�쐬�́A�ꉞ�������A�e�X�g�p�f�[�^�������x�Ȃ�Ƃ������A�R�[�X��{�i�I�ɍ�����肷��悤�ȏꍇ�͎g��Ȃ��ق�������(�ċN�������ق�������)

#define MAIN_DIALOG_SOURCE

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <vector>
#include <stdio.h>

#include "Selene.h"
#include "Tools/IMapObjectDataForTool.h"

#include "common.h"
#include "DialogProc.h"
#include "resource.h"
#include "InitDialog.h"
#include "menuSetting.h"
#include "SettingHeaderDialog.h"
#include "SetNowPosition.h"
#include "SetNowMovingObject.h"
#include "SetNowSelectedObject.h"
#include "UpdateNowPositionInfo.h"
#include "UpdateNowSelectedObjectInfo.h"
#include "ButtonFunctions.h"

#include "CommonForData.h"
#include "Game/Map.h"

#include "InitializeUtilityData.h"

using namespace std;
using namespace Selene;

#pragma comment(lib,"comctl32.lib")

namespace{
	TCHAR	now_fname[_MAX_PATH];
	bool	was_changed;
	LPCTSTR	title=TEXT("�}�b�v�쐬�c�[���R���g���[��");
};


void SettingWithChangeValue( HWND hEdit , int v );
void SettingWithChangeValueDirectly( HWND hEdit , HWND hSpin );
void ApplyNowPosition( HWND hWnd );
void ApplyNowMovingPosition( HWND hWnd );

void UpdateBasicObjectInfo( HWND hWnd );

void MenuCreateNew(HWND hWnd);
void MenuOpen(HWND hWnd);
void MenuSave(HWND hWnd);
void MenuSaveNew(HWND hWnd);
void MenuExit(HWND hWnd);

// �X�s���ƃG�f�B�b�g�{�b�N�X�̊֘A�t��
struct _spin_edit{
	int	spin_id;
	int edit_id;
}spin_edit[] = {
	{IDC_SPIN_POS_X,IDC_EDIT_POS_X},
	{IDC_SPIN_POS_Y,IDC_EDIT_POS_Y},
	{IDC_SPIN_POS_Z,IDC_EDIT_POS_Z},
	{IDC_SPIN_OBJECT_POS_X,IDC_EDIT_OBJECT_POS_X},
	{IDC_SPIN_OBJECT_POS_Y,IDC_EDIT_OBJECT_POS_Y},
	{IDC_SPIN_OBJECT_POS_Z,IDC_EDIT_OBJECT_POS_Z},
	{0,0}
};

LRESULT CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	INITCOMMONCONTROLSEX ic;
	LPNMLISTVIEW lvinfo;
	HWND hItem;
	int i;


	switch( msg )
	{
	case WM_INITDIALOG:
		// ������
		g_is_change_programmly = false;
		ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
		ic.dwICC = ICC_UPDOWN_CLASS;
		InitCommonControlsEx(&ic);
		InitDialog( hWnd );
		was_changed = false;
		now_fname[0] = 0;
		return TRUE;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		// �ݒ胁�j���[
		case ID_SETTING:
			MenuSetting( hWnd );
			break;

		// �w�b�_�ݒ�
		case ID_SET_HEADER:
			if( DialogBox( hInstance , MAKEINTRESOURCE(IDD_DIALOG_SET_HEADER) , hWnd , (DLGPROC)SettingHeaderDialog ) == IDOK )
			{
				was_changed = true;
			}
			break;

		// �V�K�쐬
		case ID_CREATE_NEW:
			MenuCreateNew(hWnd);
			break;

		// �J��
		case ID_OPEN:
			MenuOpen(hWnd);
			break;

		// �ۑ�
		case ID_SAVE:
			MenuSave(hWnd);
			break;

		// ���O�����ĕۑ�
		case ID_SAVE_NEW:
			MenuSaveNew(hWnd);
			break;

		// �I��
		case ID_EXIT:
			MenuExit(hWnd);
			break;

		// �Q�ƃ{�^��
		case IDC_BUTTON_REF_FILE:
			ButtonRefFile( hWnd );
			break;

		// �ǉ��{�^��
		case IDC_BUTTON_ADD_OBJECT:
			ButtonAddObject( hWnd );
			was_changed = true;
			break;

		// �ύX�{�^��
		case IDC_BUTTON_CHANGE_OBJECT_INFO:
			ButtonChangeObjectInfo( hWnd );
			was_changed = true;
			break;

		// �폜�{�^��
		case IDC_BUTTON_DELETE_OBJECT_FROM_LIST:
			ButtonDeleteObjectFromList( hWnd );
			was_changed = true;
			break;

		// �I�u�W�F�N�g�̐ݒu�{�^��
		case IDC_BUTTON_SET_OBJECT:
			ButtonSetObject( hWnd );
			was_changed = true;
			break;

		// �I�u�W�F�N�g�̍폜�{�^��
		case IDC_BUTTON_DELETE_OBJECT:
			ButtonDeleteObject( hWnd );
			was_changed = true;
			break;

		// �I�u�W�F�N�g�̕ύX�{�^��
		case IDC_BUTTON_CHANGE_OBJECT_ID:
			ButtonChangeObject( hWnd );
			was_changed = true;
			break;

		// �I�u�W�F�N�g�̈ړ��{�^��
		case IDC_BUTTON_MOVE_OBJECT:
			ButtonMoveObject( hWnd );
			was_changed = true;
			break;

		// �I�u�W�F�N�g�̕����{�^��
		case IDC_BUTTON_COPY_OBJECT:
			ButtonCopyObject( hWnd );
			was_changed = true;
			break;

		// ��ԕ����{�^��
		case IDC_BUTTON_DIVIDE_AREA:
			ButtonDivideArea( hWnd );
			was_changed = true;
			break;

		case IDOK:
		case IDCANCEL:
			MenuExit(hWnd);
			break;
		}
		if( HIWORD(wParam) == EN_CHANGE && !g_is_change_programmly )
		{
			hItem = (HWND)lParam;
			// �G�f�B�b�g�{�b�N�X���ڑ���
			for( i=0 ; spin_edit[i].edit_id != 0 ; i++ )
			{
				if( hItem == GetDlgItem( hWnd , spin_edit[i].edit_id ) )
				{
					SettingWithChangeValueDirectly( hItem , GetDlgItem( hWnd , spin_edit[i].spin_id ) );
					// ���݈ʒu�̕ύX�̏ꍇ
					if( spin_edit[i].edit_id == IDC_EDIT_POS_X || spin_edit[i].edit_id == IDC_EDIT_POS_Y || spin_edit[i].edit_id == IDC_EDIT_POS_Z )
					{
						ApplyNowPosition( hWnd );
					}
					else if( spin_edit[i].edit_id == IDC_EDIT_OBJECT_POS_X || spin_edit[i].edit_id == IDC_EDIT_OBJECT_POS_Y || spin_edit[i].edit_id == IDC_EDIT_OBJECT_POS_Z )
					{
						ApplyNowMovingPosition( hWnd );
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_VSCROLL:
		// �X�s���R���g���[���̑���
		hItem = (HWND)lParam;
		for( i=0 ; spin_edit[i].spin_id != 0 ; i++ )
		{
			if( hItem == GetDlgItem( hWnd , spin_edit[i].spin_id ) )
			{
				SettingWithChangeValue( GetDlgItem(hWnd,spin_edit[i].edit_id) , HIWORD(wParam) );
				if( spin_edit[i].edit_id == IDC_EDIT_POS_X || spin_edit[i].edit_id == IDC_EDIT_POS_Y || spin_edit[i].edit_id == IDC_EDIT_POS_Z )
				{
					ApplyNowPosition( hWnd );
				}
				else if( spin_edit[i].edit_id == IDC_EDIT_OBJECT_POS_X || spin_edit[i].edit_id == IDC_EDIT_OBJECT_POS_Y || spin_edit[i].edit_id == IDC_EDIT_OBJECT_POS_Z )
				{
					ApplyNowMovingPosition( hWnd );
				}
				break;
			}
		}
		break;

	case WM_NOTIFY:
		if( (int)wParam == IDC_LIST_OBJECT_LIST )
		{
			lvinfo = (NMLISTVIEW *)lParam;
			if( lvinfo->hdr.code == LVN_ITEMCHANGED )
			{
				if( lvinfo->iItem != g_now_selected_object )
				{
					g_now_selected_object = lvinfo->iItem;
//									;
				}


				// �N���b�N���ꂽ�Ƃ���̃f�[�^�𔽉f������
				UpdateBasicObjectInfo( hWnd );
			}

		}
		break;

	case WM_DESTROY:
		DestroyWindow( GetParent(hWnd) );
		break;

	}
	return FALSE;
}



// �X�s���{�^�����������Ƃ��̐ݒ�
void SettingWithChangeValue( HWND hEdit , int v )
{
	// �����ŁAEdit�̎�ނɂ���Ď�X�̃f�[�^��ς���

	TCHAR str[256];
	sprintf( str , "%.1f" , (v-SPIN_OFFSET)/10.0 );
	SetWindowText( hEdit , str );
}

// ���ڐݒ肳�ꂽ�Ƃ�
void SettingWithChangeValueDirectly( HWND hEdit , HWND hSpin )
{
	TCHAR str[256];
	GetWindowText( hEdit , str , 255 );
	int v = (int)(atof(str)*10);
	SendMessage(hSpin, UDM_SETPOS, 0, (LPARAM)MAKELONG((short)(v+SPIN_OFFSET), 0));
}

// �N���b�N���ꂽ���𔽉f
void UpdateBasicObjectInfo( HWND hWnd )
{
	if( g_now_selected_object < 0 || g_now_selected_object >= objects.size() )
	{
		return;
	}
	SetWindowText( GetDlgItem( hWnd , IDC_EDIT_ADD_OBJECT ) , objects.at(g_now_selected_object).file );
	SetWindowText( GetDlgItem( hWnd , IDC_EDIT_OBJECT_NAME ) , objects.at(g_now_selected_object).name );
}

// ���݈ʒu�̕ύX
void ApplyNowPosition( HWND hWnd )
{
	// ���݈ʒu�ύX
	TCHAR	str[256];
	Math::Vector3D n_pos;
	GetDlgItemText( hWnd , IDC_EDIT_POS_X , str , 255 );
	n_pos.x = (float)atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_POS_Y , str , 255 );
	n_pos.y = (float)atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_POS_Z , str , 255 );
	n_pos.z = (float)atof( str );

	SetNowPosition( n_pos );
}

// ���݂̈ړ���ʒu�̕ύX
void ApplyNowMovingPosition( HWND hWnd )
{
	TCHAR	str[256];
	Math::Vector3D n_pos;
	GetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_X , str , 255 );
	n_pos.x = (float)atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_Y , str , 255 );
	n_pos.y = (float)atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_Z , str , 255 );
	n_pos.z = (float)atof( str );

	SetNowMovingObjectPosition( n_pos );
}

void ClearObjectList(HWND hWnd)
{
	int i,size = objects.size();
	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(objects.at(i).model_master);
		SAFE_RELEASE(objects.at(i).actor_for_copy);
	}
	objects.clear();
	g_next_id = 0;
	g_now_selected_object = 0;

	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );
	ListView_DeleteAllItems( hList );
}

//
// ���j���[�֘A
//

#define ERROR_MESSAGE(hWnd,mes)			(MessageBox(hWnd,mes , NULL , MB_OK|MB_ICONERROR))
#define	QUESTION_SAVE_CHANGE(hWnd)		(MessageBox(hWnd,TEXT("�ύX��ۑ����܂���?") , TEXT("�ۑ�") , MB_YESNOCANCEL|MB_ICONQUESTION))

bool GetNewOpenFileName( HWND hWnd , TCHAR fname[] , TCHAR fpath[] , int fname_length , int fpath_length )
{
	OPENFILENAME of;
	memset( &of , 0 , sizeof(OPENFILENAME) );
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.lpstrFilter = "�}�b�v�t�@�C��(.m)\0*.m\0\0";
	of.lpstrFile = fpath;
	of.nMaxFile = fpath_length;
	of.lpstrFileTitle = fname;
	of.nMaxFileTitle = fname_length;
	of.Flags = OFN_FILEMUSTEXIST|OFN_EXPLORER;
	of.lpstrDefExt = ".m";

	return GetOpenFileName( &of ) ? true:false;
}

bool GetNewSaveFileName( HWND hWnd , TCHAR fname[] , TCHAR fpath[] , int fname_length , int fpath_length )
{
	OPENFILENAME of;
	memset( &of , 0 , sizeof(OPENFILENAME) );
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.lpstrFilter = "�}�b�v�t�@�C��(.m)\0*.m\0\0";
	of.lpstrFile = fpath;
	of.nMaxFile = fpath_length;
	of.lpstrFileTitle = fname;
	of.nMaxFileTitle = fname_length;
	of.Flags = OFN_EXPLORER;
	of.lpstrDefExt = ".m";

	return GetSaveFileName( &of ) ? true:false;
}

// �V�K�쐬
void MenuCreateNew(HWND hWnd)
{
	if( was_changed )
	{
		// �ۑ��𕷂�
		switch( QUESTION_SAVE_CHANGE(hWnd) )
		{
		case IDYES:
			MenuSave(hWnd);
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}
	// �I���I�u�W�F�N�g���N���A
	SetNowSelectedObject( NULL );

	// �f�[�^����蒼��
	ReCreateUtilityData();
	// �I�u�W�F�N�g���X�g��������
	ClearObjectList(hWnd);

	UpdateNowPositionInfo( hWnd );
	UpdateNowSelectedObjectInfo( hWnd );

	SetDlgItemText( hWnd , IDC_EDIT_OBJECT_NAME , "" );
	SetDlgItemText( hWnd , IDC_EDIT_ADD_OBJECT , "" );

	now_fname[0] = 0;
	was_changed = false;

	SetWindowText( hWnd , title );
}

void ApplyModelListToList( HWND hWnd , Map::MODEL_FILE_INFO *models , int model_num )
{
	int i;

	// �Ƃ肠����Model��Actor�̐���
	Renderer::Object::IModel *pModel;
	Scene::IModelActor *pActor;
	IMapObjectDataForTool *copy_actor;
	OBJECT_DATA new_data;
	int max_id=0;

	std::vector<OBJECT_DATA>	new_objects;

	// �f�[�^�̐���
	for( i=0 ; i<model_num ; i++ )
	{
		new_data.id = models[i].id;
		strcpy( new_data.file , models[i].file_name );
		strcpy( new_data.name , models[i].model_name );
		pModel = NULL;
		pActor = NULL;
		copy_actor = NULL;
		try
		{
			// ���f���A�A�N�^�[�̐���
			pModel = g_pRender->CreateModelFromFile( new_data.file );
			if( !pModel || pModel->IsInvalid() )
			{
				throw "";
			}
			pActor = g_pSceneMgr->CreateActor( pModel );
			if( !pActor || pActor->IsInvalid() )
			{
				throw "";
			}
			copy_actor = new IMapObjectDataForTool();
			if( !copy_actor )	throw "";

		}catch(const char *){
			// �G���[����
			SAFE_RELEASE(pModel);
			SAFE_RELEASE(pActor);
			SAFE_RELEASE(copy_actor);
			for( int j=0 ; j<i ; j++ )
			{
				SAFE_RELEASE(new_objects.at(i).model_master);
				SAFE_RELEASE(new_objects.at(i).actor_for_copy);
			}
			return;
		}
		new_data.model_master = pModel;
		new_data.actor_for_copy = copy_actor;
		copy_actor->m_pActor = pActor;
		copy_actor->m_id = new_data.id;
		if( max_id < new_data.id )
		{
			max_id = new_data.id;
		}

		new_objects.push_back(new_data);
	}
	g_next_id = max_id+1;


	// ���C���f�[�^�ɃR�s�[
	for( i=0 ; i<objects.size() ; i++ )
	{
		SAFE_RELEASE(objects[i].model_master);
		SAFE_RELEASE(objects[i].actor_for_copy);
	}
	objects.clear();
	LV_ITEM item;
	memset( &item , 0 , sizeof(LV_ITEM) );
	TCHAR str[256];
	HWND hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );
	for( i=0 ; i<model_num ; i++ )
	{
		OBJECT_DATA &data = new_objects.at(i);
		objects.push_back( data );
		// ���X�g�ɔ��f
	
		wsprintf( str , "%d" , data.id );

		item.mask = LVIF_TEXT;
		item.pszText = str;
		item.iItem = i;
		item.iSubItem = 0;
		ListView_InsertItem(hList, &item);

		item.pszText = data.name;
		item.iItem = i;
		item.iSubItem = 1;
		ListView_SetItem(hList,&item);

		item.pszText = data.file;
		item.iItem = i;
		item.iSubItem = 2;
		ListView_SetItem(hList,&item);
	}
}

// �t�@�C�����J��
void MenuOpen(HWND hWnd)
{
	if( was_changed )
	{
		// �ۑ��𕷂�
		switch( QUESTION_SAVE_CHANGE(hWnd) )
		{
		case IDYES:
			MenuSave(hWnd);
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}

	// �t�@�C���𕷂�
	//  �t�@�C���I�[�v�����s �� ���݂̂܂�
	//	�t�@�C���I�[�v������ �� �������񏉊������ă��[�h
	TCHAR fname[128]={0},fpath[_MAX_PATH]={0};
	if( !GetNewOpenFileName( hWnd , fname , fpath , 126 , _MAX_PATH-2 ) )
	{
		return;
	}

	HANDLE hFile = CreateFile( fpath , GENERIC_READ , 0 , NULL ,
		OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		ERROR_MESSAGE(hWnd,TEXT("�t�@�C�����J���܂���"));
		return;
	}
	CloseHandle(hFile);

	// ������
	// �I���I�u�W�F�N�g���N���A
	SetNowSelectedObject( NULL );
	// �f�[�^����蒼��
	ReCreateUtilityData();
	// �I�u�W�F�N�g���X�g��������
	ClearObjectList(hWnd);
	UpdateNowPositionInfo( hWnd );
	UpdateNowSelectedObjectInfo( hWnd );

	now_fname[0] = 0;

	// �ǂݍ���
	Map::MODEL_FILE_INFO *models;
	int kind_num=0;
	int divide_x=0,divide_y=0,divide_z=0;
	if( !Map::AccessToMapFile::ReadMapForToolFromFile( fpath , &g_mapRoute , &models , kind_num ,
					g_pRender , g_pSceneMgr ,divide_x,divide_y,divide_z ) )
	{
		ERROR_MESSAGE(hWnd,TEXT("�f�[�^�̓ǂݍ��݂Ɏ��s���܂���"));
		return;
	}

	// ���X�g�ɔ��f
	ApplyModelListToList( hWnd , models , kind_num );
	// ���݂̕�����
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_X , divide_x , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Y , divide_y , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Z , divide_z , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_DIVIDE_X , divide_x , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_DIVIDE_Y , divide_y , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_DIVIDE_Z , divide_z , FALSE );

	strcpy( now_fname , fname );
	was_changed = false;

	TCHAR new_title[1024];
	wsprintf( new_title , TEXT("%s - %s") , title , fname );
	SetWindowText( hWnd , new_title );
	
}

bool Write( HWND hWnd , LPCTSTR fname )
{
	string path = "Data\\Map\\";
	path += fname;

	// *************** ����������
	std::vector<Map::MODEL_FILE_INFO> model_list;
	Map::MODEL_FILE_INFO info;
	int i,size = objects.size();
	for( i=0 ; i<size ; i++ )
	{
		info.id = objects.at(i).id;
		strcpy( info.file_name , objects.at(i).file );
		strcpy( info.model_name , objects.at(i).name );
		model_list.push_back( info );
	}

	BOOL b;
	int depth_x = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_X , &b , FALSE );
	int depth_y = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Y , &b , FALSE );
	int depth_z = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Z , &b , FALSE );
	
	if( !Map::AccessToMapFile::WriteMap( g_mapRoute , path.c_str() , model_list , depth_x , depth_y , depth_z ) )
	{
		return false;
	}
	return true;
}

// �㏑��
void MenuSave(HWND hWnd)
{
	if( now_fname[0] == 0 )
	{
		MenuSaveNew(hWnd);
		return;
	}

	if( !Write( hWnd , now_fname ) )
	{
		ERROR_MESSAGE(hWnd,TEXT("�t�@�C���ւ̏������݂Ɏ��s���܂���"));
	}
	was_changed = false;
}

// �V�K�ۑ�
void MenuSaveNew(HWND hWnd)
{
	// *************** ����������

	TCHAR fname[128]={0},fpath[_MAX_PATH]={0};
	if( !GetNewSaveFileName( hWnd , fname , fpath , 126 , _MAX_PATH-2 ) )
	{
		return;
	}

	if( !Write(hWnd,fname) )
	{
		ERROR_MESSAGE(hWnd,TEXT("�t�@�C���ւ̏������݂Ɏ��s���܂���"));
		return;
	}

	strcpy( now_fname , fname );
	was_changed = false;

	TCHAR new_title[1024];
	wsprintf( new_title , TEXT("%s - %s") , title , fname );
	SetWindowText( hWnd , new_title );
}

// �I��
void MenuExit(HWND hWnd)
{
	if( was_changed )
	{
		// �ۑ��𕷂�
		switch( QUESTION_SAVE_CHANGE(hWnd) )
		{
		case IDYES:
			MenuSave(hWnd);
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
		}
	}

	// �I������
	g_is_end = true;
	DestroyWindow( GetParent(hWnd) );
	PostQuitMessage(0);
	EndDialog(hWnd,IDOK);
}
