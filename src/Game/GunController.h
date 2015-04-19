
#pragma once

#include "Game/Objects.h"
#include "Bullet.h"

class GunController
{
	MultipleAnimation	*m_gun_object;
	Bullet				*m_bullet;
	TCHAR				m_gun_name[256];
	int					m_rest_bullet_num;
	int					m_max_bullet_num;

public:

	// コンストラクタ、デストラクタ
	GunController();
	~GunController();

	// 銃弾数操作
	void SupplyBullet( int num );
	void DecreaseBullet( int num=1 );
	int GetBulletNum( void );

	// 属性設定関係
	void SetGunObject( MultipleAnimation *gun , bool release_old = true );
	void SetBullet( Bullet *bullet , bool release_old = true );
	void Initialize( LPCTSTR gun_name , int iniBulletNum , int maxBulletNum );

	// 属性アクセス
	MultipleAnimation *GetGunObject();
	Bullet *GetBullet();
	LPCTSTR GetGunName();

	// 弾を撃つ
	Bullet *ShootTheBullet( void );
};
