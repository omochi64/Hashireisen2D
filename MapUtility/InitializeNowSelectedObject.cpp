
#include "CommonForVisual.h"


void InitializeNowSelectedObject()
{
	// 現在選択されているオブジェクト初期化
	g_nowSelectedObject = new ModelWithAlphaAnimation();

	AlphaAnimation &animation = g_nowSelectedObject->GetAlphaAnimation();

	animation.SetStartAlpha( 128 );
	animation.SetEndAlpha( 128 );
	animation.SetAlphaSpeedByDuration( 60 );
	animation.Start();

	g_nowSelectedObject->SetVertexColor( CColor(0xff,0,0) );

	// 現在移動中のオブジェクト初期化
	g_nowMovingObject = new ModelWithAlphaAnimation();

	AlphaAnimation &animation2 = g_nowMovingObject->GetAlphaAnimation();

	animation2.SetStartAlpha( 32 );
	animation2.SetEndAlpha( 32 );
	animation2.SetAlphaSpeedByDuration( 60 );
	animation2.Start();

	g_nowMovingObject->SetVertexColor( CColor(0x00,0xff,0) );
}
