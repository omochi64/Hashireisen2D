
#include "Selene.h"
using namespace Selene;

#include "common.h"
#include "CommonForMainWin.h"
#include "CreateModelFromFile.h"

void CreateModelFromFile( RFC_MODEL &request )
{
//	Renderer::Object::IModel *pModel;

	// シーン管理に入ったら作成する

	// クリティカルセクションに突入
	EnterCriticalSection( &g_RFC_model_CS );
	// シーンに入ったら自分がイベントを取得しておく
//	ResetEvent( g_hOutOfSceneMgrEvent );

//	pModel = g_pRender->CreateModelFromFile( fname );
	g_request_for_model.push_back(request);

	// イベントを開放
//	SetEvent( g_hOutOfSceneMgrEvent );
	LeaveCriticalSection( &g_RFC_model_CS );

//	return request.pModel;
}
