
#include "Selene.h"
using namespace Selene;

#include "SetLighting.h"

void SetLighting( Scene::ISceneManager *pSceneMgr )
{
	// ���s����
	//   �V�[���ɑ΂��ĕ��s������ݒ肵�܂��B
	pSceneMgr->SetLightParameter_Directional(
		Math::Vector3D( 0.5f, -0.5f, 0.7f ),		// ���C�g�̕���
		Math::Vector3D( 0.8f, 0.8f, 0.8f ) );		// ���C�g�̐F

	// �A���r�G���g
	// �������C�e�B���O���s���Ă���̂ŁA�V���ƒn�\�̏Ƃ�Ԃ��̐F��ݒ肵�܂��B
	pSceneMgr->SetLightParameter_Ambient(
		Math::Vector3D( 0.2f, 0.2f, 0.2f ),			// �V���̐F
		Math::Vector3D( 0.5f, 0.5f, 0.5f ) );		// �n�\�̐F
}
