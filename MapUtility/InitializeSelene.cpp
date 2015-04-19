
#include "Selene.h"
using namespace Selene;

#include "InitializeSelene.h"
#include "SettingDialog.h"
#include "CommonForCamera.h"
#include "UpdateCamera.h"

bool InitializeFileMgr( ICore *pCore , File::IFileManager **ppFileMgr );
bool InitializeGraphicCard( ICore *pCore , IGraphicCard	**ppGraphicCard );
bool InitializeRender( IGraphicCard	*pGraphicCard , Renderer::IRender **ppRender );
bool InitializeScene(Renderer::IRender *pRender , Scene::ISceneManager **ppSceneMgr , int screen_width , int screen_height );
bool InitializeCamera( Scene::ISceneManager *pSceneMgr , Scene::ICamera **ppCamera , int screen_width , int screen_height );

// ������
bool InitializeSelene( ICore *pCore , File::IFileManager **ppFileMgr , IGraphicCard	**ppGraphicCard , 
					  Renderer::IRender **ppRender , Scene::ISceneManager **ppSceneMgr , Scene::ICamera **ppCamera ,
					  int screen_width , int screen_height )
{
	if( !InitializeFileMgr( pCore , ppFileMgr ) )
	{
		return false;
	}
	if( !InitializeGraphicCard( pCore , ppGraphicCard ) )
	{
		return false;
	}
	//-----------------------------------------------------------------
	// �E�B���h�E�쐬
	//-----------------------------------------------------------------
	// ICore�C���^�[�t�F�C�X���̏��ɏ]���ăA�v���P�[�V����
	// �E�B���h�E�𐶐����܂��B
	// IGraphicCard����̏������ɉ�ʉ𑜓x�Ȃǂ�ύX����ꍇ�́A
	// ICore::Start()���Ăяo���O�ɍs���Ă��������B
	pCore->Start( screen_width , screen_height , true );

	if( !InitializeRender( *ppGraphicCard , ppRender ) )
	{
		return false;
	}

	if( !InitializeScene( *ppRender , ppSceneMgr , screen_width , screen_height ) )
	{
		return false;
	}

	if( !InitializeCamera( *ppSceneMgr , ppCamera , screen_width , screen_height ) )
	{
		return false;
	}

	// �[�x�����̃X�e�[�g�ݒ�
	//   �[�x�o�b�t�@�Ɋւ��Ă̐ݒ���s���܂��B
	//   �[�x�o�b�t�@�ւ̏������݂�L���ɂ��A
	//   �[�x�o�b�t�@�̃e�X�g��L���ɂ��Ă��܂��B
	Renderer::IRender *pRender = *ppRender;
	pRender->SetDepthTestEnable( true );
	pRender->SetDepthWriteEnable( true );

	// �`��X�e�[�g
	pRender->SetAlphaTestEnable( true );		// �A���t�@�e�X�gON
	pRender->SetDepthTestEnable( true );		// Z�e�X�gON
	pRender->SetDepthWriteEnable( true );		// Z��������ON
	pRender->SetDrawType( DRAW_TYPE_BLEND );	// �ʏ�`��

	return true;
}

bool InitializeFileMgr( ICore *pCore , File::IFileManager **ppFileMgr )
{
	//-----------------------------------------------------------------
	// �t�@�C���o�^
	//-----------------------------------------------------------------
	// �t�@�C���}�l�[�W���[�𐶐�
	*ppFileMgr = pCore->CreateFileManager();
	// �R�A�ɐݒ�
	pCore->SetFileManager( *ppFileMgr );
	// ���[�g�f�B���N�g����ݒ�
	(*ppFileMgr)->SetRootPath( 0, "Data" );
	// ���[�g�f�B���N�g���ɑ΂��ēǂݍ��ރJ�����g�f�B���N�g���ݒ�
	(*ppFileMgr)->SetCurrentPath( "Model" );

	// �{���A�����̓t�@�C������ǂݍ���Őݒ�
	SetRouteFolder( "Data" );
	SetModelFolder( "Model" );

	return true;
}

bool InitializeGraphicCard( ICore *pCore , IGraphicCard	**ppGraphicCard )
{
	*ppGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
	if ( *ppGraphicCard == NULL ) return false;

	return true;
}

bool InitializeRender( IGraphicCard	*pGraphicCard , Renderer::IRender **ppRender )
{
	//-----------------------------------------------------------------
	// �����_���[���擾
	//-----------------------------------------------------------------
	// �쐬�����O���t�B�b�N�J�[�h�����ʕ`��p�̃����_���[���擾���܂��B
	// Renderer::IRender�C���^�[�t�F�C�X��1��IGraphicCard�C���^�[�t�F�C�X�ɑ΂���1�������݂��܂���B
	*ppRender = pGraphicCard->CreateRender();
	if ( *ppRender == NULL ) return false;

	// �e�N�X�`���t�H�[�}�b�g
	Renderer::CTextureConfig Config;
	Config.SetColorKey( CColor(0,0,0,0) );
	Config.SetFormat( FORMAT_TEXTURE_DXT );
	Config.SetMipmapEnable( true );
	(*ppRender)->SetTextureConfig( Config );

	return true;
}

bool InitializeScene(Renderer::IRender *pRender , Scene::ISceneManager **ppSceneMgr , int screen_width , int screen_height )
{
	//-----------------------------------------------------------------
	// �V�[��
	//-----------------------------------------------------------------
	// �V�[���}�l�[�W���[���擾
	//   �����_���[�Ɋ֘A�t����ꂽ�V�[���Ǘ��p�̃C���^�[�t�F�C�X���擾���܂��B
	//   3D��Ԃ��̂��̂��Ǘ����Ă���̂ŁA���f���Ȃǂ̃����_�����O�ɕK�v�Ƃ��܂��B
	*ppSceneMgr = pRender->CreateSceneManager( 128, true );

	
	// �����_�����O�^�[�Q�b�g�̃T�C�Y
	//   �����I�Ȏ��`��̈�
	(*ppSceneMgr)->InitParameter_RenderTargetData(
		NULL,												// �����_�����O�^�[�Q�b�g�̃e�N�X�`���iNULL�Ńo�b�N�o�b�t�@�֒��ځj
		Math::Point2DI( screen_width, screen_height ),		// �`���̃����_�����O�^�[�Q�b�g�T�C�Y�i�����w�肳��Ă��鎞�́��̃e�N�X�`���T�C�Y�Ɏ����ϊ��j
		false );											// �`���̃e�N�X�`���T�C�Y���Q�̗ݏ�ɂ��邩�ǂ���

	// �V�[���ɑ΂���G�t�F�N�g�̏�����
	//   �V�[���ɑ΂��čs���e�탌���_�����O�G�t�F�N�g�͂����ōs���܂��B
	(*ppSceneMgr)->InitParameter_Shading( SHADING_TYPE_NORMAL );							// �V�F�[�f�B���O
//		pSceneMgr->InitParameter_Fog( FOG_TYPE_DISABLE, FOG_EFFECT_LINEAR );				// �t�H�O
//		pSceneMgr->InitParameter_HighDynamicRange( HDR_TYPE_ENABLE, HDR_EFFECT_BLOOM );		// HDR�����_�����O
//		pSceneMgr->InitParameter_MotionBlur( MOTION_BLUR_TYPE_DISABLE );					// ���[�V�����u���[
//		pSceneMgr->InitParameter_DepthOfField( DOF_TYPE_DISABLE );							// ��ʊE�[�x
//		pSceneMgr->InitParameter_Shadow( SHADOW_TYPE_DISABLE, SHADOW_QUARITY_MIDDLE );		// �e����
//		pSceneMgr->InitParameter_AntiAliasType( AA_TYPE_DISABLE, 0.0f );					// �A���`�G�C���A�X

	// �V�[���̏������n�p�����[�^�[�̍X�V
	//  ������Ăяo�����ƂŃR���܂ł̐ݒ肪�L���ɂȂ�A
	//  �A�N�^�[�̐����⃌���_�����O���\�ɂȂ�B
	(*ppSceneMgr)->InitParameter_Update();

	return true;
}

bool InitializeCamera( Scene::ISceneManager *pSceneMgr , Scene::ICamera **ppCamera , int screen_width , int screen_height )
{
	//-----------------------------------------------------------------
	// �J����
	//-----------------------------------------------------------------
	// �V�[������J�������擾
	//   �V�[��1�ɑ΂��āA1�̃J�������������Ă��܂��B
	//   �V�[���̃J�����𑀍삷�邽�߂ɁA�J�����̃C���^�[�t�F�C�X��
	//   �V�[������擾���܂��B
	*ppCamera = pSceneMgr->GetCamera();
	// �J�����̃v���W�F�N�V������ݒ�
	//   �J�����̌�����͈͂ƌ�������ݒ肵�܂��B
	//   ������͈͂��L���قǂy�o�b�t�@�̐��x�̖���
	//   ������Ղ��Ȃ�̂Œ��ӂ��Đݒ肵�Ă��������B
	(*ppCamera)->UpdateProjection( 1.0f, 4096.0f, DEG_TO_ANGLE(45), screen_width , screen_height );

	// �J�����̏������ݒ�
	Camera::SetCameraToDefault( *ppCamera );

	return true;
}
