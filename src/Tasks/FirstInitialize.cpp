
#include "Selene.h"
#include "InitializeGame.h"
#include "FirstInitialize.h"
#include "GameMain.h"
#include "System/Manager/3DModelManager.h"
#include "MainMenu.h"
#include "System/Sounds.h"
#include "System/IniInfo.h"
#include "System/GameSystem.h"

#include <time.h>

const char *font_files[] = {
	"00.sff" , "02.sff" , "03.sff" , "04.sff" , "05.sff" , NULL
};
int font_spaces[] = {
	0,20,0,0,0
};

FirstInitialize::FirstInitialize( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr , Scene::ICamera *pCamera , File::IFileManager *pFileManager )
{
	m_pRender = pRender;
	m_pSceneMgr = pSceneMgr;
	m_pCamera = pCamera;
	m_pFileManager = pFileManager;
}

void FirstInitialize::run()
{
	srand((unsigned)time(NULL));
	GameSystem::tex_manager = new CTextureManager();
	GameSystem::multiple_animation_manager = new CMultipleAnimationManager();
	GameSystem::course_manager = new CoursesManager();
	m_pFileManager->SetCurrentPath( "Text" );
	GameSystem::course_manager->Load( "courses.txt" , m_pFileManager);
	m_pFileManager->SetCurrentPath( "Texture" );
	GameSystem::model_manager = new CModelManager(m_pFileManager);
	GameSound::sound_manager = new SoundManager();

	// 完全移行までの応急処置
	GameSystem::pModelMgr = GameSystem::model_manager;

	GameSystem::pRender = m_pRender;
	GameSystem::pScene = m_pSceneMgr;
	GameSystem::pCamera = m_pCamera;
	GameSystem::pFileManager = m_pFileManager;

	
	DrawableObject::Initialize( m_pRender , m_pSceneMgr );
	GameSystem::is_searching_udonge = true;

	// 全オブジェクトのロード
	GameSystem::multiple_animation_manager->LoadFromFile( GameSystem::course_manager->GetObjectsFileName() ,
		"definition_of_enemies:" , GameSystem::tex_manager , GameSystem::model_manager , GameSystem::pRender ,
		m_pFileManager );
	GameSystem::multiple_animation_manager->LoadFromFile( GameSystem::course_manager->GetGunAnimationFileName() ,
			"definition_of_gun_animations:" , GameSystem::tex_manager , GameSystem::model_manager , GameSystem::pRender ,
			m_pFileManager );	


	GameSystem::all_object = new CObjects();

	LoadUdonge();
	LoadInfo();

	// フォントのロード
	m_pFileManager->SetCurrentPath( "Font" );
	Renderer::Object::IFontSprite2D *pFont;
	int i;
	for( i=0 ; font_files[i] ; i++ )
	{
		pFont = m_pRender->CreateFontSprite2DFromFile( font_files[i] , "tga" , 1024 );
		if( !pFont )
		{
			MessageBox( NULL , "Read Font Error..." , NULL , NULL );
			exit(0);
		}
		if( pFont->IsInvalid() )
		{
			MessageBox( NULL , "Font Invalid" , NULL , NULL );
		}

		GameSystem::pFonts.push_back( pFont );
		GameSystem::fonts_spaces.push_back( font_spaces[i] );
	}
	m_pFileManager->SetCurrentPath( "Texture" );

	GameSystem::is_searching_udonge = true;
	GameSystem::is_searching_only_y = false;

	GameSystem::all_task->ChangeTask( m_id , new MainMenu() );
}

void FirstInitialize::LoadUdonge()
{
//	Math::Rect2D<Float> col_rc(-0.265625f*2,-0.5*2,0.515625f*2,0.96875f*2),col_rc2(-0.225625f*2,-0.5*2,0.515625f*2,0.96875f*2);
	
	Character::udonge = new UdongeAnimation( GameSystem::tex_manager , DEFAULT_HP);

	// うどんげのアニメーション系をロード
	if( !Character::udonge->LoadUdongeFromFile( "udonge.txt" , GameSystem::pRender , m_pFileManager ) )
	{
		MessageBox( NULL , "error" , NULL , NULL );
	}

	// 銃のロード
	if( !Character::udonge->LoadGunFromFile( "gun.txt" , GameSystem::multiple_animation_manager , m_pFileManager ) )
	{
		MessageBox( NULL , "error" , NULL , NULL );
	}

	GameSystem::all_object->Add( Character::udonge );
	Character::udonge->SetHPToDefault();
}

#define	INFO_FILE_NAME	TEXT("./info.ini")
#define	TEXTURE_INFO_APP	TEXT("TextureInfo")
#define SOUND_INFO_APP		TEXT("SoundInfo")
#define	OTHER_INFO_APP		TEXT("OtherInfo")

void FirstInitialize::LoadInfo()
{

	// 荒業。Media\Text\info.iniをパッキングファイルから読み取り、カレントフォルダにコピーし、GetPrivateProfile系を行い、削除
	m_pFileManager->SetCurrentPath( "Text" );
	char *temp_data;
	Sint32 size;
	if( !m_pFileManager->Load( "info.ini" , (void **)&temp_data , &size ) )
	{
		return;
	}
	FILE *write_fp = fopen( "./info.ini" , "wb" );
	fwrite( temp_data , size , 1 , write_fp );
	fclose(write_fp);
	m_pFileManager->Free( temp_data );
	m_pFileManager->SetCurrentPath( "Texture" );


	char str[1024];
	char *temp,*temp2;

	GetPrivateProfileString( TEXTURE_INFO_APP , "HeartFull" , "" , str , 1023 , INFO_FILE_NAME );
	{
		temp = strtok( str , ":" );
		if( !temp )
		{
			DeleteFile( "./info.ini" );
			return;
		}
		strcpy( GameSystem::game_info.heart_full_tex , temp );

		temp = strtok( NULL , ":" );
		temp2 = strtok( temp , "," );
		GameSystem::game_info.heart_full_uv.leftup.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.heart_full_uv.leftup.y = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.heart_full_uv.rightbottom.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.heart_full_uv.rightbottom.y = (float)atof(temp2);
	}
	
	GetPrivateProfileString( TEXTURE_INFO_APP , "HeartEmpty" , "" , str , 1023 , INFO_FILE_NAME );
	{
		temp = strtok( str , ":" );
		if( !temp )
		{
			DeleteFile( "./info.ini" );
			return;
		}
		strcpy( GameSystem::game_info.heart_enmpy_tex , temp );

		temp = strtok( NULL , ":" );
		temp2 = strtok( temp , "," );
		GameSystem::game_info.heart_empty_uv.leftup.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.heart_empty_uv.leftup.y = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.heart_empty_uv.rightbottom.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.heart_empty_uv.rightbottom.y = (float)atof(temp2);
	}
	
	GetPrivateProfileString( TEXTURE_INFO_APP , "PointItem" , "" , str , 1023 , INFO_FILE_NAME );
	{
		temp = strtok( str , ":" );
		if( !temp )
		{
			DeleteFile( "./info.ini" );
			return;
		}
		strcpy( GameSystem::game_info.point_item_tex , temp );

		temp = strtok( NULL , ":" );
		temp2 = strtok( temp , "," );
		GameSystem::game_info.point_item_uv.leftup.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.point_item_uv.leftup.y = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.point_item_uv.rightbottom.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.point_item_uv.rightbottom.y = (float)atof(temp2);
	}

	GetPrivateProfileString( TEXTURE_INFO_APP , "UdongeStand" , "" , str , 1023 , INFO_FILE_NAME );
	{
		temp = strtok( str , ":" );
		if( !temp )
		{
			DeleteFile( "./info.ini" );
			return;
		}
		strcpy( GameSystem::game_info.udonge_stand_tex , temp );

		temp = strtok( NULL , ":" );
		temp2 = strtok( temp , "," );
		GameSystem::game_info.udonge_stand_uv.leftup.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.udonge_stand_uv.leftup.y = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.udonge_stand_uv.rightbottom.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.udonge_stand_uv.rightbottom.y = (float)atof(temp2);

	}

	GetPrivateProfileString( TEXTURE_INFO_APP , "Title" , "" , str , 1023 , INFO_FILE_NAME );
	{
		temp = strtok( str , ":" );
		if( !temp )
		{
			DeleteFile( "./info.ini" );
			return;
		}
		strcpy( GameSystem::game_info.title , temp );

		temp = strtok( NULL , ":" );
		temp2 = strtok( temp , "," );
		GameSystem::game_info.title_uv.leftup.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.title_uv.leftup.y = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.title_uv.rightbottom.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.title_uv.rightbottom.y = (float)atof(temp2);
	}

	GetPrivateProfileString( TEXTURE_INFO_APP , "MainMenuBG" , "" , str , 1023 , INFO_FILE_NAME );
	{
		temp = strtok( str , ":" );
		if( !temp )
		{
			DeleteFile( "./info.ini" );
			return;
		}
		strcpy( GameSystem::game_info.main_menu_bg , temp );

		temp = strtok( NULL , ":" );
		temp2 = strtok( temp , "," );
		GameSystem::game_info.main_menu_uv.leftup.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.main_menu_uv.leftup.y = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.main_menu_uv.rightbottom.x = (float)atof(temp2);
		temp2 = strtok( NULL , "," );
		GameSystem::game_info.main_menu_uv.rightbottom.y = (float)atof(temp2);
	}

	GameSystem::game_info.time_to_point = GetPrivateProfileInt( OTHER_INFO_APP , "TimeToPoint" , 50 , INFO_FILE_NAME );
	GameSystem::game_info.game_over_wait_frame = GetPrivateProfileInt( OTHER_INFO_APP , "GameOverWaitFrame" , 480 , INFO_FILE_NAME );
	GameSystem::game_info.frame_limit_to_demo = GetPrivateProfileInt( OTHER_INFO_APP , "FrameLimitToDemo" , 1800 , INFO_FILE_NAME );
	GameSystem::game_info.ending_script_time_out = GetPrivateProfileInt( OTHER_INFO_APP , "EndingScriptTimeOut" , 600 , INFO_FILE_NAME );
	GetPrivateProfileString( OTHER_INFO_APP , TEXT("EndingScript") , TEXT("") , GameSystem::game_info.ending_script , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( OTHER_INFO_APP , TEXT("DemoFile") , TEXT("") , GameSystem::game_info.demo_file , 254 , INFO_FILE_NAME );
	GameSystem::game_info.add_record_demo_mode = 
		GetPrivateProfileInt( OTHER_INFO_APP , TEXT("RecordDemoMode") , 0 , INFO_FILE_NAME ) ? true : false;

	// サウンド情報
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("title_bgm") , "" , GameSound::title_bgm , 254 , INFO_FILE_NAME );
//	GetPrivateProfileString( SOUND_INFO_APP , TEXT("game_bgm") , "" , GameSound::game_bgm , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("clear_bgm") , "" , GameSound::clear_bgm , 254 , INFO_FILE_NAME );
//	GetPrivateProfileString( SOUND_INFO_APP , TEXT("ending_bgm") , "" , GameSound::ending_bgm , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("game_over_bgm") , "" , GameSound::game_over_bgm , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("menu_decide") , "" , GameSound::menu_decide , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("menu_cancel") , "" , GameSound::menu_cancel , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("block_crash") , "" , GameSound::block_crash , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("get_point") , "" , GameSound::get_point , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("appear_heart") , "" , GameSound::appear_heart , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("get_heart") , "" , GameSound::get_heart , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("damage") , "" , GameSound::damage , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("beat_enemy") , "" , GameSound::beat_enemy , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("jump") , "" , GameSound::jump , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("1up") , "" , GameSound::one_up , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("game_over") , "" , GameSound::game_over , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("menu_select") , "" , GameSound::menu_select , 254 , INFO_FILE_NAME );
	GetPrivateProfileString( SOUND_INFO_APP , TEXT("time_to_point") , "" , GameSound::time_to_point , 254 , INFO_FILE_NAME );
	

	// メニュー関連の音はこの時点で読み込んでおく
	GameSystem::pFileManager->SetCurrentPath("Sound");
	if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::menu_decide , "Wave" ) )
	{
		GameSystem::pFileManager->SetCurrentPath("Texture");
		MessageBox( NULL , TEXT("Read sound error") , NULL , MB_OK|MB_ICONERROR );
		DeleteFile( "./info.ini" );
		return;
	}
	if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::menu_cancel , "Wave" ) )
	{
		GameSystem::pFileManager->SetCurrentPath("Texture");
		MessageBox( NULL , TEXT("Read sound error") , NULL , MB_OK|MB_ICONERROR );
		DeleteFile( "./info.ini" );
		return;
	}
	if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::menu_select , "Wave" ) )
	{
		GameSystem::pFileManager->SetCurrentPath("Texture");
		MessageBox( NULL , TEXT("Read sound error") , NULL , MB_OK|MB_ICONERROR );
		DeleteFile( "./info.ini" );
		return;
	}
	if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , GameSound::title_bgm , "OggVorbis" ) )
	{
		GameSystem::pFileManager->SetCurrentPath("Texture");
		MessageBox( NULL , TEXT("Read sound error") , NULL , MB_OK|MB_ICONERROR );
		DeleteFile( "./info.ini" );
		return;
	}
	GameSystem::pFileManager->SetCurrentPath("Texture");

	DeleteFile( "./info.ini" );
}
