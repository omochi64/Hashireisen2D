
#include "IMapObjectData.h"

IMapObjectData::IMapObjectData()
{
	m_pActor = NULL;
}

IMapObjectData::~IMapObjectData()
{
	Release();
}

void IMapObjectData::Release()
{
	SAFE_RELEASE(m_pActor);
}

void IMapObjectData::Copy( IMapObjectData *copy_to )
{
	copy_to->m_pActor = m_pActor;
}

bool IMapObjectData::Compare( IMapObjectData *object )
{
	if( object->m_pActor == object->m_pActor )
	{
		return true;
	}
	return false;
}
