
#include "CommonForData.h"
#include "CommonForVisual.h"
#include "InitializeForNowPoint.h"

void InitializeForNowPoint( void )
{
	// Œ»Ý‚Ìƒ|ƒCƒ“ƒg‚ðŽ¦‚·PointActor‰Šú‰»
	g_nowPoint = new BoxActorWithAlphaAnimation();
	g_nowPosition = Math::Vector3D(0,0,0);

	AlphaAnimation &animation = g_nowPoint->GetAlphaAnimation();

	animation.SetStartAlpha(0);
	animation.SetEndAlpha(0xff);
	animation.SetAlphaSpeedByDuration(60);
	animation.SetLoop(true);
	animation.Start();

	g_nowPoint->SetVertexColor( CColor(0xff,0x00,0xff) );
}
