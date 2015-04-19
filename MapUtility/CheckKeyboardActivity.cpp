
#include "Selene.h"
using namespace Selene;

#include "CheckKeyboardActivity.h"
#include "UpdateCamera.h"

void CheckKeyboardActivity( Peripheral::IKeyboard *pKeyboard , Scene::ISceneManager *pSceneMgr )
{
	bool is_shift;
	Scene::ICamera *pCamera = pSceneMgr->GetCamera();

	is_shift = pKeyboard->GetKeyData(SELENE_VK_LSHIFT)|pKeyboard->GetKeyData(SELENE_VK_RSHIFT) ? true : false;

	// z����ֈړ�
	if( pKeyboard->GetKeyData(SELENE_VK_F1) )
	{
		if( !is_shift )
		{
			Camera::SetCameraOnZAxisPlusDirection( pCamera );
		}
		else
		{
			Camera::SetCameraOnZAxisMinusDirection( pCamera );
		}
	}

	// x����ֈړ�
	if( pKeyboard->GetKeyData(SELENE_VK_F2) )
	{
		if( !is_shift )
		{
			Camera::SetCameraOnXAxisPlusDirection( pCamera );
		}
		else
		{
			Camera::SetCameraOnXAxisMinusDirection( pCamera );
		}
	}

	// y����ֈړ�
	if( pKeyboard->GetKeyData(SELENE_VK_F3) )
	{
		if( !is_shift )
		{
			Camera::SetCameraOnYAxisMinusDirection( pCamera );
		}
		else
		{
			Camera::SetCameraOnYAxisPlusDirection( pCamera );
		}
	}

	// �f�t�H���g��
	if( pKeyboard->GetKeyData(SELENE_VK_F4) )
	{
		Camera::SetCameraToDefault( pCamera );
	}

}
