
#include "common.h"
#include "CommonForVisual.h"
#include "CommonForData.h"
#include "SetNowSelectedObject.h"

IMapObjectDataForTool *SetNowSelectedObject( IMapObjectDataForTool *selected_object )
{
	IMapObjectDataForTool *before = g_nowSelectedObjectData;
	g_nowSelectedObject->SetVertexColor( CColor(0xff,0xff,0xff) );
	g_nowSelectedObject->UpdateActor( g_pRender , g_pSceneMgr );
	if( selected_object )
	{
		g_nowSelectedObject->AttachActor( selected_object->m_pActor , false );
	}
	else
	{
		g_nowSelectedObject->AttachActor( NULL , false );
	}
	g_nowSelectedObject->SetVertexColor( CColor(0xff,0x00,0x0) );
	g_nowSelectedObjectData = selected_object;

	return before;
}

IMapObjectDataForTool *GetNowSelectedObjectData()
{
	return g_nowSelectedObjectData;
}

bool GetNowSelectedObjectDepth( int *depth )
{
	if( g_nowSelectedObjectData )
	{
		return g_mapRoute->GetObjectDepth( g_nowSelectedObjectData , depth );
	}
	return false;
}
