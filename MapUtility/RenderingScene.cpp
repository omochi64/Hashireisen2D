
#include "Selene.h"
using namespace Selene;

#include "RenderingScene.h"
#include "CommonForSynchronize.h"

void RenderingScene( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// シーンマネージャーの内容を実際にレンダリング
	/////////////////////////////////////////////////////////////////////////////////////////////


	// 画面への描画を開始
	//   Renderer::IRenderインターフェイスへ画面への描画を開始することを通知します。
	//   この関数を呼ばないと画面への描画が正しく行われません。
	pRender->Begin();

	// シーンの内容をレンダリング
	//   ここまでで構築されたシーン情報を元に実際に描画処理を行います。
	//   この関数はRenderer::IRender::End()を呼び出す前でなければ失敗します。
	pSceneMgr->Rendering( true );

	// 画面への描画を完了
	//  Renderer::IRenderインターフェイスに対して、
	//  画面への描画が完了したことを通知します。
	pRender->End();
}
