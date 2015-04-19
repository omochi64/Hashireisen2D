
#include "Selene.h"
using namespace Selene;

#include "CommonForCamera.h"
#include "UpdateCamera.h"
#include "MoveCamera.h"

void MoveCamera( Peripheral::IMouse *pMouse )
{
	// x,zŽ²‡¬‰ñ“]Ž²‚ð‹‚ß‚é
	Math::Vector3D n = Camera::g_camBasicDir;
	n.RotationY( DEG_TO_ANGLE(Camera::g_camRotationY) );
	Math::Vector3D rotation_axis = Math::Vector3D( n.z , 0 , -n.x );

	// ˆÚ“®—Ê‚ð‚±‚Ì‡¬Ž²Žü‚è‚É‰ñ“]‚³‚¹‚é
	Math::Vector3D move( -pMouse->GetMoveX()*Camera::g_dist/800 , pMouse->GetMoveY()*Camera::g_dist/800 , 0 );
	move.RotationAxis( DEG_TO_ANGLE(Camera::g_camRotationXZ) , rotation_axis );

	// ‚³‚ç‚ÉYŽü‚è‚É‰ñ“]‚³‚¹‚é
	move.RotationY( DEG_TO_ANGLE(Camera::g_camRotationY) );

	Camera::g_camAt += move;
}
