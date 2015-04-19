
#pragma once

#ifdef MAIN_WIN_DEFINE
#define EXTERN_FOR_VISUAL	/*extern*/
#else
#define EXTERN_FOR_VISUAL extern
#endif

#include "BoxActorWithAlphaAnimation.h"
#include "SquarePolygonActorWithAlphaAnimation.h"
#include "ModelWithAlphaAnimation.h"


EXTERN_FOR_VISUAL BoxActorWithAlphaAnimation	*g_nowPoint;
EXTERN_FOR_VISUAL SquarePolygonActorWithAlphaAnimation *g_nowPlane;
EXTERN_FOR_VISUAL ModelWithAlphaAnimation	*g_nowSelectedObject;
EXTERN_FOR_VISUAL ModelWithAlphaAnimation	*g_nowMovingObject;
