
#include "Selene.h"
using namespace Selene;

#include "common.h"
#include "CommonForMainWin.h"
#include "CreateModelActor.h"

void CreateModelActor( RFC_ACTOR &request )
{

	// シーン管理に入ったら作成する
	
	// クリティカルセクションに突入
	EnterCriticalSection( &g_RFC_actor_CS );
	// シーンに入ったら自分がイベントを取得しておく
//	ResetEvent( g_hOutOfSceneMgrEvent );

	g_request_for_actor.push_back( request );

	// イベントを開放
//	SetEvent( g_hOutOfSceneMgrEvent );
	LeaveCriticalSection( &g_RFC_actor_CS );

}

