
#pragma once

#include "System/IMapObjectData.h"

// マップツール用のマップデータ
class IMapObjectDataForTool:public IMapObjectData
{
public:
	int	m_id;
	IMapObjectDataForTool();
	virtual ~IMapObjectDataForTool();
	virtual void Copy( IMapObjectData *copy_to_for_tool_object );
	virtual bool Compare( IMapObjectData *for_tool_object );
};
