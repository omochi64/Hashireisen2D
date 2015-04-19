
#pragma once

#ifdef MAIN_WIN_DEFINE
#define EXTERN_FOR_CAMERA	/*extern*/
// Šî–{‚Æ‚È‚éƒJƒƒ‰‚ÌŒü‚«
#else
#define EXTERN_FOR_CAMERA	extern
#endif

#include "Selene.h"
using namespace Selene;
/*
EXTERN_FOR_CAMERA	float			g_dist;
EXTERN_FOR_CAMERA	float	g_camRotationY;
EXTERN_FOR_CAMERA	float	g_camRotationXZ;
EXTERN_FOR_CAMERA	Math::Vector3D	g_camAt;
EXTERN_FOR_CAMERA	Math::Vector3D	g_camUp;

*/