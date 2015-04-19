
#include "3DModelManager.h"
#include "System/GameSystem.h"

CModelManager::CModelManager(File::IFileManager *pFileManager)
{
	m_pFileManager = pFileManager;
}

CModelManager::~CModelManager()
{
	Release();
}

bool CModelManager::LoadModel(const char *file )
{
	Renderer::IRender *render = GameSystem::SystemGetter::GetRender();
	Scene::ISceneManager * pSceneMgr = GameSystem::SystemGetter::GetSceneManager();

	if( file == NULL )
	{
		return false;
	}
	ATOM atom = AddAtom(file);
	if( m_relation[atom] != NULL )
	{
		return true;
	}

	m_pFileManager->SetCurrentPath( "Model" );
	Renderer::Object::IModel *model;
	model = render->CreateModelFromFile( file );
	m_pFileManager->SetCurrentPath( "Texture" );
	if( model == NULL )
	{
		return false;
	}
	Scene::IModelActor *actor = pSceneMgr->CreateActor( model );
	if( actor == NULL )
	{
		model->Release();
		delete model;
		return false;
	}

	m_relation_to_model[atom] = model;
	m_relation[atom] = actor;

	m_added_models.push_back(atom);

	return true;
}

void CModelManager::Release()
{
	std::vector<ATOM>::iterator it,end;
	it = m_added_models.begin();
	end = m_added_models.end();

	while( it != end )
	{
		SAFE_RELEASE(m_relation_to_model[*it]);
		SAFE_RELEASE(m_relation[*it]);

		it++;
	}

	std::vector<Scene::IModelActor *>::iterator model_it,model_end;
	model_it = m_created_models.begin();
	model_end = m_created_models.end();

	while( model_it != model_end )
	{
		SAFE_RELEASE(*model_it);
		model_it++;
	}
	
	m_added_models.clear();
	m_created_models.clear();
}

Scene::IModelActor *CModelManager::GetOriginalModel(ATOM file_name_atom)
{
	return m_relation[file_name_atom];
}

Scene::IModelActor *CModelManager::GetOriginalModel(char *file_name)
{
	return GetOriginalModel(AddAtom(file_name));
}

Scene::IModelActor *CModelManager::GetCopyModel( char *file_name )
{
	return GetCopyModel( AddAtom(file_name) );
}

Scene::IModelActor *CModelManager::GetCopyModel( ATOM file_name_atom )
{
	Scene::ISceneManager *pSceneMgr = GameSystem::SystemGetter::GetSceneManager();

	if( m_relation_to_model[file_name_atom] && pSceneMgr )
	{
		return pSceneMgr->CreateActor(m_relation_to_model[file_name_atom]);
	}
	return NULL;
}

// モデルのコピーを生成し、そのIDを返す
int CModelManager::CreateNewModel( ATOM name )
{
	int new_id;
	Scene::IModelActor *actor = GetCopyModel(name);
	if( actor )
	{
		new_id = m_created_models.size();
		m_created_models.push_back(actor);
		return new_id;
	}
	return -1;
}

int CModelManager::CreateNewModel( LPCTSTR pFileName )
{
	return CreateNewModel( AddAtom(pFileName) );
}

// idからオブジェクトを取得
Scene::IModelActor *CModelManager::GetModel( int id )
{
	if( m_created_models.size() > id && id >= 0 )
	{
		return m_created_models[id];
	}
	else
	{
		return NULL;
	}
}

// idからオブジェクトを削除
void CModelManager::ReleaseModel( int id )
{
	if( m_created_models.size() > id && id >= 0 )
	{
		SAFE_RELEASE(m_created_models[id]);
	}
}
