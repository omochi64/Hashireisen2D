
#include "CommonForVisual.h"
#include "InitializeNowPlane.h"

void InitializeNowPlane( void )
{
	g_nowPlane = new SquarePolygonActorWithAlphaAnimation();

	g_nowPlane->SetVertexColor( CColor( 0xff , 0xff , 0 ) );
	AlphaAnimation &animation = g_nowPlane->GetAlphaAnimation();
	animation.SetStartAlpha( 32 );
	animation.SetEndAlpha( 32 );
	animation.SetAlphaSpeedByDuration( 1000 );
	animation.Start();

}
