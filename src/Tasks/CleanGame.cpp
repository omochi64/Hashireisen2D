
#include "CleanGame.h"
#include "System/GlobalNamespace.h"
#include "System/GameSystem.h"
#include "System/Sounds.h"

using namespace GameSystem;

void CleanGame::run()
{
	/*
	
namespace GameSystem
{
	EXTERN CObjects			*all_object;
	EXTERN TaskController	*all_task;
	EXTERN TaskController	*draw_task;
	EXTERN CTextureManager	*tex_manager;
	EXTERN CMultipleAnimationManager	*multiple_animation_manager;
	EXTERN ModelManager		*model_manager;
	EXTERN bool				is_searching_udonge;
	EXTERN DWORD			now_frame_time;
	EXTERN Renderer::IRender *pRender;
	EXTERN Scene::ISceneManager *pScene;
	EXTERN Renderer::ITexture *pTextureForBackground;
	EXTERN File::IFileManager *pFileManager;
	EXTERN std::vector<Renderer::Object::IFontSprite2D *> pFonts;
	EXTERN std::vector<int>	fonts_spaces;
	EXTERN std::vector<PRIMITIVE2D_SET> pPrimitive2D;
	EXTERN Scene::ICamera *pCamera;
	EXTERN CoursesManager	*course_manager;
	EXTERN INFO_ST	game_info;
};

namespace Character
{
	
	EXTERN UdongeAnimation	*udonge;
};

	*/

	pCore->Exit();
}
