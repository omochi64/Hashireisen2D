
#include "DrawGoalMessages.h"
#include "System/IniInfo.h"

DrawGoalMessages::DrawGoalMessages( AfterGoaledTask *agt )
{
	m_contact_task = agt;
	draw_pos1.x = 20;
	draw_pos1.y = 70;
	draw_pos2.x = 220;
	draw_pos2.y = 250;
	is_animating = true;
	m_now_line = 1;
	m_show_string_line1[0] = '\0';
	m_show_string_line2[0] = '\0';
	m_start_time = GameSystem::now_frame_time;
	m_pause_time = 0;
}

static char stage[] = "STAGE";
static char clear[] = "CLEAR!!";

void DrawGoalMessages::run()
{
	if( is_animating )
	{
		unsigned int len1 = strlen( m_show_string_line1 );
		unsigned int len2 = strlen( m_show_string_line2 );
			
		if( m_now_line == 1 )
		{
			if( len1*animation_duration < GameSystem::now_frame_time-m_start_time )
			{
				m_show_string_line1[len1] = stage[len1];
				m_show_string_line1[len1+1] = '\0';
				if( len1 == strlen( stage ) )
				{
					m_now_line = 2;
				}
			}
		}
		else if( m_now_line == 2 )
		{
			if( (len1+len2)*animation_duration < GameSystem::now_frame_time-m_start_time )
			{
				m_show_string_line2[len2] = clear[len2];
				m_show_string_line2[len2+1] = '\0';
				if( len2 == strlen( clear ) )
				{
					m_contact_task->startRunningUdonge();
					is_animating = false;
				}
			}
		}
	}

	Renderer::Object::IFontSprite2D *font = GameSystem::pFonts[GameSystem::FONT_STAGECLEAR];
	font->DrawString( m_show_string_line1 , draw_pos1 , CColor(0xff,0xff,0xff) );
	font->DrawString( m_show_string_line2 , draw_pos2 , CColor(0xff,0xff,0xff) );
}

// ˆêŽž’âŽ~AÄŠJ
void DrawGoalMessages::Pause()
{
	if( is_animating && m_pause_time == 0 )
	{
		m_pause_time = GameSystem::now_frame_time;
		is_animating = false;
	}
}

void DrawGoalMessages::Resume()
{
	if( !is_animating && m_pause_time > 0 )
	{
		m_start_time += GameSystem::now_frame_time-m_pause_time;
		is_animating = true;
		m_pause_time = 0;
	}
}
