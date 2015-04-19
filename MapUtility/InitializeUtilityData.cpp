
#include "common.h"
#include "CommonForData.h"
#include "CommonForVisual.h"
#include "CommonForCamera.h"
#include "UpdateCamera.h"
#include "InitializeUtilityData.h"

#include "InitializeForNowPoint.h"
#include "InitializeNowPlane.h"
#include "InitializeNowSelectedObject.h"

void InitializeUtilityData()
{
	// マップオブジェクトの生成
	g_mapRoute = new IOctreeMapNodeForTool();

	InitializeForNowPoint();
	InitializeNowPlane();
	InitializeNowSelectedObject();

	// カメラの設定
	Camera::SetCameraToDefault(g_pSceneMgr->GetCamera());
}


void DestroyUtilityData()
{
	SAFE_RELEASE(g_mapRoute);
	SAFE_RELEASE(g_nowSelectedObjectData);
	SAFE_RELEASE(g_nowMovingObjectData);
	g_nowPosition = Math::Vector3D(0,0,0);

	SAFE_RELEASE(g_nowPoint);
	SAFE_RELEASE(g_nowPlane);
	g_nowSelectedObject->AttachActor( NULL , false );
	g_nowMovingObject->AttachActor( NULL , false );
	SAFE_RELEASE(g_nowSelectedObject);
	SAFE_RELEASE(g_nowMovingObject);
}

void ReCreateUtilityData()
{
	DestroyUtilityData();
	InitializeUtilityData();
}
