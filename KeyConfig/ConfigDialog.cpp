
#include <windows.h>
#include <windowsx.h>
#include "resource.h"

#include "directx.h"
#include "JoyStick.h"

static JoyStick *joy_stick;

static int is_fullscreen;
static int is_halfdraw;

static int keyboard_or_joyid[2];
static DWORD joy_count;

HINSTANCE hInst;

#define INI_FILE_NAME	TEXT("./config.ini")

LRESULT CALLBACK KeyConfigDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool WriteInputInfo( TCHAR *file_name , DWORD pad_id , JOY_SET *joy_set );
void UpdateJoyPadInfoToGUI( HWND hWnd , int pad_id , JOY_SET *joy_set , int now_select , int next_select );
bool ReadInputInfo( TCHAR *file_name , int &pad_id , JOY_SET *joy_set );


LRESULT CALLBACK ConfigDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect,my_rect;
	TCHAR str[256];

	switch(msg)
	{
	case WM_INITDIALOG:

		GetClientRect( GetDesktopWindow() , &rect );
		GetWindowRect( hWnd , &my_rect );

		MoveWindow( hWnd , (rect.right+rect.left)/2-(my_rect.right+my_rect.left)/2 , (rect.top+rect.bottom)/2-(my_rect.top+my_rect.bottom)/2 ,
			my_rect.right-my_rect.left , my_rect.bottom-my_rect.top , FALSE );

		is_halfdraw = GetPrivateProfileInt( TEXT("GAME") , TEXT("IsHalfDraw") , 0 , INI_FILE_NAME );
		is_fullscreen = GetPrivateProfileInt( TEXT("GAME") , TEXT("IsFullScreen") , 1 , INI_FILE_NAME );

		Button_SetCheck(GetDlgItem(hWnd,IDC_CHECK_DRAW_HALF_FRAMES),is_halfdraw);
		Button_SetCheck(GetDlgItem(hWnd,IDC_RADIO_FULLSCREEN),is_fullscreen);
		Button_SetCheck(GetDlgItem(hWnd,IDC_RADIO_WINDOWSIZE),(!is_fullscreen));

		return TRUE;

	case WM_COMMAND:
		
		switch( LOWORD(wParam) )
		{
		case IDC_BUTTON_KEYCONFIG_PLAYER1:
			DialogBox( hInst , MAKEINTRESOURCE(IDD_DIALOG_KEYCONFIG),hWnd,(DLGPROC)KeyConfigDlgProc );
			break;


		case IDOK:

			// データの保存
//			wsprintf( str , TEXT("%d") , Button_GetCheck(GetDlgItem(hWnd,IDC_CHECK_DRAW_HALF_FRAMES)) );
//			WritePrivateProfileString( TEXT("GAME") , TEXT("IsHalfDraw") , str , INI_FILE_NAME );
			wsprintf( str , TEXT("%d") , Button_GetCheck(GetDlgItem(hWnd,IDC_RADIO_FULLSCREEN)) );
			WritePrivateProfileString( TEXT("GAME") , TEXT("IsFullScreen") , str , INI_FILE_NAME );


			EndDialog(hWnd,IDOK);
			break;

		case IDCANCEL:

			EndDialog(hWnd,IDCANCEL);
			break;
		}


	}
	return FALSE;
}


#define		TIMER_ID		12354


//---------------------------------------------------------------
// キーボードのキーセッティングもできるようにしたいなぁ…
//---------------------------------------------------------------


const static int tab_order_id[GAME_KEY_KIND_NUM-4] = {
	IDC_RADIO_JUMP , IDC_RADIO_DASH , IDC_RADIO_PAUSE ,  IDC_RADIO_CAMERA_UP , IDC_RADIO_CAMERA_DOWN ,
	IDC_RADIO_CAMERA_LEFT , IDC_RADIO_CAMERA_RIGHT , 
	IDC_RADIO_CAMERA_ZOOM , IDC_RADIO_CAMERA_ZOOM_OUT
};
const static int edit_order_id[GAME_KEY_KIND_NUM-4] = {
	IDC_EDIT_JUMP , IDC_EDIT_DASH , IDC_EDIT_PAUSE , IDC_EDIT_CAMERA_UP , IDC_EDIT_CAMERA_DOWN , 
	IDC_EDIT_CAMERA_LEFT , IDC_EDIT_CAMERA_RIGHT , 
	IDC_EDIT_CAMERA_ZOOM , IDC_EDIT_CAMERA_ZOOM_OUT
};

LRESULT CALLBACK KeyConfigDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int now_radio_button;

	TCHAR	temp_str[256];
	static JOY_SET	joy_set[GAME_KEY_KIND_NUM+1-4];
	static JoyButton	before_time_pad;
	static bool is_first_input;
	static int setting_joystick;
	int id;
	int i,j;

	switch(msg)
	{
	case WM_INITDIALOG:

		memset( joy_set , 0 , sizeof(KEY_SET)*(GAME_KEY_KIND_NUM+1-4) );

		joy_set[0].g_key = KEY_JUMP;
		joy_set[1].g_key = KEY_DASH;
		joy_set[2].g_key = KEY_PAUSE;
		joy_set[3].g_key = KEY_CAM_UP;
		joy_set[4].g_key = KEY_CAM_DOWN;
		joy_set[5].g_key = KEY_CAM_LEFT;
		joy_set[6].g_key = KEY_CAM_RIGHT;
		joy_set[7].g_key = KEY_CAM_ZOOM;
		joy_set[8].g_key = KEY_CAM_ZOOM_OUT;

		//---------------------------------------------------------------------------------------
		// 本来、ここはファイルで読み込み
		//---------------------------------------------------------------------------------------
		for( i=0 ; i<GAME_KEY_KIND_NUM-4 ; i++ )
		{
			joy_set[i].joy_button = -1;
		}

		// 有効/無効 を読み込む
		if( GetPrivateProfileInt( TEXT("KeyConfig") , TEXT("Valiable") , 1 , INI_FILE_NAME ) == 0 )
		{
			Button_SetCheck( GetDlgItem(hWnd,IDC_CHECK_INVALIABLE_PAD) , TRUE );
		}
		else
		{
			Button_SetCheck( GetDlgItem(hWnd,IDC_CHECK_INVALIABLE_PAD) , FALSE );
		}
		
		// ファイルから読み込み
		ReadInputInfo( INI_FILE_NAME , setting_joystick , joy_set );

		memset( &before_time_pad , 0 , sizeof(JoyButton) );

		is_first_input = true;

		Button_SetCheck(GetDlgItem(hWnd,tab_order_id[0]),TRUE);

		wsprintf( temp_str , TEXT("キーコンフィグ") );
		SetWindowText( hWnd , temp_str );

		SetTimer( hWnd , TIMER_ID , 1000/20 , NULL );

		joy_stick = new JoyStick();

		if( FAILED(joy_stick->Init( hInst , hWnd , 10 , 10 )) )
		{
			MessageBox( NULL , TEXT("DirectInput初期化エラー") , NULL , MB_ICONERROR|MB_OK );
			return TRUE;
		}
		joy_count = joy_stick->GetJoyCount();

			

		break;

	// キーボードのセッティングもできるようにするには、仮想キー番号と表示用の文字列のマッピング配列を準備する必要がある
	case WM_TIMER:

		joy_stick->UpData();

		int check_tab;
		int before_check;
		check_tab = 0;
		for( i=0 ; i<GAME_KEY_KIND_NUM-4 ; i++ )
		{
			if( Button_GetCheck(GetDlgItem(hWnd,tab_order_id[i])) )
			{
				check_tab = i;
				break;
			}
		}
		before_check = check_tab;

		for( i=0 ; i<joy_count ; i++ )
		{
			JoyButton &button = joy_stick->GetJoyButton(i);
			for( j=0 ; j<BUTTON_NUM ; j++ )
			{
				if( button.button[j] )
				{
					setting_joystick = i;
					i = joy_count;
					break;
				}
			}
		}

		{
			JoyButton &button = joy_stick->GetJoyButton(setting_joystick);

			for( i=0 ; i<BUTTON_NUM-4 ; i++ )
			{
				// 前のフレームで押していなく、かつこのフレームで押していたら有効とみなす(連続押しの防止)
				if( button.button[i] && !before_time_pad.button[i] )
				{
				//	Button_SetCheck(GetDlgItem(hWnd,tab_order_id[check_tab]),FALSE);
					joy_set[check_tab].joy_button = i;
					check_tab++;
					check_tab %= GAME_KEY_KIND_NUM;
				//	wsprintf( temp_str , "PAD%d_%d" , setting_joystick , i );
				//	SetWindowText( GetDlgItem( hWnd , edit_order_id[now_radio_button] ) , temp_str );
				//	Button_SetCheck(GetDlgItem(hWnd,tab_order_id[now_radio_button]),TRUE);
					break;
				}
			}
			// 連続押し防止のため、押したキーを保存
			memcpy( &before_time_pad , &button , sizeof(JoyButton) );
		
			UpdateJoyPadInfoToGUI( hWnd , setting_joystick , joy_set , before_check , check_tab );
		}

		break;

	case WM_COMMAND:
		id = LOWORD(wParam);
		for( i=0 ; i<GAME_KEY_KIND_NUM-4 ; i++ )
		{
			if( id == tab_order_id[i] )
			{
				now_radio_button = i;
				break;
			}
		}

		switch(id)
		{
		case IDOK:
			WriteInputInfo( INI_FILE_NAME , setting_joystick , joy_set );
			// 有効/無効 を書き込む
			wsprintf( temp_str , TEXT("%d") , Button_GetCheck(GetDlgItem(hWnd,IDC_CHECK_INVALIABLE_PAD))==TRUE?0:1 );
			WritePrivateProfileString( TEXT("KeyConfig") , TEXT("Valiable") , temp_str , INI_FILE_NAME );
			joy_stick->UnInit();
			delete joy_stick;
			EndDialog(hWnd,IDOK);
			break;

		case IDCANCEL:
			joy_stick->UnInit();
			delete joy_stick;
			EndDialog(hWnd,IDCANCEL);
			break;
		}
	}

	return FALSE;
}

const TCHAR *key_list[] = {
	TEXT("JUMP") , TEXT("DASH") , TEXT("PAUSE") ,
	TEXT("CAM_UP") , TEXT("CAM_DOWN") ,
	 TEXT("CAM_LEFT") , TEXT("CAM_RIGHT") ,
	 TEXT("CAM_ZOOM") , TEXT("CAM_ZOOM_OUT")
};

// ジョイパッド情報をラジオボタン、エディタ部分に反映
void UpdateJoyPadInfoToGUI( HWND hWnd , int pad_id , JOY_SET *joy_set , int now_select , int next_select )
{
	if( pad_id < 0 )
	{
		return;
	}

	TCHAR	str[1024];

	int i;
	for( i=0 ; i<GAME_KEY_KIND_NUM-4 ; i++ )
	{
		if( joy_set[i].joy_button < 0 )
		{
			continue;
		}
		wsprintf( str , TEXT("PAD%d_%d") , pad_id , joy_set[i].joy_button );
		SetWindowText( GetDlgItem( hWnd , edit_order_id[i] ) , str );
/*		switch( joy_set[i].g_key )
		{
		case KEY_SPACE:
			SetWindowText( GetDlgItem(hWnd,IDC_EDIT_ROTATE) , str );
			break;
		case KEY_ESCAPE:
			SetWindowText( GetDlgItem(hWnd,IDC_EDIT_ESCAPE) , str );
			break;
		case KEY_HOLD:
			SetWindowText( GetDlgItem(hWnd,IDC_EDIT_HOLD) , str );
			break;
		case KEY_PAUSE:
			SetWindowText( GetDlgItem(hWnd,IDC_EDIT_PAUSE) , str );
			break;
		}
*/	}

	Button_SetCheck( GetDlgItem(hWnd,tab_order_id[now_select]) , FALSE );
	Button_SetCheck( GetDlgItem(hWnd,tab_order_id[next_select]) , TRUE );
}

// 設定ファイルからパッド情報読み込み
bool ReadInputInfo( TCHAR *file_name , int &pad_id , JOY_SET *joy_set )
{
	int i;

	TCHAR	app_name[1024];
	wsprintf( app_name , TEXT("KeyConfig") );

	int id = GetPrivateProfileInt( app_name , TEXT("ID") , -1 , file_name );
	if( id == -1 )
	{
		return false;
	}
	pad_id = id;

	for( i=0 ; i<GAME_KEY_KIND_NUM-4 ; i++ )
	{
		joy_set[i].joy_button = GetPrivateProfileInt( app_name , key_list[i] , -1 , file_name );
/*		switch( joy_set[i].g_key )
		{
		case KEY_SPACE:
			joy_set[i].joy_button = GetPrivateProfileInt( app_name , TEXT("Rotate") , -1 , file_name );
			break;

		case KEY_ESCAPE:
			joy_set[i].joy_button = GetPrivateProfileInt( app_name , TEXT("Escape") , -1 , file_name );
			break;

		case KEY_HOLD:
			joy_set[i].joy_button = GetPrivateProfileInt( app_name , TEXT("Hold") , -1 , file_name );
			break;

		case KEY_PAUSE:
			joy_set[i].joy_button = GetPrivateProfileInt( app_name , TEXT("Pause") , -1 , file_name );
			break;
		}*/
	}


	return true;
}

// 設定ファイルにパッド情報を書く
bool WriteInputInfo( TCHAR *file_name , DWORD pad_id , JOY_SET *joy_set )
{


	TCHAR	app_name[1024],write_str[1024];

	wsprintf( app_name , TEXT("KeyConfig") );

	// PAD_ID
	wsprintf( write_str , TEXT("%d") , pad_id );
	WritePrivateProfileString( app_name , TEXT("ID") , write_str , file_name );
	
	int i;
	for( i=0 ; i<GAME_KEY_KIND_NUM-4 ; i++ )
	{
		wsprintf( write_str , TEXT("%d") , joy_set[i].joy_button );
		WritePrivateProfileString( app_name , key_list[i] , write_str , file_name );
/*		switch( joy_set[i].g_key )
		{
		case KEY_SPACE:
			WritePrivateProfileString( app_name , TEXT("Rotate") , write_str , file_name );
			break;

		case KEY_ESCAPE:
			WritePrivateProfileString( app_name , TEXT("Escape") , write_str , file_name );
			break;

		case KEY_HOLD:
			WritePrivateProfileString( app_name , TEXT("Hold") , write_str , file_name );
			break;

		case KEY_PAUSE:
			WritePrivateProfileString( app_name , TEXT("Pause") , write_str , file_name );
			break;
		}
*/	}

	return true;
}
