
#include "System/GameSystem.h"

using namespace Selene;

namespace GameSystem
{
	Selene::Scene::ISceneManager	*pSceneMgr;
	Selene::Renderer::IRender *pRender;
	Selene::File::IFileManager *pFileMgr;
	Selene::ICore *pCore;
	IGraphicCard *pGraphicCard;
	Scene::ICamera *pCamera;
	CModelManager *pModelMgr;
	extern const int SCREEN_WIDTH,SCREEN_HEIGHT;

// Selene�̐F�X�ȏ�����
bool Initialize(char *pWinTitle , Selene::eFrameRate frameRate , bool is_full_screen , float fovAngle )
{
	Renderer::CTextureConfig Config;

	//----------------------------------------------------------------------------
	// �V�X�e���̏�����
	//----------------------------------------------------------------------------
	// Selene�̃V�X�e�������������Ă��܂��B
	// �N�����ɕK���Ăяo���Ă��������B
	System::Initialize( true , false /*false Log.html�̍쐬�ɂ���true/false*/ );

	//----------------------------------------------------------------------------
	// �R�A�̐���
	//----------------------------------------------------------------------------
	// �V�X�e������ICore�C���^�[�t�F�C�X���擾���Ă��܂��B
	// �A�v���P�[�V�����E�B���h�E�Ɋւ���C���^�[�t�F�C�X�ł���A
	// �V�X�e�����ɂP�������݂��܂��B
	pCore = System::CreateCore();

	if( !pCore->Initialize( pWinTitle , frameRate ) )
	{
		return false;
	}

	//-----------------------------------------------------------------
	// �^�C�g���o�[�ɏ���\��
	//-----------------------------------------------------------------
	pCore->EnableDrawTitleInformation( false );
	pCore->EnableDrawMouseCursor( false );

	//-----------------------------------------------------------------
	// �t�@�C���o�^
	//-----------------------------------------------------------------
	// �t�@�C���}�l�[�W���[�𐶐�
	pFileMgr = pCore->CreateFileManager();
	// �R�A�ɐݒ�
	pCore->SetFileManager( pFileMgr );
	// ���[�g�f�B���N�g����ݒ�
	pFileMgr->SetRootPath( 0, "Data" , "PasswordToUnpack" );
	// ���[�g�f�B���N�g���ɑ΂��ēǂݍ��ރJ�����g�f�B���N�g���ݒ�
	//  ����́c�����̂��낤���c
	pFileMgr->SetCurrentPath( "Texture" );

	//-----------------------------------------------------------------
	// �O���t�B�b�N�J�[�h���擾
	//-----------------------------------------------------------------
	// �O���t�B�b�N�J�[�h�Ɋւ��Ă̏��������s���܂��B
	// IGraphicCard�C���^�[�t�F�C�X����͐ڑ����ꂽ
	// �O���t�B�b�N�J�[�h�̔\�͂��擾���鎖���\�ł��B
	// IGraphicCard�C���^�[�t�F�C�X��1��ICore�C���^�[�t�F�C�X�ɑ΂���1�������݂��܂���B
	pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
	if ( pGraphicCard == NULL ) return false;

	//-----------------------------------------------------------------
	// �E�B���h�E�쐬
	//-----------------------------------------------------------------
	// ICore�C���^�[�t�F�C�X���̏��ɏ]���ăA�v���P�[�V����
	// �E�B���h�E�𐶐����܂��B
	// IGraphicCard����̏������ɉ�ʉ𑜓x�Ȃǂ�ύX����ꍇ�́A
	// ICore::Start()���Ăяo���O�ɍs���Ă��������B
	pCore->Start( SCREEN_WIDTH, SCREEN_HEIGHT, !is_full_screen );

	//-----------------------------------------------------------------
	// �����_���[���擾
	//-----------------------------------------------------------------
	// �쐬�����O���t�B�b�N�J�[�h�����ʕ`��p�̃����_���[���擾���܂��B
	// Renderer::IRender�C���^�[�t�F�C�X��1��IGraphicCard�C���^�[�t�F�C�X�ɑ΂���1�������݂��܂���B
	pRender = pGraphicCard->CreateRender();
	if ( pRender == NULL ) return false;

	// �e�N�X�`���t�H�[�}�b�g
//		Config.SetColorKey( CColor(0,255,0,0xff) );
//		Config.
	Config.SetFormat( FORMAT_TEXTURE_DXT );
	Config.SetMipmapEnable( true );
	pRender->SetTextureConfig( Config );

	//-----------------------------------------------------------------
	// �V�[��
	//-----------------------------------------------------------------
	// �V�[���}�l�[�W���[���擾
	//   �����_���[�Ɋ֘A�t����ꂽ�V�[���Ǘ��p�̃C���^�[�t�F�C�X���擾���܂��B
	//   3D��Ԃ��̂��̂��Ǘ����Ă���̂ŁA���f���Ȃǂ̃����_�����O�ɕK�v�Ƃ��܂��B

	// ����̑��������ő�|���S�����I�I
	pSceneMgr = pRender->CreateSceneManager( 4096, true );

	// �����_�����O�^�[�Q�b�g�̃T�C�Y
	//   �����I�Ȏ��`��̈�
	pSceneMgr->InitParameter_RenderTargetData(
		NULL,												// �����_�����O�^�[�Q�b�g�̃e�N�X�`���iNULL�Ńo�b�N�o�b�t�@�֒��ځj
		Math::Point2DI( SCREEN_WIDTH, SCREEN_HEIGHT ),		// �`���̃����_�����O�^�[�Q�b�g�T�C�Y�i�����w�肳��Ă��鎞�́��̃e�N�X�`���T�C�Y�Ɏ����ϊ��j
		false );											// �`���̃e�N�X�`���T�C�Y���Q�̗ݏ�ɂ��邩�ǂ���

	// �V�[���ɑ΂���G�t�F�N�g�̏�����
	//   �V�[���ɑ΂��čs���e�탌���_�����O�G�t�F�N�g�͂����ōs���܂��B
	pSceneMgr->InitParameter_Shading( SHADING_TYPE_NORMAL );							// �V�F�[�f�B���O
//		pSceneMgr->InitParameter_Fog( FOG_TYPE_DISABLE, FOG_EFFECT_LINEAR );				// �t�H�O
//		pSceneMgr->InitParameter_HighDynamicRange( HDR_TYPE_ENABLE, HDR_EFFECT_BLOOM );		// HDR�����_�����O
//		pSceneMgr->InitParameter_MotionBlur( MOTION_BLUR_TYPE_DISABLE );					// ���[�V�����u���[
//		pSceneMgr->InitParameter_DepthOfField( DOF_TYPE_DISABLE );							// ��ʊE�[�x
//		pSceneMgr->InitParameter_Shadow( SHADOW_TYPE_DISABLE, SHADOW_QUARITY_MIDDLE );		// �e����
//		pSceneMgr->InitParameter_AntiAliasType( AA_TYPE_DISABLE, 0.0f );					// �A���`�G�C���A�X

	// �V�[���̏������n�p�����[�^�[�̍X�V
	//  ������Ăяo�����ƂŃR���܂ł̐ݒ肪�L���ɂȂ�A
	//  �A�N�^�[�̐����⃌���_�����O���\�ɂȂ�B
	pSceneMgr->InitParameter_Update();

	//-----------------------------------------------------------------
	// �J����
	//-----------------------------------------------------------------
	// �V�[������J�������擾
	//   �V�[��1�ɑ΂��āA1�̃J�������������Ă��܂��B
	//   �V�[���̃J�����𑀍삷�邽�߂ɁA�J�����̃C���^�[�t�F�C�X��
	//   �V�[������擾���܂��B
	pCamera = pSceneMgr->GetCamera();
	// �J�����̃v���W�F�N�V������ݒ�
	//   �J�����̌�����͈͂ƌ�������ݒ肵�܂��B
	//   ������͈͂��L���قǂy�o�b�t�@�̐��x�̖���
	//   ������Ղ��Ȃ�̂Œ��ӂ��Đݒ肵�Ă��������B
	pCamera->UpdateProjection( 1.0f, 4096.0f, fovAngle , SCREEN_WIDTH, SCREEN_HEIGHT );

	//-----------------------------------------------------------------
	// �ύX���Ȃ��X�e�[�g
	//-----------------------------------------------------------------
	// �t�B���^�����O
	pRender->SetTextureFilterType( TEXTURE_STAGE_COLOR, TEXTURE_FILTER_3D_LOW );

	// �[�x�����̃X�e�[�g�ݒ�
	//   �[�x�o�b�t�@�Ɋւ��Ă̐ݒ���s���܂��B
	//   �[�x�o�b�t�@�ւ̏������݂�L���ɂ��A
	//   �[�x�o�b�t�@�̃e�X�g��L���ɂ��Ă��܂��B
	pRender->SetDepthTestEnable( true );
	pRender->SetDepthWriteEnable( true );

	// �`��X�e�[�g
	pRender->SetAlphaTestEnable( true );		// �A���t�@�e�X�gON
	pRender->SetDepthTestEnable( true );		// Z�e�X�gON
	pRender->SetDepthWriteEnable( true );		// Z��������ON
	pRender->SetDrawType( DRAW_TYPE_BLEND );	// �ʏ�`��

	return true;

}

// Selene�̃N���[���A�b�v
void CleanUp()
{
}

// Selene�I�u�W�F�N�g�AManager�n�ւ̃A�N�Z�X�N���X
Selene::Scene::ISceneManager *SystemGetter::GetSceneManager()
{
	return pSceneMgr;
}
Selene::Renderer::IRender *SystemGetter::GetRender()
{
	return pRender;
}

Selene::File::IFileManager *SystemGetter::GetFileManager()
{
	return pFileMgr;
}

Selene::ICore *SystemGetter::GetCore()
{
	return pCore;
}
Selene::Scene::ICamera *SystemGetter::GetCamera()
{
	return pCamera;
}
CModelManager *SystemGetter::GetModelManager()
{
	return pModelMgr;
}



};
