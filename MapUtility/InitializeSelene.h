
#pragma once

// Selene‚Ì‰Šú‰»FX
bool InitializeSelene( ICore *pCore , File::IFileManager **ppFileMgr , IGraphicCard	**ppGraphicCard , 
					  Renderer::IRender **ppRender , Scene::ISceneManager **ppSceneMgr , Scene::ICamera **ppCamera , 
					  int screen_width , int screen_height );