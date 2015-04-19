
// ToDo
//	//・リストをクリックして修正
//	//・リストからの削除
//	//・フォルダ階層構造の問題解決
//	//・メインウィンドウとのオブジェクトの関連付け(IModel,IModelActorなどをどこで管理するか決定する)
//	//・ダイアログ上からのマップサイズの変更
//	//・追加
//	//・メインウィンドウ上での表示 → オブジェクト、軸矢印、現在位置、現在XZ平面、現在選択オブジェクトの表示完了。使っていって使いにくいと感じたら変えていく
//	//	→現在位置には、現在選択しているオブジェクトのプレビューを表示する？
//	×・オブジェクト情報の表示	→	無理かもしれない…情報が取得できない…
//	//・メインウィンドウ上での操作。オブジェクトの選択、視点変更が終わった。
//	×・隣接追加	→	無理かもしれない…情報が取得できない…
//	//・既存オブジェクトの削除
//	//・既存オブジェクトの置き換え
//	//・オブジェクトリスト情報変更によるマップの変更 → 時間かかるかも(めんどくささ的な意味で)
//	//・DivideBoxSpaceの修正
//	//・Octree分割、分割領域の可視化
//	///*・ファイルへの保存、ファイルからの読み込み→処理自体は実装完了。後は呼び出すフロントエンド周り。データの削除/作成と絡んでるからめんどくさいよ*/
//	//	→後はデバッグ。/*ApplyModelListToList修正と、*/開く→保存→新規作成したときのバグ修正(test_map2のときしか再現しない？)→あれ…直った？
//	//	　テストケース(簡単なもの)：色々いじる→新規作成 or 開く、開く→いじる→名前を(ry、保存連打
//	//△・オブジェクトの位置の変更がまだ(薄くしたゴーストで移動先を示す)。どうせだから「複製」ボタンもつける？
//	//	→一応できたがデバッグが不十分。ああ怖い
//	//・当たり判定バグ在り。ボックスの当たり判定に、厚0さのCBoxを用いてるのが原因？
//	//	model->TransformUpdateCollisionOnlyで直った
//	//・視点の移動をしたとき、オブジェクトの選択にバグ在り(現在位置の指定は問題なし)
//	//・マップデータにバグ在り。
//	//	空データ(マップサイズとかは指定在り)保存→開く、モデルをリストに追加、マップに配置→保存、終了→開く、モデルをリストに追加、終了→開く
//	//	これにより、後に追加したモデルもIDが0となってしまっている
//	//・オブジェクトの変更時(1つのオブジェクトに対してID変更時)、Movingが変更されていない(ButtonChangeObject内)
//	//・マップ外にも移動できてしまう(複製は無理)
//	・全体的に当たり判定がおかしい？
//		SetObjectやMoveObjectでは、端近くではみ出していないのにはみ出したと判定される場合がある。
//		CopyObjectでは、端近くで少しはみ出していても大丈夫と判定される。
//			場合によるかも。場合によっては移動でNoHitと判断され、複製でHitと判断される。いったい何なんだこれは…
//		やっぱり当たり判定おかしい…。Depthが明らかに変
//	↑どうやらデータがおかしかったよう。Seleneのせいか、自分の作ったマップデータのせいかよくわからない。とりあえず、複雑な地形チップは作らないほうがいい
//	・OctreeMapNodeのCheckVisibleが不十分。見えるのに見えないと判定されることがある→数学系の本を読んだほうがいい？Gemsとか

// 以下必須ではない実装事項
//	//・選択オブジェクトの深さ(Octree上の位置)も表示できたほうがいい ← あるとデバッグ時も便利
//	//・移動用オブジェクトが選択オブジェクトより前に表示されて、選択オブジェクトと重なってるときに赤色のオブジェクトが見えなくなってしまう。
//	//	直さないと実用できないほどのバグではないが、気になる
//	//・時間があれば、F1等で視点をデフォルトに戻す

// ファイルオープン、保存回りは多分大丈夫だと思うが、無茶するときちんと保存されない可能性があることをきちんと言っておいたほうがいい
// 起動→編集→(名前をつけて)保存、起動→開く→(名前をつけて)保存、程度のことにしたほうがいい。
// 新規作成は、一応動くが、テスト用データを作る程度ならともかく、コースを本格的に作ったりするような場合は使わないほうがいい(再起動したほうがいい)

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
	LPCTSTR	title=TEXT("マップ作成ツールコントローラ");
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

// スピンとエディットボックスの関連付け
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
		// 初期化
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
		// 設定メニュー
		case ID_SETTING:
			MenuSetting( hWnd );
			break;

		// ヘッダ設定
		case ID_SET_HEADER:
			if( DialogBox( hInstance , MAKEINTRESOURCE(IDD_DIALOG_SET_HEADER) , hWnd , (DLGPROC)SettingHeaderDialog ) == IDOK )
			{
				was_changed = true;
			}
			break;

		// 新規作成
		case ID_CREATE_NEW:
			MenuCreateNew(hWnd);
			break;

		// 開く
		case ID_OPEN:
			MenuOpen(hWnd);
			break;

		// 保存
		case ID_SAVE:
			MenuSave(hWnd);
			break;

		// 名前をつけて保存
		case ID_SAVE_NEW:
			MenuSaveNew(hWnd);
			break;

		// 終了
		case ID_EXIT:
			MenuExit(hWnd);
			break;

		// 参照ボタン
		case IDC_BUTTON_REF_FILE:
			ButtonRefFile( hWnd );
			break;

		// 追加ボタン
		case IDC_BUTTON_ADD_OBJECT:
			ButtonAddObject( hWnd );
			was_changed = true;
			break;

		// 変更ボタン
		case IDC_BUTTON_CHANGE_OBJECT_INFO:
			ButtonChangeObjectInfo( hWnd );
			was_changed = true;
			break;

		// 削除ボタン
		case IDC_BUTTON_DELETE_OBJECT_FROM_LIST:
			ButtonDeleteObjectFromList( hWnd );
			was_changed = true;
			break;

		// オブジェクトの設置ボタン
		case IDC_BUTTON_SET_OBJECT:
			ButtonSetObject( hWnd );
			was_changed = true;
			break;

		// オブジェクトの削除ボタン
		case IDC_BUTTON_DELETE_OBJECT:
			ButtonDeleteObject( hWnd );
			was_changed = true;
			break;

		// オブジェクトの変更ボタン
		case IDC_BUTTON_CHANGE_OBJECT_ID:
			ButtonChangeObject( hWnd );
			was_changed = true;
			break;

		// オブジェクトの移動ボタン
		case IDC_BUTTON_MOVE_OBJECT:
			ButtonMoveObject( hWnd );
			was_changed = true;
			break;

		// オブジェクトの複製ボタン
		case IDC_BUTTON_COPY_OBJECT:
			ButtonCopyObject( hWnd );
			was_changed = true;
			break;

		// 空間分割ボタン
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
			// エディットボックス直接操作
			for( i=0 ; spin_edit[i].edit_id != 0 ; i++ )
			{
				if( hItem == GetDlgItem( hWnd , spin_edit[i].edit_id ) )
				{
					SettingWithChangeValueDirectly( hItem , GetDlgItem( hWnd , spin_edit[i].spin_id ) );
					// 現在位置の変更の場合
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
		// スピンコントロールの操作
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


				// クリックされたところのデータを反映させる
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



// スピンボタンを押したときの設定
void SettingWithChangeValue( HWND hEdit , int v )
{
	// ここで、Editの種類によって主々のデータを変える

	TCHAR str[256];
	sprintf( str , "%.1f" , (v-SPIN_OFFSET)/10.0 );
	SetWindowText( hEdit , str );
}

// 直接設定されたとき
void SettingWithChangeValueDirectly( HWND hEdit , HWND hSpin )
{
	TCHAR str[256];
	GetWindowText( hEdit , str , 255 );
	int v = (int)(atof(str)*10);
	SendMessage(hSpin, UDM_SETPOS, 0, (LPARAM)MAKELONG((short)(v+SPIN_OFFSET), 0));
}

// クリックされた所を反映
void UpdateBasicObjectInfo( HWND hWnd )
{
	if( g_now_selected_object < 0 || g_now_selected_object >= objects.size() )
	{
		return;
	}
	SetWindowText( GetDlgItem( hWnd , IDC_EDIT_ADD_OBJECT ) , objects.at(g_now_selected_object).file );
	SetWindowText( GetDlgItem( hWnd , IDC_EDIT_OBJECT_NAME ) , objects.at(g_now_selected_object).name );
}

// 現在位置の変更
void ApplyNowPosition( HWND hWnd )
{
	// 現在位置変更
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

// 現在の移動先位置の変更
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
// メニュー関連
//

#define ERROR_MESSAGE(hWnd,mes)			(MessageBox(hWnd,mes , NULL , MB_OK|MB_ICONERROR))
#define	QUESTION_SAVE_CHANGE(hWnd)		(MessageBox(hWnd,TEXT("変更を保存しますか?") , TEXT("保存") , MB_YESNOCANCEL|MB_ICONQUESTION))

bool GetNewOpenFileName( HWND hWnd , TCHAR fname[] , TCHAR fpath[] , int fname_length , int fpath_length )
{
	OPENFILENAME of;
	memset( &of , 0 , sizeof(OPENFILENAME) );
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.lpstrFilter = "マップファイル(.m)\0*.m\0\0";
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
	of.lpstrFilter = "マップファイル(.m)\0*.m\0\0";
	of.lpstrFile = fpath;
	of.nMaxFile = fpath_length;
	of.lpstrFileTitle = fname;
	of.nMaxFileTitle = fname_length;
	of.Flags = OFN_EXPLORER;
	of.lpstrDefExt = ".m";

	return GetSaveFileName( &of ) ? true:false;
}

// 新規作成
void MenuCreateNew(HWND hWnd)
{
	if( was_changed )
	{
		// 保存を聞く
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
	// 選択オブジェクトをクリア
	SetNowSelectedObject( NULL );

	// データを作り直し
	ReCreateUtilityData();
	// オブジェクトリストも初期化
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

	// とりあえずModelとActorの生成
	Renderer::Object::IModel *pModel;
	Scene::IModelActor *pActor;
	IMapObjectDataForTool *copy_actor;
	OBJECT_DATA new_data;
	int max_id=0;

	std::vector<OBJECT_DATA>	new_objects;

	// データの生成
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
			// モデル、アクターの生成
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
			// エラー処理
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


	// メインデータにコピー
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
		// リストに反映
	
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

// ファイルを開く
void MenuOpen(HWND hWnd)
{
	if( was_changed )
	{
		// 保存を聞く
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

	// ファイルを聞く
	//  ファイルオープン失敗 → 現在のまま
	//	ファイルオープン成功 → いったん初期化してロード
	TCHAR fname[128]={0},fpath[_MAX_PATH]={0};
	if( !GetNewOpenFileName( hWnd , fname , fpath , 126 , _MAX_PATH-2 ) )
	{
		return;
	}

	HANDLE hFile = CreateFile( fpath , GENERIC_READ , 0 , NULL ,
		OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		ERROR_MESSAGE(hWnd,TEXT("ファイルが開けません"));
		return;
	}
	CloseHandle(hFile);

	// 初期化
	// 選択オブジェクトをクリア
	SetNowSelectedObject( NULL );
	// データを作り直し
	ReCreateUtilityData();
	// オブジェクトリストも初期化
	ClearObjectList(hWnd);
	UpdateNowPositionInfo( hWnd );
	UpdateNowSelectedObjectInfo( hWnd );

	now_fname[0] = 0;

	// 読み込み
	Map::MODEL_FILE_INFO *models;
	int kind_num=0;
	int divide_x=0,divide_y=0,divide_z=0;
	if( !Map::AccessToMapFile::ReadMapForToolFromFile( fpath , &g_mapRoute , &models , kind_num ,
					g_pRender , g_pSceneMgr ,divide_x,divide_y,divide_z ) )
	{
		ERROR_MESSAGE(hWnd,TEXT("データの読み込みに失敗しました"));
		return;
	}

	// リストに反映
	ApplyModelListToList( hWnd , models , kind_num );
	// 現在の分割数
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

	// *************** ここ未実装
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

// 上書き
void MenuSave(HWND hWnd)
{
	if( now_fname[0] == 0 )
	{
		MenuSaveNew(hWnd);
		return;
	}

	if( !Write( hWnd , now_fname ) )
	{
		ERROR_MESSAGE(hWnd,TEXT("ファイルへの書き込みに失敗しました"));
	}
	was_changed = false;
}

// 新規保存
void MenuSaveNew(HWND hWnd)
{
	// *************** ここ未実装

	TCHAR fname[128]={0},fpath[_MAX_PATH]={0};
	if( !GetNewSaveFileName( hWnd , fname , fpath , 126 , _MAX_PATH-2 ) )
	{
		return;
	}

	if( !Write(hWnd,fname) )
	{
		ERROR_MESSAGE(hWnd,TEXT("ファイルへの書き込みに失敗しました"));
		return;
	}

	strcpy( now_fname , fname );
	was_changed = false;

	TCHAR new_title[1024];
	wsprintf( new_title , TEXT("%s - %s") , title , fname );
	SetWindowText( hWnd , new_title );
}

// 終了
void MenuExit(HWND hWnd)
{
	if( was_changed )
	{
		// 保存を聞く
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

	// 終了する
	g_is_end = true;
	DestroyWindow( GetParent(hWnd) );
	PostQuitMessage(0);
	EndDialog(hWnd,IDOK);
}
