
#include "Selene.h"
using namespace Selene;

#include "common.h"
#include "CommonForMainWin.h"
#include "CreateModelActor.h"

void CreateModelActor( RFC_ACTOR &request )
{

	// �V�[���Ǘ��ɓ�������쐬����
	
	// �N���e�B�J���Z�N�V�����ɓ˓�
	EnterCriticalSection( &g_RFC_actor_CS );
	// �V�[���ɓ������玩�����C�x���g���擾���Ă���
//	ResetEvent( g_hOutOfSceneMgrEvent );

	g_request_for_actor.push_back( request );

	// �C�x���g���J��
//	SetEvent( g_hOutOfSceneMgrEvent );
	LeaveCriticalSection( &g_RFC_actor_CS );

}

