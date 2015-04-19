
#pragma once

#include <vector>
using namespace std;

#include "Selene.h"
using namespace Selene;

#include "Tools/IMapObjectDataForTool.h"

// オブジェクトデータ
// これは変えるかも(Actorの生成に関わってくるので、メインウィンドウとの関連が強い)。
typedef struct _object_data{
	int	id;
	TCHAR	name[64];
	TCHAR	file[128];
	Renderer::Object::IModel	*model_master;
	IMapObjectDataForTool		*actor_for_copy;
}OBJECT_DATA;

#ifdef MAIN_DIALOG_SOURCE
#define EXTERN_COMMON /*extern*/
#else
#define EXTERN_COMMON extern
#endif

// 一覧
EXTERN_COMMON vector<OBJECT_DATA>	objects;
EXTERN_COMMON int	g_now_selected_object;
EXTERN_COMMON HINSTANCE hInstance;
EXTERN_COMMON bool				g_is_end;
EXTERN_COMMON bool				g_is_change_programmly;
EXTERN_COMMON Renderer::IRender	*g_pRender;
EXTERN_COMMON Scene::ISceneManager	*g_pSceneMgr;
EXTERN_COMMON File::IFileManager	*g_pFileMgr;
EXTERN_COMMON int				g_next_id;
