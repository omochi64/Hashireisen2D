
#ifdef MAIN_WIN_DEFINE
#define EXTERN_FOR_DATA	/*extern*/
#else
#define EXTERN_FOR_DATA extern
#endif

#include "Tools/IOctreeMapNodeForTool.h"

EXTERN_FOR_DATA	IOctreeMapNodeForTool	*g_mapRoute;
EXTERN_FOR_DATA Math::Vector3D g_nowPosition;
EXTERN_FOR_DATA IMapObjectDataForTool	*g_nowSelectedObjectData;
EXTERN_FOR_DATA IMapObjectDataForTool	*g_nowMovingObjectData;
