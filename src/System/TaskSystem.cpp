
#include <string.h>

#include "System/TaskSystem.h"

// TaskController

TaskController::TaskController()
{
	memset( m_tasks , 0 , sizeof(TASK_LIST)*MAX_TASK_COUNT );

	head = new HeadTaskBlock();

	// ループ用に空ブロックを追加
	m_tasks[0].block = head;
	m_tasks[0].Prio = 0x000;
	m_tasks[0].prev = NULL;
	m_tasks[0].Flag = TASK_USE;

	m_tasks[0].next = m_tasks;
}

TaskController::~TaskController()
{
	Release();
	delete m_tasks[0].block;
}

int TaskController::AddTask(TaskControllBlock *block, unsigned int priority)
{
	if( !block )
	{
		return -1;
	}

	// 開いている部分を探す
	int i;
	int id;
	TASK_LIST *insert_point;
	for( i=1 ; i<MAX_TASK_COUNT ; i++ )
	{
		if( m_tasks[i].Flag == TASK_FREE )
		{
			insert_point = m_tasks+i;
			id = i;
			break;
		}
	}

	if( i==MAX_TASK_COUNT )
	{
		return -1;
	}

	// 優先度の昇順で挿入
	TASK_LIST *prev_point = m_tasks;
	while( prev_point->next->Prio != 0x000 )
	{
		if( prev_point->next->Prio > priority )
		{
			break;
		}
		prev_point = prev_point->next;
	}

	// 挿入
	if( insert_point->block )
	{
		delete insert_point->block;
		insert_point->block = NULL;
	}
	insert_point->block = block;
	insert_point->Flag = TASK_USE;
	insert_point->next = prev_point->next;
	insert_point->prev = prev_point;
	insert_point->Prio = priority;
	insert_point->next->prev = insert_point;
	insert_point->prev->next = insert_point;
	insert_point->id = id;

	block->setID( id );
	block->setPriority( priority );

	return id;
}

bool TaskController::DeleteTask(int id)
{
	if( id <= 0 || id >= MAX_TASK_COUNT || m_tasks[id].Flag == TASK_FREE )
	{
		return false;
	}

	m_tasks[id].Flag = TASK_FREE;

	delete m_tasks[id].block;
	m_tasks[id].block = NULL;
	m_tasks[id].next->prev = m_tasks[id].prev;
	m_tasks[id].prev->next = m_tasks[id].next;
	m_tasks[id].next = NULL;
	m_tasks[id].prev = NULL;

	return true;
}

TaskControllBlock *TaskController::GetTaskBlock( int id )
{
	if( id <= 0 || id >= MAX_TASK_COUNT || m_tasks[id].Flag == TASK_FREE )
	{
		return NULL;
	}

	return m_tasks[id].block;
}

bool TaskController::ChangeTask( int id , TaskControllBlock *change_block )
{
	if( id <= 0 || id >= MAX_TASK_COUNT || m_tasks[id].Flag == TASK_FREE )
	{
		return false;
	}

	change_block->setID(id);
	change_block->setPriority(m_tasks[id].block->getPriority());

	delete m_tasks[id].block;
	m_tasks[id].block = change_block;
	
	return true;
}

void TaskController::ExecAllTask( void )
{
	TASK_LIST *top = m_tasks;

	while( top && top->next && top->next->Prio != 0x000 )
	{
		top = top->next;
		top->block->run();
	}

	CleanUp();
}

void TaskController::Release( void )
{
	int i;
	for( i=1 ; i<MAX_TASK_COUNT ; i++ )
	{
		if( m_tasks[i].block )
		{
			delete m_tasks[i].block;
		}
		m_tasks[i].block = NULL;

		if( m_tasks[i].Flag == TASK_USE )
		{
			m_tasks[i].next = m_tasks[i].prev = NULL;
			m_tasks[i].Flag = TASK_FREE;
		}
	}
	m_tasks[0].Prio = 0x000;
	m_tasks[0].prev = NULL;
	m_tasks[0].Flag = TASK_USE;

	m_tasks[0].next = m_tasks;
}

void TaskController::CleanUp( void )
{
	// ゴミ消去
	int i;
	for( i=1 ; i<MAX_TASK_COUNT ; i++ )
	{
		if( m_tasks[i].Flag == TASK_FREE && m_tasks[i].block )
		{
			delete m_tasks[i].block;
			m_tasks[i].block = NULL;
		}
	}
}
