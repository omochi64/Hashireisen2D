
#include "common.h"

Renderer::Object::IModel *GetModelByID( int id )
{
	vector<OBJECT_DATA>::iterator it,end;
	it = objects.begin();
	end = objects.end();

	while( it != end )
	{
		if( (*it).id == id )
		{
			return (*it).model_master;
		}
		it++;
	}
	return NULL;
}
