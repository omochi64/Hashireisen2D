
#include "UdongeActionConstances.h"

namespace Udonge
{
	const float MAX_WALK_SPEED = 0.08f;
	const float MAX_RUN_SPEED = 0.16f;
	const float ANTI_DIRECTION_ACCEL_IN_SKY = 0.003f;
	const float ANTI_DIRECTION_DASH_ACCEL_IN_SKY = 0.004f;
	const int	CANNOT_MOVE_TO_ANTI_DIRECTION_FRAME = 0;
	const float	STOP_ACCEL_X_SPEED_IN_SKY = 0.08f;
	const float WALK_ACCEL = 0.004f;
	const float RUN_ACCEL = 0.006f;
	const float DECCEL_ACCEL_IN_SKY = -0.0005f;
	const float DECCEL_ACCEL = -0.01f;
	const float	DECCEL_ACCEL_DASH = -0.015f;
	const float GRAVITY_ACCEL = -0.015f;
	const float FIRST_JUMPING_GRAVITY_ACCEL = (GRAVITY_ACCEL+0.011f);
	const float	JUMP_SPEED = 0.20f;
	const float RUNNING_JUMP_SPEED = (JUMP_SPEED+0.03f);
	const float RUNNING_JUMP_BOUNDARY = (MAX_RUN_SPEED-0.03f);
	const float	LAST_SPEED = (-(JUMP_SPEED+0.1f));		// èIí[ë¨ìx
	const float CAMERA_LOWEST_HEIGHT = -1.0f;
};
