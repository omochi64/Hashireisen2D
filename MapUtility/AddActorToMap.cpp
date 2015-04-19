
#include "Selene.h"
#include "CommonForSynchronize.h"
#include "CommonForData.h"

using namespace Selene;

#include "AddActorToMap.h"
#include "SetNowSelectedObject.h"
#include "SetNowPosition.h"

bool AddActorToMap( IMapObjectDataForTool *pData )
{
	// �}�b�v�f�[�^�̃N���e�B�J���Z�N�V�����ɓ���
	EnterCriticalSection( &g_hMapRouteCS );

	// �ǉ�
	pData->m_pActor->TransformUpdateCollisionOnly();
	bool ret_v = g_mapRoute->AddObject( pData );

	LeaveCriticalSection( &g_hMapRouteCS );

	if( ret_v )
	{
		SetNowSelectedObject( pData );
	}

	return ret_v;
}
