
#include "CommonForData.h"
#include "DeleteObjectFromMap.h"
#include "CommonForSynchronize.h"

bool DeleteObjectFromMap( IMapObjectDataForTool *data )
{
	EnterCriticalSection( &g_hMapRouteCS );
	bool r = g_mapRoute->DeleteObjectPrimirilyChild( data , false );
	LeaveCriticalSection( &g_hMapRouteCS );

	return r;
}
