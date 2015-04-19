#pragma once

#include "Bullet.h"

class Bullet2D:public Bullet
{
protected:
	MultipleAnimation	*m_bullet_object;

	virtual const Math::Vector3D &GetNowBulletPosition();
public:
	Bullet2D(int pierceful_count , float flying_distance , int power );
	Bullet2D();
	virtual ~Bullet2D();
	virtual void Release();

	virtual void SetBulletObject( MultipleAnimation *bullet );
	virtual MultipleAnimation *GetBulletObject();
	virtual Bullet *Clone();
};
