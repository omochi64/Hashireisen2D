
#pragma once

#include "Selene.h"
using namespace Selene;

#include "System/TaskSystem.h"
#include "Game/Objects.h"
#include "System/Manager/TextureManager.h"
#include "System/Manager/MultipleAnimationManager.h"
#include "System/Manager/CoursesManager.h"
#include "Game/UdongeAnimation.h"
#include "System/Manager/3DModelManager.h"
#include "BasicStructure.h"

#ifdef _MAIN_SOURCE_
#define	EXTERN	/*extern*/
#else
#define EXTERN extern
#endif

namespace GameSystem
{
	typedef struct _primitive2D_set{
		Renderer::Object::IPrimitive2D *pPrimitive;
		Renderer::ITexture *setTexture;
		Renderer::ITexture *pTargetTexture;
	}PRIMITIVE2D_SET;

	extern const int SCREEN_WIDTH,SCREEN_HEIGHT;
	extern File::IFileManager *pFileManager;
//	extern Renderer::IRender *pRender;
	extern Scene::ISceneManager *pScene;
//	extern ICore	*pCore;
//	extern Scene::ICamera *pCamera;
	extern Renderer::ITexture *pTextureForBackground;
	extern std::vector<PRIMITIVE2D_SET> pPrimitive2D;
	extern CTextureManager	*tex_manager;
	extern CMultipleAnimationManager	*multiple_animation_manager;
	extern CoursesManager	*course_manager;
	extern CObjects			*all_object;
	extern TaskController	*all_task;
	extern TaskController	*draw_task;
	extern CModelManager		*model_manager;
	
	extern bool				is_searching_udonge;
	extern bool				is_searching_only_y;
	extern DWORD			now_frame_time;
		// �����͏����d�l��ύX����K�v������B
		// �`��p�v���~�e�B�u�A�^�[�Q�b�g�e�N�X�`�����^�X�N����ǉ��ł���悤�ɂ���(���t���[���ǉ�����K�v������悤�ɂ���H)
	//	EXTERN Renderer::Object::IPrimitive2D *pPrimitive2D;
	extern int GAME_FPS;
	
};

namespace Character
{
	EXTERN UdongeAnimation	*udonge;
};
