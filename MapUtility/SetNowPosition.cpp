
#include "Selene.h"
using namespace Selene;

#include "CommonForData.h"

void SetNowPosition( const Math::Vector3D &now_position )
{
	g_nowPosition = now_position;
}

const Math::Vector3D &GetNowPosition()
{
	return g_nowPosition;
}
