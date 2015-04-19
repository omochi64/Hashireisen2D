
#pragma once


#include <vector>
#include <map>
#include "Selene.h"


using namespace std;
using namespace Selene;

class CTextureManager{
//	vector<Renderer::ITexture*>	m_textures;
//	vector<bool>				m_is_enable_texture;
	int m_texture_num;
	map<ATOM,Renderer::ITexture*>	m_atom_to_tex;

	vector<ATOM>				m_atom_key_list;

public:
	CTextureManager(){};
	~CTextureManager(){};

	Renderer::ITexture* AddTexture( LPCTSTR pTexName , Renderer::IRender *render );
	Renderer::ITexture* GetAt( LPCTSTR pFname );
	Renderer::ITexture* GetAt( ATOM key );
	void DeleteTexture( LPCTSTR pFname );
	void DeleteTexture( ATOM key );
	void Release( void );
};
