
#pragma once

IMapObjectDataForTool *SetNowMovingObject( IMapObjectDataForTool *next_moving_object );
IMapObjectDataForTool *GetNowMovingObject();
void SetNowMovingObjectPosition( const Math::Vector3D &pos );
