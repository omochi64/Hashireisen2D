
#include "System/GlobalNamespace.h"
#include "DrawStatus.h"
#include "System/IniInfo.h"
#include "System/GameSystem.h"

DrawStatus::DrawStatus( int world , int course , GameMain *main_task )
{
	m_world = world;
	m_course = course;
	m_main_task = main_task;

	int i;

	m_heart_full_tex = GameSystem::tex_manager->GetAt( GameSystem::game_info.heart_full_tex );
	m_heart_empty_tex = GameSystem::tex_manager->GetAt( GameSystem::game_info.heart_enmpy_tex );
	m_point_item_tex = GameSystem::tex_manager->GetAt( GameSystem::game_info.point_item_tex );

	m_heart_full = g_pRender->CreatePrimitive2D( 1024 );
	m_heart_empty = g_pRender->CreatePrimitive2D( 1024 );
	m_point_item = g_pRender->CreatePrimitive2D( 1024 );
	for( i=0 ; i<MAX_HP ; i++ )
	{
//		m_heart_full[i] = g_pRender->CreatePrimitive2D( 1024 );
//		m_heart_empty[i] = g_pRender->CreatePrimitive2D( 1024 );

		m_heart_vertics[i][0].v1.Pos = Math::Vector4D( 10.0f , 10.0f+32*i , 0 , 1.0f );
		m_heart_vertics[i][0].v2.Pos = Math::Vector4D( 42.0f , 10.0f+32*i , 0 , 1.0f );
		m_heart_vertics[i][0].v3.Pos = Math::Vector4D( 42.0f , 42.0f+32*i , 0 , 1.0f );
		m_heart_vertics[i][1].v1.Pos = Math::Vector4D( 42.0f , 42.0f+32*i , 0 , 1.0f );
		m_heart_vertics[i][1].v2.Pos = Math::Vector4D( 10.0f , 42.0f+32*i , 0 , 1.0f );
		m_heart_vertics[i][1].v3.Pos = Math::Vector4D( 10.0f , 10.0f+32*i , 0 , 1.0f );

		m_heart_vertics[i][0].v1.Col = m_heart_vertics[i][0].v2.Col = m_heart_vertics[i][0].v3.Col = CColor(0xff,0xff,0xff);
		m_heart_vertics[i][1].v1.Col = m_heart_vertics[i][1].v2.Col = m_heart_vertics[i][1].v3.Col = CColor(0xff,0xff,0xff);
	}

	m_point_vertex[0].v1.Pos = Math::Vector4D( 240.0f , 4.0f , 0 , 1.0f );
	m_point_vertex[0].v2.Pos = Math::Vector4D( 272.0f , 4.0f , 0 , 1.0f );
	m_point_vertex[0].v3.Pos = Math::Vector4D( 272.0f , 36.0f , 0 , 1.0f );
	m_point_vertex[1].v1.Pos = Math::Vector4D( 272.0f , 36.0f , 0 , 1.0f );
	m_point_vertex[1].v2.Pos = Math::Vector4D( 240.0f , 36.0f , 0 , 1.0f );
	m_point_vertex[1].v3.Pos = Math::Vector4D( 240.0f , 4.0f , 0 , 1.0f );
	m_point_vertex[0].v1.Col = m_point_vertex[0].v2.Col = m_point_vertex[0].v3.Col = CColor(0xff,0xff,0xff);
	m_point_vertex[1].v1.Col = m_point_vertex[1].v2.Col = m_point_vertex[1].v3.Col = CColor(0xff,0xff,0xff);
	m_point_vertex[0].v1.Tex[0] = m_point_vertex[1].v3.Tex[0] = GameSystem::game_info.point_item_uv.leftup;
	m_point_vertex[0].v3.Tex[0] = m_point_vertex[1].v1.Tex[0] = GameSystem::game_info.point_item_uv.rightbottom;
	m_point_vertex[0].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.point_item_uv.rightbottom.x , GameSystem::game_info.point_item_uv.leftup.y );
	m_point_vertex[1].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.point_item_uv.leftup.x , GameSystem::game_info.point_item_uv.rightbottom.y );

}

DrawStatus::~DrawStatus()
{
//	int i;
//	for( i=0 ; i<MAX_HP ; i++ )
//	{
//		SAFE_RELEASE(m_heart_full[i]);
//		SAFE_RELEASE(m_heart_empty[i]);
//	}
	SAFE_RELEASE(m_heart_full);
	SAFE_RELEASE(m_heart_empty);
	SAFE_RELEASE(m_point_item);
}

void DrawStatus::run()
{

	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_NUMBERS];

	char str[512];
	sprintf( str , "%08d" , Character::udonge->GetPoint() );
	font->DrawString( str , Math::Point2DF(65,10) , CColor(0xff,0xff,0xff) );

	sprintf( str , "X %02d" , Character::udonge->GetCoin() );
	font->DrawString( str , Math::Point2DF(277,10) , CColor(0xff,0xff,0xff) );

	font->DrawString( "WORLD" , Math::Point2DF(400,10) , CColor(0xff,0xff,0xff) );
	sprintf( str , "%d - %d"  , m_world , m_course );
	font->DrawString( str , Math::Point2DI(400,34) , CColor(0xff,0xff,0xff) );

	font->DrawString( "TIME" , Math::Point2DF(540,10) , CColor(0xff,0xff,0xff) );
	sprintf( str , "%03d" , m_main_task->GetRestTime()/GameSystem::GAME_FPS );
	font->DrawString( str , Math::Point2DF(558,34) , CColor(0xff,0xff,0xff) );

	Math::Point2DI size;
	sprintf( str , "%.1fFPS" , GameSystem::pCore->GetFPS() );
	size = font->GetStringSize( str );
	font->DrawString( str , Math::Point2DF(GameSystem::SCREEN_WIDTH-size.x-10,GameSystem::SCREEN_HEIGHT-size.y-10) , CColor(0xff,0xff,0xff,0xff/2) );


	GameSystem::PRIMITIVE2D_SET set;
	int i;
	int hp = Character::udonge->GetHP();

	m_heart_full->Begin();
	for( i=0 ; i<hp ; i++ )
	{
		m_heart_vertics[MAX_HP-i-1][0].v1.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][0].v1.Tex[1] =*/ GameSystem::game_info.heart_full_uv.leftup;
		m_heart_vertics[MAX_HP-i-1][0].v3.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][0].v3.Tex[1] =*/ GameSystem::game_info.heart_full_uv.rightbottom;
		m_heart_vertics[MAX_HP-i-1][1].v1.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][1].v1.Tex[1] =*/ GameSystem::game_info.heart_full_uv.rightbottom;
		m_heart_vertics[MAX_HP-i-1][1].v3.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][1].v3.Tex[1] =*/ GameSystem::game_info.heart_full_uv.leftup;
		m_heart_vertics[MAX_HP-i-1][0].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.heart_full_uv.rightbottom.x , GameSystem::game_info.heart_full_uv.leftup.y );
//		m_heart_vertics[MAX_HP-i-1][0].v2.Tex[1] = m_heart_vertics[MAX_HP-i-1][0].v2.Tex[0];
		m_heart_vertics[MAX_HP-i-1][1].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.heart_full_uv.leftup.x , GameSystem::game_info.heart_full_uv.rightbottom.y );
//		m_heart_vertics[MAX_HP-i-1][1].v2.Tex[1] = m_heart_vertics[MAX_HP-i-1][1].v2.Tex[0];

		m_heart_full->Push( m_heart_vertics[MAX_HP-i-1] , 2 );
	}
	m_heart_full->End();
	m_heart_empty->Begin();
	for( i=hp ; i<MAX_HP ; i++ )
	{
		m_heart_vertics[MAX_HP-i-1][0].v1.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][0].v1.Tex[1] =*/ GameSystem::game_info.heart_empty_uv.leftup;
		m_heart_vertics[MAX_HP-i-1][0].v3.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][0].v3.Tex[1] =*/ GameSystem::game_info.heart_empty_uv.rightbottom;
		m_heart_vertics[MAX_HP-i-1][1].v1.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][1].v1.Tex[1] =*/ GameSystem::game_info.heart_empty_uv.rightbottom;
		m_heart_vertics[MAX_HP-i-1][1].v3.Tex[0] = /*m_heart_vertics[MAX_HP-i-1][1].v3.Tex[1] =*/ GameSystem::game_info.heart_empty_uv.leftup;
		m_heart_vertics[MAX_HP-i-1][0].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.heart_empty_uv.rightbottom.x , GameSystem::game_info.heart_empty_uv.leftup.y );
//		m_heart_vertics[MAX_HP-i-1][0].v2.Tex[1] = m_heart_vertics[MAX_HP-i-1][0].v2.Tex[0];
		m_heart_vertics[MAX_HP-i-1][1].v2.Tex[0] = Math::Vector2D( GameSystem::game_info.heart_empty_uv.leftup.x , GameSystem::game_info.heart_empty_uv.rightbottom.y );
//		m_heart_vertics[MAX_HP-i-1][1].v2.Tex[1] = m_heart_vertics[MAX_HP-i-1][1].v2.Tex[0];

		m_heart_empty->Push( m_heart_vertics[MAX_HP-i-1] , 2 );
	}
	m_heart_empty->End();

	m_point_item->Begin();
	m_point_item->Push( m_point_vertex , 2 );
	m_point_item->End();
	
	set.pPrimitive = m_heart_full;
	set.setTexture = m_heart_full_tex;
	set.pTargetTexture = NULL;
	GameSystem::pPrimitive2D.push_back( set );

	set.pPrimitive = m_heart_empty;
	set.setTexture = m_heart_empty_tex;
	set.pTargetTexture = NULL;
	GameSystem::pPrimitive2D.push_back( set );

	set.pPrimitive = m_point_item;
	set.setTexture = m_point_item_tex;
	set.pTargetTexture = NULL;
	GameSystem::pPrimitive2D.push_back( set );

//	GameSystem::pScene->CreateActor
//	GameSystem::pRender->SetDepthTestEnable( true );
}
