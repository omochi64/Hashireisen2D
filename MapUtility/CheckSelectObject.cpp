
#include <math.h>
#include "CommonForMainWin.h"
#include "CommonForData.h"
#include "CheckSelectObject.h"
#include "SetNowSelectedObject.h"
#include "SetNowPosition.h"
#include "UpdateNowPositionInfo.h"
#include "UpdateNowSelectedObjectInfo.h"
#include "SetNowMovingObject.h"
#include "GetCopyActorByID.h"
#include "GetObjectForCopyByID.h"

// �I�u�W�F�N�g�I���`�F�b�N

// �������o�O�b�Ă�L��

//Collision::CLine3D selec_line;

void CheckSelectObject( const Math::Vector3D &mouse_pos , const Math::Vector3D &cam_pos , Scene::ISceneManager *pSceneMgr )
{
	
	// cam_pos��mouse_pos�Ń��C���Ƃ΂��Bmap���Ɍ�������I�u�W�F�N�g���������`�F�b�N
	Math::Vector3D ray = mouse_pos-cam_pos;
	ray.Normalize();

	// ���C���ő���܂ŐL�΂�
	const Math::Vector3D &minXYZ = g_mapRoute->GetMinXYZ();
	const Math::Vector3D &maxXYZ = g_mapRoute->GetMaxXYZ();

	Math::Vector3D diff = maxXYZ-minXYZ;

	float max_length = (float)sqrt( diff.x*diff.x+diff.y*diff.y+diff.z*diff.z );

	Collision::CLine3D max_ray( cam_pos , cam_pos+ray*max_length );
//	selec_line = max_ray;

	std::vector<Renderer::SCollisionResult>	result;
	std::vector<IMapObjectData *>		result_object;

	if( g_mapRoute->HitCheckByRay( max_ray , result , result_object ) )
	{
		// �ł��������߂������I�u�W�F�N�g��I��
		int i,size = result.size();
		int nearest = 0;
		float nearest_dist = (result.at(0).vHitPosition-cam_pos).Length();
		float dist;

		for( i=1 ; i<size ; i++ )
		{
			Renderer::SCollisionResult &sr = result.at(i);
			dist = (sr.vHitPosition-cam_pos).Length();
			if( dist < nearest_dist )
			{
				nearest_dist = dist;
				nearest = i;
			}
		}

		// �I��
		IMapObjectDataForTool *mod = (IMapObjectDataForTool *)result_object.at(nearest);

		Math::Vector3D now_new_pos;
		mod->m_pActor->GetPosition( now_new_pos );
		SetNowSelectedObject( mod );
		SetNowPosition( now_new_pos );
		UpdateNowPositionInfo( g_hDialog );
		UpdateNowSelectedObjectInfo( g_hDialog );

		// ���݈ړ����̃I�u�W�F�N�g�����ύX
		IMapObjectDataForTool *moving = GetObjectForCopyByID(mod->m_id);
		SetNowMovingObject( moving );
		SetNowMovingObjectPosition( now_new_pos );
	}
	else
	{
		SetNowSelectedObject( NULL );
		UpdateNowSelectedObjectInfo( g_hDialog );

		// ���݈ړ����I�u�W�F�N�g����ɂ���
		SetNowMovingObject( NULL );
	}
}
