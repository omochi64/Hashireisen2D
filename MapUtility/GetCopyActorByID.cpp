
#include "common.h"
#include "GetCopyActorByID.h"

Scene::IModelActor *GetCopyActorByID( int id )
{
	vector<OBJECT_DATA>::iterator it,end;
	it = objects.begin(),end = objects.end();

	while( it != end )
	{
		if( (*it).id == id )
		{
			return (*it).actor_for_copy->m_pActor;
		}
	}
	return NULL;
}
