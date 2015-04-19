
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

	// �R���X�g���N�^�A�f�X�g���N�^
	GunController();
	~GunController();

	// �e�e������
	void SupplyBullet( int num );
	void DecreaseBullet( int num=1 );
	int GetBulletNum( void );

	// �����ݒ�֌W
	void SetGunObject( MultipleAnimation *gun , bool release_old = true );
	void SetBullet( Bullet *bullet , bool release_old = true );
	void Initialize( LPCTSTR gun_name , int iniBulletNum , int maxBulletNum );

	// �����A�N�Z�X
	MultipleAnimation *GetGunObject();
	Bullet *GetBullet();
	LPCTSTR GetGunName();

	// �e������
	Bullet *ShootTheBullet( void );
};
