#pragma once

#include "Bullet.h"

class Bullet3D:public Bullet
{
protected:
	// ������SMFAnimationObject��u���B���͂Ȃ�

	virtual const Math::Vector3D &GetNowBulletPosition();
public:
	Bullet3D();
	virtual ~Bullet3D();
	virtual void Release();

	virtual void SetBulletObject( MultipleAnimation *bullet );
	MultipleAnimation *GetBulletObject();
	virtual Bullet *Clone();
};
