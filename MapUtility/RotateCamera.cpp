
#include "Selene.h"
using namespace Selene;

#include "CommonForCamera.h"
#include "UpdateCamera.h"

void RotateCamera( Peripheral::IMouse *pMouse )
{
	int deg_x = pMouse->GetMoveX();
	int deg_y = pMouse->GetMoveY();

	Camera::g_camRotationY += deg_x;
	Camera::g_camRotationXZ -= deg_y;
}
