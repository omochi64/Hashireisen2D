
#include "CommonForVisual.h"
#include "CommonForCamera.h"
#include "UpdateCamera.h"
#include "CommonForData.h"

#include "UpdateVisualActors.h"

void UpdateVisualActors( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	// ���݈ʒu�\��
	g_nowPoint->UpdateAlphaAnimation();

	g_nowPoint->SetWidth( Camera::g_dist/96 );
	g_nowPoint->SetHeight( Camera::g_dist/96 );
	g_nowPoint->SetDepth( Camera::g_dist/96 );
	g_nowPoint->SetPosition( g_nowPosition );

	g_nowPoint->UpdateActor( pRender , pSceneMgr );

	// ���݂̑I�𕽖ʕ\��
	Math::Vector3D poses[4] = {
		Math::Vector3D( -Camera::g_dist/2+Camera::g_camAt.x , g_nowPosition.y , -Camera::g_dist/2+Camera::g_camAt.z ) , 
		Math::Vector3D( -Camera::g_dist/2+Camera::g_camAt.x , g_nowPosition.y , +Camera::g_dist/2+Camera::g_camAt.z ) , 
		Math::Vector3D( +Camera::g_dist/2+Camera::g_camAt.x , g_nowPosition.y , +Camera::g_dist/2+Camera::g_camAt.z ) , 
		Math::Vector3D( +Camera::g_dist/2+Camera::g_camAt.x , g_nowPosition.y , -Camera::g_dist/2+Camera::g_camAt.z )
	};
	g_nowPlane->SetVertexPosition( poses );
	g_nowPlane->UpdateAlphaAnimation();

	g_nowPlane->UpdateActor( pRender , pSceneMgr );

	// �I���I�u�W�F�N�g
	g_nowSelectedObject->UpdateAlphaAnimation();
	g_nowSelectedObject->UpdateActor( pRender , pSceneMgr );

	// �ړ��I�u�W�F�N�g
	g_nowMovingObject->UpdateAlphaAnimation();
	g_nowMovingObject->UpdateActor( pRender , pSceneMgr );
}
