
#include "Selene.h"
using namespace Selene;

#include "common.h"
#include "CommonForMainWin.h"
#include "CreateModelFromFile.h"

void CreateModelFromFile( RFC_MODEL &request )
{
//	Renderer::Object::IModel *pModel;

	// �V�[���Ǘ��ɓ�������쐬����

	// �N���e�B�J���Z�N�V�����ɓ˓�
	EnterCriticalSection( &g_RFC_model_CS );
	// �V�[���ɓ������玩�����C�x���g���擾���Ă���
//	ResetEvent( g_hOutOfSceneMgrEvent );

//	pModel = g_pRender->CreateModelFromFile( fname );
	g_request_for_model.push_back(request);

	// �C�x���g���J��
//	SetEvent( g_hOutOfSceneMgrEvent );
	LeaveCriticalSection( &g_RFC_model_CS );

//	return request.pModel;
}
