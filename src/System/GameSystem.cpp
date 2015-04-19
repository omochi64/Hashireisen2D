
#include "System/GameSystem.h"

using namespace Selene;

namespace GameSystem
{
	Selene::Scene::ISceneManager	*pSceneMgr;
	Selene::Renderer::IRender *pRender;
	Selene::File::IFileManager *pFileMgr;
	Selene::ICore *pCore;
	IGraphicCard *pGraphicCard;
	Scene::ICamera *pCamera;
	CModelManager *pModelMgr;
	extern const int SCREEN_WIDTH,SCREEN_HEIGHT;

// Seleneの色々な初期化
bool Initialize(char *pWinTitle , Selene::eFrameRate frameRate , bool is_full_screen , float fovAngle )
{
	Renderer::CTextureConfig Config;

	//----------------------------------------------------------------------------
	// システムの初期化
	//----------------------------------------------------------------------------
	// Seleneのシステムを初期化しています。
	// 起動時に必ず呼び出してください。
	System::Initialize( true , false /*false Log.htmlの作成についてtrue/false*/ );

	//----------------------------------------------------------------------------
	// コアの生成
	//----------------------------------------------------------------------------
	// システムからICoreインターフェイスを取得しています。
	// アプリケーションウィンドウに関するインターフェイスであり、
	// システム内に１つだけ存在します。
	pCore = System::CreateCore();

	if( !pCore->Initialize( pWinTitle , frameRate ) )
	{
		return false;
	}

	//-----------------------------------------------------------------
	// タイトルバーに情報を表示
	//-----------------------------------------------------------------
	pCore->EnableDrawTitleInformation( false );
	pCore->EnableDrawMouseCursor( false );

	//-----------------------------------------------------------------
	// ファイル登録
	//-----------------------------------------------------------------
	// ファイルマネージャーを生成
	pFileMgr = pCore->CreateFileManager();
	// コアに設定
	pCore->SetFileManager( pFileMgr );
	// ルートディレクトリを設定
	pFileMgr->SetRootPath( 0, "Data" , "PasswordToUnpack" );
	// ルートディレクトリに対して読み込むカレントディレクトリ設定
	//  これは…いいのだろうか…
	pFileMgr->SetCurrentPath( "Texture" );

	//-----------------------------------------------------------------
	// グラフィックカードを取得
	//-----------------------------------------------------------------
	// グラフィックカードに関しての初期化を行います。
	// IGraphicCardインターフェイスからは接続された
	// グラフィックカードの能力を取得する事が可能です。
	// IGraphicCardインターフェイスは1つのICoreインターフェイスに対して1つしか存在しません。
	pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
	if ( pGraphicCard == NULL ) return false;

	//-----------------------------------------------------------------
	// ウィンドウ作成
	//-----------------------------------------------------------------
	// ICoreインターフェイス内の情報に従ってアプリケーション
	// ウィンドウを生成します。
	// IGraphicCardからの情報を元に画面解像度などを変更する場合は、
	// ICore::Start()を呼び出す前に行ってください。
	pCore->Start( SCREEN_WIDTH, SCREEN_HEIGHT, !is_full_screen );

	//-----------------------------------------------------------------
	// レンダラーを取得
	//-----------------------------------------------------------------
	// 作成したグラフィックカードから画面描画用のレンダラーを取得します。
	// Renderer::IRenderインターフェイスは1つのIGraphicCardインターフェイスに対して1つしか存在しません。
	pRender = pGraphicCard->CreateRender();
	if ( pRender == NULL ) return false;

	// テクスチャフォーマット
//		Config.SetColorKey( CColor(0,255,0,0xff) );
//		Config.
	Config.SetFormat( FORMAT_TEXTURE_DXT );
	Config.SetMipmapEnable( true );
	pRender->SetTextureConfig( Config );

	//-----------------------------------------------------------------
	// シーン
	//-----------------------------------------------------------------
	// シーンマネージャーを取得
	//   レンダラーに関連付けられたシーン管理用のインターフェイスを取得します。
	//   3D空間そのものを管理しているので、モデルなどのレンダリングに必要とします。

	// これの第一引数が最大ポリゴン数！！
	pSceneMgr = pRender->CreateSceneManager( 4096, true );

	// レンダリングターゲットのサイズ
	//   内部的な実描画領域
	pSceneMgr->InitParameter_RenderTargetData(
		NULL,												// レンダリングターゲットのテクスチャ（NULLでバックバッファへ直接）
		Math::Point2DI( SCREEN_WIDTH, SCREEN_HEIGHT ),		// 描画先のレンダリングターゲットサイズ（↑が指定されている時は↑のテクスチャサイズに自動変換）
		false );											// 描画先のテクスチャサイズを２の累乗にするかどうか

	// シーンに対するエフェクトの初期化
	//   シーンに対して行う各種レンダリングエフェクトはここで行います。
	pSceneMgr->InitParameter_Shading( SHADING_TYPE_NORMAL );							// シェーディング
//		pSceneMgr->InitParameter_Fog( FOG_TYPE_DISABLE, FOG_EFFECT_LINEAR );				// フォグ
//		pSceneMgr->InitParameter_HighDynamicRange( HDR_TYPE_ENABLE, HDR_EFFECT_BLOOM );		// HDRレンダリング
//		pSceneMgr->InitParameter_MotionBlur( MOTION_BLUR_TYPE_DISABLE );					// モーションブラー
//		pSceneMgr->InitParameter_DepthOfField( DOF_TYPE_DISABLE );							// 被写界深度
//		pSceneMgr->InitParameter_Shadow( SHADOW_TYPE_DISABLE, SHADOW_QUARITY_MIDDLE );		// 影生成
//		pSceneMgr->InitParameter_AntiAliasType( AA_TYPE_DISABLE, 0.0f );					// アンチエイリアス

	// シーンの初期化系パラメーターの更新
	//  これを呼び出すことでコレまでの設定が有効になり、
	//  アクターの生成やレンダリングが可能になる。
	pSceneMgr->InitParameter_Update();

	//-----------------------------------------------------------------
	// カメラ
	//-----------------------------------------------------------------
	// シーンからカメラを取得
	//   シーン1つに対して、1つのカメラを所持しています。
	//   シーンのカメラを操作するために、カメラのインターフェイスを
	//   シーンから取得します。
	pCamera = pSceneMgr->GetCamera();
	// カメラのプロジェクションを設定
	//   カメラの見える範囲と見え方を設定します。
	//   見える範囲が広いほどＺバッファの精度の問題で
	//   ちらつき易くなるので注意して設定してください。
	pCamera->UpdateProjection( 1.0f, 4096.0f, fovAngle , SCREEN_WIDTH, SCREEN_HEIGHT );

	//-----------------------------------------------------------------
	// 変更しないステート
	//-----------------------------------------------------------------
	// フィルタリング
	pRender->SetTextureFilterType( TEXTURE_STAGE_COLOR, TEXTURE_FILTER_3D_LOW );

	// 深度処理のステート設定
	//   深度バッファに関しての設定を行います。
	//   深度バッファへの書き込みを有効にし、
	//   深度バッファのテストを有効にしています。
	pRender->SetDepthTestEnable( true );
	pRender->SetDepthWriteEnable( true );

	// 描画ステート
	pRender->SetAlphaTestEnable( true );		// アルファテストON
	pRender->SetDepthTestEnable( true );		// ZテストON
	pRender->SetDepthWriteEnable( true );		// Z書き込みON
	pRender->SetDrawType( DRAW_TYPE_BLEND );	// 通常描画

	return true;

}

// Seleneのクリーンアップ
void CleanUp()
{
}

// Seleneオブジェクト、Manager系へのアクセスクラス
Selene::Scene::ISceneManager *SystemGetter::GetSceneManager()
{
	return pSceneMgr;
}
Selene::Renderer::IRender *SystemGetter::GetRender()
{
	return pRender;
}

Selene::File::IFileManager *SystemGetter::GetFileManager()
{
	return pFileMgr;
}

Selene::ICore *SystemGetter::GetCore()
{
	return pCore;
}
Selene::Scene::ICamera *SystemGetter::GetCamera()
{
	return pCamera;
}
CModelManager *SystemGetter::GetModelManager()
{
	return pModelMgr;
}



};
