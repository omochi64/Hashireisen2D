
#pragma once

// α値のアニメーションを管理
class AlphaAnimation
{
protected:
	float	m_now_alpha;
	float	m_alpha_speed;
	int	m_start_alpha,m_end_alpha;
	bool	m_is_loop;

public:
	AlphaAnimation( void );
	virtual ~AlphaAnimation( void );

	void SetStartAlpha( int alpha );
	void SetEndAlpha( int alpha );
	void SetAlphaSpeed( float speed );
	void SetAlphaSpeedByDuration( int animation_frame );
	void SetLoop( bool is_loop );
	void Start();
	void Reset();
	
	bool UpdateAnimation();
	int	GetNowAlpha();
};
