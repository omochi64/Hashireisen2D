
#include "Tools/IMapObjectDataForTool.h"

// �R���X�g���N�^�A�f�X�g���N�^
IMapObjectDataForTool::IMapObjectDataForTool():IMapObjectData()
{
	m_id = 0;
}
IMapObjectDataForTool::~IMapObjectDataForTool()
{
}

// ���ҁ[
void IMapObjectDataForTool::Copy( IMapObjectData *copy_to_for_tool_object )
{
	IMapObjectData::Copy( copy_to_for_tool_object );
	((IMapObjectDataForTool *)copy_to_for_tool_object)->m_id = m_id;
}

// ����؂�
bool IMapObjectDataForTool::Compare( IMapObjectData *for_tool_object )
{
	IMapObjectDataForTool *d = (IMapObjectDataForTool *)for_tool_object;
	if( d->m_id == m_id && d->m_pActor == m_pActor )
	{
		return true;
	}

	return false;
}
