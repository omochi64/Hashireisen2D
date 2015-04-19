
#include "Game/BackgroundController.h"

/*
// 2D画像の管理．IPrimitive2Dを使った描画
class BackgroundController
{
	Renderer::ITexture *m_pTexture;
	Renderer::Object::IPrimitive2D	*m_pPrimitive;

	Math::Vector2D	m_firstPos,m_lastPos;
	Math::Vector2D	m_nowPos;
	CColor			m_firstColor,m_lastColor;
	CColor			m_nowColor;
	DWORD			m_posAnimationTime,m_colorAnimationTime;
	DWORD			m_posAnimationStartTime,m_colorAnimationStartTime;
	DWORD			m_posAnimationStopTime,m_colorAnimationStopTime;

	bool			m_did_start_pos_animation;
	bool			m_did_start_color_animation;

	Renderer::SPrimitiveVertex2D	m_vertex[2];
public:

	void StartPosAnimation( DWORD now_time );
	void StartColorAnimation( DWORD now_time );
	void StopPosAnimation( DWORD now_time );
	void StopColorAnimation( DWORD now_time );
	void ResumePosAnimation( DWORD now_time );
	void ResumeColorAnimation( DWORD now_time );

	bool UpdatePosAnimation( DWORD now_time );
	bool UpdateColorAnimation( DWORD now_time );

	void Draw();

	void Release();
};
*/

BackgroundController::BackgroundController(Renderer::IRender *pRender)
{
	m_pPrimitive = pRender->CreatePrimitive2D( 1024 );

	m_firstPos = Math::Vector2D(0,0);
	m_speedPos = Math::Vector2D(0,0);
	m_nowPos = Math::Vector2D(0,0);
	m_firstColor = Math::Vector4D(255,255,255,255);
	m_speedColor = Math::Vector4D(0,0,0,0);
	m_nowColor = Math::Vector4D(255,255,255,255);
	m_posAnimationPastFrame = 0 ,m_colorAnimationPastFrame = 0;

	m_pTexture = NULL;
	m_size = Math::Point2DI(0,0);

	m_did_start_pos_animation = false;
	m_did_start_color_animation = false;

	m_vertex[0].v1.Tex[0] = m_vertex[1].v3.Tex[0] = Math::Vector2D(0,0);
	m_vertex[1].v1.Tex[0] = m_vertex[0].v3.Tex[0] = Math::Vector2D(1.0f,1.0f);
	m_vertex[0].v2.Tex[0] = Math::Vector2D(1.0f,0.0f);
	m_vertex[1].v2.Tex[0] = Math::Vector2D(0.0f,1.0f);

	m_z = 0.0f;
}

BackgroundController::~BackgroundController()
{
	Release();
}

// テクスチャ関連
void BackgroundController::SetTexture( Renderer::ITexture *pTexture , Math::Vector2D uv_leftup , Math::Vector2D uv_rightbottom )
{
	m_pTexture = pTexture;
	if( pTexture )
	{
		m_size = pTexture->GetSize();
		m_size.x *= uv_rightbottom.x-uv_leftup.x;
		m_size.y *= uv_rightbottom.y-uv_leftup.y;
	}

	m_vertex[0].v1.Tex[0] = m_vertex[1].v3.Tex[0] = uv_leftup;
	m_vertex[1].v1.Tex[0] = m_vertex[0].v3.Tex[0] = uv_rightbottom;
	m_vertex[0].v2.Tex[0] = Math::Vector2D(uv_rightbottom.x,uv_leftup.y);
	m_vertex[1].v2.Tex[0] = Math::Vector2D(uv_leftup.x,uv_rightbottom.y);
}
Renderer::ITexture *BackgroundController::GetTexture()
{
	return m_pTexture;
}

Renderer::Object::IPrimitive2D *BackgroundController::GetPrimitive()
{
	return m_pPrimitive;
}

// 直接サイズ設定
void BackgroundController::SetSize( Math::Point2DI size )
{
	m_size = size;
}
const Math::Point2DI &BackgroundController::GetSize()
{
	return m_size;
}

void BackgroundController::SetZ( float z )
{
	m_z = z;
}

// 位置設定
void BackgroundController::SetPosWithoutAnimation( Math::Vector2D pos )
{
	m_nowPos = m_firstPos = pos;
}

// カラー設定
void BackgroundController::SetColorWithoutAnimation( CColor color )
{
	m_nowColor = m_firstColor = Math::Vector4D(color.r,color.g,color.b,color.a);
}

// 位置アニメーション設定
void BackgroundController::SetPosAnimation1( Math::Vector2D firstPos , Math::Vector2D lastPos , DWORD duration )
{
	SetPosAnimation2( firstPos , (lastPos-firstPos)/duration , duration );
}

void BackgroundController::SetPosAnimation2( Math::Vector2D firstPos , Math::Vector2D speed , DWORD duration )
{
	m_firstPos = firstPos;
	m_speedPos = speed;
	m_posAnimationDuration = duration;
}

// カラーアニメーション設定
void BackgroundController::SetColorAnimation1( CColor firstColor , CColor lastColor , DWORD duration )
{
	Math::Vector4D speed( lastColor.r-firstColor.r , lastColor.g-firstColor.g , lastColor.b-firstColor.b , lastColor.a-firstColor.a );
	speed /= duration;
	SetColorAnimation2( firstColor , speed , duration );
}

void BackgroundController::SetColorAnimation2( CColor firstColor , Math::Vector4D speed , DWORD duration )
{
	m_firstColor = Math::Vector4D( firstColor.r , firstColor.g , firstColor.b , firstColor.a );
	m_speedColor = speed;
	m_colorAnimationDuration = duration;
}

// 各種アニメーションスタート
void BackgroundController::StartPosAnimation()
{
	m_did_start_pos_animation = true;
	m_posAnimationPastFrame = 0;
}

void BackgroundController::StartColorAnimation()
{
	m_did_start_color_animation = true;
	m_colorAnimationPastFrame = 0;
}

// 各種アニメーションストップ
void BackgroundController::StopPosAnimation()
{
	if( !m_did_start_pos_animation )
	{
		return;
	}
	m_did_start_pos_animation = false;
}

void BackgroundController::StopColorAnimation()
{
	if( !m_did_start_color_animation )
	{
		return;
	}
	m_did_start_color_animation = false;
}

// 各種アニメーション再開
void BackgroundController::ResumePosAnimation()
{
	if( m_did_start_pos_animation )
	{
		return;
	}
	m_did_start_pos_animation = true;
}

void BackgroundController::ResumeColorAnimation()
{
	if( m_did_start_color_animation )
	{
		return;
	}
	m_did_start_color_animation = true;
}

// 各種アニメーションを即座に最後まで回す
void BackgroundController::EndPosAnimationAtOnce()
{
	m_did_start_pos_animation = false;
	m_nowPos = m_firstPos + m_speedPos*m_posAnimationDuration;
}

void BackgroundController::EndColorAnimationAtOnce()
{
	m_did_start_color_animation = false;
	m_nowColor = m_firstColor + m_speedColor*m_colorAnimationDuration;
}


// 各種アニメーションアップデート
bool BackgroundController::UpdatePosAnimation()
{
	if( !m_did_start_pos_animation )
	{
		return false;
	}

	m_posAnimationPastFrame++;

	if( m_posAnimationPastFrame >= m_posAnimationDuration )
	{
		m_nowPos = m_firstPos + m_speedPos*m_posAnimationDuration;
		m_did_start_pos_animation = false;
		return false;
	}
	else
	{
		m_nowPos = m_firstPos + m_speedPos*m_posAnimationPastFrame;
		return true;
	}
}

bool BackgroundController::UpdateColorAnimation()
{
	if( !m_did_start_color_animation )
	{
		return false;
	}

	m_colorAnimationPastFrame++;

	if( m_colorAnimationPastFrame >= m_colorAnimationDuration )
	{
		m_nowColor = m_firstColor + m_speedColor*m_colorAnimationDuration;
		m_did_start_color_animation = false;
		return false;
	}
	else
	{
		m_nowColor = m_firstColor + m_speedColor*m_colorAnimationPastFrame;
		return true;
	}
}

// 描画
void BackgroundController::Draw()
{
	if( !m_pPrimitive )
	{
		return;
	}

	int i;
	for( i=0 ; i<2 ; i++ )
	{
		m_vertex[i].v1.Col = CColor( m_nowColor.x , m_nowColor.y , m_nowColor.z , m_nowColor.w );
		m_vertex[i].v2.Col = m_vertex[i].v3.Col = m_vertex[i].v1.Col;
	}

	m_vertex[0].v1.Pos = m_vertex[1].v3.Pos = Math::Vector3D( m_nowPos.x , m_nowPos.y , m_z );
	m_vertex[1].v1.Pos = m_vertex[0].v3.Pos = Math::Vector3D( m_nowPos.x + m_size.x , m_nowPos.y + m_size.y , m_z );
	m_vertex[0].v2.Pos = Math::Vector3D( m_nowPos.x + m_size.x , m_nowPos.y , m_z );
	m_vertex[1].v2.Pos = Math::Vector3D( m_nowPos.x , m_nowPos.y + m_size.y , m_z );

	m_pPrimitive->Begin();

	m_pPrimitive->Push( m_vertex , 2 );

	m_pPrimitive->End();

}

// 解放
void BackgroundController::Release()
{
	SAFE_RELEASE(m_pPrimitive);

	m_firstPos = Math::Vector2D(0,0);
	m_speedPos = Math::Vector2D(0,0);
	m_nowPos = Math::Vector2D(0,0);
	m_firstColor = Math::Vector4D(0,0,0,0);
	m_speedColor = Math::Vector4D(0,0,0,0);
	m_nowColor = Math::Vector4D(0,0,0,0);
	m_posAnimationPastFrame = 0 ,m_colorAnimationPastFrame = 0;

	m_pTexture = NULL;
	m_size = Math::Point2DI(0,0);

	m_did_start_pos_animation = false;
	m_did_start_color_animation = false;
}
