
#include "Tools/IMapObjectDataForTool.h"

// コンストラクタ、デストラクタ
IMapObjectDataForTool::IMapObjectDataForTool():IMapObjectData()
{
	m_id = 0;
}
IMapObjectDataForTool::~IMapObjectDataForTool()
{
}

// こぴー
void IMapObjectDataForTool::Copy( IMapObjectData *copy_to_for_tool_object )
{
	IMapObjectData::Copy( copy_to_for_tool_object );
	((IMapObjectDataForTool *)copy_to_for_tool_object)->m_id = m_id;
}

// こんぺあ
bool IMapObjectDataForTool::Compare( IMapObjectData *for_tool_object )
{
	IMapObjectDataForTool *d = (IMapObjectDataForTool *)for_tool_object;
	if( d->m_id == m_id && d->m_pActor == m_pActor )
	{
		return true;
	}

	return false;
}
