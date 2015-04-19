
#include "Selene.h"
#include "CommonForSynchronize.h"
#include "CommonForData.h"

using namespace Selene;

#include "AddActorToMap.h"
#include "SetNowSelectedObject.h"
#include "SetNowPosition.h"

bool AddActorToMap( IMapObjectDataForTool *pData )
{
	// マップデータのクリティカルセクションに入る
	EnterCriticalSection( &g_hMapRouteCS );

	// 追加
	pData->m_pActor->TransformUpdateCollisionOnly();
	bool ret_v = g_mapRoute->AddObject( pData );

	LeaveCriticalSection( &g_hMapRouteCS );

	if( ret_v )
	{
		SetNowSelectedObject( pData );
	}

	return ret_v;
}
