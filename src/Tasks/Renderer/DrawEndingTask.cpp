
#include "DrawEndingTask.h"
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"

DrawEndingTask::DrawEndingTask( EndingScript *pDrawEnding )
{
	m_pDrawEnding = pDrawEnding;
}

DrawEndingTask::~DrawEndingTask()
{
}

void DrawEndingTask::run()
{
	m_pDrawEnding->Draw();

	GameSystem::PRIMITIVE2D_SET set;

	std::vector<Renderer::Object::IPrimitive2D *> &draw_list = m_pDrawEnding->GetDrawPrimitives();
	std::vector<Renderer::ITexture *> &draw_tex_list = m_pDrawEnding->GetDrawTextures();

	set.pTargetTexture = NULL;

	int i,size = draw_list.size();
	for( i=0 ; i<size ; i++ )
	{
		set.pPrimitive = draw_list[i];
		set.setTexture = draw_tex_list[i];
		GameSystem::pPrimitive2D.push_back( set );		
	}

	if( m_pDrawEnding->IsScriptEnd() )
	{
		Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_NUMBERS];
//		font->Begin(GameSystem::fonts_spaces[FONT_NUMBERS]);

		Math::Point2DF size;
		size = font->GetStringSize( "PRESS ANY KEY" );
		font->DrawString( "PRESS ANY KEY" , Math::Point2DF( GameSystem::SCREEN_WIDTH-size.x-10 , GameSystem::SCREEN_HEIGHT-size.y-10 ) ,
			CColor(0xff,0xff,0xff) );
//		font->End();
	}
}
