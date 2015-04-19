
#include "Selene.h"
using namespace Selene;

#include "RenderingScene.h"
#include "CommonForSynchronize.h"

void RenderingScene( Renderer::IRender *pRender , Scene::ISceneManager *pSceneMgr )
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// �V�[���}�l�[�W���[�̓��e�����ۂɃ����_�����O
	/////////////////////////////////////////////////////////////////////////////////////////////


	// ��ʂւ̕`����J�n
	//   Renderer::IRender�C���^�[�t�F�C�X�։�ʂւ̕`����J�n���邱�Ƃ�ʒm���܂��B
	//   ���̊֐����Ă΂Ȃ��Ɖ�ʂւ̕`�悪�������s���܂���B
	pRender->Begin();

	// �V�[���̓��e�������_�����O
	//   �����܂łō\�z���ꂽ�V�[���������Ɏ��ۂɕ`�揈�����s���܂��B
	//   ���̊֐���Renderer::IRender::End()���Ăяo���O�łȂ���Ύ��s���܂��B
	pSceneMgr->Rendering( true );

	// ��ʂւ̕`�������
	//  Renderer::IRender�C���^�[�t�F�C�X�ɑ΂��āA
	//  ��ʂւ̕`�悪�����������Ƃ�ʒm���܂��B
	pRender->End();
}
