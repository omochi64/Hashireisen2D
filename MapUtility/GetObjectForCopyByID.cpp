
#include <windows.h>
#include "common.h"
#include "GetObjectForCopyByID.h"

IMapObjectDataForTool *GetObjectForCopyByID(int id)
{
	vector<OBJECT_DATA>::iterator it,end;

	it = objects.begin(),end = objects.end();

	while( it != end )
	{
		if( (*it).id == id )
		{
			return (*it).actor_for_copy;
		}
		it++;
	}
	return NULL;
}
