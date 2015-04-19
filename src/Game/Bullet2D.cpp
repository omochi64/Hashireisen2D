
#include "Bullet2D.h"

// �R���X�g���N�^
Bullet2D::Bullet2D( int pierceful_count , float flying_distance , int power )
:Bullet(pierceful_count,flying_distance,power)
{
	m_bullet_object = new MultipleAnimation();
}

Bullet2D::Bullet2D():Bullet()
{
	m_bullet_object = new MultipleAnimation();
}

// �f�X�g���N�^
Bullet2D::~Bullet2D()
{
	Release();
}

void Bullet2D::Release()
{
	SAFE_RELEASE(m_bullet_object);
}

// �|�W�V�����擾
const Math::Vector3D &Bullet2D::GetNowBulletPosition()
{
	return m_bullet_object->GetPos();
}

// �I�u�W�F�N�g��ݒ�
void Bullet2D::SetBulletObject( MultipleAnimation *bullet )
{
	SAFE_RELEASE(m_bullet_object);
	m_bullet_object = bullet;
}

// �I�u�W�F�N�g�擾
MultipleAnimation *Bullet2D::GetBulletObject()
{
	return m_bullet_object;
}

// �I�u�W�F�N�g�̕���
Bullet *Bullet2D::Clone()
{
	Bullet2D *bullet = new Bullet2D( m_pierceful_count , m_flying_distance , m_power );
	bullet->SetBulletObject( (MultipleAnimation *)m_bullet_object->Clone() );

	if( m_is_flying )
	{
		bullet->BeginFlying();
	}

	return bullet;
}
