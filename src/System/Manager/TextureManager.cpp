
#include "TextureManager.h"


// テクスチャをAdd
Renderer::ITexture* CTextureManager::AddTexture( LPCTSTR pTexFname , Renderer::IRender *render )
{
	ATOM new_atom = AddAtom( pTexFname );

	if( m_atom_to_tex[new_atom] != NULL )
	{
		return m_atom_to_tex[new_atom];
	}

	Renderer::ITexture *new_tex;


	new_tex = render->CreateTextureFromFile( pTexFname );
	if( !new_tex )
	{
		return NULL;
	}

	m_atom_to_tex[new_atom] = new_tex;
//	m_textures.push_back(new_tex);
//	m_is_enable_texture.push_back(true);
	m_atom_key_list.push_back( new_atom );
	m_texture_num++;

	return new_tex;
}

Renderer::ITexture* CTextureManager::GetAt( LPCTSTR pFname )
{
	ATOM at = AddAtom( pFname );
	return m_atom_to_tex[at];
}

Renderer::ITexture* CTextureManager::GetAt( ATOM key )
{
	return m_atom_to_tex[key];
}

// テクスチャを削除
void CTextureManager::DeleteTexture( LPCTSTR pFname )
{
	ATOM atom = AddAtom( pFname );
	DeleteTexture( atom );
}
void CTextureManager::DeleteTexture( ATOM key )
{
	Renderer::ITexture *texture = m_atom_to_tex[key];
	if( texture == NULL )
	{
		return;
	}

	SAFE_RELEASE(texture);
	m_atom_to_tex[key] = NULL;

	vector<ATOM>::iterator it;
	for( it=m_atom_key_list.begin() ; it != m_atom_key_list.end() ; it++ )
	{
		if( *it == key )
		{
			m_atom_key_list.erase(it);
			break;
		}
	}

	m_texture_num--;
	return;
}


// テクスチャを解放
void CTextureManager::Release( void )
{
	int i;
	int size = m_atom_key_list.size();

	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(m_atom_to_tex[m_atom_key_list[i]]);
	}
	m_atom_to_tex.clear();
	m_atom_key_list.clear();
	m_texture_num=0;
}
