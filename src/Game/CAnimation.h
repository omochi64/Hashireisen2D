
#pragma once

#include "Selene.h"

#define	MAX_SCRIPT_MOTION_NUM	16

class CAnimation
{
public:
	// スクリプトの情報
	struct SCRIPT_ACTION{
		unsigned int frame_to_next_action;
		struct TIMELINE_ANIMATION{
			unsigned int call_animation;
			unsigned int delay;
			bool will_call;
		}tm_animation;
		struct MOVING_STATE{
			Selene::Math::Vector3D	speed,accel;
			bool	valiable_vx,valiable_vy,valiable_vz;
			bool	valiable_ax,valiable_ay,valiable_az;
			unsigned int delay;
		}pos_action,rot_action;
		int	jump_to_line_number;
	};
protected:
	// 位置、回転管理
	Selene::Math::Vector3D	m_pos,m_speed;//,m_accel;
	Selene::Math::Vector3D	m_rot,m_rot_speed;//,m_rot_accel;

	// 向きの管理
	//  向きはこのクラス内では特に操作しない。が、向きによってspeedやaccelが変わるので保持する必要がある
	Selene::Math::Vector3D	m_front_rotation_axis;
	float	m_front_rot;
//	Selene::Math::Vector3D	m_default_front;
//	Selene::Math::Vector3D	m_now_front;

	// タイムラインアニメーション
	//  とりあえずアニメーション番号かな？
	unsigned int	m_now_animation_no;

	SCRIPT_ACTION m_script_action[MAX_SCRIPT_MOTION_NUM];

	unsigned int m_line_num;
	unsigned int m_now_exec_line;
	unsigned int m_past_frame_in_a_line;

	bool m_is_executing;
	unsigned int m_now_frame;

public:
	CAnimation();
	CAnimation( const CAnimation &object );
	virtual ~CAnimation();

	void SetPos( const Selene::Math::Vector3D &pos );
	void SetRotation( const Selene::Math::Vector3D &rotation );
	void SetSpeed( const Selene::Math::Vector3D &speed );
	void SetRotationSpeed( const Selene::Math::Vector3D &rot_speed );

	void SetFrontRotationAxis( Selene::Math::Vector3D &axis );
	void SetFrontRotation( float rot );

	void GetPosition( Selene::Math::Vector3D &pos );
	void GetRotation( Selene::Math::Vector3D &rot );
	unsigned int GetTimelineAnimationNumber();

	bool AddScriptLine( SCRIPT_ACTION &script_line );
	bool SetNextLine( unsigned int next_line );

	bool Update( unsigned int update_frame_count = 1 );
	unsigned int GetFrameInThisAnimation();
	void ResetFrame();
};
