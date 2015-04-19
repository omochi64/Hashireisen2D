
#include "Bullet.h"
/*
int	m_pierceful_count;
	Math::Vector3D	m_firstPos;
	float	m_flying_distance;
	int	m_power;

public:
	Bullet();
	virtual ~Bullet();

	void BeginFlying();
	bool CanContinueToFly();
	int DecreasePiercefulCount();*/


// �R���X�g���N�^
Bullet::Bullet(int pierceful_count , float flying_distance , int power )
{
	m_pierceful_count = pierceful_count;
	m_flying_distance = flying_distance;
	m_power = power;
	m_firstPos = Math::Vector3D(0,0,0);
	m_is_flying = false;

	m_bullet_object = NULL;
}

Bullet::Bullet()
{
	m_pierceful_count = 0;
	m_firstPos = Math::Vector3D(0,0,0);
	m_flying_distance = 0;
	m_power = 0;
	m_is_flying = false;
	m_bullet_object = NULL;
}

Bullet::~Bullet()
{
	Release();
}

void Bullet::Release()
{
	SAFE_RELEASE(m_bullet_object);
}

// �e�e��s�J�n
void Bullet::BeginFlying()
{
	m_firstPos = GetNowBulletPosition();
	m_is_flying = true;
}

// ��s�I��
void Bullet::EndFlying()
{
	m_is_flying = false;
}

bool Bullet::CanContinueToFly()
{
	if( m_is_flying )
	{
		const Math::Vector3D &now_pos = GetNowBulletPosition();
		float x_diff = now_pos.x-m_firstPos.x;
		float y_diff = now_pos.y-m_firstPos.y;
		float z_diff = now_pos.z-m_firstPos.z;

		float dist_square = x_diff*x_diff + y_diff*y_diff + z_diff*z_diff;

		// �܂��ő��s�����ɒB���Ă��Ȃ����H
		if( m_flying_distance*m_flying_distance >= dist_square )
		{
			// �B���Ă��Ȃ�
			return true;
		}
	}

	return false;
}

int Bullet::DecreasePiercefulCount()
{
	if( --m_pierceful_count == -1 )
	{
		m_pierceful_count = 0;
	}
	return m_pierceful_count;
}

// �����o�A�N�Z�X�n
int Bullet::GetPiercefulCount()
{
	return m_pierceful_count;
}

float Bullet::GetFlyingDistance()
{
	return m_flying_distance;
}

int Bullet::GetPower()
{
	return m_power;
}

void Bullet::SetPiercefulCount(int count)
{
	if( count >= -1 )
	{
		m_pierceful_count = count;
	}
}

void Bullet::SetFlyingDistance( float distance )
{
	if( distance > 0.0f )
	{
		m_flying_distance = distance;
	}
}

void Bullet::SetPower( int power )
{
	if( power >= 0 )
	{
		m_power = power;
	}
}

// �e�e�A�j���[�V�����ݒ�
void Bullet::SetBulletObject( MultipleAnimation *bullet )
{
	SAFE_RELEASE(m_bullet_object);
	m_bullet_object = bullet;
}

// �擾
MultipleAnimation *Bullet::GetBulletObject()
{
	return m_bullet_object;
}


// ����
Bullet *Bullet::Clone()
{
	Bullet *bullet = new Bullet( m_pierceful_count , m_flying_distance , m_power );
	bullet->SetBulletObject( (MultipleAnimation *)m_bullet_object->Clone() );

	if( m_is_flying )
	{
		bullet->BeginFlying();
	}

	return bullet;
}

const Math::Vector3D &Bullet::GetNowBulletPosition()
{
	return m_bullet_object->GetPos();
}
