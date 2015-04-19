
#include "Selene.h"
using namespace Selene;

#include "UpdateCamera.h"
#include "CommonForCamera.h"

namespace Camera
{

float	g_default_dist = 10.0f;
float	g_dist;
float	g_camRotationY;
float	g_camRotationXZ;
Math::Vector3D	g_camAt;
Math::Vector3D	g_camUp;
Math::Vector3D g_camBasicDir = Math::Vector3D(0,0,1.0f);


void UpdateCamera( Scene::ICamera *pCamera )
{
	// �J�����̏�Ԃ�������
	pCamera->Reset();

	// x,z��������]�������߂�
	Math::Vector3D n = g_camBasicDir;
	n.RotationY( DEG_TO_ANGLE(g_camRotationY) );
	Math::Vector3D rotation_axis = Math::Vector3D( n.z , 0 , -n.x );

	// Math::Style�̕ϊ���K�p
	Math::Vector3D cam_pos;
	cam_pos = -g_camBasicDir*g_dist;
	cam_pos.RotationY( DEG_TO_ANGLE(g_camRotationY) );
	cam_pos.RotationAxis( DEG_TO_ANGLE(g_camRotationXZ) , rotation_axis );
//	cam_pos.RotationZ( DEG_TO_ANGLE(g_camRotation.z) );
	cam_pos += g_camAt;

	Math::Vector3D up = g_camUp;
	up.RotationAxis( DEG_TO_ANGLE(g_camRotationXZ) , rotation_axis );


	Math::Style style;
	style.LookAt( cam_pos , g_camAt , up );
	pCamera->SetTransform( style );

	// �J�����̓����f�[�^�𗘗p�\�ȏ�ԂɍX�V
	//   ���̊֐����ĂԂ��ƂŎ��ۂɂ��̃J�����ł̃����_�����O�\�ɂȂ�܂��B
	pCamera->Update();

}

// �J���������Z�b�g
void SetCameraToDefault( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = -45;
	g_camRotationXZ = -30;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);
}

// �J������Z�����(������+����)
void SetCameraOnZAxisPlusDirection( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = Camera::g_camRotationXZ = 0;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);
}

// �J������Y�����(������+����)
void SetCameraOnYAxisPlusDirection( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = 0;
	g_camRotationXZ = 90;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);
}

// �J������X�����(������+����)
void SetCameraOnXAxisPlusDirection( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = 90;
	g_camRotationXZ = 0;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);	
}

// �J������Z�����(������-����)
void SetCameraOnZAxisMinusDirection( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = 180;
	Camera::g_camRotationXZ = 0;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);
}

// �J������Y�����(������-����)
void SetCameraOnYAxisMinusDirection( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = 0;
	g_camRotationXZ = -90;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);
}

// �J������X�����(������-����)
void SetCameraOnXAxisMinusDirection( Scene::ICamera *pCamera )
{
	g_dist = g_default_dist;
	g_camRotationY = -90;
	g_camRotationXZ = 0;
	g_camAt = Math::Vector3D(0,0,0);
	g_camUp = Math::Vector3D(0,1.0f,0.0f);	
}

};
