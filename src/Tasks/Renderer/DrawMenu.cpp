
#include "DrawMenu.h"
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"

DrawMenu::DrawMenu( std::vector<BackgroundController *> *draw_list , bool will_release_draw_list , bool will_release_menu )
{
	m_pMenu = NULL;
	m_draw_list = draw_list;
	m_will_release_draw_list = will_release_draw_list;
	m_will_release_menu = will_release_menu;
}

DrawMenu::~DrawMenu()
{
	if( m_will_release_draw_list )
	{
		int i,size = m_draw_list->size();
		for( i=0 ; i<size ; i++ )
		{
			SAFE_RELEASE(m_draw_list->at(i));
		}
		delete m_draw_list;
		m_draw_list = NULL;
	}

	if( m_will_release_menu )
	{
		SAFE_RELEASE(m_pMenu);
	}
}

void DrawMenu::SetMenu( MenuController *pMenu )
{
	m_pMenu = pMenu;
}

void DrawMenu::run()
{
	if( m_pMenu )
	{
		m_pMenu->DrawMenu();
	}
	int i,size = m_draw_list->size();

	GameSystem::PRIMITIVE2D_SET set;
	BackgroundController *bc;

	set.pTargetTexture = NULL;

	for( i=0 ; i<size ; i++ )
	{
		bc = m_draw_list->at(i);
		bc->Draw();
		set.pPrimitive = bc->GetPrimitive();
		set.setTexture = bc->GetTexture();

		GameSystem::pPrimitive2D.push_back( set );
	}
}
