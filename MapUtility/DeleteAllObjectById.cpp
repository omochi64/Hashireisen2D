
#include "Selene.h"
using namespace Selene;

#include "CommonForSynchronize.h"
#include "CommonForData.h"
#include "DeleteAllObjectById.h"
#include "SetNowSelectedObject.h"

void DeleteAllObjectById( int id )
{
	// íœ‚§
	EnterCriticalSection( &g_hMapRouteCS );

	IMapObjectDataForTool *data = GetNowSelectedObjectData();
	if( data && data->m_id == id )
	{
		SetNowSelectedObject( NULL );
	}
	g_mapRoute->DeleteObjectById( id );

	LeaveCriticalSection( &g_hMapRouteCS );
}
