
#include "Selene.h"
using namespace Selene;

#include "CommonForMainWin.h"
#include "CommonForData.h"
#include "CommonForCamera.h"
#include "CheckMouseActivity.h"
#include "CheckSelectObject.h"
#include "RotateCamera.h"
#include "MoveCamera.h"
#include "UpdateCamera.h"

#include "SetNowPosition.h"
#include "UpdateNowPositionInfo.h"



void ResponseToMouseState( Peripheral::IMouse *pMouse , const Math::Vector3D &cross_point , const Math::Vector3D &cam_pos , Scene::ISceneManager *pSceneMgr );

void CheckMouseActivity( Peripheral::IMouse *pMouse , ICore *pCore , Scene::ISceneManager *pSceneMgr , Renderer::IRender *pRender )
{
	// �}�E�X���W on Screen���}�E�X���W on World�ɕϊ�
	Sint32 x = pMouse->GetPosX();
	Sint32 y = pMouse->GetPosY();
	Math::Vector3D sc = pSceneMgr->TransformToScreen( g_nowPosition );
	sc.x = x;
	sc.y = y;
	Math::Vector3D w = pSceneMgr->TransformFromScreen( sc );

	// �J��������I��n�_�փ��C���Ƃ΂������ݕ��ʂƂ̌�_��V���ȑI��_�Ƃ���
	Scene::ICamera *pCamera = pSceneMgr->GetCamera();
	const Math::Vector3D &cam_pos =pCamera->Position();
	Math::Vector3D ray = cam_pos-w;

	// ��_���o
	Math::Vector3D cross_point;
	if( Camera::g_camRotationXZ != 0 )
	{
		cross_point.x = (g_nowPosition.y-cam_pos.y)*ray.x/ray.y+cam_pos.x;
		cross_point.y = (g_nowPosition.y-cam_pos.y)+cam_pos.y;
		cross_point.z = (g_nowPosition.y-cam_pos.y)*ray.z/ray.y+cam_pos.z;
	}
	else
	{
		cross_point.x = w.x;
		cross_point.y = 0;
		cross_point.z = w.z;
	}

	pRender->DebugPrint( Math::Point2DI(100,100) , CColor(0xff,0xff,0xff) , "MousePos:x = %f , y = %f , z = %f" , cross_point.x , cross_point.y , cross_point.z );

	// ���݃E�B���h�E����A�N�e�B�u�Ȃ�ړ����Ȃ�
	if( GetActiveWindow() != pCore->GetWindowHandle() )
	{
		return;
	}

	ResponseToMouseState( pMouse , cross_point , cam_pos , pSceneMgr );

}

void ResponseToMouseState( Peripheral::IMouse *pMouse , const Math::Vector3D &cross_point , const Math::Vector3D &cam_pos , Scene::ISceneManager *pSceneMgr )
{
	// ���V���O���N���b�N�F���̈ʒu�Ɍ��݈ʒu���ڂ�
	if( pMouse->GetClickL() )
	{
		int state = pMouse->GetStateL();
		if( pMouse->GetClickR() )
		{
			// ���_�ړ�
			MoveCamera( pMouse );
		}
		else if( state != MOUSE_HOLD )
		{
			SetNowPosition(cross_point);
			UpdateNowPositionInfo( g_hDialog );
		}
	}
	// �E�N���b�N
	else if( pMouse->GetClickR() )
	{
		int state = pMouse->GetStateR();
		if( state == MOUSE_HOLD )
		{
			// ���_��]
			RotateCamera( pMouse );
		}
	}

	// �}�E�X�̃N���b�N���o
	// ���_�u���N���b�N�F���C���Ƃ΂��ăI�u�W�F�N�g�̑I��
	if( pMouse->GetDoubleClickL() )
	{
		CheckSelectObject( cross_point , cam_pos , pSceneMgr );
	}

	// �Z���^�[�N���b�N
//	if( pMouse->GetClickW() )
	{
		Camera::g_dist -= pMouse->GetMoveW()/20;
		if( Camera::g_dist <= 0.0f )
		{
			Camera::g_dist +=pMouse->GetMoveW()/20;
		}
	}
}
