
#include "EndingScript.h"
#include "System/FileManagerSupporter.h"

// 中間言語書式
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
//	texture_number,primitive_numberはLoad時に割り当てる
//


// *waitコマンドに関するオプション引数用列挙型
typedef enum _script_wait{
	SCRIPT_WAIT_FRAME , SCRIPT_WAIT_DID_PROCESSED
}SCRIPT_WAIT;

// *showコマンドに関するオプション引数用列挙型
typedef enum _script_show{
	SCRIPT_SHOW_TEX_NUM , SCRIPT_SHOW_PRIMITIVE_NUM , SCRIPT_SHOW_LEFT_X ,
	SCRIPT_SHOW_WIDTH , SCRIPT_SHOW_HEIGHT
}SCRIPT_SHOW;

// コンストラクタ
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

// デストラクタ
EndingScript::~EndingScript()
{
	Release();
}

// フォント
void EndingScript::SetFont( Renderer::Object::IFontSprite2D *pFont )
{
	m_pFont = pFont;
}

// ファイルから読み込み
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

	// 背景用プリミティブを作っておく
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

		// ステータス読み込み
		/*
		FontSize=フォントサイズ(プログラムの都合上プロポーショナルフォントがよさげです)
		StringLeft=文字描画をする際、そろえる左端のX座標
		ScrollSpeed=スクロールスピード(pixel/msec)
		StopWhenFinished=0 or 1
		Music=BGMファイル名
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
			// フォントサイズ
			temp = strtok( NULL , "\r\n" );
			m_font_size = atoi(temp);
		}
		else if( strcmp( temp , "StringLeft" ) == 0 )
		{
			// 文字左端X
			temp = strtok( NULL , "\r\n" );
			m_left_x = (float)atof(temp);
		}
		else if( strcmp( temp , "ScrollSpeed" ) == 0 )
		{
			// スクロールスピード
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
	
	// スクリプト本文
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
			// コマンド
			if( strstr( str , "*wait" ) == str )
			{
				// waitコマンド
				// *wait,1000
				temp = strtok( str , "," );
				strcpy( add_line.string , temp );
				temp = strtok( NULL , "\r\n" );
				add_line.option[SCRIPT_WAIT_FRAME] = atoi(temp);
				add_line.option[SCRIPT_WAIT_DID_PROCESSED] = 0;
			}
			else if( strstr( str , "*show" ) == str )
			{
				// showコマンド
				// *show,表示画像名,表示位置左x,表示幅,表示高さ
				temp = strtok( str , "," );
				strcpy( add_line.string , temp );
				temp = strtok( NULL , "," );
				// 画像読み込み
				texture = tex_manager->GetAt( temp );
				if( !texture )
				{
					texture = tex_manager->AddTexture( temp , pRender );
					if( !texture )
					{
						// テクスチャ読み込み失敗。この行はなかったことにする
						continue;
					}
				}
				// プリミティブ生成
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

				// 左X
				temp = strtok( NULL , "," );
				add_line.option[SCRIPT_SHOW_LEFT_X] = atoi(temp);
				// 幅
				temp = strtok( NULL , "," );
				add_line.option[SCRIPT_SHOW_WIDTH] = atoi(temp);
				// 高さ
				temp = strtok( NULL , "," );
				add_line.option[SCRIPT_SHOW_HEIGHT] = atoi(temp);
			}
		}
		else if( strcmp( str , "\\\\" ) == 0 )
		{
			// 改行コマンド
			add_line.string[0] = '\0';
		}
		else
		{
			// 通常文章
			strcpy( add_line.string , str );
		}

		lines.push_back( add_line );
	}

	SAFE_RELEASE(fp);

	// メモリ確保、コピー
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

// スクリプトアニメーションスタート
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
			// *wait実行
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

// スクリプトアニメーションアップデート
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
		// *wait実行中
		// ここはmsecじゃなくてフレームに直したほうがいいかな？
		// それならspeedupによるwaitのスキップ処理も簡単になる
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

	// スクリプトを進める
	m_nextShowLineYPos -= /*past_time*/m_scroll_speed*m_scroll_speed_times;

	float pos_bottom = m_nextShowLineYPos;
	
	// 表示位置、表示行の補正
	while(m_nextShowLine < m_lineNum)
	{
		// 次は画像か文字か？
		if( strcmp( m_script[m_nextShowLine].string , "*show" ) == 0 )
		{
			// 画像の表示
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

	// 現在表示しえる範囲の中に*waitがあるか？
	pos_bottom = m_nextShowLineYPos;

	int i=0;
	while( pos_bottom <= m_screen_height && m_nextShowLine+i < m_lineNum )
	{
		if( strcmp( m_script[m_nextShowLine+i].string , "*wait" ) == 0 &&
			m_script[m_nextShowLine+i].option[SCRIPT_WAIT_DID_PROCESSED] == 0 )
		{
			// *wait実行
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

// 最後まで来たか？
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

// 描画
void EndingScript::Draw()
{
	// 頭から順に描画

	float pos_bottom = m_nextShowLineYPos;

	m_draw_primitives.clear();
	m_draw_textures.clear();

	int width,height;
	int left;
	Renderer::Object::IPrimitive2D *primitive;
	Renderer::ITexture *texture;
	Renderer::SPrimitiveVertex2D vertex[2];

	// 背景情報
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
			// vertexの作成
			primitive = m_primitives[m_script[m_nextShowLine+i].option[SCRIPT_SHOW_PRIMITIVE_NUM]];
			texture = m_textures[m_script[m_nextShowLine+i].option[SCRIPT_SHOW_TEX_NUM]];
			if( !primitive )
			{
				// primitiveがない。よって無効
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


// スクリプトの状態を初期化
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

// 資源解放
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

// スクロールスピードを指定された数だけ倍する
void EndingScript::SpeedUpTimes( int speed_up_times )
{
	if( speed_up_times >= 1 )
	{
		m_scroll_speed_times = speed_up_times;
	}
}

// スクロールスピードを元に戻す
void EndingScript::ResetSpeed( void )
{
	m_scroll_speed_times = 1;
}
	

// 描画関連情報取得
vector<Renderer::Object::IPrimitive2D *> &EndingScript::GetDrawPrimitives()
{
	return m_draw_primitives;
}

vector<Renderer::ITexture *> &EndingScript::GetDrawTextures()
{
	return m_draw_textures;
}

// BGM情報取得
const char *EndingScript::GetSoundName()
{
	return m_endingBGM;
}

// BGM操作
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
