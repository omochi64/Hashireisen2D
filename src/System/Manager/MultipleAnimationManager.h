
#pragma once

#include <stdio.h>
#include <vector>
#include <map>
#include "Selene.h"
#include "TextureManager.h"
#include "Game/Objects.h"
#include "Game/UdongeAnimation.h"
#include "3DModelManager.h"

using namespace Selene;
using namespace std;

class CMultipleAnimationManager{
	
	map<ATOM, MultipleAnimation *>	m_atom_to_animation;
	vector<ATOM>					m_atom_list;

	bool LoadFromFileVer0_1( File::IResourceFile *fp , char* loadKeyWord ,
		CTextureManager *tex_manager ,  CModelManager *model_manager , Renderer::IRender *render );
	
public:

	CMultipleAnimationManager():m_atom_to_animation(),m_atom_list(){}
	~CMultipleAnimationManager(){Release();}
	bool LoadFromFile( LPCTSTR pFname , char* loadKeyWord , CTextureManager *tex_manager , 
    CModelManager *model_manager, Renderer::IRender *render, File::IFileManager *pFileManager, bool do_append = true);
	MultipleAnimation *GetCopyMultipleAnimationObject( LPCTSTR pName ){return GetCopyMultipleAnimationObject(AddAtom(pName));}
	MultipleAnimation *GetCopyMultipleAnimationObject( ATOM key );
	MultipleAnimation *GetOriginalMultipleAnimationObject( LPCTSTR pName ){return GetOriginalMultipleAnimationObject(AddAtom(pName));};
	MultipleAnimation *GetOriginalMultipleAnimationObject( ATOM key );

	void DeleteMultipleAnimationObject( LPCTSTR pName ){DeleteMultipleAnimationObject(AddAtom(pName));}
	void DeleteMultipleAnimationObject( ATOM key );
	void Release( void );
};
