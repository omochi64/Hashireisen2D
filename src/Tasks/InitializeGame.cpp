
#include "InitializeGame.h"
#include "GameMain.h"
#include "System/GlobalNamespace.h"
#include "Tasks/Renderer/Draw.h"
#include "Tasks/Renderer/DrawStatus.h"
#include "System/Manager/TextureManager.h"
#include "System/Manager/MultipleAnimationManager.h"
#include "Game/UdongeAnimation.h"
#include "Tasks/Renderer/DrawInfoBeforeGame.h"
#include "LoadDataBeforeGame.h"

#include "Game/SMFObject.h"

using namespace Character;

InitializeGame::InitializeGame(int world,int course)
{
	m_course = course;
	m_world = world;
}


void InitializeGame::run()
{

	// �{���A���̃^�X�N����
	// ���R�[�X�J�n�O�`��^�X�N
	// ���R�[�X��񃍁[�h�^�X�N
	// �ւƂ����Ă���

	// �܂�A���̃^�X�N�ɂ́A���Ɏ��s����R�[�X�ԍ���n���K�v������

	// �R�[�X���擾
	COURSE_INFO	ci;
	if( !GameSystem::course_manager->GetCourseInfo( m_world , m_course , ci ) )
	{
		MessageBox( NULL , "�R�[�X���擾���s" , NULL , NULL );
		exit(0);
	}

	// ���`��^�X�N��ǉ�
	TaskControllBlock *draw_task = new DrawInfoBeforeGame(m_world,m_course);
	LoadDataBeforeGame *load_task = new LoadDataBeforeGame( m_world , m_course , 3000 , draw_task );
	// �R�[�X���[�h�^�X�N�ɑJ��
	GameSystem::draw_task->Release();
	load_task->StartLoadAndWait();
	GameSystem::draw_task->AddTask( draw_task , 0xfffe );
	//GameSystem::all_task->ChangeTask( m_id , load_task );
	GameSystem::all_task->Release();
	GameSystem::all_task->AddTask( load_task , 0x01 );
	// 3000msec�͂Ƃ肠�����K��

}
