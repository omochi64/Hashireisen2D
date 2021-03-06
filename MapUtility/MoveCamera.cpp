
#include "Selene.h"
using namespace Selene;

#include "CommonForCamera.h"
#include "UpdateCamera.h"
#include "MoveCamera.h"

void MoveCamera( Peripheral::IMouse *pMouse )
{
	// x,z軸合成回転軸を求める
	Math::Vector3D n = Camera::g_camBasicDir;
	n.RotationY( DEG_TO_ANGLE(Camera::g_camRotationY) );
	Math::Vector3D rotation_axis = Math::Vector3D( n.z , 0 , -n.x );

	// 移動量をこの合成軸周りに回転させる
	Math::Vector3D move( -pMouse->GetMoveX()*Camera::g_dist/800 , pMouse->GetMoveY()*Camera::g_dist/800 , 0 );
	move.RotationAxis( DEG_TO_ANGLE(Camera::g_camRotationXZ) , rotation_axis );

	// さらにY周りに回転させる
	move.RotationY( DEG_TO_ANGLE(Camera::g_camRotationY) );

	Camera::g_camAt += move;
}
