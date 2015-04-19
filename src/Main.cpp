
/*
AddTexture���O���[�o���ɂ�����ςɂȂ����I�I�I�I
*/

#define _MAIN_SOURCE_
#define	_SOUND_GLOBAL
//----------------------------------------------------------------------------------
// INCLUDE
//----------------------------------------------------------------------------------
#include "Selene.h"
//#pragma comment(lib,"Selene.lib")
#pragma comment(lib,"winmm.lib")
//----------------------------------------------------------------------------------
// NAMESPACE
//----------------------------------------------------------------------------------
using namespace Selene;


#include "System/TaskSystem.h"
#include "Game/Objects.h"
#include "System/GlobalNamespace.h"
#include "System/IniInfo.h"
#include "Tasks/InitializeGame.h"
#include "Tasks/FirstInitialize.h"
#include "System/Sounds.h"
#include "resource.h"
#include "System/InputData.h"
#include "System/GameSystem.h"

using namespace Character;
using namespace GameSystem;

//----------------------------------------------------------------------------------
// DEFINE
//----------------------------------------------------------------------------------
#define CONFIG_FILE_NAME	"./config.ini"

//----------------------------------------------------------------------------------
// PROTOTYPE
//----------------------------------------------------------------------------------
// �߂�l���R���g���[����ID
int LoadJoySettings( InputData::JOY_SET *set , char *ini_file );


//----------------------------------------------------------------------------------
/**
	@brief ���C���֐�

	�R���\�[���A�v���P�[�V�����ł������� main() �֐��ł��B
	�E�B���h�E�Y�A�v���P�[�V�����ł͈ȉ��̊֐��� main() �ɂȂ�܂��B
*/
//----------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInst , HINSTANCE, LPSTR, int )
{
	ICore							*pCore			= NULL;
	IGraphicCard					*pGraphicCard	= NULL;
	File::IFileManager				*pFileMgr		= NULL;
	Renderer::IRender				*pRender		= NULL;
	Scene::ISceneManager			*pSceneMgr		= NULL;
	Scene::ICamera					*pCamera		= NULL;

	Peripheral::IKeyboard	*pKeyboard		= NULL;
	Peripheral::IJoystick	*pJoyStick		= NULL;

	

	bool do_30_fps , is_full_screen;
	

	// �Ƃ肠����KeyMapping���Œ肵�Ă���
	/*
	#define		KEY_JUMP		0x00000002
#define		KEY_DASH		0x00000004
#define		KEY_RIGHT		0x00000008
#define		KEY_LEFT		0x00000010
#define		KEY_UP			0x00000020
#define		KEY_DOWN		0x00000040
#define		KEY_CAM_TRIG	0x00000080
#define		KEY_PAUSE		0x00000100
	*/
	InputData::KEY_SET key_set[] = {
		{SELENE_VK_Z,KEY_JUMP},{SELENE_VK_X,KEY_DASH},{SELENE_VK_RIGHT,KEY_RIGHT},
		{SELENE_VK_LEFT,KEY_LEFT},{SELENE_VK_UP,KEY_UP},{SELENE_VK_DOWN,KEY_DOWN},
		{SELENE_VK_ESCAPE,KEY_PAUSE},{SELENE_VK_W,KEY_CAM_UP},{SELENE_VK_S,KEY_CAM_DOWN},
		{SELENE_VK_D,KEY_CAM_RIGHT},{SELENE_VK_A,KEY_CAM_LEFT},
		{SELENE_VK_F,KEY_CAM_ZOOM},{SELENE_VK_G,KEY_CAM_ZOOM_OUT}
	};

	InputData::JOY_SET *joy_set = new InputData::JOY_SET[InputData::GAME_KEY_KIND_NUM];
	int joy_id;
//	bool is_draw = false;

	int		input_keys_keyboard,input_keys_joystick;
	std::map<int,int> input_key_strokes_keyboard,input_key_strokes_joystick;

	joy_id = LoadJoySettings( joy_set , CONFIG_FILE_NAME );

//	KEY_SET key_set_for_joystick[GAME_KEY_KIND]

	timeBeginPeriod(1);

	//-----------------------------------------------------------------
	// �N�����擾
	//-----------------------------------------------------------------
	is_full_screen = GetPrivateProfileInt( "GAME" , "IsFullScreen" , 0 , CONFIG_FILE_NAME ) == 1 ? true : false;
//	do_30_fps = GetPrivateProfileInt( "GAME" , "IsHalfDraw" , 0 , CONFIG_FILE_NAME ) == 1 ? true : false;
	do_30_fps = false;

//	if( do_30_fps )
//	{
//		GAME_FPS = 30;
//		Drawable2DObject::Set30FPSMode();
//	}
//	else
	{
		GAME_FPS = 60;
	}

	//----------------------------------------------------------------------------
	// ������
	//----------------------------------------------------------------------------
	// ICore�C���^�[�t�F�C�X�̏��������s���܂��B
	// �A�v���P�[�V�����E�B���h�E�Ɋւ��Ă̊e�평�������s���܂��B
	if ( GameSystem::Initialize( "�͂���[����!!����ver. 0.12", do_30_fps ? FRAME_RATE_30 : FRAME_RATE_60 , is_full_screen , DEG_TO_ANGLE(60) ) )
	{
		pFileMgr = GameSystem::pFileMgr;
		pCore = GameSystem::pCore;
		pSceneMgr = GameSystem::pSceneMgr;
		pRender = GameSystem::pRender;
		pCamera = GameSystem::pCamera;
		
		pKeyboard = pCore->GetKeyboard();

		if( joy_id >= 0 )
		{
			pJoyStick = pCore->GetJoystick(joy_id);
			if( pJoyStick->IsInvalid() )
			{
				SAFE_RELEASE(pJoyStick);
			}
		}
		else
		{
			pJoyStick = NULL;
		}

		HWND hWnd = pCore->GetWindowHandle();
		SendMessage( hWnd , WM_SETICON , 1 , (LPARAM)LoadIcon( hInst , MAKEINTRESOURCE(IDI_ICON1) ) );
		SendMessage( hWnd , WM_SETICON , 0 , (LPARAM)LoadIcon( hInst , MAKEINTRESOURCE(IDI_ICON2) ) );

		

		//-----------------------------------------------------------------
		// 2DPrimitive
		//-----------------------------------------------------------------
//		pPrimitive = pRender->CreatePrimitive2D( 1024 , true );
		
		/*
		Renderer::IRender *renderer , Scene::ISceneManager *SceneMgr , 
			LPCTSTR pTexFileName , Math::Vector3D center_pos , float width , float height ,
			Math::Vector2D speed , Math::Vector2D accel , Math::Vector2D uv_lefttop , Math::Vector2D uv_rightbottom
		*/

	//	pTargetTexture = pRender->CreateTextureRenderTarget( SCREEN_WIDTH , SCREEN_HEIGHT , FORMAT_TARGET_32BIT );
		
		
		// Task�̏�����
		GameSystem::all_task = new TaskController();
		GameSystem::all_task->AddTask( new FirstInitialize( pRender , pSceneMgr , pCamera , pFileMgr ) , 0x001 );
		GameSystem::draw_task = new TaskController();

	
		//-----------------------------------------------------------------
		// ���C�����[�v
		//-----------------------------------------------------------------
		// �A�v���P�[�V�����E�B���h�E�̃E�B���h�E���b�Z�[�W�̏����ƁA
		// ICore�C���^�[�t�F�C�X�o�R�Őݒ肳�ꂽ�t���[��������s���܂��B
		// �܂��L�[���͂̍X�V���ʂ̍X�V�����̊֐����ōs���܂��B
		// ���̊֐��̓E�B���h�E���I��������false��Ԃ��܂��B
		// ���̏ꍇ�A�������ɃA�v���P�[�V�������I������悤�ɂ��ĉ������B
		while (1)
		{
			

			/////////////////////////////////////////////////////////////////////////////////////////////
			// �V�[�����\�z
			/////////////////////////////////////////////////////////////////////////////////////////////

			// �V�[����񃊃Z�b�g
			//   Scene::ISceneManager�C���^�[�t�F�C�X���g�����V�[���Ǘ��������������܂��B
			pSceneMgr->Reset();

			// �J�����̏�Ԃ�������
			pCamera->Reset();
			// Math::Style�̕ϊ���K�p

			// ���݂̃t���[�����Ԃ��擾
			GameSystem::now_frame_time = timeGetTime();

			// �L�[�{�[�h�̏�Ԏ擾
			unsigned int i;
			InputData::input_keys = 0;
			input_keys_keyboard = 0;
			for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
			{
				if( pKeyboard->GetKeyData( key_set[i].v_key ) )
				{
					input_keys_keyboard |= key_set[i].g_key;
					input_key_strokes_keyboard[key_set[i].g_key]++;
				}
				else
				{
					input_key_strokes_keyboard[key_set[i].g_key] = 0;
				}
			}

			// �W���C�X�e�B�b�N�̏�Ԏ擾
			int input;
			input_keys_joystick = 0;
				
			if( pJoyStick )
			{
				if( pJoyStick )
				{
					// ���E
					input = pJoyStick->GetAxis( PAD_AXIS_POSITION  , PAD_AXIS_X );
					if( input > 2048 )
					{
						input_key_strokes_joystick[KEY_RIGHT]++;
						input_key_strokes_joystick[KEY_LEFT] = 0;
						input_keys_joystick |= KEY_RIGHT;
					}
					else if( input < -2048 )
					{
						input_key_strokes_joystick[KEY_LEFT]++;
						input_key_strokes_joystick[KEY_RIGHT] = 0;
						input_keys_joystick |= KEY_LEFT;
					}
					else
					{
						input_key_strokes_joystick[KEY_LEFT] = 0;
						input_key_strokes_joystick[KEY_RIGHT] = 0;
					}
					// �㉺
					input = pJoyStick->GetAxis( PAD_AXIS_POSITION  , PAD_AXIS_Y );
					if( input > 2048 )
					{
						input_key_strokes_joystick[KEY_DOWN]++;
						input_key_strokes_joystick[KEY_UP] = 0;
						input_keys_joystick |= KEY_DOWN;
					}
					else if( input < -2048 )
					{
						input_key_strokes_joystick[KEY_UP]++;
						input_key_strokes_joystick[KEY_DOWN] = 0;
						input_keys_joystick |= KEY_UP;
					}
					else
					{
						input_key_strokes_joystick[KEY_UP] = 0;
						input_key_strokes_joystick[KEY_DOWN] = 0;
					}

					// ����ȊO
					for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
					{
						if( ( joy_set[i].g_key == KEY_UP ) ||
							( joy_set[i].g_key == KEY_DOWN ) ||
							( joy_set[i].g_key == KEY_LEFT ) ||
							( joy_set[i].g_key == KEY_RIGHT ) )
						{
							continue;
						}

						if( pJoyStick->GetButton( (ePadButtonType)joy_set[i].joy_button ) )
						{
							input_keys_joystick |= joy_set[i].g_key;
							input_key_strokes_joystick[joy_set[i].g_key]++;
						}
						else
						{
							input_key_strokes_joystick[joy_set[i].g_key] = 0;
						}
					}
				}
			}

			// ���҂����킹��
			InputData::input_keys = input_keys_keyboard | input_keys_joystick;
			int keyboard_stroke,joystick_stroke;
			int key;
			for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
			{
				key = 1<<i;
				keyboard_stroke = input_key_strokes_keyboard[key];
				joystick_stroke = input_key_strokes_joystick[key];
				InputData::input_key_strokes[key] = MAX(keyboard_stroke,joystick_stroke);
			}

			// �L�����N�^�[�̓���
			GameSystem::all_task->ExecAllTask();

			
			/////////////////////////////////////////////////////////////////////////////////////////////
			// �V�[���ւ̃����_�����O���N�G�X�g
			/////////////////////////////////////////////////////////////////////////////////////////////

			// �V�[���Ǘ�
			//   Scene::ISceneManager�C���^�[�t�F�C�X���g�����V�[���Ǘ����J�n���܂��B
			pSceneMgr->Begin( false );

			
			// �t�H���g�X�v���C�g�̍\�z�J�n
			for( i=0 ; i<GameSystem::pFonts.size() ; i++ )
			{
				GameSystem::pFonts[i]->Begin(GameSystem::fonts_spaces[i]);
			}
			// ���W�ϊ�			//   Math::Style�N���X�ŕϊ������������̂�
			//   �v���~�e�B�u�ɓn���܂��B

			GameSystem::draw_task->ExecAllTask();
			
			// �t�H���g�X�v���C�g�̍\�z�I��
			for( i=0 ; i<GameSystem::pFonts.size() ; i++ )
			{
				GameSystem::pFonts[i]->End();
			}
			
			// �V�[���Ǘ����I��
			//   �����܂łō\�z���ꂽ�V�[���������Ɏ��ۂɕ`��p�f�[�^�̍\�z���s���܂��B
			pSceneMgr->End();

			if( !pCore->Run() )
			{
				break;
			}
			/////////////////////////////////////////////////////////////////////////////////////////////
			// �V�[���}�l�[�W���[�̓��e�����ۂɃ����_�����O
			/////////////////////////////////////////////////////////////////////////////////////////////

			

			// ��ʂւ̕`����J�n
			//   Renderer::IRender�C���^�[�t�F�C�X�։�ʂւ̕`����J�n���邱�Ƃ�ʒm���܂��B
			//   ���̊֐����Ă΂Ȃ��Ɖ�ʂւ̕`�悪�������s���܂���B
			pRender->Begin();

			// �V�[���̔w�i�̐F
			pSceneMgr->SetParameter_BGColor( CColor(0,0,0) );

			if( GameSystem::pTextureForBackground )
			{
				pSceneMgr->SetParameter_BGTexture( GameSystem::pTextureForBackground );
			}
			
			// �V�[���̓��e�������_�����O
			//   �����܂łō\�z���ꂽ�V�[���������Ɏ��ۂɕ`�揈�����s���܂��B
			//   ���̊֐���Renderer::IRender::End()���Ăяo���O�łȂ���Ύ��s���܂��B
			pSceneMgr->Rendering();

			// 2D�v���~�e�B�u�����_�����O
			for( i=0 ; i<GameSystem::pPrimitive2D.size() ; i++ )
			{
				if( GameSystem::pPrimitive2D[i].pTargetTexture )
				{
					pRender->SetRenderTarget( GameSystem::pPrimitive2D[i].pTargetTexture );
				}

				pRender->SetTexture( TEXTURE_STAGE_COLOR , GameSystem::pPrimitive2D[i].setTexture );
				GameSystem::pPrimitive2D[i].pPrimitive->Rendering();
				pRender->ResetRenderTarget();

			}
			GameSystem::pPrimitive2D.clear();

			// �t�H���g�X�v���C�g�̕`��
			pRender->SetDrawType( DRAW_TYPE_BLEND );	// ������ON
			for( i=0 ; i<GameSystem::pFonts.size() ; i++ )
			{
				GameSystem::pFonts[i]->Rendering();
			}

			// ��ʂւ̕`�������
			//  Renderer::IRender�C���^�[�t�F�C�X�ɑ΂��āA
			//  ��ʂւ̕`�悪�����������Ƃ�ʒm���܂��B
			pRender->End();

			/////////////////////////////////////////////////////////////////////////////////////////////
		}

		// �{�����ׂĂ����ŉ��
		// ���������[�N�̌����𒲂ׂ邽�߂Ɉꉞ�����̓R�����g�A�E�g

		SAFE_RELEASE(tex_manager);
		SAFE_RELEASE(multiple_animation_manager);
		SAFE_RELEASE(model_manager);
		SAFE_RELEASE(course_manager);
		SAFE_RELEASE(GameSound::sound_manager);
		SAFE_RELEASE(Character::udonge);
		all_task->Release();
		draw_task->Release();

		unsigned int i;
		for( i=0 ; i<pFonts.size() ; i++ )
		{
			SAFE_RELEASE(pFonts[i]);
		}
		pFonts.clear();

		delete [] joy_set;


		// �J�����̉��
		SAFE_RELEASE( pCamera );

		// �V�[���}�l�[�W���[���
		SAFE_RELEASE( pSceneMgr );

		// �����_���[�̉��
		SAFE_RELEASE( pRender );

		// �O���t�B�b�N�J�[�h�̉��
		SAFE_RELEASE( pGraphicCard );

		// �t�@�C���}�l�[�W���[���
		SAFE_RELEASE( pFileMgr );

		SAFE_RELEASE( pKeyboard );

		SAFE_RELEASE( pJoyStick );

		GameSystem::CleanUp();
	}

	// �R�A�̉��
	SAFE_RELEASE( pCore );

	timeEndPeriod(1);

	//--------------------------------------------------------
	// �V�X�e���̉��
	//--------------------------------------------------------
	// �A�v���P�[�V�����̏I�����ɕK���Ăяo���ĉ������B
	// �܂����̊֐����Ăяo���O�Ɏ擾�����S�Ă�
	// �C���^�[�t�F�C�X��������Ă��������B
	System::Finalize();

	return 0;
}

#define	APP_NAME	TEXT("KeyConfig")

int LoadJoySettings( InputData::JOY_SET *set , char *ini_file )
{
	int g_keys [] = {
		KEY_UP , KEY_DOWN , KEY_LEFT , KEY_RIGHT , 
		KEY_JUMP , KEY_DASH , KEY_PAUSE , KEY_CAM_UP , KEY_CAM_DOWN ,
		KEY_CAM_LEFT , KEY_CAM_RIGHT , KEY_CAM_ZOOM , KEY_CAM_ZOOM_OUT
	};
	char *file_keys[] = {
		"UP" , "DOWN" , "LEFT" , "RIGHT" , "JUMP" , "DASH" , "PAUSE" , 
		"CAM_UP" , "CAM_DOWN" , "CAM_LEFT" , "CAM_RIGHT" , "CAM_ZOOM" , "CAM_ZOOM_OUT"
	};
	
	// ���������L���H
	if( GetPrivateProfileInt( APP_NAME , "Valiable" , 1 , ini_file ) == 0 )
	{
		return -1;
	}

 	int i;
	int id;
	id = GetPrivateProfileInt( APP_NAME , "ID" , -1 , ini_file );

	if( id == -1 )
	{
		// �f�t�H���g�ݒ�
		for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
		{
			set[i].g_key = g_keys[i];
			set[i].joy_button = i-4;
		}
		return 0;
	}

	for( i=0 ; i<InputData::GAME_KEY_KIND_NUM ; i++ )
	{
		set[i].g_key = g_keys[i];
		set[i].joy_button = GetPrivateProfileInt( APP_NAME , file_keys[i] , -1 , ini_file );
	}

	return id;

}
