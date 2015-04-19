
#pragma once

#include "Tools/IMapObjectDataForTool.h"

IMapObjectDataForTool *SetNowSelectedObject( IMapObjectDataForTool *selected_object );
IMapObjectDataForTool *GetNowSelectedObjectData();
bool GetNowSelectedObjectDepth( int *depth );
