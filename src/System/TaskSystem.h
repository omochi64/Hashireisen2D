
#pragma once

class TaskController;

class TaskControllBlock
{
private:
	void setID(int id){m_id = id;};
	void setPriority(unsigned int priority){m_priority = priority;}

protected:
	int	m_id;
	unsigned int m_priority;
	
public:
	friend TaskController;
	TaskControllBlock(){};
	virtual ~TaskControllBlock(){};

	int getID(void){return m_id;};
	unsigned int getPriority(void){return m_priority;}
	

	// 実行はここでされる
	virtual void run()=0;
};

#define		TASK_FREE	0
#define		TASK_USE	1

// タスクループのための空タスクブロック
class HeadTaskBlock:public TaskControllBlock
{
	
public:
	void run(){};
};

class TaskController
{
public:
	const static unsigned int MAX_TASK_COUNT = 300;

protected:
	typedef struct _task_list{
		TaskControllBlock	*block;
		struct _task_list	*next;
		struct _task_list	*prev;
		unsigned int		Flag;
		unsigned int		Prio;
		int					id;
	}TASK_LIST;

	TASK_LIST	m_tasks[MAX_TASK_COUNT];

public:
	TaskController();
	virtual ~TaskController();

	int AddTask( TaskControllBlock *block , unsigned int priority );
	bool DeleteTask( int id );
	TaskControllBlock *GetTaskBlock( int id );
	bool ChangeTask( int id , TaskControllBlock *change_block );

	void CleanUp( void );
	void Release( void );

	void ExecAllTask();

private:
	HeadTaskBlock	*head;
};
