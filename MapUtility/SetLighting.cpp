
#include "Selene.h"
using namespace Selene;

#include "SetLighting.h"

void SetLighting( Scene::ISceneManager *pSceneMgr )
{
	// 平行光源
	//   シーンに対して平行光源を設定します。
	pSceneMgr->SetLightParameter_Directional(
		Math::Vector3D( 0.5f, -0.5f, 0.7f ),		// ライトの方向
		Math::Vector3D( 0.8f, 0.8f, 0.8f ) );		// ライトの色

	// アンビエント
	// 半球ライティングを行っているので、天球と地表の照り返しの色を設定します。
	pSceneMgr->SetLightParameter_Ambient(
		Math::Vector3D( 0.2f, 0.2f, 0.2f ),			// 天球の色
		Math::Vector3D( 0.5f, 0.5f, 0.5f ) );		// 地表の色
}
