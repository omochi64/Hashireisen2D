
#include <stdio.h>
#include <windows.h>
#include "resource.h"
#include "UpdateNowSelectedObjectInfo.h"
#include "common.h"

#include "SetNowSelectedObject.h"

void UpdateNowSelectedObjectInfo( HWND hWnd )
{
	IMapObjectDataForTool *now_selected = GetNowSelectedObjectData();
	if( !now_selected )
	{
		SetDlgItemInt( hWnd , IDC_EDIT_OBJECT_POS_X , 0 , false );
		SetDlgItemInt( hWnd , IDC_EDIT_OBJECT_POS_Y , 0 , false );
		SetDlgItemInt( hWnd , IDC_EDIT_OBJECT_POS_Z , 0 , false );
		SetDlgItemText( hWnd , IDC_EDIT_SELECTED_OBJECT_ID , "" );
		SetDlgItemText( hWnd , IDC_EDIT_SELECTED_OBJECT_NAME , "" );
		return;
	}
	else
	{
		// オブジェクトの現在位置を更新
		Math::Vector3D now_pos;
		now_selected->m_pActor->GetPosition( now_pos );

		TCHAR str[256];
		sprintf( str , "%.1f" , now_pos.x );
		SetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_X , str );

		sprintf( str , "%.1f" , now_pos.y );
		SetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_Y , str );

		sprintf( str , "%.1f" , now_pos.z );
		SetDlgItemText( hWnd , IDC_EDIT_OBJECT_POS_Z , str );

		// 現在のオブジェクトID反映
		SetDlgItemInt( hWnd , IDC_EDIT_SELECTED_OBJECT_ID , now_selected->m_id , FALSE );

		// 現在のオブジェクト名反映
		int i,size = objects.size();
		for( i=0 ; i<size ; i++ )
		{
			OBJECT_DATA &data = objects.at(i);
			if( now_selected->m_id == data.id )
			{
				SetDlgItemText( hWnd , IDC_EDIT_SELECTED_OBJECT_NAME , data.name );
				break;
			}
		}

		// 現在のオブジェクトの深さ反映
		BOOL b;
		int max_depth = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_X , &b , FALSE );
		int temp = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Y , &b , FALSE );
		if( max_depth < temp )
		{
			max_depth = temp;
		}
		temp = GetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Z , &b , FALSE );
		if( max_depth < temp )
		{
			max_depth = temp;
		}
		int *depth = new int[max_depth+1];
		if( GetNowSelectedObjectDepth( depth ) )
		{
			TCHAR temp[256];
			wsprintf( temp , TEXT("%d") , depth[0] );
			lstrcpy( str , temp );
			for( i=1 ; depth[i] != -1 ; i++ )
			{
				wsprintf( temp , TEXT("-%d") , depth[i] );
				lstrcat( str , temp );
			}
			SetDlgItemText( hWnd , IDC_EDIT_OBJECT_DEPTH , str );
		}
		else
		{
			SetDlgItemText( hWnd , IDC_EDIT_OBJECT_DEPTH , TEXT("") );
		}
	}
}
