
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>

#include "InitDialog.h"
#include "resource.h"

// スピンの設定
void SetSpin( HWND hDlg , int spin_id , int edit_id , int init )
{
	HWND hSpin = GetDlgItem(hDlg,spin_id);
	SendMessage(hSpin, UDM_SETBUDDY, (WPARAM)GetDlgItem(hDlg,edit_id), 0);
	SendMessage(hSpin, UDM_SETRANGE32, (WPARAM)0, (LPARAM)60000);
	SendMessage(hSpin, UDM_SETPOS, 0, (LPARAM)MAKELONG((short)(init+SPIN_OFFSET), 0));
	TCHAR str[256];
	sprintf( str , "%.1f" , init-SPIN_OFFSET );
	SetWindowText( GetDlgItem(hDlg,edit_id) , str );
	
}

// 初期化関数
void InitDialog( HWND hWnd )
{
	HWND hList;
	hList = GetDlgItem( hWnd , IDC_LIST_OBJECT_LIST );
	LV_COLUMN col;

	long temp = SendMessage(hList, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	temp |= LVS_EX_FULLROWSELECT;
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)temp);


	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 25;
	col.pszText = "ID";
	col.iSubItem = 0;
	ListView_InsertColumn(hList, 0, &col);

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 75;
	col.pszText = "Object Name";
	col.iSubItem = 1;
	ListView_InsertColumn(hList, 1, &col);

	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt = LVCFMT_LEFT;
	col.cx = 200;
	col.pszText = "FILE";
	col.iSubItem = 2;
	ListView_InsertColumn(hList, 2, &col);

	// スピンコントロールのバディを設定
	SetSpin( hWnd , IDC_SPIN_POS_X , IDC_EDIT_POS_X , 0 );
	SetSpin( hWnd , IDC_SPIN_POS_Y , IDC_EDIT_POS_Y , 0 );
	SetSpin( hWnd , IDC_SPIN_POS_Z , IDC_EDIT_POS_Z , 0 );

	SetSpin( hWnd , IDC_SPIN_OBJECT_POS_X , IDC_EDIT_OBJECT_POS_X , 0 );
	SetSpin( hWnd , IDC_SPIN_OBJECT_POS_Y , IDC_EDIT_OBJECT_POS_Y , 0 );
	SetSpin( hWnd , IDC_SPIN_OBJECT_POS_Z , IDC_EDIT_OBJECT_POS_Z , 0 );

	// エディタ初期化
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_X , 0 , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Y , 0 , FALSE );
	SetDlgItemInt( hWnd , IDC_EDIT_NOW_DIVIDE_Z , 0 , FALSE );
}
