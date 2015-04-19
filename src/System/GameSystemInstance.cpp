
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"
#include <vector>
#include "Selene.h"

using namespace Selene;

namespace GameSystem
{
	INFO_SET	game_info;

	std::vector<Selene::Renderer::Object::IFontSprite2D *> pFonts;
	std::vector<int>	fonts_spaces;
	int			GAME_FPS;

	File::IFileManager *pFileManager;
//	Renderer::IRender *pRender;
	Scene::ISceneManager *pScene;
//	ICore	*pCore;
//	Scene::ICamera *pCamera;
	Renderer::ITexture *pTextureForBackground;
	std::vector<PRIMITIVE2D_SET> pPrimitive2D;
	CTextureManager	*tex_manager;
	CMultipleAnimationManager	*multiple_animation_manager;
	CoursesManager	*course_manager;
	CObjects			*all_object;
	TaskController	*all_task;
	TaskController	*draw_task;
	CModelManager		*model_manager;
	
	bool				is_searching_udonge;
	bool				is_searching_only_y;
	DWORD			now_frame_time;

	
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
};
