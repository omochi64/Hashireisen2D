
#include <windows.h>

#include "Selene.h"
using namespace Selene;

#include "CommonForSynchronize.h"
#include "CommonForData.h"
#include "ChangeAllObjectById.h"

void ChangeAllObjectById( int id , Renderer::Object::IModel *new_model , Scene::ISceneManager *pSceneMgr )
{
	// •ÏX
	EnterCriticalSection( &g_hMapRouteCS );

	g_mapRoute->ChangeObjectById( id , new_model , pSceneMgr );
	std::vector<IMapObjectData *>out_of_objects;
	g_mapRoute->Re_PlaceObjects( out_of_objects );
	int i,size = out_of_objects.size();
	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(out_of_objects.at(i));
	}

	LeaveCriticalSection( &g_hMapRouteCS );
}
