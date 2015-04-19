
#pragma once

#include "System/GlobalNamespace.h"
#include "System/TaskSystem.h"



class FirstInitialize:public TaskControllBlock
{
	Renderer::IRender *m_pRender;
	Scene::ISceneManager *m_pSceneMgr;
	Scene::ICamera *m_pCamera;
	Renderer::Object::IPrimitive2D *m_pPrimitive;
	File::IFileManager *m_pFileManager;
public:
	FirstInitialize( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , Scene::ICamera *pCamera ,
		File::IFileManager *pFileManager );

	void run();

	void LoadUdonge();
	void LoadInfo();
};
