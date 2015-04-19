
#include <windows.h>

#include "common.h"
#include "ButtonFunctions.h"
#include "resource.h"
#include "CreateModelActor.h"
#include "Tools/IMapObjectDataForTool.h"
#include "AddActorToMap.h"
#include "UpdateNowPositionInfo.h"
#include "UpdateNowSelectedObjectInfo.h"
#include "SetNowMovingObject.h"

#include "CreateModelFromFile.h"

static bool finished_create = true;

void FinishedCreateActor( HWND hWnd , Scene::IModelActor *actor , int selected_object )
{
	finished_create = true;

	if( !actor || actor->IsInvalid() )
	{
		MessageBox( hWnd , "���f���̐����Ɏ��s���܂���" , NULL , MB_OK|MB_ICONERROR );
		SAFE_RELEASE(actor);
		return;
	}
	
	OBJECT_DATA & data = objects[selected_object];

	// �A�N�^�[���ړ�������
	// ���݈ʒu�̎擾
	TCHAR str[256];
	Math::Vector3D pos;
	GetDlgItemText( hWnd , IDC_EDIT_POS_X , str , 255 );
	pos.x = atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_POS_Y , str , 255 );
	pos.y = atof( str );
	GetDlgItemText( hWnd , IDC_EDIT_POS_Z , str , 255 );
	pos.z = atof( str );
	// �ړ�
	actor->Translation( pos );
	actor->Collision_Create();

	// �ǉ�
	IMapObjectDataForTool *add_data = new IMapObjectDataForTool();
	add_data->m_id = data.id;
	add_data->m_pActor = actor;
	if( !AddActorToMap( add_data ) )
	{
		SAFE_RELEASE(add_data);
		MessageBox( hWnd , "�}�b�v�ɒǉ��ł��܂���ł����B�I�u�W�F�N�g���}�b�v���͂ݏo���Ă���\��������܂��B" , NULL , MB_OK|MB_ICONERROR );
		return;
	}
	SetNowMovingObject( data.actor_for_copy );

	UpdateNowPositionInfo( hWnd );
	UpdateNowSelectedObjectInfo( hWnd );
}

void ButtonSetObject( HWND hWnd )
{
	if( !finished_create )
	{
		return;
	}
	// IModel����IModelActor�𐶐����Ag_mapRoute�ɒǉ�(�ǉ���MainWin���̓����̖�肪���邩��A�ǉ��֐���p����)

	// �I������Ă��郂�f�����擾
	if( objects.size() == 0 )
	{
		return;
	}
	
	OBJECT_DATA & data = objects[g_now_selected_object];

	// �A�N�^�[����̃��N�G�X�g
	RFC_ACTOR request;
	request.call_func_when_finish = FinishedCreateActor;
	request.hWnd = hWnd;
	request.pModel = data.model_master;
	request.other_info = g_now_selected_object;
	CreateModelActor( request );
	finished_create = false;
	
}
