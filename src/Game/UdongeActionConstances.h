
#pragma once

// ここを変更。namespaceでexternのみにして定数定義はcppでするか、クラスにする

namespace Udonge
{
	extern const float MAX_WALK_SPEED;
	extern const float MAX_RUN_SPEED;
	extern const float ANTI_DIRECTION_ACCEL_IN_SKY;
	extern const float ANTI_DIRECTION_DASH_ACCEL_IN_SKY;
	extern const int	CANNOT_MOVE_TO_ANTI_DIRECTION_FRAME;
	extern const float	STOP_ACCEL_X_SPEED_IN_SKY;
	extern const float WALK_ACCEL;
	extern const float RUN_ACCEL;
	extern const float DECCEL_ACCEL_IN_SKY;
	extern const float DECCEL_ACCEL;
	extern const float	DECCEL_ACCEL_DASH;
	extern const float GRAVITY_ACCEL;
	extern const float FIRST_JUMPING_GRAVITY_ACCEL;
	extern const float	JUMP_SPEED;
	extern const float RUNNING_JUMP_SPEED;
	extern const float RUNNING_JUMP_BOUNDARY;
	extern const float	LAST_SPEED;		// 終端速度
	extern const float CAMERA_LOWEST_HEIGHT;
};
