#pragma once

#include "Game/Objects.h"

// e’e‚Ì’ŠÛƒNƒ‰ƒX
// ‚±‚±‚©‚çA2ŸŒ³‚Ìe’eA3ŸŒ³‚Ìe’e‚É”h¶
class Bullet
{
protected:
	int	m_pierceful_count;
	Math::Vector3D	m_firstPos;
	float	m_flying_distance;
	int	m_power;
	bool	m_is_flying;
	MultipleAnimation	*m_bullet_object;

	const Math::Vector3D &GetNowBulletPosition();

public:
	Bullet(int pierceful_count , float flying_distance , int power );
	Bullet();
	~Bullet();
	void Release();

	void BeginFlying();
	void EndFlying();
	bool CanContinueToFly();
	int DecreasePiercefulCount();
	void SetBulletObject( MultipleAnimation *bullet );
	MultipleAnimation *GetBulletObject();
	Bullet *Clone();

	// ƒAƒNƒZƒXŒn
	int GetPiercefulCount();
	float GetFlyingDistance();
	int GetPower();
	void SetPiercefulCount(int count);
	void SetFlyingDistance( float distance );
	void SetPower( int power );
};
