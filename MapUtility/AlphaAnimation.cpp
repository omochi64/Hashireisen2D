
#include "AlphaAnimation.h"

AlphaAnimation::AlphaAnimation()
{
	m_now_alpha = m_alpha_speed = 0.0f;
	m_start_alpha = m_end_alpha = 0;
	m_is_loop = true;
}

AlphaAnimation::~AlphaAnimation()
{
}

// アニメーション値設定
void AlphaAnimation::SetStartAlpha( int alpha )
{
	m_start_alpha = alpha;
}

void AlphaAnimation::SetEndAlpha( int alpha )
{
	m_end_alpha = alpha;
}

void AlphaAnimation::SetAlphaSpeed( float speed )
{
	m_alpha_speed = speed;
}
void AlphaAnimation::SetAlphaSpeedByDuration( int animation_frame )
{
	m_alpha_speed = (m_end_alpha-m_start_alpha)/((float)animation_frame);
}
void AlphaAnimation::SetLoop( bool is_loop )
{
	m_is_loop = is_loop;
}

// アニメーション開始
void AlphaAnimation::Start()
{
	m_now_alpha = m_start_alpha;
}
// リセット(結局開始と同じ)
void AlphaAnimation::Reset()
{
	m_now_alpha = m_start_alpha;
}

// アニメーションアップデート
bool AlphaAnimation::UpdateAnimation()
{
	m_now_alpha += m_alpha_speed;
	if( ( m_now_alpha >= m_end_alpha && m_end_alpha>m_start_alpha ) ||
		( m_now_alpha <= m_end_alpha && m_end_alpha<=m_start_alpha ))
	{
		m_now_alpha = m_end_alpha;
		if( m_is_loop )
		{
			m_alpha_speed = -m_alpha_speed;
		}
		else
		{
			return false;
		}
	}
	if( ( m_now_alpha <= m_start_alpha && m_end_alpha>m_start_alpha ) ||
		( m_now_alpha >= m_start_alpha && m_end_alpha<=m_start_alpha ) )
	{
		m_now_alpha = m_start_alpha;
		if( m_is_loop )
		{
			m_alpha_speed = -m_alpha_speed;
		}
		else
		{
			return false;
		}
	}

	return true;
}

// 取得
int AlphaAnimation::GetNowAlpha()
{
	return (int)m_now_alpha;
}