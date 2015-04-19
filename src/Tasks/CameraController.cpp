
#include "System/InputData.h"
#include "CameraController.h"
#include "System/GameSystem.h"

CameraController::CameraController(GameMain *main_task)
{
	before_pos = Math::Vector3D(0,0,-DEFAULT_DISTANCE_TO_CAMERA);
	before_lookat = Math::Vector3D(0,0,-DEFAULT_DISTANCE_TO_CAMERA);
	move = Math::Vector3D(0,0,-DEFAULT_DISTANCE_TO_CAMERA);
	m_is_stopped = false;
	m_main_task = main_task;
}

void CameraController::run()
{
	// カメラの位置設定
	Math::Vector3D pos(0,0,-1),cam_pos;
	
	int move_rate = 60/GameSystem::GAME_FPS;

	if( GameSystem::is_searching_udonge && !m_is_stopped )
	{
		if( Character::udonge )
		{
			pos = Character::udonge->GetPos();
		}

		if( InputData::input_keys & KEY_CAM_LEFT )

		{
			move.x -= CAM_MOVE_SPEED*move_rate;
			if( move.x < -MAX_MOVE_X )
			{
				move.x = -MAX_MOVE_X;
			}
		}
		if( InputData::input_keys & KEY_CAM_RIGHT )
		{
			move.x += CAM_MOVE_SPEED*move_rate;
			if( move.x > MAX_MOVE_X )
			{
				move.x = MAX_MOVE_X;
			}
		}
		if( InputData::input_keys & KEY_CAM_UP )
		{
			move.y += CAM_MOVE_SPEED*move_rate;
			if( move.y > MAX_MOVE_Y )
			{
				move.y = MAX_MOVE_Y;
			}
		}
		if( InputData::input_keys & KEY_CAM_DOWN )
		{
			move.y -= CAM_MOVE_SPEED*move_rate;
			if( move.y < -MAX_MOVE_Y )
			{
				move.y = -MAX_MOVE_Y;
			}
		}
		if( InputData::input_keys & KEY_CAM_ZOOM )
		{
			move.z += CAM_MOVE_SPEED_Z*move_rate;
			if( -move.z < MAX_MOVE_Z_NEAREST )
			{
				move.z = -MAX_MOVE_Z_NEAREST;
			}
		}
		if( InputData::input_keys & KEY_CAM_ZOOM_OUT )
		{
			move.z -= CAM_MOVE_SPEED_Z*move_rate;
			if( -move.z > MAX_MOVE_Z_FAREST )
			{
				move.z = -MAX_MOVE_Z_FAREST;
			}
		}
		Math::Matrix rot_mat_x,rot_mat_y,move_mat;
		move_mat.Identity();
		move_mat.Translation( move.x , move.y , move.z );
		cam_pos = Math::Vector3D(0,0,0).TransformCoord( move_mat );
		GameSystem::pCamera->SetTransformSimple( pos+cam_pos , Math::Vector3D(pos) , DEG_TO_ANGLE( 0 ) );
		before_pos = pos+cam_pos;
		before_lookat = pos;
	}
	else
	{
		if( GameSystem::is_searching_only_y )
		{
			if( Character::udonge )
			{
				pos = Character::udonge->GetPos();
			}
			before_pos.y += pos.y-before_lookat.y;
			before_lookat.y = pos.y;
		}

		GameSystem::pCamera->SetTransformSimple( before_pos , before_lookat , 0 );
	}

	// 可視チェックの範囲指定
	Math::Rect2DF check_area;
	GetCheckVisibleArea( check_area );	
	Course *course = m_main_task->GetCourse();
	course->SetCheckVisibleArea( check_area );


	GameSystem::pCamera->Update();

	// 平行光源
	//   シーンに対して平行光源を設定します。
	GameSystem::pScene->SetLightParameter_Directional(
		Math::Vector3D( 0.0f, -1.0f, 0.3f ),		// ライトの方向
		Math::Vector3D( 0.8f, 0.8f, 0.8f ) );		// ライトの色

	// アンビエント
	// 半球ライティングを行っているので、天球と地表の照り返しの色を設定します。
	GameSystem::pScene->SetLightParameter_Ambient(
		Math::Vector3D( 0.2f, 0.2f, 0.2f ),			// 天球の色
		Math::Vector3D( 0.5f, 0.5f, 0.5f ) );		// 地表の色
}

void CameraController::GetCheckVisibleArea( Math::Rect2DF &area )
{
	float move_x = before_lookat.x-before_pos.x;
	float move_y = before_lookat.y-before_pos.y;

	area.x = before_lookat.x+move_x-CHECK_VISIBLE_AREA_WIDTH/2.0f;
	area.y = before_lookat.y+move_y-CHECK_VISIBLE_AREA_HEIGHT/2.0f;
	area.w = CHECK_VISIBLE_AREA_WIDTH;
	area.h = CHECK_VISIBLE_AREA_HEIGHT;
}
