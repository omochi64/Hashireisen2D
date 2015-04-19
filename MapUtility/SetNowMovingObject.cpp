
#include "common.h"
#include "CommonForVisual.h"
#include "CommonForData.h"
#include "SetNowMovingObject.h"

IMapObjectDataForTool *SetNowMovingObject( IMapObjectDataForTool *next_moving_object )
{
	IMapObjectDataForTool *before = g_nowMovingObjectData;
	g_nowMovingObject->SetVertexColor( CColor(0xff,0xff,0xff) );
	g_nowMovingObject->UpdateActor( g_pRender , g_pSceneMgr );
	if( next_moving_object )
	{
		g_nowMovingObject->AttachActor( next_moving_object->m_pActor , false );
	}
	else
	{
		g_nowMovingObject->AttachActor( NULL , false );
	}
	g_nowMovingObject->SetVertexColor( CColor(0x00,0xff,0x0) );
	g_nowMovingObjectData = next_moving_object;

	return before;
}

IMapObjectDataForTool *GetNowMovingObject()
{
	return g_nowMovingObjectData;
}

void SetNowMovingObjectPosition( const Math::Vector3D &pos )
{
	if( g_nowMovingObjectData && g_nowMovingObjectData->m_pActor )
	{
		g_nowMovingObjectData->m_pActor->Translation( pos );
	}
}
