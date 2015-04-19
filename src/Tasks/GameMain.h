
#pragma once

#include "Selene.h"
using namespace Selene;
#include "System/TaskSystem.h"
#include "Game/Objects.h"
#include "Game/Course.h"
#include "System/GlobalNamespace.h"


#define GAME_OVER_LINE	-3.0f

using namespace Character;

typedef enum HIT_DIRECTION{
	TOP , BOTTOM , LEFT , RIGHT , NONE
}HIT_DIRECTION;

class CameraController;

class GameMain:public TaskControllBlock
{
	Course			*m_course;

	std::vector<DETAIL_ANIMATION_OBJECT>*	m_course_enemies;
	std::vector<MultipleAnimation *>	m_beaten_enemies;	// 倒された敵たち
	std::vector<MultipleAnimation *>	m_limited_objects;	// アニメーション終了で消えるオブジェクトたち
	std::vector<Bullet *>				m_flying_bullets;	// 銃弾たち

	bool		is_jumping;
	bool		is_first_input_jump;
	int			is_jumping_direction_right;
//	DWORD		speed_zero_frame_in_anti_direction;

	bool		did_hit_wall_before_one_frame;
	bool		is_beaten;
	bool		is_goaled;

	COURSE_INFO	m_course_info;

	bool		m_is_stopped_udonge,m_is_stopped_enemies;

	int			m_world_num,m_course_num;
	int			m_rest_time;
//	DWORD		m_time_count_last_time;
	bool		m_is_counting_time;
	DWORD		m_resume_time;

	CameraController	*m_camera;
public:
	GameMain();
	virtual ~GameMain();

	// コース関連
	bool CreateCourse( int course_number_big , int course_number_small );
	Course *GetCourse( void ){return m_course;}
	void ReleaseCourse();

	int GetCourseNum(){return m_course_num;}
	int GetWorldNum(){return m_world_num;}
	Math::Point2DI GetCourseSize(){return Math::Point2DI(m_course->GetCourseWidth(),m_course->GetCourseHeight());}
	
	// カメラ
	void SetCamera( CameraController *camera ){m_camera = camera;}
	void StopAllEnemies();
	void StartAllEnemies();
	bool IsStoppedEnemies(){return m_is_stopped_enemies;}

	void StopUdonge();
	void StartUdonge();
	bool IsStoppedUdonge(){return m_is_stopped_udonge;}

	bool IsBeaten(){return is_beaten;}
	bool IsGoaled(){return is_goaled;}

	void StartCountingTime( int limit_time );
	void StopCountingTime( void );
	void ResumeCountingTime( void );
	void ReduceTime( int reduce_time );
	void SetTime( int time ){m_rest_time = time;}

	int GetRestTime( void ){return m_rest_time;}

	void PlayBGM();
	void StopBGM();
	void PauseBGM();

	void run();

private:
	void ControlUdonge();
	void ControlEnemies();
	void ControlBullets();
	void GameOverAnimation();

	void WinToEnemy( MultipleAnimation *enemy , int index_of_enemies , float first_y_speed =0.0f );

	void CheckCollisionUdonge();
	void CheckCollisionWithEnemy();
	
	void CheckCollisionEnemy( DETAIL_ANIMATION_OBJECT *enemy_object );

	bool ProcessForHittingEnemy();
	

	HIT_DIRECTION CheckHitDirection( Drawable2DObject *from , Drawable2DObject *to );
	float GetShareY( DrawableObject *ob1 , DrawableObject *ob2 );
	float GetShareX( DrawableObject *ob1 , DrawableObject *ob2 );
};

#include "CameraController.h"