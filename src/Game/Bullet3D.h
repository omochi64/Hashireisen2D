#pragma once

#include "Bullet.h"

class Bullet3D:public Bullet
{
protected:
	// Ç±Ç±Ç…SMFAnimationObjectÇíuÇ≠ÅBç°ÇÕÇ»Çµ

	virtual const Math::Vector3D &GetNowBulletPosition();
public:
	Bullet3D();
	virtual ~Bullet3D();
	virtual void Release();

	virtual void SetBulletObject( MultipleAnimation *bullet );
	MultipleAnimation *GetBulletObject();
	virtual Bullet *Clone();
};
