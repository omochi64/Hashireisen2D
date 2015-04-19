
#include "EndingScript.h"
#include "System/FileManagerSupporter.h"

// ���Ԍ��ꏑ��
//	string = "*wait"
//	 option[0]:wait_msec
//	 option[1]:did_processed
//	string = "*show"
//	 option[0]:texture_number
//	 option[1]:primitive_number
//	 option[2]:show_left_x
//	 option[3]:width
//	 option[4]:height
//
//	texture_number,primitive_number��Load���Ɋ��蓖�Ă�
//


// *wait�R�}���h�Ɋւ���I�v�V���������p�񋓌^
typedef enum _script_wait{
	SCRIPT_WAIT_FRAME , SCRIPT_WAIT_DID_PROCESSED
}SCRIPT_WAIT;

// *show�R�}���h�Ɋւ���I�v�V���������p�񋓌^
typedef enum _script_show{
	SCRIPT_SHOW_TEX_NUM , SCRIPT_SHOW_PRIMITIVE_NUM , SCRIPT_SHOW_LEFT_X ,
	SCRIPT_SHOW_WIDTH , SCRIPT_SHOW_HEIGHT
}SCRIPT_SHOW;

// �R���X�g���N�^
EndingScript::EndingScript( Renderer::Object::IFontSprite2D *pFont , int screen_width , int screen_height )
{
	m_primitives.clear();
	m_draw_primitives.clear();
	m_textures.clear();
	m_draw_textures.clear();

	m_screen_height = screen_height;
	m_screen_width = screen_width;
	m_pFont = pFont;
	m_left_x = 0.0f;
	m_scroll_speed = DEFAULT_SCROLL_SPEED;	// default
	m_bgColor = CColor(0,0,0);
	m_textColor = CColor( 0xff , 0xff , 0xff );
	m_nextShowLine = 0;
	m_nextShowLineYPos = 0.0f;
	m_is_waiting = false;
	m_is_animating = false;
	m_do_stop_when_finished = false;
//	m_start_wait_frame = 0;
	m_rest_wait_frame = 0;
//	m_lastUpdatedTime = 0;
	m_scroll_speed_times = 1;
	m_pastFrames = 0;

	m_endingBGM[0] = '\0';


	m_lineNum = 0;
	m_script = NULL;
}
EndingScript::EndingScript( int screen_width , int screen_height)
{
	m_primitives.clear();
	m_draw_primitives.clear();
	m_textures.clear();
	m_draw_textures.clear();

	m_pFont = NULL;
	m_left_x = 0.0f;
	m_scroll_speed = DEFAULT_SCROLL_SPEED;	// default
	m_bgColor = CColor(0,0,0);
	m_textColor = CColor( 0xff , 0xff , 0xff );
	m_nextShowLine = 0;
	m_nextShowLineYPos = 0.0f;
	m_is_waiting = false;
	m_is_animating = false;
	m_do_stop_when_finished = false;
//	m_start_wait_frame = 0;
	m_rest_wait_frame = 0;
//	m_lastUpdatedTime = 0;
	m_scroll_speed_times = 1;
	m_pastFrames = 0;

	m_endingBGM[0] = '\0';

	m_screen_height = screen_height;
	m_screen_width = screen_width;
	
	m_lineNum = 0;
	m_script = NULL;
}

// �f�X�g���N�^
EndingScript::~EndingScript()
{
	Release();
}

// �t�H���g
void EndingScript::SetFont( Renderer::Object::IFontSprite2D *pFont )
{
	m_pFont = pFont;
}

// �t�@�C������ǂݍ���
bool EndingScript::LoadFromFile( char *fname , Renderer::IRender *pRender , CTextureManager *tex_manager ,
								SoundManager *sound_manager , ICore *pCore )
{
	File::IFileManager *pFileManager = pCore->GetFileMgrPointer();

//	FILE *fp = fopen( fname , "r" );
	pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = pFileManager->FileOpen( fname );
	pFileManager->SetCurrentPath( "Texture" );
	if(fp->IsInvalid())
	{
		SAFE_RELEASE(fp);
		return false;
	}

//	File::IFileManager *pFileManager = pCore->GetFileMgrPointer();

	// �w�i�p�v���~�e�B�u������Ă���
	Renderer::Object::IPrimitive2D *backPrimitive = pRender->CreatePrimitive2D(12);
	Renderer::ITexture *texture = NULL;
	m_primitives.push_back( backPrimitive );
	m_textures.push_back( texture );

	char str[512];
	char *temp;

	while( GameSystem::Supporter::fgets_from_fr( str , 511 , fp ) != NULL )
	{
		str[strlen(str)-1] = '\0';

		if( strlen(str) == 0 )
		{
			continue;
		}

		if( strcmp( str , "*begin" ) == 0 )
		{
			break;
		}

		// �X�e�[�^�X�ǂݍ���
		/*
		FontSize=�t�H���g�T�C�Y(�v���O�����̓s����v���|�[�V���i���t�H���g���悳���ł�)
		StringLeft=�����`�������ہA���낦�鍶�[��X���W
		ScrollSpeed=�X�N���[���X�s�[�h(pixel/msec)
		StopWhenFinished=0 or 1
		Music=BGM�t�@�C����
		BGColor=R,G,B
		TextColor=R,G,B
		*/

		temp = strtok( str , "=" );
		if( !temp )
		{
			continue;
		}

		if( strcmp( temp , "FontSize" ) == 0 )
		{
			// �t�H���g�T�C�Y
			temp = strtok( NULL , "\r\n" );
			m_font_size = atoi(temp);
		}
		else if( strcmp( temp , "StringLeft" ) == 0 )
		{
			// �������[X
			temp = strtok( NULL , "\r\n" );
			m_left_x = (float)atof(temp);
		}
		else if( strcmp( temp , "ScrollSpeed" ) == 0 )
		{
			// �X�N���[���X�s�[�h
			temp = strtok( NULL , "\r\n" );
			m_scroll_speed = (float)atof(temp);
		}
		else if( strcmp( temp , "StopWhenFinished" ) == 0 )
		{
			temp = strtok( NULL , "\r\n" );
			if( atoi(temp) == 1 )
			{
				m_do_stop_when_finished = true;
			}
			else
			{
				m_do_stop_when_finished = false;
			}
		}
		else if( strcmp( temp , "Music" ) == 0 )
		{
			// BGM
			temp = strtok( NULL , "\r\n" );
			if( !sound_manager->GetSound( temp ) )
			{
				pFileManager->SetCurrentPath( "Sound" );
				if( sound_manager->LoadSound( pCore , temp , "OggVorbis" ) )
				{
					strcpy( m_endingBGM , temp );
				}
				pFileManager->SetCurrentPath( "Texture" );
			}
			else
			{
				strcpy( m_endingBGM , temp );
			}
		}
		else if( strcmp( temp , "BGColor" ) == 0 )
		{
			// BGColor=R,G,B
			int r,g,b;
			r = atoi(strtok(NULL,","));
			g = atoi(strtok(NULL,","));
			b = atoi(strtok(NULL,","));
			m_bgColor = CColor(r,g,b);
		}
		else if( strcmp( temp , "TextColor" ) == 0 )
		{
			// TextColor=R,G,B
			int r,g,b;
			r = atoi(strtok(NULL,","));
			g = atoi(strtok(NULL,","));
			b = atoi(strtok(NULL,","));
			m_textColor = CColor(r,g,b);
		}

	}

	std::vector<SCRIPT_LINE>	lines;

	SCRIPT_LINE	add_line;

	pFileManager->SetCurrentPath( "Texture" );
	
	// �X�N���v�g�{��
	while( GameSystem::Supporter::fgets_from_fr( str , 511 , fp ) != NULL )
	{
		str[strlen(str)-1] = '\0';

		if( strlen(str) == 0 )
		{
			continue;
		}

		if( strcmp( str , "*end" ) == 0 )
		{
			break;
		}

		if( str[0] == '*' )
		{
			// �R�}���h
			if( strstr( str , "*wait" ) == str )
			{
				// wait�R�}���h
				// *wait,1000
				temp = strtok( str , "," );
				strcpy( add_line.string , temp );
				temp = strtok( NULL , "\r\n" );
				add_line.option[SCRIPT_WAIT_FRAME] = atoi(temp);
				add_line.option[SCRIPT_WAIT_DID_PROCESSED] = 0;
			}
			else if( strstr( str , "*show" ) == str )
			{
				// show�R�}���h
				// *show,�\���摜��,�\���ʒu��x,�\����,�\������
				temp = strtok( str , "," );
				strcpy( add_line.string , temp );
				temp = strtok( NULL , "," );
				// �摜�ǂݍ���
				texture = tex_manager->GetAt( temp );
				if( !texture )
				{
					texture = tex_manager->AddTexture( temp , pRender );
					if( !texture )
					{
						// �e�N�X�`���ǂݍ��ݎ��s�B���̍s�͂Ȃ��������Ƃɂ���
						continue;
					}
				}
				// �v���~�e�B�u����
				Renderer::Object::IPrimitive2D *primitive;
				primitive = pRender->CreatePrimitive2D( 12 );
				if( !primitive )
				{
					continue;
				}
				m_textures.push_back( texture );
				m_primitives.push_back( primitive );
				add_line.option[SCRIPT_SHOW_TEX_NUM] = m_textures.size()-1;
				add_line.option[SCRIPT_SHOW_PRIMITIVE_NUM] = m_primitives.size()-1;

				// ��X
				temp = strtok( NULL , "," );
				add_line.option[SCRIPT_SHOW_LEFT_X] = atoi(temp);
				// ��
				temp = strtok( NULL , "," );
				add_line.option[SCRIPT_SHOW_WIDTH] = atoi(temp);
				// ����
				temp = strtok( NULL , "," );
				add_line.option[SCRIPT_SHOW_HEIGHT] = atoi(temp);
			}
		}
		else if( strcmp( str , "\\\\" ) == 0 )
		{
			// ���s�R�}���h
			add_line.string[0] = '\0';
		}
		else
		{
			// �ʏ핶��
			strcpy( add_line.string , str );
		}

		lines.push_back( add_line );
	}

	SAFE_RELEASE(fp);

	// �������m�ہA�R�s�[
	m_lineNum = lines.size();
	m_script = new SCRIPT_LINE[m_lineNum];
	int i;
	for( i=0 ; i<m_lineNum ; i++ )
	{
		memcpy( &m_script[i] , &lines[i] , sizeof(SCRIPT_LINE) );
	}

	m_backVertex[0].v1.Pos = m_backVertex[1].v3.Pos = Math::Vector4D( 0.0f , 0.0f , 0.0f , 1.0f );
	m_backVertex[1].v1.Pos = m_backVertex[0].v3.Pos = Math::Vector4D( (float)m_screen_width , (float)m_screen_height , 0.0f , 1.0f );
	m_backVertex[0].v2.Pos = Math::Vector4D( (float)m_screen_width , 0.0f , 0.0f , 1.0f );
	m_backVertex[1].v2.Pos = Math::Vector4D( 0.0f , (float)m_screen_height , 0.0f , 1.0f );

	m_backVertex[0].v1.Col = m_backVertex[0].v2.Col = m_backVertex[0].v3.Col = m_bgColor;
	m_backVertex[1].v1.Col = m_backVertex[1].v2.Col = m_backVertex[1].v3.Col = m_bgColor;
	

	return true;
}

// �X�N���v�g�A�j���[�V�����X�^�[�g
void EndingScript::StartScript()
{
	m_is_animating = true;
//	m_lastUpdatedTime = now_time;

	float pos_bottom = m_nextShowLineYPos;

	m_pastFrames = 0;

	int i=0;
	while( pos_bottom <= m_screen_height && m_nextShowLine+i < m_lineNum )
	{
		if( strcmp( m_script[m_nextShowLine+i].string , "*wait" ) == 0 &&
			m_script[m_nextShowLine+i].option[SCRIPT_WAIT_DID_PROCESSED] == 0 )
		{
			// *wait���s
			m_is_waiting = true;
			m_script[m_nextShowLine+i].option[SCRIPT_WAIT_DID_PROCESSED] = 1;
//			m_start_wait_frame = 0;
			m_rest_wait_frame = m_script[m_nextShowLine+i].option[SCRIPT_WAIT_FRAME];
			break;
		}
		else if( strcmp( m_script[m_nextShowLine+i].string , "*show" ) == 0 )
		{
			pos_bottom += m_script[m_nextShowLine+i].option[SCRIPT_SHOW_HEIGHT];
		}
		else
		{
			pos_bottom += m_font_size;
		}

		i++;
	}
}

// �X�N���v�g�A�j���[�V�����A�b�v�f�[�g
bool EndingScript::UpdateScript()
{
	if( !m_is_animating )
	{
		return false;
	}

	m_pastFrames++;

//	DWORD past_time = now_time-m_lastUpdatedTime;

	if( m_is_waiting )
	{
		// *wait���s��
		// ������msec����Ȃ��ăt���[���ɒ������ق����������ȁH
		// ����Ȃ�speedup�ɂ��wait�̃X�L�b�v�������ȒP�ɂȂ�
		m_rest_wait_frame -= m_scroll_speed_times;
		if( m_rest_wait_frame <= 0 )
		{
			if( IsScriptEnd() )
			{
				if( m_do_stop_when_finished )
				{
					m_is_animating = false;
				}
				return false;
			}
			else
			{
				m_is_waiting = false;
//				m_start_wait_frame = 0;
				m_rest_wait_frame = 0;
			}
		}
		else
		{
//			m_lastUpdatedTime = now_time;
			return true;
		}
	}

	// �X�N���v�g��i�߂�
	m_nextShowLineYPos -= /*past_time*/m_scroll_speed*m_scroll_speed_times;

	float pos_bottom = m_nextShowLineYPos;
	
	// �\���ʒu�A�\���s�̕␳
	while(m_nextShowLine < m_lineNum)
	{
		// ���͉摜���������H
		if( strcmp( m_script[m_nextShowLine].string , "*show" ) == 0 )
		{
			// �摜�̕\��
			pos_bottom += m_script[m_nextShowLine].option[SCRIPT_SHOW_HEIGHT];
		}
		else if( strcmp( m_script[m_nextShowLine].string , "*wait" ) != 0 )
		{
			pos_bottom += m_font_size;
		}
		if( pos_bottom < 0 )
		{
			m_nextShowLine++;
			m_nextShowLineYPos = pos_bottom;
			continue;
		}
		break;
	}

	// ���ݕ\��������͈͂̒���*wait�����邩�H
	pos_bottom = m_nextShowLineYPos;

	int i=0;
	while( pos_bottom <= m_screen_height && m_nextShowLine+i < m_lineNum )
	{
		if( strcmp( m_script[m_nextShowLine+i].string , "*wait" ) == 0 &&
			m_script[m_nextShowLine+i].option[SCRIPT_WAIT_DID_PROCESSED] == 0 )
		{
			// *wait���s
			m_is_waiting = true;
			m_script[m_nextShowLine+i].option[SCRIPT_WAIT_DID_PROCESSED] = 1;
//			m_start_wait_time = m_pastFrames;
			m_rest_wait_frame = m_script[m_nextShowLine+i].option[SCRIPT_WAIT_FRAME];
			break;
		}
		else if( strcmp( m_script[m_nextShowLine+i].string , "*show" ) == 0 )
		{
			pos_bottom += m_script[m_nextShowLine+i].option[SCRIPT_SHOW_HEIGHT];
		}
		else
		{
			pos_bottom += m_font_size;
		}

		i++;
	}

//	m_lastUpdatedTime = now_time;

	if( !m_is_waiting && m_nextShowLine+i >= m_lineNum )
	{
		if( m_do_stop_when_finished )
		{
			m_is_animating = false;
		}
		return false;
	}

	return true;
}

// �Ō�܂ŗ������H
bool EndingScript::IsScriptEnd()
{
	if( m_is_waiting )
	{
		return false;
	}

	float pos_bottom = m_nextShowLineYPos;

	int i=0;
	while( pos_bottom <= m_screen_height && m_nextShowLine+i < m_lineNum )
	{
		if( strcmp( m_script[m_nextShowLine+i].string , "*show" ) == 0 )
		{
			pos_bottom += m_script[m_nextShowLine+i].option[SCRIPT_SHOW_HEIGHT];
		}
		else if( strcmp( m_script[m_nextShowLine+i].string , "*wait" ) != 0 )
		{
			pos_bottom += m_font_size;
		}

		i++;
	}

	if( m_nextShowLine+i >= m_lineNum )
	{
		return true;
	}

	return false;
}

// �`��
void EndingScript::Draw()
{
	// �����珇�ɕ`��

	float pos_bottom = m_nextShowLineYPos;

	m_draw_primitives.clear();
	m_draw_textures.clear();

	int width,height;
	int left;
	Renderer::Object::IPrimitive2D *primitive;
	Renderer::ITexture *texture;
	Renderer::SPrimitiveVertex2D vertex[2];

	// �w�i���
	primitive = m_primitives[0];
	texture = m_textures[0];
	primitive->Begin();
	primitive->Push( m_backVertex , 2 );
	primitive->End();
	m_draw_primitives.push_back( primitive );
	m_draw_textures.push_back( texture );

	int i=0;
	while( pos_bottom <= m_screen_height && m_nextShowLine+i < m_lineNum )
	{
		if( strcmp( m_script[m_nextShowLine+i].string , "*show" ) == 0 )
		{
			// vertex�̍쐬
			primitive = m_primitives[m_script[m_nextShowLine+i].option[SCRIPT_SHOW_PRIMITIVE_NUM]];
			texture = m_textures[m_script[m_nextShowLine+i].option[SCRIPT_SHOW_TEX_NUM]];
			if( !primitive )
			{
				// primitive���Ȃ��B����Ė���
				i++;
				continue;
			}
			width = m_script[m_nextShowLine+i].option[SCRIPT_SHOW_WIDTH];
			height = m_script[m_nextShowLine+i].option[SCRIPT_SHOW_HEIGHT];
			left = m_script[m_nextShowLine+i].option[SCRIPT_SHOW_LEFT_X];
			vertex[0].v1.Pos = vertex[1].v3.Pos = Math::Vector4D( left , pos_bottom , 0.0f , 1.0f );
			vertex[1].v1.Pos = vertex[0].v3.Pos = Math::Vector4D( left+width , pos_bottom+height , 0.0f , 1.0f );
			vertex[0].v2.Pos = Math::Vector4D( left+width , pos_bottom , 0.0f , 1.0f );
			vertex[1].v2.Pos = Math::Vector4D( left , pos_bottom+height , 0.0f , 1.0f );

			vertex[0].v1.Col = vertex[0].v2.Col = vertex[0].v3.Col = CColor(0xff,0xff,0xff);
			vertex[1].v1.Col = vertex[1].v2.Col = vertex[1].v3.Col = CColor(0xff,0xff,0xff);

			vertex[0].v1.Tex[0] = vertex[1].v3.Tex[0] = Math::Vector2D( 0.0f , 0.0f );
			vertex[1].v1.Tex[0] = vertex[0].v3.Tex[0] = Math::Vector2D( 1.0f , 1.0f );
			vertex[0].v2.Tex[0] = Math::Vector2D( 1.0f , 0.0f );
			vertex[1].v2.Tex[0] = Math::Vector2D( 0.0f , 1.0f );

			primitive->Begin();
			primitive->Push( vertex , 2 );
			primitive->End();

			m_draw_primitives.push_back( primitive );
			m_draw_textures.push_back( texture );

			pos_bottom += m_script[m_nextShowLine+i].option[SCRIPT_SHOW_HEIGHT];
		}
		else if( strcmp( m_script[m_nextShowLine+i].string , "*wait" ) != 0 )
		{
			if( m_pFont )
			{
				m_pFont->DrawString( m_script[m_nextShowLine+i].string , Math::Point2DF( m_left_x , pos_bottom ) , m_textColor );
			}
			pos_bottom += m_font_size;
		}

		i++;
	}


}


// �X�N���v�g�̏�Ԃ�������
void EndingScript::ResetStatus()
{
	int i;
	for( i=0 ; i<m_lineNum ; i++ )
	{
		if( strcmp( m_script[i].string , "*wait" ) == 0 )
		{
			m_script[i].option[SCRIPT_WAIT_DID_PROCESSED] = 0;
		}
	}

	m_nextShowLine = 0;
	m_nextShowLineYPos = 0.0f;
	m_is_waiting = false;
	m_is_animating = false;
//	m_start_wait_frame = 0;
	m_pastFrames = 0;
//	m_lastUpdatedTime = 0;
	m_rest_wait_frame = 0;
}

// �������
void EndingScript::Release()
{
	delete [] m_script;
	m_lineNum = 0;

	int i,size = m_primitives.size();

	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(m_primitives[i]);
	}

	m_primitives.clear();
	m_draw_primitives.clear();
	m_textures.clear();
	m_draw_textures.clear();

	m_pFont = NULL;
	m_left_x = 0.0f;
	m_scroll_speed = DEFAULT_SCROLL_SPEED;	// default
	m_bgColor = CColor(0,0,0);
	m_textColor = CColor( 0xff , 0xff , 0xff );
	m_nextShowLine = 0;
	m_nextShowLineYPos = 0.0f;
	m_is_waiting = false;
	m_is_animating = false;
//	m_start_wait_time = 0;
//	m_lastUpdatedTime = 0;
	m_rest_wait_frame = 0;
	m_scroll_speed_times = 1;
	m_pastFrames = 0;
}

// �X�N���[���X�s�[�h���w�肳�ꂽ�������{����
void EndingScript::SpeedUpTimes( int speed_up_times )
{
	if( speed_up_times >= 1 )
	{
		m_scroll_speed_times = speed_up_times;
	}
}

// �X�N���[���X�s�[�h�����ɖ߂�
void EndingScript::ResetSpeed( void )
{
	m_scroll_speed_times = 1;
}
	

// �`��֘A���擾
vector<Renderer::Object::IPrimitive2D *> &EndingScript::GetDrawPrimitives()
{
	return m_draw_primitives;
}

vector<Renderer::ITexture *> &EndingScript::GetDrawTextures()
{
	return m_draw_textures;
}

// BGM���擾
const char *EndingScript::GetSoundName()
{
	return m_endingBGM;
}

// BGM����
void EndingScript::PlayBGM( SoundManager *sound_manager )
{
	sound_manager->Play( m_endingBGM , 0 );
}

void EndingScript::StopBGM( SoundManager *sound_manager )
{
	sound_manager->Stop( m_endingBGM );
}

void EndingScript::PauseBGM( SoundManager *sound_manager )
{
	sound_manager->Pause( m_endingBGM );
}
