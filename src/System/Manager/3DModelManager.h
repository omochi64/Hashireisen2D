
#pragma once

#include <map>
#include <vector>
#include "Selene.h"
#include "IManager.h"

using namespace std;
using namespace Selene;

class CModelManager
{
	map<ATOM,Renderer::Object::IModel *>	m_relation_to_model;
	map<ATOM,Scene::IModelActor *>	m_relation;

	std::vector<ATOM>	m_added_models;
	File::IFileManager *m_pFileManager;

	std::vector<Scene::IModelActor *>	m_created_models;

public:
	CModelManager(File::IFileManager *pFileManager);
	~CModelManager();

	bool LoadModel( const char *file );

	int CreateNewModel( ATOM name );
	int CreateNewModel( LPCTSTR pFileName );
	Scene::IModelActor *GetModel( int id );
	void ReleaseModel( int id );

	// Ç±Ç¢Ç¬ÇÁÇÕÇ¢Ç∏ÇÍprivateÇ»ä÷êîÇ…ÇµÅAäOïîÇ…ÇÕCreateNewModel,GetModelÇÃÇ›íÒãüÇ∑ÇÈ
	Scene::IModelActor *GetOriginalModel( char *file_name );
	Scene::IModelActor *GetOriginalModel( ATOM file_name_atom );
	Scene::IModelActor *GetCopyModel( char *file_name );
	Scene::IModelActor *GetCopyModel( ATOM file_name_atom );

	void Release();
};
