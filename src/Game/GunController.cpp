
#include "GunController.h"
/*
class GunController
{
	MultipleAnimation	*m_gun_object;
	Bullet				*m_bullet;
	TCHAR				m_gun_name[256];
	int					m_rest_bullet_num;
	int					m_max_bullet_num;

public:

	// ÉRÉìÉXÉgÉâÉNÉ^ÅAÉfÉXÉgÉâÉNÉ^
	GunController();
	~GunController();

	// èeíeêîëÄçÏ
	void SupplyBullet( int num );
	void DecreaseBullet( int num=1 );
	int GetBulletNum( void );

	// ëÆê´ê›íËä÷åW
	void SetGunObject( MultipleAnimation *gun );
	void setBullet( Bullet *bullet );
	void Initialize( LPCTSTR gun_name , int iniBulletNum , int maxBulletNum );

	// íeÇåÇÇ¬
	Bullet *ShootTheBullet( void );
};
*/

GunController::GunController()
{
	m_gun_object = NULL;
	m_bullet = NULL;
	m_gun_name[0] = TEXT('\0');
	m_rest_bullet_num = 0;
	m_max_bullet_num = 0;
}

GunController::~GunController()
{
	SAFE_RELEASE(m_gun_object);
	SAFE_RELEASE(m_bullet);
}

// èeíeêîëÄçÏ

void GunController::SupplyBullet( int num )
{
	// íeêîñ≥êßå¿
	if( m_rest_bullet_num == -1 )
	{
		return;
	}

	m_rest_bullet_num += num;
	if( m_rest_bullet_num > m_max_bullet_num )
	{
		m_rest_bullet_num = m_max_bullet_num;
	}
}

void GunController::DecreaseBullet( int num )
{
	// íeêîñ≥êßå¿
	if( m_rest_bullet_num == -1 )
	{
		return;
	}

	m_rest_bullet_num -= num;
	if( m_rest_bullet_num < 0 )
	{
		m_rest_bullet_num = 0;
	}
}

int GunController::GetBulletNum( void )
{
	return m_rest_bullet_num;
}

// ëÆê´ê›íËä÷åW
void GunController::SetGunObject( MultipleAnimation *gun , bool release_old )
{
	if( release_old )
	{
		SAFE_RELEASE(m_gun_object);
	}
	m_gun_object = gun;
}

void GunController::SetBullet( Bullet *bullet , bool release_old )
{
	if( release_old )
	{
		SAFE_RELEASE(m_bullet);
	}
	m_bullet = bullet;
}

void GunController::Initialize( LPCTSTR gun_name , int iniBulletNum , int maxBulletNum )
{
	wsprintf( m_gun_name , gun_name );
	m_rest_bullet_num = iniBulletNum;
	m_max_bullet_num = maxBulletNum;
}

// ëÆê´ÉAÉNÉZÉX
MultipleAnimation *GunController::GetGunObject()
{
	return m_gun_object;
}

Bullet *GunController::GetBullet()
{
	return m_bullet;
}
LPCTSTR GunController::GetGunName()
{
	return m_gun_name;
}


// íeÇåÇÇ¬
Bullet *GunController::ShootTheBullet( void )
{
	if( m_rest_bullet_num == 0 )
	{
		return NULL;
	}
	DecreaseBullet();
	return m_bullet->Clone();
}
