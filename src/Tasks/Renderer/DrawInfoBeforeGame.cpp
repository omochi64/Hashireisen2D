
#include "System/GlobalNamespace.h"
#include "DrawInfoBeforeGame.h"
#include "System/IniInfo.h"
#include "System/GameSystem.h"

DrawInfoBeforeGame::DrawInfoBeforeGame(int world, int course)
{
	m_world = world;
	m_course = course;

	m_udonge_tex = GameSystem::tex_manager->GetAt( GameSystem::game_info.udonge_stand_tex );
	if( !m_udonge_tex )
	{
		m_pUdongePrimitive = NULL;
		return;
	}
	m_pUdongePrimitive = GameSystem::pRender->CreatePrimitive2D( 1024 );

	char str[512];

	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_NUMBERS];

	sprintf( str , "WORLD %d - %d" , m_world , m_course );
	m_worldStringPos = font->GetStringSize( str );
	m_worldStringPos.x = (GameSystem::SCREEN_WIDTH-m_worldStringPos.x)/2;
	m_worldStringPos.y = 150;
	
	sprintf( str , "  X  %02d" , Character::udonge->GetRestLife() );
	m_restLifeStringPos = font->GetStringSize( str );
	m_restLifeStringPos.x = (GameSystem::SCREEN_WIDTH-(m_restLifeStringPos.x+33))/2+33;
	m_restLifeStringPos.y = 280;
	

	float point_item_left_x = m_restLifeStringPos.x-(33+15);
	
	m_udonge_vertex[0].v1.Col = m_udonge_vertex[0].v2.Col = m_udonge_vertex[0].v3.Col = 
		m_udonge_vertex[1].v1.Col = m_udonge_vertex[1].v2.Col = m_udonge_vertex[1].v3.Col = CColor(0xff,0xff,0xff);
	m_udonge_vertex[0].v1.Pos = Math::Vector3D( point_item_left_x , 250 , 0 );
	m_udonge_vertex[0].v2.Pos = Math::Vector3D( point_item_left_x+64 , 250 , 0 );
	m_udonge_vertex[0].v3.Pos = Math::Vector3D( point_item_left_x+64 , 314 , 0 );
	m_udonge_vertex[1].v1.Pos = Math::Vector3D( point_item_left_x+64 , 314 , 0 );
	m_udonge_vertex[1].v2.Pos = Math::Vector3D( point_item_left_x , 314 , 0 );
	m_udonge_vertex[1].v3.Pos = Math::Vector3D( point_item_left_x , 250 , 0 );

	m_udonge_vertex[1].v3.Tex[0] = m_udonge_vertex[0].v1.Tex[0] = GameSystem::game_info.udonge_stand_uv.leftup;
	m_udonge_vertex[0].v3.Tex[0] = m_udonge_vertex[1].v1.Tex[0] = GameSystem::game_info.udonge_stand_uv.rightbottom;
	m_udonge_vertex[0].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.udonge_stand_uv.rightbottom.x , GameSystem::game_info.udonge_stand_uv.leftup.y );
	m_udonge_vertex[1].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.udonge_stand_uv.leftup.x , GameSystem::game_info.udonge_stand_uv.rightbottom.y );

	m_set.pPrimitive = m_pUdongePrimitive;
	m_set.pTargetTexture = NULL;
	m_set.setTexture = m_udonge_tex;

	
	
}

DrawInfoBeforeGame::~DrawInfoBeforeGame()
{
	SAFE_RELEASE(m_pUdongePrimitive);
}

// î•ñ•`‰æ
void DrawInfoBeforeGame::run( void )
{
	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_NUMBERS];

	char str[512];

	sprintf( str , "WORLD %d - %d" , m_world , m_course );
	font->DrawString( str , m_worldStringPos , CColor(0xff,0xff,0xff) );

	m_pUdongePrimitive->Begin();
	m_pUdongePrimitive->Push( m_udonge_vertex , 2 );
	m_pUdongePrimitive->End();
	GameSystem::pPrimitive2D.push_back( m_set );

	sprintf( str , "  X  %02d" , Character::udonge->GetRestLife() );
	font->DrawString( str , m_restLifeStringPos , CColor(0xff,0xff,0xff) );
}
