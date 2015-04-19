
#include "Selene.h"
using namespace Selene;

#include "InitializeSelene.h"
#include "SettingDialog.h"
#include "CommonForCamera.h"
#include "UpdateCamera.h"

bool InitializeFileMgr( ICore *pCore , File::IFileManager **ppFileMgr );
bool InitializeGraphicCard( ICore *pCore , IGraphicCard	**ppGraphicCard );
bool InitializeRender( IGraphicCard	*pGraphicCard , Renderer::IRender **ppRender );
bool InitializeScene(Renderer::IRender *pRender , Scene::ISceneManager **ppSceneMgr , int screen_width , int screen_height );
bool InitializeCamera( Scene::ISceneManager *pSceneMgr , Scene::ICamera **ppCamera , int screen_width , int screen_height );

// 初期化
bool InitializeSelene( ICore *pCore , File::IFileManager **ppFileMgr , IGraphicCard	**ppGraphicCard , 
					  Renderer::IRender **ppRender , Scene::ISceneManager **ppSceneMgr , Scene::ICamera **ppCamera ,
					  int screen_width , int screen_height )
{
	if( !InitializeFileMgr( pCore , ppFileMgr ) )
	{
		return false;
	}
	if( !InitializeGraphicCard( pCore , ppGraphicCard ) )
	{
		return false;
	}
	//-----------------------------------------------------------------
	// ウィンドウ作成
	//-----------------------------------------------------------------
	// ICoreインターフェイス内の情報に従ってアプリケーション
	// ウィンドウを生成します。
	// IGraphicCardからの情報を元に画面解像度などを変更する場合は、
	// ICore::Start()を呼び出す前に行ってください。
	pCore->Start( screen_width , screen_height , true );

	if( !InitializeRender( *ppGraphicCard , ppRender ) )
	{
		return false;
	}

	if( !InitializeScene( *ppRender , ppSceneMgr , screen_width , screen_height ) )
	{
		return false;
	}

	if( !InitializeCamera( *ppSceneMgr , ppCamera , screen_width , screen_height ) )
	{
		return false;
	}

	// 深度処理のステート設定
	//   深度バッファに関しての設定を行います。
	//   深度バッファへの書き込みを有効にし、
	//   深度バッファのテストを有効にしています。
	Renderer::IRender *pRender = *ppRender;
	pRender->SetDepthTestEnable( true );
	pRender->SetDepthWriteEnable( true );

	// 描画ステート
	pRender->SetAlphaTestEnable( true );		// アルファテストON
	pRender->SetDepthTestEnable( true );		// ZテストON
	pRender->SetDepthWriteEnable( true );		// Z書き込みON
	pRender->SetDrawType( DRAW_TYPE_BLEND );	// 通常描画

	return true;
}

bool InitializeFileMgr( ICore *pCore , File::IFileManager **ppFileMgr )
{
	//-----------------------------------------------------------------
	// ファイル登録
	//-----------------------------------------------------------------
	// ファイルマネージャーを生成
	*ppFileMgr = pCore->CreateFileManager();
	// コアに設定
	pCore->SetFileManager( *ppFileMgr );
	// ルートディレクトリを設定
	(*ppFileMgr)->SetRootPath( 0, "Data" );
	// ルートディレクトリに対して読み込むカレントディレクトリ設定
	(*ppFileMgr)->SetCurrentPath( "Model" );

	// 本来、ここはファイルから読み込んで設定
	SetRouteFolder( "Data" );
	SetModelFolder( "Model" );

	return true;
}

bool InitializeGraphicCard( ICore *pCore , IGraphicCard	**ppGraphicCard )
{
	*ppGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
	if ( *ppGraphicCard == NULL ) return false;

	return true;
}

bool InitializeRender( IGraphicCard	*pGraphicCard , Renderer::IRender **ppRender )
{
	//-----------------------------------------------------------------
	// レンダラーを取得
	//-----------------------------------------------------------------
	// 作成したグラフィックカードから画面描画用のレンダラーを取得します。
	// Renderer::IRenderインターフェイスは1つのIGraphicCardインターフェイスに対して1つしか存在しません。
	*ppRender = pGraphicCard->CreateRender();
	if ( *ppRender == NULL ) return false;

	// テクスチャフォーマット
	Renderer::CTextureConfig Config;
	Config.SetColorKey( CColor(0,0,0,0) );
	Config.SetFormat( FORMAT_TEXTURE_DXT );
	Config.SetMipmapEnable( true );
	(*ppRender)->SetTextureConfig( Config );

	return true;
}

bool InitializeScene(Renderer::IRender *pRender , Scene::ISceneManager **ppSceneMgr , int screen_width , int screen_height )
{
	//-----------------------------------------------------------------
	// シーン
	//-----------------------------------------------------------------
	// シーンマネージャーを取得
	//   レンダラーに関連付けられたシーン管理用のインターフェイスを取得します。
	//   3D空間そのものを管理しているので、モデルなどのレンダリングに必要とします。
	*ppSceneMgr = pRender->CreateSceneManager( 128, true );

	
	// レンダリングターゲットのサイズ
	//   内部的な実描画領域
	(*ppSceneMgr)->InitParameter_RenderTargetData(
		NULL,												// レンダリングターゲットのテクスチャ（NULLでバックバッファへ直接）
		Math::Point2DI( screen_width, screen_height ),		// 描画先のレンダリングターゲットサイズ（↑が指定されている時は↑のテクスチャサイズに自動変換）
		false );											// 描画先のテクスチャサイズを２の累乗にするかどうか

	// シーンに対するエフェクトの初期化
	//   シーンに対して行う各種レンダリングエフェクトはここで行います。
	(*ppSceneMgr)->InitParameter_Shading( SHADING_TYPE_NORMAL );							// シェーディング
//		pSceneMgr->InitParameter_Fog( FOG_TYPE_DISABLE, FOG_EFFECT_LINEAR );				// フォグ
//		pSceneMgr->InitParameter_HighDynamicRange( HDR_TYPE_ENABLE, HDR_EFFECT_BLOOM );		// HDRレンダリング
//		pSceneMgr->InitParameter_MotionBlur( MOTION_BLUR_TYPE_DISABLE );					// モーションブラー
//		pSceneMgr->InitParameter_DepthOfField( DOF_TYPE_DISABLE );							// 被写界深度
//		pSceneMgr->InitParameter_Shadow( SHADOW_TYPE_DISABLE, SHADOW_QUARITY_MIDDLE );		// 影生成
//		pSceneMgr->InitParameter_AntiAliasType( AA_TYPE_DISABLE, 0.0f );					// アンチエイリアス

	// シーンの初期化系パラメーターの更新
	//  これを呼び出すことでコレまでの設定が有効になり、
	//  アクターの生成やレンダリングが可能になる。
	(*ppSceneMgr)->InitParameter_Update();

	return true;
}

bool InitializeCamera( Scene::ISceneManager *pSceneMgr , Scene::ICamera **ppCamera , int screen_width , int screen_height )
{
	//-----------------------------------------------------------------
	// カメラ
	//-----------------------------------------------------------------
	// シーンからカメラを取得
	//   シーン1つに対して、1つのカメラを所持しています。
	//   シーンのカメラを操作するために、カメラのインターフェイスを
	//   シーンから取得します。
	*ppCamera = pSceneMgr->GetCamera();
	// カメラのプロジェクションを設定
	//   カメラの見える範囲と見え方を設定します。
	//   見える範囲が広いほどＺバッファの精度の問題で
	//   ちらつき易くなるので注意して設定してください。
	(*ppCamera)->UpdateProjection( 1.0f, 4096.0f, DEG_TO_ANGLE(45), screen_width , screen_height );

	// カメラの初期情報設定
	Camera::SetCameraToDefault( *ppCamera );

	return true;
}
