

//----------------------------------------------------------------------------------
// INCLUDE
//----------------------------------------------------------------------------------
#include <math.h>

#define MAIN_WIN_DEFINE


#include "Selene.h"
#include "resource.h"
#include "DialogProc.h"
#include "common.h"
#include "InitializeSelene.h"
#include "RenderingScene.h"
#include "SetLighting.h"
#include "InitializeUtilityData.h"

#include "CheckMouseActivity.h"
#include "UpdateCamera.h"
#include "UpdateVisualActors.h"
#include "CheckKeyboardActivity.h"


#include "CommonForMainWin.h"
#include "CommonForSynchronize.h"
#include "CommonForVisual.h"
#include "CommonForData.h"
#include "CommonForCamera.h"
#include "RequestForCreate.h"

#include "SettingDialog.h"
#include "CreateModelFromFile.h"
#include "CreateModelActor.h"


//----------------------------------------------------------------------------------
// NAMESPACE
//----------------------------------------------------------------------------------
using namespace Selene;


//----------------------------------------------------------------------------------
// DEFINE
//----------------------------------------------------------------------------------
#define SCREEN_WIDTH			(640)
#define SCREEN_HEIGHT			(480)


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Scene::IPrimitiveActor *CreateXArrowActor( float cam_distance );
Scene::IPrimitiveActor *CreateYArrowActor( float cam_distance );
Scene::IPrimitiveActor *CreateZArrowActor( float cam_distance );

//extern Collision::CLine3D selec_line;


//----------------------------------------------------------------------------------
/**
	@brief メイン関数

	コンソールアプリケーションでいう所の main() 関数です。
	ウィンドウズアプリケーションでは以下の関数が main() になります。
*/
//----------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	int i,size;
	hInstance = hInst;
	ICore						*pCore			= NULL;
	IGraphicCard				*pGraphicCard	= NULL;
	File::IFileManager			*pFileMgr		= NULL;
	Renderer::IRender			*pRender		= NULL;
	Scene::ISceneManager		*pSceneMgr		= NULL;
	Scene::ICamera				*pCamera		= NULL;
	Peripheral::IMouse			*pMouse			= NULL;
	Peripheral::IKeyboard		*pKeyboard		= NULL;
	Scene::IPrimitiveActor		*pAreaActor		= NULL;
	Renderer::Object::ILine3D	*pAreaLine		= NULL;

//	Renderer::Object::ILine3D *pForDebugLine = NULL;
//	Scene::IPrimitiveActor		*pForDebugLineActor = NULL;
//	Renderer::SLineVertex3D l_vertex;
//	l_vertex.v1.Col = l_vertex.v2.Col = CColor(0xff,0xff,0xff);

	Scene::IPrimitiveActor		*pXActor = NULL,*pYActor = NULL,*pZActor = NULL;

	//----------------------------------------------------------------------------
	// システムの初期化
	//----------------------------------------------------------------------------
	// Seleneのシステムを初期化しています。
	// 起動時に必ず呼び出してください。
	System::Initialize();

	//----------------------------------------------------------------------------
	// コアの生成
	//----------------------------------------------------------------------------
	// システムからICoreインターフェイスを取得しています。
	// アプリケーションウィンドウに関するインターフェイスであり、
	// システム内に１つだけ存在します。
	pCore = System::CreateCore();
	g_is_end = false;

	// ・スレッド同期関係
	InitializeCriticalSection( &g_hOutOfSceneMgrCS );
	InitializeCriticalSection( &g_hMapRouteCS );
	InitializeCriticalSection( &g_RFC_model_CS );
	InitializeCriticalSection( &g_RFC_actor_CS );

	//----------------------------------------------------------------------------
	// 初期化
	//----------------------------------------------------------------------------
	// ICoreインターフェイスの初期化を行います。
	// アプリケーションウィンドウに関しての各種初期化が行われます。
	if ( pCore->Initialize( "Selene Sample Program", FRAME_RATE_60 ) )
	{
		// &pFileMgr , &pGraphicCard , &pRender , &pScene
		//-----------------------------------------------------------------
		// タイトルバーに情報を表示
		//-----------------------------------------------------------------
		pCore->EnableDrawTitleInformation( true );

		// 色々初期化
		if( !InitializeSelene( pCore , &pFileMgr , &pGraphicCard , &pRender , &pSceneMgr , &pCamera , SCREEN_WIDTH , SCREEN_HEIGHT ) )
		{
			goto EXIT;
		}

		g_pFileMgr = pFileMgr;
		g_pRender = pRender;
		g_pSceneMgr = pSceneMgr;

		pMouse = pCore->GetMouse();
		pKeyboard = pCore->GetKeyboard();

		InitializeUtilityData();
		
		// ダイアログボックスの生成
		RECT rc;
		HWND hWnd = pCore->GetWindowHandle();
		GetWindowRect( hWnd , &rc );
		MoveWindow( hWnd , 0 , 0 , rc.right-rc.left , rc.bottom-rc.top , TRUE );
		HWND hDlg = CreateDialog( hInst , MAKEINTRESOURCE(IDD_CONTROL_DIALOG) , hWnd , (DLGPROC)DialogProc );
		g_hDialog = hDlg;
		GetWindowRect( hDlg , &rc );
		MoveWindow( hDlg , rc.left+SCREEN_WIDTH , rc.top+SCREEN_HEIGHT , rc.right-rc.left , rc.bottom-rc.top , TRUE );
		ShowWindow( hDlg , SW_SHOW );

		//-----------------------------------------------------------------
		// 描画オブジェクト生成
		//-----------------------------------------------------------------
		// エリア描画用LineObject生成
		pAreaLine = pRender->CreateLine3D( 32768 , true );
//		pForDebugLine = pRender->CreateLine3D( 1024 , true );
		pAreaActor = pSceneMgr->CreateActor( pAreaLine );
//		pForDebugLineActor = pSceneMgr->CreateActor( pForDebugLine );

		//-----------------------------------------------------------------
		// 変更しないステート
		//-----------------------------------------------------------------
		// フィルタリング
		pRender->SetTextureFilterType( TEXTURE_STAGE_COLOR, TEXTURE_FILTER_3D_LOW );

		// アクター作成
		SAFE_RELEASE(pXActor);
		SAFE_RELEASE(pYActor);
		SAFE_RELEASE(pZActor);
		pXActor = CreateXArrowActor( Camera::g_dist );
		pYActor = CreateYArrowActor( Camera::g_dist );
		pZActor = CreateZArrowActor( Camera::g_dist );

		UpdateVisualActors( pRender , pSceneMgr );

		//-----------------------------------------------------------------
		// メインループ
		//-----------------------------------------------------------------
		// アプリケーションウィンドウのウィンドウメッセージの処理と、
		// ICoreインターフェイス経由で設定されたフレーム制御を行います。
		// またキー入力の更新や画面の更新もこの関数内で行われます。
		// この関数はウィンドウが終了されるとfalseを返します。
		// その場合、ただちにアプリケーションを終了するようにして下さい。
		while ( pCore->Run() && !g_is_end )
		{
			/////////////////////////////////////////////////////////////////////////////////////////////
			// シーンへのレンダリングリクエスト
			/////////////////////////////////////////////////////////////////////////////////////////////

			EnterCriticalSection( &g_hOutOfSceneMgrCS );
			EnterCriticalSection( &g_hMapRouteCS );
			// シーン管理
			//   Scene::ISceneManagerインターフェイスを使ったシーン管理を開始します。
			pSceneMgr->Begin( false );

			// レンダリング
			//   IActorに関連付けられたRenderer::Object::IModelインターフェイスの内容を描画します。
			//   この関数ではRenderer::IRenderインターフェイスに関連付けられたScene::ISceneManagerインターフェイスに対して
			//   描画リクエストを発行するだけで、描画を待たずに帰ってきます。
//			pActor->RenderingRequest();

			// マップのレンダリング
			g_mapRoute->DrawVisibleArea( pSceneMgr , SCREEN_WIDTH , SCREEN_HEIGHT );


			pXActor->TransformReset();
			pXActor->TransformUpdate();
			pXActor->RenderingRequest();

			pYActor->TransformReset();
			pYActor->TransformUpdate();
			pYActor->RenderingRequest();

			pZActor->TransformReset();
			pZActor->TransformUpdate();
			pZActor->RenderingRequest();

			Scene::IPrimitiveActor *pActor;
			pActor = g_nowPoint->GetActor();
			if( pActor )
			{
				pActor->TransformUpdate();
				pActor->RenderingRequest();
			}
			pActor = g_nowPlane->GetActor();
			if( pActor )
			{
				pActor->TransformUpdate();
				pActor->RenderingRequest();
			}

			// 動いているオブジェクトの描画
			if( g_nowMovingObject->GetActor() )
			{
				g_nowMovingObject->GetActor()->TransformUpdate();
				g_nowMovingObject->GetActor()->RenderingRequest();
			}

			// 空間領域の描画
			pAreaLine->Begin();
			g_mapRoute->DrawOctreeArea( pAreaLine , pSceneMgr , SCREEN_WIDTH , SCREEN_HEIGHT );
			pAreaLine->End();

			pAreaActor->RenderingRequest();

//			pForDebugLine->Begin();
//			l_vertex.v1.Pos = selec_line.vStart;
//			l_vertex.v2.Pos = selec_line.vEnd;
//			pForDebugLine->Push( &l_vertex , 1 );
//			pForDebugLine->End();
//			pForDebugLineActor->RenderingRequest();

			// シーン管理を終了
			//   ここまでで構築されたシーン情報を元に実際に描画用データの構築を行います。
			pSceneMgr->End();

			/////////////////////////////////////////////////////////////////////////////////////////////
			// シーンマネージャーの内容を実際にレンダリング
			/////////////////////////////////////////////////////////////////////////////////////////////
			RenderingScene( pRender , pSceneMgr );

			LeaveCriticalSection( &g_hOutOfSceneMgrCS );
			LeaveCriticalSection( &g_hMapRouteCS );


			/////////////////////////////////////////////////////////////////////////////////////////////
			

			/////////////////////////////////////////////////////////////////////////////////////////////
			// シーン情報構築
			/////////////////////////////////////////////////////////////////////////////////////////////

			// シーン情報リセット
			//   Scene::ISceneManagerインターフェイスを使ったシーン管理情報を初期化します。
			pSceneMgr->Reset();

			// シーンの背景の色
			pSceneMgr->SetParameter_BGColor( Math::Vector4D(0.5f,0.5f,0.5f,1.0f) );

			// 要求されたモデル作成の実行
			EnterCriticalSection( &g_RFC_model_CS );
			size = g_request_for_model.size();
			for( i=0 ; i<size ; i++ )
			{
				RFC_MODEL &request = g_request_for_model[i];
				// 作成
				Renderer::Object::IModel *pModel = pRender->CreateModelFromFile( request.fname );
				(*request.call_func_when_finish)(request.hWnd,pModel,request.other_info);
			}
			g_request_for_model.clear();
			LeaveCriticalSection( &g_RFC_model_CS );
			
			// 要求されたアクター作成の実行
			EnterCriticalSection( &g_RFC_actor_CS );
			size = g_request_for_actor.size();
			for( i=0 ; i<size ; i++ )
			{
				RFC_ACTOR &request = g_request_for_actor[i];
				// 作成
				Scene::IModelActor *pActor = pSceneMgr->CreateActor( request.pModel );
				pActor->Collision_Create();
				(*request.call_func_when_finish)(request.hWnd,pActor,request.other_info);
			}
			g_request_for_actor.clear();
			LeaveCriticalSection( &g_RFC_actor_CS );
			

			// マウスによる操作
			CheckMouseActivity( pMouse , pCore , pSceneMgr , pRender );
			// キーボードによる操作
			CheckKeyboardActivity( pKeyboard , pSceneMgr );

			// カメラ情報アップデート
			Camera::UpdateCamera( pCamera );

			// アクター作成
			SAFE_RELEASE(pXActor);
			SAFE_RELEASE(pYActor);
			SAFE_RELEASE(pZActor);
			pXActor = CreateXArrowActor( Camera::g_dist );
			pYActor = CreateYArrowActor( Camera::g_dist );
			pZActor = CreateZArrowActor( Camera::g_dist );

			UpdateVisualActors( pRender , pSceneMgr );

			// ライティングの設定
			SetLighting( pSceneMgr );
		}

EXIT:
		// ツール用データ解放
		DestroyUtilityData();


		// カメラの解放
		SAFE_RELEASE( pCamera );

		// シーンマネージャー解放
		SAFE_RELEASE( pSceneMgr );

		// レンダラーの解放
		SAFE_RELEASE( pRender );

		// グラフィックカードの解放
		SAFE_RELEASE( pGraphicCard );

		// ファイルマネージャー解放
		SAFE_RELEASE( pFileMgr );
	}

	// コアの解放
	SAFE_RELEASE( pCore );

	DeleteCriticalSection( &g_hOutOfSceneMgrCS );

	//--------------------------------------------------------
	// システムの解放
	//--------------------------------------------------------
	// アプリケーションの終了時に必ず呼び出して下さい。
	// またこの関数を呼び出す前に取得した全ての
	// インターフェイスを解放してください。
	System::Finalize();

	return 0;
}

// 矢印アクター作成
// dir
Scene::IPrimitiveActor *CreateXArrowActor( float cam_distance )
{
	Renderer::Object::ILine3D *pLine;
	Renderer::SLineVertex3D line;
	pLine = g_pRender->CreateLine3D( 6 , false );
	pLine->Begin();

	line.v1.Pos = Math::Vector3D( -cam_distance/3 , 0 , 0 );
	line.v2.Pos = Math::Vector3D( cam_distance/3 , 0 , 0 );
	line.v1.Col = line.v2.Col = CColor( 0xff , 0 , 0 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance*(1.0f/3-1.0f/64) , 0 , -cam_distance/64 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance*(1.0f/3-1.0f/64) , 0 , cam_distance/64 );
	pLine->Push( &line , 1 );

	pLine->End();

	Scene::IPrimitiveActor *actor = g_pSceneMgr->CreateActor( pLine );
	SAFE_RELEASE(pLine);
	return actor;
}

Scene::IPrimitiveActor *CreateYArrowActor( float cam_distance )
{
	Renderer::Object::ILine3D *pLine;
	Renderer::SLineVertex3D line;
	pLine = g_pRender->CreateLine3D( 6 , false );
	pLine->Begin();

	line.v1.Pos = Math::Vector3D( 0 , -cam_distance/3 , 0 );
	line.v2.Pos = Math::Vector3D( 0 , cam_distance/3 , 0 );
	line.v1.Col = line.v2.Col = CColor( 0 , 0xff , 0 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( -cam_distance/64 , cam_distance*(1.0f/3-1.0f/64) , 0 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance/64 , cam_distance*(1.0f/3-1.0f/64) , 0 );
	pLine->Push( &line , 1 );

	pLine->End();

	Scene::IPrimitiveActor *actor = g_pSceneMgr->CreateActor( pLine );
	SAFE_RELEASE(pLine);
	return actor;
}

Scene::IPrimitiveActor *CreateZArrowActor( float cam_distance )
{
	Renderer::Object::ILine3D *pLine;
	Renderer::SLineVertex3D line;
	pLine = g_pRender->CreateLine3D( 6 , false );
	pLine->Begin();

	line.v1.Pos = Math::Vector3D( 0 , 0 , -cam_distance/3 );
	line.v2.Pos = Math::Vector3D( 0 , 0 , cam_distance/3 );
	line.v1.Col = line.v2.Col = CColor( 0 , 0 , 0xff );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( -cam_distance/64 , 0 , cam_distance*(1.0f/3-1.0f/64) );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance/64 , 0 , cam_distance*(1.0f/3-1.0f/64) );
	pLine->Push( &line , 1 );

	pLine->End();

	Scene::IPrimitiveActor *actor = g_pSceneMgr->CreateActor( pLine );
	SAFE_RELEASE(pLine);
	return actor;
}
