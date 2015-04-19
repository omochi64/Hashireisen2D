

//----------------------------------------------------------------------------------
// INCLUDE
//----------------------------------------------------------------------------------
#include <math.h>

#define MAIN_WIN_DEFINE


#include "Selene.h"
#include "resource.h"
#include "DialogProc.h"
#include "common.h"
#include "InitializeSelene.h"
#include "RenderingScene.h"
#include "SetLighting.h"
#include "InitializeUtilityData.h"

#include "CheckMouseActivity.h"
#include "UpdateCamera.h"
#include "UpdateVisualActors.h"
#include "CheckKeyboardActivity.h"


#include "CommonForMainWin.h"
#include "CommonForSynchronize.h"
#include "CommonForVisual.h"
#include "CommonForData.h"
#include "CommonForCamera.h"
#include "RequestForCreate.h"

#include "SettingDialog.h"
#include "CreateModelFromFile.h"
#include "CreateModelActor.h"


//----------------------------------------------------------------------------------
// NAMESPACE
//----------------------------------------------------------------------------------
using namespace Selene;


//----------------------------------------------------------------------------------
// DEFINE
//----------------------------------------------------------------------------------
#define SCREEN_WIDTH			(640)
#define SCREEN_HEIGHT			(480)


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
Scene::IPrimitiveActor *CreateXArrowActor( float cam_distance );
Scene::IPrimitiveActor *CreateYArrowActor( float cam_distance );
Scene::IPrimitiveActor *CreateZArrowActor( float cam_distance );

//extern Collision::CLine3D selec_line;


//----------------------------------------------------------------------------------
/**
	@brief ���C���֐�

	�R���\�[���A�v���P�[�V�����ł������� main() �֐��ł��B
	�E�B���h�E�Y�A�v���P�[�V�����ł͈ȉ��̊֐��� main() �ɂȂ�܂��B
*/
//----------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	int i,size;
	hInstance = hInst;
	ICore						*pCore			= NULL;
	IGraphicCard				*pGraphicCard	= NULL;
	File::IFileManager			*pFileMgr		= NULL;
	Renderer::IRender			*pRender		= NULL;
	Scene::ISceneManager		*pSceneMgr		= NULL;
	Scene::ICamera				*pCamera		= NULL;
	Peripheral::IMouse			*pMouse			= NULL;
	Peripheral::IKeyboard		*pKeyboard		= NULL;
	Scene::IPrimitiveActor		*pAreaActor		= NULL;
	Renderer::Object::ILine3D	*pAreaLine		= NULL;

//	Renderer::Object::ILine3D *pForDebugLine = NULL;
//	Scene::IPrimitiveActor		*pForDebugLineActor = NULL;
//	Renderer::SLineVertex3D l_vertex;
//	l_vertex.v1.Col = l_vertex.v2.Col = CColor(0xff,0xff,0xff);

	Scene::IPrimitiveActor		*pXActor = NULL,*pYActor = NULL,*pZActor = NULL;

	//----------------------------------------------------------------------------
	// �V�X�e���̏�����
	//----------------------------------------------------------------------------
	// Selene�̃V�X�e�������������Ă��܂��B
	// �N�����ɕK���Ăяo���Ă��������B
	System::Initialize();

	//----------------------------------------------------------------------------
	// �R�A�̐���
	//----------------------------------------------------------------------------
	// �V�X�e������ICore�C���^�[�t�F�C�X���擾���Ă��܂��B
	// �A�v���P�[�V�����E�B���h�E�Ɋւ���C���^�[�t�F�C�X�ł���A
	// �V�X�e�����ɂP�������݂��܂��B
	pCore = System::CreateCore();
	g_is_end = false;

	// �E�X���b�h�����֌W
	InitializeCriticalSection( &g_hOutOfSceneMgrCS );
	InitializeCriticalSection( &g_hMapRouteCS );
	InitializeCriticalSection( &g_RFC_model_CS );
	InitializeCriticalSection( &g_RFC_actor_CS );

	//----------------------------------------------------------------------------
	// ������
	//----------------------------------------------------------------------------
	// ICore�C���^�[�t�F�C�X�̏��������s���܂��B
	// �A�v���P�[�V�����E�B���h�E�Ɋւ��Ă̊e�평�������s���܂��B
	if ( pCore->Initialize( "Selene Sample Program", FRAME_RATE_60 ) )
	{
		// &pFileMgr , &pGraphicCard , &pRender , &pScene
		//-----------------------------------------------------------------
		// �^�C�g���o�[�ɏ���\��
		//-----------------------------------------------------------------
		pCore->EnableDrawTitleInformation( true );

		// �F�X������
		if( !InitializeSelene( pCore , &pFileMgr , &pGraphicCard , &pRender , &pSceneMgr , &pCamera , SCREEN_WIDTH , SCREEN_HEIGHT ) )
		{
			goto EXIT;
		}

		g_pFileMgr = pFileMgr;
		g_pRender = pRender;
		g_pSceneMgr = pSceneMgr;

		pMouse = pCore->GetMouse();
		pKeyboard = pCore->GetKeyboard();

		InitializeUtilityData();
		
		// �_�C�A���O�{�b�N�X�̐���
		RECT rc;
		HWND hWnd = pCore->GetWindowHandle();
		GetWindowRect( hWnd , &rc );
		MoveWindow( hWnd , 0 , 0 , rc.right-rc.left , rc.bottom-rc.top , TRUE );
		HWND hDlg = CreateDialog( hInst , MAKEINTRESOURCE(IDD_CONTROL_DIALOG) , hWnd , (DLGPROC)DialogProc );
		g_hDialog = hDlg;
		GetWindowRect( hDlg , &rc );
		MoveWindow( hDlg , rc.left+SCREEN_WIDTH , rc.top+SCREEN_HEIGHT , rc.right-rc.left , rc.bottom-rc.top , TRUE );
		ShowWindow( hDlg , SW_SHOW );

		//-----------------------------------------------------------------
		// �`��I�u�W�F�N�g����
		//-----------------------------------------------------------------
		// �G���A�`��pLineObject����
		pAreaLine = pRender->CreateLine3D( 32768 , true );
//		pForDebugLine = pRender->CreateLine3D( 1024 , true );
		pAreaActor = pSceneMgr->CreateActor( pAreaLine );
//		pForDebugLineActor = pSceneMgr->CreateActor( pForDebugLine );

		//-----------------------------------------------------------------
		// �ύX���Ȃ��X�e�[�g
		//-----------------------------------------------------------------
		// �t�B���^�����O
		pRender->SetTextureFilterType( TEXTURE_STAGE_COLOR, TEXTURE_FILTER_3D_LOW );

		// �A�N�^�[�쐬
		SAFE_RELEASE(pXActor);
		SAFE_RELEASE(pYActor);
		SAFE_RELEASE(pZActor);
		pXActor = CreateXArrowActor( Camera::g_dist );
		pYActor = CreateYArrowActor( Camera::g_dist );
		pZActor = CreateZArrowActor( Camera::g_dist );

		UpdateVisualActors( pRender , pSceneMgr );

		//-----------------------------------------------------------------
		// ���C�����[�v
		//-----------------------------------------------------------------
		// �A�v���P�[�V�����E�B���h�E�̃E�B���h�E���b�Z�[�W�̏����ƁA
		// ICore�C���^�[�t�F�C�X�o�R�Őݒ肳�ꂽ�t���[��������s���܂��B
		// �܂��L�[���͂̍X�V���ʂ̍X�V�����̊֐����ōs���܂��B
		// ���̊֐��̓E�B���h�E���I��������false��Ԃ��܂��B
		// ���̏ꍇ�A�������ɃA�v���P�[�V�������I������悤�ɂ��ĉ������B
		while ( pCore->Run() && !g_is_end )
		{
			/////////////////////////////////////////////////////////////////////////////////////////////
			// �V�[���ւ̃����_�����O���N�G�X�g
			/////////////////////////////////////////////////////////////////////////////////////////////

			EnterCriticalSection( &g_hOutOfSceneMgrCS );
			EnterCriticalSection( &g_hMapRouteCS );
			// �V�[���Ǘ�
			//   Scene::ISceneManager�C���^�[�t�F�C�X���g�����V�[���Ǘ����J�n���܂��B
			pSceneMgr->Begin( false );

			// �����_�����O
			//   IActor�Ɋ֘A�t����ꂽRenderer::Object::IModel�C���^�[�t�F�C�X�̓��e��`�悵�܂��B
			//   ���̊֐��ł�Renderer::IRender�C���^�[�t�F�C�X�Ɋ֘A�t����ꂽScene::ISceneManager�C���^�[�t�F�C�X�ɑ΂���
			//   �`�惊�N�G�X�g�𔭍s���邾���ŁA�`���҂����ɋA���Ă��܂��B
//			pActor->RenderingRequest();

			// �}�b�v�̃����_�����O
			g_mapRoute->DrawVisibleArea( pSceneMgr , SCREEN_WIDTH , SCREEN_HEIGHT );


			pXActor->TransformReset();
			pXActor->TransformUpdate();
			pXActor->RenderingRequest();

			pYActor->TransformReset();
			pYActor->TransformUpdate();
			pYActor->RenderingRequest();

			pZActor->TransformReset();
			pZActor->TransformUpdate();
			pZActor->RenderingRequest();

			Scene::IPrimitiveActor *pActor;
			pActor = g_nowPoint->GetActor();
			if( pActor )
			{
				pActor->TransformUpdate();
				pActor->RenderingRequest();
			}
			pActor = g_nowPlane->GetActor();
			if( pActor )
			{
				pActor->TransformUpdate();
				pActor->RenderingRequest();
			}

			// �����Ă���I�u�W�F�N�g�̕`��
			if( g_nowMovingObject->GetActor() )
			{
				g_nowMovingObject->GetActor()->TransformUpdate();
				g_nowMovingObject->GetActor()->RenderingRequest();
			}

			// ��ԗ̈�̕`��
			pAreaLine->Begin();
			g_mapRoute->DrawOctreeArea( pAreaLine , pSceneMgr , SCREEN_WIDTH , SCREEN_HEIGHT );
			pAreaLine->End();

			pAreaActor->RenderingRequest();

//			pForDebugLine->Begin();
//			l_vertex.v1.Pos = selec_line.vStart;
//			l_vertex.v2.Pos = selec_line.vEnd;
//			pForDebugLine->Push( &l_vertex , 1 );
//			pForDebugLine->End();
//			pForDebugLineActor->RenderingRequest();

			// �V�[���Ǘ����I��
			//   �����܂łō\�z���ꂽ�V�[���������Ɏ��ۂɕ`��p�f�[�^�̍\�z���s���܂��B
			pSceneMgr->End();

			/////////////////////////////////////////////////////////////////////////////////////////////
			// �V�[���}�l�[�W���[�̓��e�����ۂɃ����_�����O
			/////////////////////////////////////////////////////////////////////////////////////////////
			RenderingScene( pRender , pSceneMgr );

			LeaveCriticalSection( &g_hOutOfSceneMgrCS );
			LeaveCriticalSection( &g_hMapRouteCS );


			/////////////////////////////////////////////////////////////////////////////////////////////
			

			/////////////////////////////////////////////////////////////////////////////////////////////
			// �V�[�����\�z
			/////////////////////////////////////////////////////////////////////////////////////////////

			// �V�[����񃊃Z�b�g
			//   Scene::ISceneManager�C���^�[�t�F�C�X���g�����V�[���Ǘ��������������܂��B
			pSceneMgr->Reset();

			// �V�[���̔w�i�̐F
			pSceneMgr->SetParameter_BGColor( Math::Vector4D(0.5f,0.5f,0.5f,1.0f) );

			// �v�����ꂽ���f���쐬�̎��s
			EnterCriticalSection( &g_RFC_model_CS );
			size = g_request_for_model.size();
			for( i=0 ; i<size ; i++ )
			{
				RFC_MODEL &request = g_request_for_model[i];
				// �쐬
				Renderer::Object::IModel *pModel = pRender->CreateModelFromFile( request.fname );
				(*request.call_func_when_finish)(request.hWnd,pModel,request.other_info);
			}
			g_request_for_model.clear();
			LeaveCriticalSection( &g_RFC_model_CS );
			
			// �v�����ꂽ�A�N�^�[�쐬�̎��s
			EnterCriticalSection( &g_RFC_actor_CS );
			size = g_request_for_actor.size();
			for( i=0 ; i<size ; i++ )
			{
				RFC_ACTOR &request = g_request_for_actor[i];
				// �쐬
				Scene::IModelActor *pActor = pSceneMgr->CreateActor( request.pModel );
				pActor->Collision_Create();
				(*request.call_func_when_finish)(request.hWnd,pActor,request.other_info);
			}
			g_request_for_actor.clear();
			LeaveCriticalSection( &g_RFC_actor_CS );
			

			// �}�E�X�ɂ�鑀��
			CheckMouseActivity( pMouse , pCore , pSceneMgr , pRender );
			// �L�[�{�[�h�ɂ�鑀��
			CheckKeyboardActivity( pKeyboard , pSceneMgr );

			// �J�������A�b�v�f�[�g
			Camera::UpdateCamera( pCamera );

			// �A�N�^�[�쐬
			SAFE_RELEASE(pXActor);
			SAFE_RELEASE(pYActor);
			SAFE_RELEASE(pZActor);
			pXActor = CreateXArrowActor( Camera::g_dist );
			pYActor = CreateYArrowActor( Camera::g_dist );
			pZActor = CreateZArrowActor( Camera::g_dist );

			UpdateVisualActors( pRender , pSceneMgr );

			// ���C�e�B���O�̐ݒ�
			SetLighting( pSceneMgr );
		}

EXIT:
		// �c�[���p�f�[�^���
		DestroyUtilityData();


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
	}

	// �R�A�̉��
	SAFE_RELEASE( pCore );

	DeleteCriticalSection( &g_hOutOfSceneMgrCS );

	//--------------------------------------------------------
	// �V�X�e���̉��
	//--------------------------------------------------------
	// �A�v���P�[�V�����̏I�����ɕK���Ăяo���ĉ������B
	// �܂����̊֐����Ăяo���O�Ɏ擾�����S�Ă�
	// �C���^�[�t�F�C�X��������Ă��������B
	System::Finalize();

	return 0;
}

// ���A�N�^�[�쐬
// dir
Scene::IPrimitiveActor *CreateXArrowActor( float cam_distance )
{
	Renderer::Object::ILine3D *pLine;
	Renderer::SLineVertex3D line;
	pLine = g_pRender->CreateLine3D( 6 , false );
	pLine->Begin();

	line.v1.Pos = Math::Vector3D( -cam_distance/3 , 0 , 0 );
	line.v2.Pos = Math::Vector3D( cam_distance/3 , 0 , 0 );
	line.v1.Col = line.v2.Col = CColor( 0xff , 0 , 0 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance*(1.0f/3-1.0f/64) , 0 , -cam_distance/64 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance*(1.0f/3-1.0f/64) , 0 , cam_distance/64 );
	pLine->Push( &line , 1 );

	pLine->End();

	Scene::IPrimitiveActor *actor = g_pSceneMgr->CreateActor( pLine );
	SAFE_RELEASE(pLine);
	return actor;
}

Scene::IPrimitiveActor *CreateYArrowActor( float cam_distance )
{
	Renderer::Object::ILine3D *pLine;
	Renderer::SLineVertex3D line;
	pLine = g_pRender->CreateLine3D( 6 , false );
	pLine->Begin();

	line.v1.Pos = Math::Vector3D( 0 , -cam_distance/3 , 0 );
	line.v2.Pos = Math::Vector3D( 0 , cam_distance/3 , 0 );
	line.v1.Col = line.v2.Col = CColor( 0 , 0xff , 0 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( -cam_distance/64 , cam_distance*(1.0f/3-1.0f/64) , 0 );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance/64 , cam_distance*(1.0f/3-1.0f/64) , 0 );
	pLine->Push( &line , 1 );

	pLine->End();

	Scene::IPrimitiveActor *actor = g_pSceneMgr->CreateActor( pLine );
	SAFE_RELEASE(pLine);
	return actor;
}

Scene::IPrimitiveActor *CreateZArrowActor( float cam_distance )
{
	Renderer::Object::ILine3D *pLine;
	Renderer::SLineVertex3D line;
	pLine = g_pRender->CreateLine3D( 6 , false );
	pLine->Begin();

	line.v1.Pos = Math::Vector3D( 0 , 0 , -cam_distance/3 );
	line.v2.Pos = Math::Vector3D( 0 , 0 , cam_distance/3 );
	line.v1.Col = line.v2.Col = CColor( 0 , 0 , 0xff );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( -cam_distance/64 , 0 , cam_distance*(1.0f/3-1.0f/64) );
	pLine->Push( &line , 1 );

	line.v1.Pos = Math::Vector3D( cam_distance/64 , 0 , cam_distance*(1.0f/3-1.0f/64) );
	pLine->Push( &line , 1 );

	pLine->End();

	Scene::IPrimitiveActor *actor = g_pSceneMgr->CreateActor( pLine );
	SAFE_RELEASE(pLine);
	return actor;
}
