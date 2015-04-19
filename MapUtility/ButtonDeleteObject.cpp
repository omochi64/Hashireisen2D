
#include <windows.h>
#include "ButtonFunctions.h"
#include "DeleteObjectFromMap.h"
#include "SetNowSelectedObject.h"
#include "UpdateNowSelectedObjectInfo.h"

void ButtonDeleteObject( HWND hWnd )
{
	IMapObjectDataForTool *now_object = GetNowSelectedObjectData();

	if( now_object )
	{
		if( !DeleteObjectFromMap( now_object ) )
		{
			MessageBox( hWnd , "çÌèúÇ…é∏îsÇµÇ‹ÇµÇΩ" , NULL , MB_OK|MB_ICONERROR );
			return;
		}
		SetNowSelectedObject( NULL );
		SAFE_RELEASE(now_object);
		UpdateNowSelectedObjectInfo( hWnd );
	}
}
