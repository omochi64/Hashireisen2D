
#include <windows.h>
#include "SetNowSelectedObject.h"
#include "ButtonFunctions.h"
#include "CommonForData.h"
#include "resource.h"

void ButtonMoveObject( HWND hWnd )
{
	// �ʒu�̕ύX
	Math::Vector3D new_pos;
	TCHAR	str[256];

	GetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_X , str , 255 );
	new_pos.x = atof(str);
	GetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_Y , str , 255 );
	new_pos.y = atof(str);
	GetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_Z , str , 255 );
	new_pos.z = atof(str);

	IMapObjectDataForTool *data = GetNowSelectedObjectData();
	if( data && data->m_pActor && g_mapRoute->DeleteObjectPrimirilyParent( data , false ) )
	{
		Math::Vector3D old_pos;
		data->m_pActor->GetPosition( old_pos );

		data->m_pActor->Translation( new_pos );
		data->m_pActor->TransformUpdateCollisionOnly();
		if( !g_mapRoute->AddObject( data ) )
		{
			data->m_pActor->Translation( old_pos );
			data->m_pActor->TransformUpdateCollisionOnly();
			g_mapRoute->AddObject( data );
			MessageBox( hWnd , TEXT("�ړ��Ɏ��s���܂����B�I�u�W�F�N�g���}�b�v�̊O�ɂ͂ݏo���Ă���\��������܂��B") , NULL , MB_OK|MB_ICONERROR );
		}
	}
}
