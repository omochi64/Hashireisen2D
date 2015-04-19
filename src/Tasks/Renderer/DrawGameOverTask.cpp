
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"
#include "DrawGameOverTask.h"
#include "Tasks/MainMenu.h"
#include "System/Sounds.h"

DrawGameOverTask::DrawGameOverTask( GameMain *main_task , DWORD wait_frame )
{
	m_main_task = main_task;
	m_wait_frame = wait_frame;
	m_past_frame = 0;

	m_str_size = GameSystem::pFonts[GameSystem::FONT_NUMBERS]->GetStringSize( "GAME OVER" );

	GameSound::sound_manager->Play( GameSound::game_over_bgm );

}


void DrawGameOverTask::run()
{
	// GAME OVER‚Ì•`‰æ
	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_NUMBERS];

	font->DrawString( "GAME OVER" , Math::Point2DF( GameSystem::SCREEN_WIDTH-m_str_size.x , GameSystem::SCREEN_HEIGHT-m_str_size.y )/2 , CColor(0xff,0xff,0xff) );

	m_past_frame++;

	// ŠÔ‚ª‚½‚Á‚½‚çƒƒCƒ“ƒƒjƒ…[‚É‘JˆÚ‚·‚é
	if( m_past_frame >= m_wait_frame )
	{
		GameSound::sound_manager->Stop( GameSound::game_over_bgm );

		GameSystem::draw_task->Release();
		GameSystem::all_task->Release();

		// ‚±‚±‚Å–{—ˆ‚È‚çƒƒCƒ“ƒƒjƒ…[‚É‘JˆÚ
		GameSystem::all_task->AddTask( new MainMenu() , 0x01 );
	}
}
