
#pragma once

#include "Selene.h"
#include "System/Manager/3DModelManager.h"

// �\�Ȃ�A���̂悤��extern�ŕϐ����O���ɎN���̂ł͂Ȃ��Astatic�ȃN���X����Getter�Ŏ擾�ł���悤�ɂ��Ă���(�J�v�Z�������ړI)

// �e�f�[�^�������Ă���t�H���_��(Map�Ƃ�Model�Ƃ�Texture�Ƃ�...)���O���f�[�^����ǂݍ��ތ`�ɂ��āA���L�����ق���������

namespace GameSystem
{
	// �������namespace���̕ϐ��łȂ��A�N���X���̐ÓI�����o�Ƃ��ēo�^���邱�ƂɂȂ邩������Ȃ��B
	// �Ƃ肠�����A���ꂩ��������Ă����Ƃ��́A�����ɕϐ���G�邱�Ƃ͂��Ȃ�
	extern Selene::Scene::ISceneManager	*pSceneMgr;
	extern Selene::Renderer::IRender *pRender;
	extern Selene::File::IFileManager *pFileMgr;
	extern Selene::ICore *pCore;
//	extern Selene::IGraphicCard *pGraphicCard;
	extern Selene::Scene::ICamera *pCamera;

	class SystemGetter{
	public:
		static Selene::Scene::ISceneManager *GetSceneManager();
		static Selene::Renderer::IRender *GetRender();
		static Selene::File::IFileManager *GetFileManager();
		static Selene::ICore *GetCore();
		static Selene::Scene::ICamera *GetCamera();
		static CModelManager *GetModelManager();
	};
	
	// ���Manager�n���Q���\��
	// �{������ModelManager�͊O�ɂ͌����Ȃ�(�R�C�c�̏�������GameSystem.cpp���̏������֐��Ŗ{���s��)
	extern CModelManager *pModelMgr;

	
	// ���\�b�h
	bool Initialize(char *pWinTitle , Selene::eFrameRate frameRate , bool is_full_screen , float fovAngle );
	void CleanUp();
};
