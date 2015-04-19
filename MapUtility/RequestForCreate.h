
#pragma once

#ifdef MAIN_WIN_DEFINE
#define EXTERN_RFC	/*extern*/
#else
#define EXTERN_RFC	extern
#endif

#include <vector>
#include "Selene.h"
using namespace Selene;

// モデル作成要求用構造体
typedef struct _request_for_create_model{
	char fname[256];
	void (*call_func_when_finish)(HWND,Renderer::Object::IModel *,int);
	HWND hWnd;
	int	other_info;
}RFC_MODEL;

// アクター作成要求用構造体
typedef struct _request_for_create_actor{
	Renderer::Object::IModel *pModel;
	void (*call_func_when_finish)(HWND,Scene::IModelActor *,int);
	HWND hWnd;
	int	other_info;
}RFC_ACTOR;

EXTERN_RFC	CRITICAL_SECTION		g_RFC_model_CS,g_RFC_actor_CS;
EXTERN_RFC	std::vector<RFC_MODEL>	g_request_for_model;
EXTERN_RFC	std::vector<RFC_ACTOR>	g_request_for_actor;
