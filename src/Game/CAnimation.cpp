
#include "CAnimation.h"

using namespace Selene;

// コンストラクタ
CAnimation::CAnimation()
{
	m_rot = m_pos = Math::Vector3D(0,0,0);
	m_speed = m_rot_speed = Math::Vector3D(0,0,0);
	m_front_rotation_axis = Math::Vector3D(0,1,0);
	m_front_rot = 0.0f;

	m_now_animation_no = 0;
	memset( m_script_action , 0 , sizeof(SCRIPT_ACTION)*MAX_SCRIPT_MOTION_NUM );
	m_is_executing = true;
	m_line_num = 0;
	m_now_exec_line = 0;
	m_past_frame_in_a_line = 0;
	m_now_frame = 0;
}

// コピーコンストラクタ
CAnimation::CAnimation( const CAnimation &object )
{
	m_pos = object.m_pos;
	m_rot = object.m_rot;
	m_speed = object.m_speed;
	m_rot_speed = object.m_rot_speed;
	m_front_rotation_axis = object.m_front_rotation_axis;
	m_front_rot = object.m_front_rot;

	m_now_animation_no = object.m_now_animation_no;
	memcpy( m_script_action , object.m_script_action , sizeof(SCRIPT_ACTION)*MAX_SCRIPT_MOTION_NUM );
	m_is_executing = object.m_is_executing;
	m_line_num = object.m_line_num;
	m_now_exec_line = object.m_now_exec_line;
	m_past_frame_in_a_line = object.m_past_frame_in_a_line;
	m_now_frame = object.m_now_frame;
}

CAnimation::~CAnimation()
{
}

// スクリプト以外の設定
void CAnimation::SetPos( const Math::Vector3D &pos )
{
	m_pos = pos;
}
void CAnimation::SetRotation( const Math::Vector3D &rot )
{
	m_rot = rot;
}
void CAnimation::SetSpeed( const Selene::Math::Vector3D &speed )
{
	m_speed = speed;
}

void CAnimation::SetRotationSpeed( const Selene::Math::Vector3D &rot_speed )
{
	m_rot_speed = rot_speed;
}

// 情報取得
//  frontの処理はここで大丈夫だろうか…UpdateのたびにRotationAxisをしなくて大丈夫なのだろうか…
void CAnimation::GetPosition(Math::Vector3D &pos)
{
	pos = m_pos;
	pos.RotationAxis( m_front_rot , m_front_rotation_axis );
}
void CAnimation::GetRotation(Math::Vector3D &rot)
{
	rot = m_rot;
	rot.RotationAxis( m_front_rot , m_front_rotation_axis );
}
unsigned int CAnimation::GetTimelineAnimationNumber()
{
	return m_now_animation_no;
}

// スクリプトライン追加
bool CAnimation::AddScriptLine(SCRIPT_ACTION &script_line)
{
	if( m_line_num >= MAX_SCRIPT_MOTION_NUM )
	{
		return false;
	}

	// 追加
	memcpy( m_script_action+m_line_num , &script_line , sizeof(SCRIPT_ACTION) );

	return true;
}

// スクリプト操作更新
bool CAnimation::Update( unsigned int update_frame_count )
{
	if( !m_is_executing )
	{
		return false;
	}
	// この再帰は…大丈夫だろうか
	unsigned int i;
	if( update_frame_count > 1 )
	{
		for( i=0 ; i<update_frame_count ; i++ )
		{
			if( !Update(1) )
			{
				return false;
			}
		}
	}
	else
	{
		// 処理
		SCRIPT_ACTION &now_line = m_script_action[m_now_exec_line];

		// posの処理
		int diff = (int)m_past_frame_in_a_line-(int)now_line.pos_action.delay;
		if( diff == 0 )
		{
			if( now_line.pos_action.valiable_vx )
			{
				m_speed.x = now_line.pos_action.speed.x;
			}
			if( now_line.pos_action.valiable_vy )
			{
				m_speed.y = now_line.pos_action.speed.y;
			}
			if( now_line.pos_action.valiable_vz )
			{
				m_speed.z = now_line.pos_action.speed.z;
			}

			m_pos += m_speed;
			m_speed += now_line.pos_action.accel;
		}
		else if( diff > 0 )
		{
			m_pos += m_speed;
			m_speed = now_line.pos_action.accel;
		}
		// rotationの処理
		diff = (int)m_past_frame_in_a_line -(int)now_line.rot_action.delay;
		if( diff == 0 )
		{
			if( now_line.rot_action.valiable_vx )
			{
				m_rot_speed.x = now_line.rot_action.speed.x;
			}
			if( now_line.rot_action.valiable_vy )
			{
				m_rot_speed.y = now_line.rot_action.speed.y;
			}
			if( now_line.rot_action.valiable_vz )
			{
				m_rot_speed.z = now_line.rot_action.speed.z;
			}
			m_rot += m_rot_speed;
			m_rot_speed += now_line.rot_action.accel;
		}
		else if( diff > 0 )
		{
			m_rot += m_rot_speed;
			m_rot_speed += now_line.rot_action.accel;
		}
		// timeline_anmationの処理
		if( m_past_frame_in_a_line >= now_line.tm_animation.delay && now_line.tm_animation.will_call )
		{
			m_now_animation_no = now_line.tm_animation.call_animation;
		}

		// アニメーション遷移処理
		m_now_frame++;
		if( ++m_past_frame_in_a_line == now_line.frame_to_next_action )
		{
			m_past_frame_in_a_line = 0;

			// ジャンプするか？
			if( now_line.jump_to_line_number >= 0 && now_line.jump_to_line_number < (int)m_line_num )
			{
				m_now_exec_line = now_line.jump_to_line_number;
			}
			else
			{
				// 終了？
				if( ++m_now_exec_line >= m_line_num )
				{
					m_is_executing = false;
					return false;
				}
			}
		}
	}

	return true;
}

// 次の実行ライン番号を変更
bool CAnimation::SetNextLine( unsigned int next_line )
{
	if( next_line >= m_line_num )
	{
		return false;
	}
	m_now_exec_line = next_line;

	return true;
}

// 実行状態を初期化
void CAnimation::ResetFrame()
{
	m_is_executing = true;
	m_now_exec_line = 0;
	m_past_frame_in_a_line = 0;
	m_now_frame = 0;
	m_speed = m_rot_speed = Math::Vector3D(0,0,0);
}

// 現在のアニメーションフレームを取得
unsigned int CAnimation::GetFrameInThisAnimation()
{
	return m_past_frame_in_a_line;
}
