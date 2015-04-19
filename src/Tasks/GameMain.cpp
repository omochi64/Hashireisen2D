
#include <stdlib.h>
#include "GameMain.h"
#include "BeatenTask.h"
#include "GoToNextStage.h"
#include "Game/UdongeAnimation.h"
#include "AfterGoaledTask.h"
#include "Tasks/Renderer/DrawBackgroundTask.h"
#include "System/Sounds.h"
#include "Game/UdongeActionConstances.h"
#include "System/InputData.h"
#include "System/GameSystem.h"

#define	ENEMY_STOP_TIME_LIMIT		(312)	// �G�������Ȃ��Ȃ��Ă���~�܂�܂ł̃t���[�����~�b�g


using namespace Udonge;



GameMain::GameMain()
{
	m_course = NULL;
	m_course_enemies = NULL;
	is_jumping = false;
	is_first_input_jump = false;
	did_hit_wall_before_one_frame = false;
	m_world_num = m_course_num = 0;
	m_rest_time = 0;
	m_is_counting_time = false;
	is_beaten = false;
	is_goaled = false;
	m_is_stopped_udonge = false;
	m_is_stopped_enemies = false;
	is_jumping_direction_right = 0;
	m_camera = NULL;
}
GameMain::~GameMain()
{
	if( m_course )
	{
		ReleaseCourse();
	}
}

void GameMain::run()
{
	if( !is_beaten )
	{
		if( !m_is_stopped_udonge )
		{
			ControlUdonge();
			if( is_beaten )
			{
				return;
			}
		}
		if( !m_is_stopped_enemies )
		{
			ControlEnemies();
			ControlBullets();
		}

		if( m_is_counting_time )
		{
//			m_rest_time -= GameSystem::now_frame_time - m_time_count_last_time;
			m_rest_time--;
//			m_time_count_last_time = GameSystem::now_frame_time;

			if( m_rest_time/GameSystem::GAME_FPS <= 0 )
			{
				m_rest_time = 0;
				GameOverAnimation();
			}
		}
	}
}




void GameMain::ControlUdonge()
{
	if( !udonge )
	{
		return;
	}

	const UDONGE_ACTIONS & actions = udonge->GetAnimations();
	int now_animation = udonge->GetNowAnimation();
	Math::Vector3D accel = udonge->GetAccel();
	Math::Vector3D speed = udonge->GetSpeed();

	bool is_input_down = (InputData::input_keys & KEY_DOWN)?true:false;
	bool is_input_left = (InputData::input_keys & KEY_LEFT)?true:false;
	bool is_input_right = (InputData::input_keys & KEY_RIGHT)?true:false;
	bool is_input_jump = (InputData::input_keys & KEY_JUMP)?true:false;
	bool is_input_dash = (InputData::input_keys & KEY_DASH)?true:false;

	bool is_shooting_gun = udonge->IsShootingGun();

	bool is_anti_direction_in_sky = false;

	if( !is_goaled )
	{

		// actions.sit_down�����܂������Ȃ��c
		if( is_input_down && !is_jumping )
		{
			if( now_animation != actions.sit_down )
			{
				udonge->SetAnimationToSitDown();
				if( udonge->IsDirectionRight() && udonge->GetSpeed().x > 0 )
				{
					udonge->SetAccelX( -WALK_ACCEL );
				}
				else if( !udonge->IsDirectionRight() && udonge->GetSpeed().x < 0 )
				{
					udonge->SetAccelX( WALK_ACCEL );
				}
			}
			else
			{
				if( (udonge->IsDirectionRight() && udonge->GetSpeed().x <= 0) ||
					(!udonge->IsDirectionRight() && udonge->GetSpeed().x >= 0))
				{
					udonge->SetAccelX(0);
					udonge->SetSpeedX(0);
				}

			}
		}
		else if( ((!is_input_down||is_jumping) && now_animation == actions.sit_down ) ||
			now_animation != actions.sit_down )
		{
			if( is_input_right )
			{
				// �n�㐧��
				if( !is_jumping )
				{
					if( !is_input_dash )
					{
						// b�_�b�V������
						if( speed.x < 0 )
						{
							udonge->SetAccelX(-DECCEL_ACCEL);
						}
						else
						{
							if( speed.x >= MAX_WALK_SPEED )
							{
								udonge->SetAccelX(DECCEL_ACCEL);
							}
							else
							{
								udonge->SetAccelX(WALK_ACCEL);
								if( speed.x < MAX_WALK_SPEED && accel.x == DECCEL_ACCEL )
								{
									udonge->SetSpeedX(MAX_WALK_SPEED);
									udonge->SetAccelX(0);
								}
							}
						}
					}
					else
					{
						// b�_�b�V������
						if( speed.x < 0 )
						{
							udonge->SetAccelX(-DECCEL_ACCEL_DASH);
						}
						else
						{
							udonge->SetAccelX(RUN_ACCEL);
						}
						if( speed.x >= MAX_RUN_SPEED )
						{
							udonge->SetSpeedX(MAX_RUN_SPEED);
							udonge->SetAccelX(0);
						}
					}
				}
				else
				{
					if( is_jumping_direction_right == 0 )
					{
						is_jumping_direction_right = 1;
					}
					if( is_jumping_direction_right == -1 )
					{
						is_anti_direction_in_sky = true;
					}

					// �󒆐���
					if( !is_input_dash )
					{
						// b�_�b�V������
						if( speed.x > STOP_ACCEL_X_SPEED_IN_SKY )
						{
							// STOP_ACCEL_X_SPEED_IN_SKY�𒴂��Ă�����ASTOP_ACCEL_X_SPEED_IN_SKY�܂Ō���
							udonge->SetAccelX(DECCEL_ACCEL_IN_SKY);
						}
						else
						{
							if( accel.x == DECCEL_ACCEL_IN_SKY )
							{
								udonge->SetSpeedX(STOP_ACCEL_X_SPEED_IN_SKY);
								udonge->SetAccelX(0);
							}
							else
							{
								if( !is_anti_direction_in_sky )
								{
									// ������
									udonge->SetAccelX(WALK_ACCEL);
								}
								else
								{
									// �t����
									if( speed.x <= -ANTI_DIRECTION_ACCEL_IN_SKY || speed.x >= 0.0f )
									{
										udonge->SetAccelX(ANTI_DIRECTION_ACCEL_IN_SKY);
									}
									else
									{
										udonge->SetAccelX(-speed.x);
									}
								}
							}
						}
					}
					else
					{
						// B�_�b�V������
						// B�_�b�V���������Ă��ASTOP_ACCEL_X_SPEED_IN_SKY�ȏ�Ȃ�������Ȃ�
						if( speed.x >= STOP_ACCEL_X_SPEED_IN_SKY )
						{
							udonge->SetAccelX(0);
						}
						else
						{
							if( !is_anti_direction_in_sky )
							{
								udonge->SetAccelX(RUN_ACCEL);
							}
							else
							{
								// �t����
								if( speed.x <= -ANTI_DIRECTION_DASH_ACCEL_IN_SKY || speed.x >= 0.0f )
								{
									udonge->SetAccelX(ANTI_DIRECTION_DASH_ACCEL_IN_SKY);
								}
								else if( speed.x < 0 )
								{
									udonge->SetAccelX(-speed.x);
								}
							}
						}
					}
				}

				if( !is_jumping && !is_shooting_gun )
				{
					if( ( !is_input_dash && now_animation != actions.walk ) )
					{
						udonge->SetAnimationToWalk();
//						udonge->UpdateVertex();
					}
					else if( is_input_dash && now_animation != actions.run )
					{
						udonge->SetAnimationToRun();
//						udonge->UpdateVertex();
					}
					else if( !udonge->IsDirectionRight() )
					{
						udonge->SetDirection( true );
//						udonge->UpdateVertex();
					}
				}
				
			}

			if( is_input_left )
			{
//				if( !did_hit_wall_before_one_frame )
				{
					if( !is_jumping )
					{
						// �n�㐧��
						if( !is_input_dash )
						{
							if( speed.x > 0 )
							{
								udonge->SetAccelX(DECCEL_ACCEL);
							}
							else
							{
								if( speed.x < -MAX_WALK_SPEED )
								{
									udonge->SetAccelX(-DECCEL_ACCEL);
								}
								else
								{
									udonge->SetAccelX(-WALK_ACCEL);
									if( speed.x > -MAX_WALK_SPEED && accel.x == -DECCEL_ACCEL )
									{
										udonge->SetSpeedX(-MAX_WALK_SPEED);
										udonge->SetAccelX(0);
									}
								}
							}
						}
						else
						{
							// b�_�b�V������
							if( speed.x > 0 )
							{
								udonge->SetAccelX(DECCEL_ACCEL_DASH);
							}
							else
							{
								udonge->SetAccelX(-RUN_ACCEL);
							}
							if( speed.x <= -MAX_RUN_SPEED )
							{
								udonge->SetSpeedX(-MAX_RUN_SPEED);
								udonge->SetAccelX(0);
							}
						}
					}
					else
					{
						if( is_jumping_direction_right == 0 )
						{
							is_jumping_direction_right = -1;
						}
						if( is_jumping_direction_right == 1 )
						{
							is_anti_direction_in_sky = true;
						}

						// �󒆐���
						if( !is_input_dash )
						{
							// b�_�b�V������
							if( speed.x <= -STOP_ACCEL_X_SPEED_IN_SKY )
							{
								udonge->SetAccelX(-DECCEL_ACCEL_IN_SKY);
							}
							else
							{
								if( accel.x == -DECCEL_ACCEL_IN_SKY )
								{
									udonge->SetSpeedX(-MAX_WALK_SPEED);
									udonge->SetAccelX(0);
								}
								else
								{
									if( !is_anti_direction_in_sky )
									{
										udonge->SetAccelX(-WALK_ACCEL);
									}
									else
									{
										// �t����
										//speed_zero_frame_in_anti_direction
										if( speed.x >= ANTI_DIRECTION_ACCEL_IN_SKY || speed.x <= 0.0f )
										{
											udonge->SetAccelX(-ANTI_DIRECTION_ACCEL_IN_SKY);
										}
										else if( speed.x > 0 )
										{
											udonge->SetAccelX(-speed.x);
										}
									}
								}
							}
						}
						else
						{
							// b�_�b�V������
							if( speed.x <= -STOP_ACCEL_X_SPEED_IN_SKY )
							{
								udonge->SetAccelX(0);
							}
							else
							{
								if( !is_anti_direction_in_sky )
								{
									udonge->SetAccelX(-RUN_ACCEL);
								}
								else
								{
									// �t����
									if( speed.x >= ANTI_DIRECTION_DASH_ACCEL_IN_SKY || speed.x <= 0 )
									{
										udonge->SetAccelX(-ANTI_DIRECTION_DASH_ACCEL_IN_SKY);
									}
									else
									{
										udonge->SetAccelX(-speed.x);
									}
								}
							}
						}
					}
				}

				if( !is_jumping && !is_shooting_gun )
				{
					// �A�j���[�V�����̕ύX
					if( ( !is_input_dash && now_animation != actions.walk ) )
					{
						udonge->SetAnimationToWalk();
//						udonge->UpdateVertex();
					}
					else if( is_input_dash && now_animation != actions.run )
					{
						udonge->SetAnimationToRun();
//						udonge->UpdateVertex();
					}
					else if( udonge->IsDirectionRight() )
					{
						udonge->SetDirection( false );
//						udonge->UpdateVertex();
					}
				}
			}
		}

		// �e�����Ƃ��Ƃ��Ă���
		if( InputData::input_key_strokes[KEY_DASH] == 1 && !is_shooting_gun && now_animation != actions.sit_down )
		{
			Bullet *bullet = udonge->ShootTheBullet();
			if( bullet )
			{
				is_shooting_gun = true;
				is_first_input_jump = false;
				udonge->SetAnimationToShootingGun();
				bullet->GetBulletObject()->SetDirection( udonge->IsDirectionRight() );
				bullet->GetBulletObject()->SetCenterPosition( udonge->GetPos() );
				bullet->GetBulletObject()->Start();
				bullet->GetBulletObject()->SetWillChangeSpeedAccelWithDirectioni( true );
				bullet->BeginFlying();
				GameSystem::all_object->Add( bullet->GetBulletObject() );
				m_flying_bullets.push_back( bullet );
			}
		}

		
		

		
		// X�������ɑ΂��Ă͉������Ă��Ȃ��H
		if( !InputData::input_key_strokes[KEY_LEFT] && !InputData::input_key_strokes[KEY_DOWN] &&
			!InputData::input_key_strokes[KEY_RIGHT] )
		{
//			udonge->SetSpeedX(0);
			float deccel;
			if( is_jumping )
			{
				deccel = DECCEL_ACCEL_IN_SKY;
			}
			else
			{
				deccel = DECCEL_ACCEL;
			}

			// ������MAX_WALK_SPEED�ȏォ�H
			if( speed.x > MAX_WALK_SPEED )
			{
				udonge->SetAccelX(deccel);
			}
			else if( speed.x < -MAX_WALK_SPEED )
			{
				udonge->SetAccelX(-deccel);
			}
			else
			{
				// ������MAX_WALK_SPEED�ȉ��B�ŏ�����MAX_WALK_SPEED�ȉ��̏ꍇ���������̂܂܌�����������
				if( accel.x == deccel || is_jumping )
				{
					if( speed.x < -deccel )
					{
						udonge->SetAccelX(-speed.x);
					}
					else
					{
						udonge->SetAccelX(deccel);
					}
				}
				else if( accel.x == -deccel || is_jumping )
				{
					if( speed.x > deccel )
					{
						udonge->SetAccelX(-speed.x);
					}
					else
					{
						udonge->SetAccelX(-deccel);
					}
				}
				else
				{
					udonge->SetSpeedX(0);
					udonge->SetAccelX(0);
				}
			}


			if( speed.x == 0 )
			{
				udonge->SetAccelX(0);
			}
			else if( !is_jumping && now_animation != actions.walk && !is_shooting_gun )
			{
				udonge->SetAnimationToWalk();
//				udonge->UpdateVertex();
				now_animation = actions.walk;
			}

			if( now_animation != actions.stand && speed.x == 0 && speed.z == 0 && !is_jumping && !is_shooting_gun )
			{
				udonge->Stop();
				udonge->Start();
				now_animation = udonge->GetNowAnimation();
			}
		}

		
		/*****�e�X�g*****/
		// ��Ɉړ�
		//if( InputData::input_keys & KEY_UP )
		//{
		//	udonge->SetSpeedZ(MAX_WALK_SPEED);
		//	if( now_animation != actions.walk )
		//	{
		//		udonge->SetAnimationToWalk();
		//		now_animation = actions.walk;
		//	}
		//}
		//
		//// ���Ɉړ�
		//else if( InputData::input_keys & KEY_DOWN )
		//{
		//	if( now_animation != actions.walk )
		//	{
		//		udonge->SetAnimationToWalk();
		//		now_animation = actions.walk;
		//	}
		//	udonge->SetSpeedZ(-MAX_WALK_SPEED);
		//}
		//else
		//{
		//	udonge->SetSpeedZ(0);
		//}
		/*****�e�X�g*****/

		const Math::Vector3D &mari_pos = udonge->GetPos();
		static float jumping_start_y = mari_pos.y;

		// �W�����v�Ɋւ���`�F�b�N
		if( is_input_jump )
		{
			if( is_jumping && is_first_input_jump && speed.y <= 0 )
			{
				// �������͒ʏ�d�͉����x�ɖ߂�
				udonge->SetAccelY( GRAVITY_ACCEL );
				is_first_input_jump = false;
			}

			if( !is_jumping && InputData::input_key_strokes[KEY_JUMP] == 1 )
			{
				if( now_animation != actions.sit_down )
				{
					udonge->SetAnimationToJump();
				}
				is_jumping = true;
				is_first_input_jump = true;
				if( speed.x > 0  )
				{
					// �E�����ɐi��ł���
					is_jumping_direction_right = 1;
					if( speed.x >= RUNNING_JUMP_BOUNDARY )
					{
						udonge->SetSpeedY( RUNNING_JUMP_SPEED );
					}
					else
					{
						udonge->SetSpeedY( JUMP_SPEED );
					}
				}
				else if( speed.x < 0 )
				{
					// �������ɐi��ł���
					is_jumping_direction_right = -1;
					if( speed.x <= -RUNNING_JUMP_BOUNDARY )
					{
						udonge->SetSpeedY( RUNNING_JUMP_SPEED );
					}
					else
					{
						udonge->SetSpeedY( JUMP_SPEED );
					}
				}
				else
				{
					// �ǂ���ɂ��i��ł��Ȃ�
					udonge->SetSpeedY( JUMP_SPEED );
					is_jumping_direction_right = 0;
				}
				
				udonge->SetAccelY( FIRST_JUMPING_GRAVITY_ACCEL );
				jumping_start_y = mari_pos.y;

				GameSound::sound_manager->Stop( GameSound::jump );
				GameSound::sound_manager->Play( GameSound::jump );
			}
		}
		else
		{
			is_first_input_jump = false;
			if( is_jumping )
			{
				// �W�����v�{�^���𗣂����̂Œʏ�d�͉����x�ɖ߂�
				udonge->SetAccelY( GRAVITY_ACCEL );
			}
		}
	}

	if( !is_goaled )
	{
		float pos_y = udonge->GetPos().y;
		if( pos_y < CAMERA_LOWEST_HEIGHT )
		{
			GameSystem::is_searching_udonge = false;
			GameSystem::is_searching_only_y = false;
		}
		else
		{
			GameSystem::is_searching_udonge = true;
		}
		
		if( pos_y < GAME_OVER_LINE )
		{
			GameOverAnimation();
		}
		else
		{
			CheckCollisionUdonge();
			CheckCollisionWithEnemy();
		}
	}
	else
	{
		CheckCollisionUdonge();
	}

	if( udonge->GetSpeed().y < LAST_SPEED )
	{
		udonge->SetSpeedY(LAST_SPEED);
	}
	
	if( !udonge->UpdateAnimation()  )
	{
		udonge->Start();
	}
}

void GameMain::ControlEnemies( void )
{
	if( !m_course_enemies || is_beaten || m_is_stopped_enemies )
	{
		return;
	}

	DETAIL_ANIMATION_OBJECT *enemy_object;
	MultipleAnimation *enemy;
	
	int i;
	int e_size = m_course_enemies->size();
	DWORD invisible_period;
	Math::Rect2DF check_visible_area;
	if( m_camera )
	{
		m_camera->GetCheckVisibleArea( check_visible_area );
	}
	
	for( i=0 ; i < e_size ; i++ )
	{
		enemy_object = &m_course_enemies->at(i);
		enemy = enemy_object->object;

		// Action���s�����ǂ����̔�����ŏ��ɂ���
		invisible_period = enemy->GetInvisiblePeriod();

		// ��x�ł��p���������H
		if( enemy->GetLastVisibleTime() > 0 )
		{
			// ��x�ł�������
			// �����ĂȂ����Ԃ�Limit�𒴂��Ă�H
			if( invisible_period >= ENEMY_STOP_TIME_LIMIT )
			{
				GameSystem::all_object->SetAction( enemy , false );
			}
			else
			{
				// ������on�ɂ���
				GameSystem::all_object->SetAction( enemy , true );

				// �G�̃t�B�[���h�ɑ΂��铖���蔻��c
				CheckCollisionEnemy( enemy_object );
				
				if( enemy_object->object->GetPos().y < GAME_OVER_LINE )
				{
					GameSystem::all_object->Delete( enemy_object->object );
					SAFE_RELEASE(enemy_object->object);
					m_course_enemies->erase( m_course_enemies->begin()+i );
					i--,e_size--;
				}
				else
				{
					enemy_object->object->UpdateAnimation();
					if( enemy_object->object->GetSpeed().y < LAST_SPEED )
					{
						enemy_object->object->SetSpeedY(LAST_SPEED);
					}
				}
			}

			// ������̈�ɓ��������H
			const Math::Vector3D &pos = enemy->GetPos();
			if( !(pos.x>=check_visible_area.x && pos.x<=check_visible_area.x+check_visible_area.w &&
				pos.y>=check_visible_area.y && pos.y<=check_visible_area.y+check_visible_area.h) )
			{
				// �����Ă��Ȃ�
				GameSystem::all_object->SetDraw( enemy , false );
			}
			else
			{
				// ������
				GameSystem::all_object->SetDraw( enemy , true );
			}
			
		}

	}

	// �����A�j���[�V�����̓G����
	// �������G�̏������܂����̂��A�G���������Actor�̃��[�N�������Ă��܂�

	e_size = m_beaten_enemies.size();
	for( i=0 ; i<e_size ; i++ )
	{
		enemy = m_beaten_enemies.at(i);
		if( enemy->GetPos().y < GAME_OVER_LINE )
		{
			GameSystem::all_object->Delete( enemy );
			m_beaten_enemies.erase( m_beaten_enemies.begin()+i );
			SAFE_RELEASE(enemy);
			i--,e_size--;
			continue;
		}

		// ������̈�ɓ��������H
		const Math::Vector3D &pos = enemy->GetPos();
		if( !(pos.x>=check_visible_area.x && pos.x<=check_visible_area.x+check_visible_area.w &&
			pos.y>=check_visible_area.y && pos.y<=check_visible_area.y+check_visible_area.h) )
		{
			// �����Ă��Ȃ�
			GameSystem::all_object->SetDraw( enemy , false );
		}
		else
		{
			// ������
			GameSystem::all_object->SetDraw( enemy , true );
		}
	}

	// ����A�j���[�V����
	e_size = m_limited_objects.size();
	MultipleAnimation *limited;
	for( i=0 ; i<e_size ; i++ )
	{
		limited = m_limited_objects.at(i);
		if( !limited->UpdateAnimation() )
		{
			GameSystem::all_object->Delete( limited );
			SAFE_RELEASE(limited);
			m_limited_objects.erase( m_limited_objects.begin()+i );
			i--,e_size--;
		}
	}
}

// �e�e����
void GameMain::ControlBullets()
{
	// �e
	int i;
	int e_size;
	
	int enemy_size = m_course_enemies->size();
	e_size = m_flying_bullets.size();

	
	MultipleAnimation *bullet_object;

	Bullet *bullet;
	for( i=0 ; i<e_size ; i++ )
	{
		bullet = m_flying_bullets.at(i);
		bullet_object = bullet->GetBulletObject();
		if( !bullet->CanContinueToFly() || bullet_object->GetInvisiblePeriod() > 0 )
		{
			GameSystem::all_object->Delete( bullet_object );
			SAFE_RELEASE(bullet);
			m_flying_bullets.erase( m_flying_bullets.begin()+i );
			i--,e_size--;
			continue;
		}
		else
		{
			bullet_object->UpdateAnimation();
		
			// �R�[�X�Ƃ̓�����`�F�b�N
			// �e�e�ƃR�[�X�̓����蔻�肪���������H�����������Ă���Ƃ���������
			// �����蔻��̈�̐ݒ�~�X���Ais_right_direction == false�̂Ƃ���GetCollisionRect����������
			const Math::Vector3D & bullet_pos = bullet_object->GetPos();
			// �����蔻����{�G���A
			float width = bullet_object->GetWidth() , height = bullet_object->GetHeight();

			Math::Rect2DF check_area;
			check_area.SetCenter( bullet_pos.x , bullet_pos.y , width*2 , height*2 );
	
			m_course->SetCheckCollisionRect( check_area );
			if( m_course->DetectCollision( *bullet_object ) )
			{
				GameSystem::all_object->Delete( bullet_object );
				SAFE_RELEASE(bullet);
				m_flying_bullets.erase( m_flying_bullets.begin()+i );
				i--,e_size--;
				continue;
			}

			// �G�Ƃ̓�����`�F�b�N
			int j;
			
			for( j=0 ; j<enemy_size ; j++ )
			{
				DETAIL_ANIMATION_OBJECT &enemy_object = m_course_enemies->at(j);
				MultipleAnimation *enemy = enemy_object.object;

				const Math::Vector3D &pos = enemy->GetPos();
				if( pos.x < check_area.x || pos.x > check_area.x+check_area.w ||
					pos.y < check_area.y || pos.y > check_area.y+check_area.h )
				{
					continue;
				}

				// �G�Ɠ����������H
				if( bullet_object->DetectCollision( *enemy ) )
				{
					// �G��HP�����炷(�܂��G��HP���������Ă��Ȃ��̂ŁA���i�K�ł͂����|��)
					udonge->IncreasePoint( enemy_object.hit_info.point  );
					WinToEnemy( enemy , j );

					// bullet�̎c��ђʐ���1���炷
					if( bullet->DecreasePiercefulCount() <= 0 )
					{
						// bullet�͎���
						GameSystem::all_object->Delete( bullet_object );
						SAFE_RELEASE(bullet);
						m_flying_bullets.erase( m_flying_bullets.begin()+i );
						i--,e_size--;
						enemy_size--;
						break;
					}
				}
			}
		}
	}
}

// ���ǂ񂰂ƃR�[�X�̓����蔻��
void GameMain::CheckCollisionUdonge( void )
{
	// �����蔻����{�G���A
	const UDONGE_ACTIONS & actions = udonge->GetAnimations();
	Math::Rect2D<Float> check_area;
	float width = udonge->GetWidth() , height = udonge->GetHeight();
	
	Math::Vector2D direction;
	int i;
	int object_num;	

	bool is_flying = true;
	float arriving_y;

	float m_w , m_h;
	
	bool is_input_down = (InputData::input_keys & KEY_DOWN)?true:false;
	bool is_input_left = (InputData::input_keys & KEY_LEFT)?true:false;
	bool is_input_right = (InputData::input_keys & KEY_RIGHT)?true:false;
//	bool is_input_jump = (InputData::input_keys & KEY_JUMP)?true:false;
	bool is_input_dash = (InputData::input_keys & KEY_DASH)?true:false;
	
	// �R�[�X�Ƃ̓����蔻��
	Math::Vector3D marisa_pos = udonge->GetPos();
	Math::Vector3D marisa_speed = udonge->GetSpeed();
	Math::Vector3D marisa_accel = udonge->GetAccel();
//	Math::Vector2D changed_pos = udonge->GetSpeed();
//	changed_pos.x = 0;
//	udonge->SetCenterPosition( Math::Vector3D( marisa_pos.x + changed_pos.x , 
//		marisa_pos.y + changed_pos.y , marisa_pos.z ) );

	marisa_pos = udonge->GetPos();
	Math::Rect2D<Float> col_rc;
	udonge->GetCollisionRect( col_rc );
	m_w = col_rc.w , m_h = col_rc.h;


	check_area.SetCenter( marisa_pos.x , marisa_pos.y , width*2 , height*2 );

	did_hit_wall_before_one_frame = false;

	float share_y_total = 0.0f;
	float share_x_total = 0.0f;

	int size;
	Math::Vector3D enemy_pos;


	m_course->SetCheckCollisionRect( check_area );
	if( m_course->DetectCollision( *udonge ) )
	{
		vector<DETAIL_ANIMATION_OBJECT> &col_result = m_course->GetCollisionResult();
		object_num = col_result.size();
		for( i=0 ; i<object_num ; i++ )
		{
			DETAIL_ANIMATION_OBJECT result = col_result.at(i);
			MultipleAnimation *object = result.object;


			const Math::Vector3D & pos = object->GetPos();

			Math::Rect2D<Float> char_col,obj_col;
			udonge->GetCollisionRect( char_col );
			object->GetCollisionRect( obj_col );

			HIT_DIRECTION res = CheckHitDirection( udonge , object );

			int j;
			int temp_i;
			DETAIL_ANIMATION_OBJECT info_temp;
			
			switch( result.type )
			{
			case GOAL:
				// �S�[���ɂԂ�����
				if( !is_goaled )
				{
					is_goaled = true;
					GameSystem::all_task->AddTask( new AfterGoaledTask( this , m_course ) , 0xffff );
					// �S�[���I�u�W�F������
					m_course->DeleteObject( object , Math::Rect2DF( pos.x-obj_col.w/2 , pos.y-obj_col.h/2 , obj_col.w , obj_col.h ) );
					StopCountingTime();
					return;
				}
				break;

			case OBJECT_HEART:
				// ���ǂ񂰂�HP���܂��܂����ăI�u�W�F�N�g������
				udonge->IncreaseHP();
				m_course->DeleteObject( object , Math::Rect2DF( pos.x-m_w , pos.y-m_h , m_w*2 , m_h*2 ) );
				udonge->IncreasePoint( result.hit_info.point );
				GameSound::sound_manager->Stop( GameSound::get_heart );
				GameSound::sound_manager->Play( GameSound::get_heart );
				break;

			case OBJECT_ONE_UP:
				// 1��p
				udonge->IncreaseRestLife();
				m_course->DeleteObject( object , Math::Rect2DF( pos.x-m_w , pos.y-m_h , m_w*2 , m_h*2 ) );
				udonge->IncreasePoint( result.hit_info.point );
				GameSound::sound_manager->Stop( GameSound::one_up );
				GameSound::sound_manager->Play( GameSound::one_up );
				break;


			case OBJECT_POINT:
				// �|�C���g�I�u�W�F�N�g
				if( udonge->IncreaseCoin() )
				{
					// RestLife�A�b�v�B���Ƃ��炷
					GameSound::sound_manager->Stop( GameSound::one_up );
					GameSound::sound_manager->Play( GameSound::one_up );
				}
				else
				{
					GameSound::sound_manager->Stop( GameSound::get_point );
					GameSound::sound_manager->Play( GameSound::get_point );
				}
				udonge->IncreasePoint( result.hit_info.point );
				m_course->DeleteObject( object , Math::Rect2DF( pos.x-m_w , pos.y-m_h , m_w*2 , m_h*2 ) );
				break;


			default:
				// ���̑�(�ʏ�u���b�N��)
				switch( res )
				{
				case LEFT:
					// ������Ԃ�����
					// �i�s�����͉E�H
					if( result.type == OBJECT_KOUMEI_POINT_BLOCK ||
						result.type == OBJECT_KOUMEI_ITEM_BLOCK )
					{
						break;
					}

					marisa_speed = udonge->GetSpeed();
					if( marisa_speed.x > 0 )
					{
						share_x_total += GetShareX( udonge , object );

						udonge->SetSpeedX(0.0f);
						udonge->SetAccelX(0.0f);
						marisa_pos.x -= char_col.x+char_col.w-obj_col.x;
						did_hit_wall_before_one_frame = true;
					}
					else if( (marisa_speed.x == 0 && marisa_speed.y > 0 && share_x_total == 0 ))
					{
						share_x_total += GetShareX( udonge , object );
						marisa_pos.x -= char_col.x+char_col.w-obj_col.x;
						did_hit_wall_before_one_frame = true;
					}
					break;

				case RIGHT:
					// �E����Ԃ�����
					// �i�s�����͍��H
					if( result.type == OBJECT_KOUMEI_POINT_BLOCK ||
						result.type == OBJECT_KOUMEI_ITEM_BLOCK )
					{
						break;

					}
					marisa_speed = udonge->GetSpeed();
					if( marisa_speed.x < 0 )
					{
						share_x_total += GetShareX( udonge , object );

						udonge->SetSpeedX(0.0f);
						udonge->SetAccelX(0.0f);
						marisa_pos.x += obj_col.x+obj_col.w-char_col.x;
						did_hit_wall_before_one_frame = true;
					}
					else if( (marisa_speed.x == 0 && marisa_speed.y > 0 && share_x_total == 0 ))
					{
						share_x_total += GetShareX( udonge , object );

						marisa_pos.x += obj_col.x+obj_col.w-char_col.x;
						did_hit_wall_before_one_frame = true;
					}
					break;

				case TOP:
					// �ォ��Ԃ�����
					if( result.type == OBJECT_KOUMEI_POINT_BLOCK ||
						result.type == OBJECT_KOUMEI_ITEM_BLOCK )
					{
						break;
					}

					share_y_total += GetShareY( udonge , object );

					is_flying = false;
					arriving_y = marisa_pos.y+(obj_col.y+obj_col.h-char_col.y);//height/2;
					break;

				case BOTTOM:
					// ������Ԃ�����
					switch( result.type )
					{
					case OBJECT_BROKEN_BLOCK:
						// 8�ق�hahen�I�u�W�F�N�g���쐬�A�ǉ�
						MultipleAnimation *hahen;
						temp_i = (int)(MAX_WALK_SPEED*100);
						for( j=0 ; j<4 ; j++ )
						{
							hahen = GameSystem::multiple_animation_manager->GetCopyMultipleAnimationObject( "hahen" );
							hahen->Start();
							hahen->SetCenterPosition( pos );
							hahen->SetSpeedX( (rand()%2==0?-1:1)*(rand()%temp_i)/100.0f );
							hahen->SetRotation( Math::Vector3D( (float)(rand()%360) , (float)(rand()%360) , (float)(rand()%360) ) );
							hahen->SetSpeedY( JUMP_SPEED/2 );
							hahen->SetAccelY( GRAVITY_ACCEL/2 );
							GameSystem::all_object->Add( hahen );
							m_beaten_enemies.push_back( hahen );
						}

						// �󂵂��u���b�N�̏�ɓG�����邩�H
						size = m_course_enemies->size();
						//Math::Rect2DF cl;
						for( j=0 ; j<size ; j++ )
						{
							DETAIL_ANIMATION_OBJECT &enemy_temp = m_course_enemies->at(j);
							if( enemy_temp.object->GetInvisiblePeriod() > 0 )
							{
								continue;
							}
							if( enemy_temp.object->DetectCollision( *object ) &&
								CheckHitDirection( enemy_temp.object , object ) == TOP &&
								enemy_temp.type != OBJECT_FLYING_ENEMY )
							{
								udonge->IncreasePoint( enemy_temp.hit_info.point );
								WinToEnemy( enemy_temp.object , j , JUMP_SPEED/2 );
								j--,size--;
							}
						}

						// ���_�Q�b�g
						udonge->IncreasePoint( result.hit_info.point );

						udonge->SetSpeedY( 0 );

						// ������BrokenBlockAnimation��m_beaten_enemies�ɒǉ�
						m_course->DeleteObject( object , Math::Rect2DF( pos.x-m_w , pos.y-m_h , m_w*2 , m_h*2 ) );

						GameSound::sound_manager->Stop( GameSound::block_crash );
						GameSound::sound_manager->Play( GameSound::block_crash );

						break;

					case OBJECT_ITEM_BLOCK:
					case OBJECT_POINT_BLOCK:
					case OBJECT_KOUMEI_POINT_BLOCK:
					case OBJECT_KOUMEI_ITEM_BLOCK:

						marisa_pos.y -= (char_col.y+char_col.h-obj_col.y);
						udonge->SetSpeedY( -udonge->GetSpeed().y/2 );

						// �u���b�N��ɂ̂��Ă���G��|��
						size = m_course_enemies->size();
						
						bool is_riding;
						//Math::Rect2DF cl;
						for( j=0 ; j<size ; j++ )
						{
							DETAIL_ANIMATION_OBJECT &enemy_temp = m_course_enemies->at(j);
							if( enemy_temp.object->GetInvisiblePeriod() > 0 )
							{
								continue;
							}
							enemy_pos = enemy_temp.object->GetPos();
							enemy_temp.object->SetCenterPosition( Math::Vector3D(enemy_pos.x,enemy_pos.y+enemy_temp.object->GetSpeed().y,enemy_pos.z) );
							is_riding = enemy_temp.object->DetectCollision( *object );
							enemy_temp.object->SetCenterPosition( enemy_pos );
							if( is_riding && CheckHitDirection( enemy_temp.object , object ) == TOP &&
								enemy_temp.type != OBJECT_FLYING_ENEMY )
							{
								udonge->IncreasePoint( enemy_temp.hit_info.point );
								WinToEnemy( enemy_temp.object , j , JUMP_SPEED/2 );
								j--,size--;
							}
						}


						if( result.type == OBJECT_ITEM_BLOCK ||
							result.type == OBJECT_KOUMEI_ITEM_BLOCK )
						{
							// �A�C�e���o���u���b�N
							// �A�C�e���o��
							info_temp.object = GameSystem::multiple_animation_manager->GetCopyMultipleAnimationObject( 
								result.hit_info.item_block_info.object_name );
							if( info_temp.object == NULL )
							{
								break;
							}
//							info_temp.object->UpdateVertex();
							info_temp.type = result.hit_info.item_block_info.object_type;
							info_temp.hit_info.point = result.hit_info.item_block_info.object_point;
							info_temp.object->Start();
							if( !m_course->AddObject( &info_temp , Math::Point2DF( pos.x , pos.y+object->GetHeight() ) ) )
							{
								delete info_temp.object;
								break;
							}
							GameSound::sound_manager->Stop( GameSound::appear_heart );
							GameSound::sound_manager->Play( GameSound::appear_heart );
						}
						else if( result.type == OBJECT_POINT_BLOCK ||
							result.type ==  OBJECT_KOUMEI_POINT_BLOCK )
						{
							// �|�C���g�u���b�N
							info_temp.object = GameSystem::multiple_animation_manager->GetCopyMultipleAnimationObject( 
								result.hit_info.item_block_info.object_name );
							if( info_temp.object == NULL )
							{
								break;
							}
							Math::Rect2DF col_temp;
							info_temp.object->GetCollisionRect( col_temp );
							info_temp.object->SetCenterPosition( 
								Math::Vector3D( pos.x , pos.y+object->GetHeight()/2+col_temp.h/2 , 0 )
								);
							info_temp.object->SetAnimationNumber(0);
							info_temp.object->Start();
							info_temp.object->SetLoop(0,false);
//							info_temp.object->UpdateVertex();
							udonge->IncreaseCoin();
							udonge->IncreasePoint( result.hit_info.item_block_info.object_point );

							// m_beaten_animation�Ƀ|�C���g�A�C�e��������c���Ⴞ�߂��Ȃ�
							// m_limited_objects�ɓ����
							m_limited_objects.push_back( info_temp.object );
							GameSystem::all_object->Add( info_temp.object );

							GameSound::sound_manager->Stop( GameSound::get_point );
							GameSound::sound_manager->Play( GameSound::get_point );
						}

						// ���݂̃u���b�N��after_block�ɂ���
						info_temp.object = GameSystem::multiple_animation_manager->GetCopyMultipleAnimationObject( 
							result.hit_info.item_block_info.after_object );
						if( info_temp.object == NULL )
						{
							break;
						}
						info_temp.type = result.hit_info.item_block_info.after_object_type;
						info_temp.hit_info.point = result.hit_info.item_block_info.after_object_point;
						info_temp.object->Start();

						if( !m_course->ChangeObject( object , &info_temp , Math::Rect2DF( pos.x-m_w , pos.y-m_h , m_w*2 , m_h*2 ) ) )
						{
							delete info_temp.object;
							break;
						}

						break;

					default:
						marisa_pos.y -= (char_col.y+char_col.h-obj_col.y);//object->GetHeight()/2-height/2;
						if( udonge->GetSpeed().y > 0 )
						{
							udonge->SetSpeedY( -udonge->GetSpeed().y/2 );
						}
					}
					break;
					
				}
			}
		}
	}

	if( share_y_total*0.5 > share_x_total )
	{
		udonge->SetSpeedX( marisa_speed.x );
		udonge->SetAccelX( marisa_accel.x );
		marisa_pos.x = udonge->GetPos().x;
	}

//	marisa_pos.x -= changed_pos.x;
//	marisa_pos.y -= changed_pos.y;
	// �󒆂ɂ��邩�ǂ���
	if( is_flying )
	{
		if( udonge->GetNowAnimation() != actions.sit_down &&
//			udonge->GetNowAnimation() != actions.reduce_hp &&
			!is_goaled && !udonge->IsShootingGun() )
		{
			udonge->SetAnimationToJump();
		}
		if( !is_first_input_jump )
		{
			udonge->SetAccelY(GRAVITY_ACCEL);
		}
		is_jumping = true;
//		udonge->SetSpeedY( 0.3f );
		udonge->SetCenterPosition( marisa_pos );
	}
	else if( udonge->GetSpeed().y < 0.0f && is_jumping )
	{
		is_jumping_direction_right = 0;
//		speed_zero_frame_in_anti_direction = 0;
		udonge->SetCenterPosition( Math::Vector3D( marisa_pos.x , arriving_y , marisa_pos.z ) );
		udonge->SetSpeedY( 0.0f );
		udonge->SetAccelY( 0.0f );

		if( !is_goaled )
		{
//			if( udonge->GetNowAnimation() != actions.reduce_hp )
			{
				if( is_input_down )
				{
					udonge->SetAnimationToSitDown();
					if( udonge->IsDirectionRight() )
					{
						udonge->SetAccelX(-0.01f);
					}
					else
					{
						udonge->SetAccelX(0.01f);
					}
		//			udonge->SetSpeedX(0);
//					udonge->UpdateVertex();
				}
				else if( is_input_right && !is_input_left )
				{
					if( is_input_dash )
					{
						udonge->SetAnimationToRun();
					}
					else
					{
						udonge->SetAnimationToWalk();
					}
//					udonge->SetAccelX(0);
		//			udonge->SetSpeedX( 0.1f );
					if( !udonge->IsDirectionRight() )
					{
						udonge->SetDirection( true );
					}
//					udonge->UpdateVertex();
				}
				else if( !is_input_right && is_input_left )
				{
					if( is_input_dash )
					{
						udonge->SetAnimationToRun();
					}
					else
					{
						udonge->SetAnimationToWalk();
					}
//					udonge->SetAccelX(0);
		//			udonge->SetSpeedX( -0.1f );
					if( udonge->IsDirectionRight() )
					{
						udonge->SetDirection( false );
					}
//					udonge->UpdateVertex();
				}
				else
				{
					udonge->SetSpeedX(0);
					udonge->Stop();
					udonge->Start();
				}
			}
		}
		else
		{
			udonge->SetSpeedX(0);
			udonge->Stop();
			udonge->Start();
		}
		is_jumping = false;
	}
	else
	{
		udonge->SetCenterPosition( marisa_pos );
	}

}

// �G�Ƃ̏Փ˔���
void GameMain::CheckCollisionWithEnemy( void )
{
	// �G�Ƃ̓����蔻��
	const UDONGE_ACTIONS &actions = udonge->GetAnimations();
	Math::Rect2D<Float> check_area;
	Math::Vector3D marisa_pos = udonge->GetPos();
	float width = udonge->GetWidth() , height = udonge->GetHeight();
	check_area.SetCenter( marisa_pos.x , marisa_pos.y , width*2 , height*2 );

	Math::Vector2D direction;
	int i;

//	bool is_input_down = (InputData::input_keys & KEY_DOWN)?true:false;
//	bool is_input_left = (InputData::input_keys & KEY_LEFT)?true:false;
//	bool is_input_right = (InputData::input_keys & KEY_RIGHT)?true:false;
	bool is_input_jump = (InputData::input_keys & KEY_JUMP)?true:false;
//	bool is_input_dash = (InputData::input_keys & KEY_DASH)?true:false;

	float m_w , m_h;
	Math::Rect2D<Float> col_rc;
	m_w = col_rc.w , m_h = col_rc.h;
	udonge->GetCollisionRect( col_rc );

//	const Math::Vector3D &udonge_pos = udonge->GetPos();

	
	if( m_course_enemies )
	{
		int enemy_num = m_course_enemies->size();
		for( i=0 ; i<enemy_num ; i++ )
		{
			MultipleAnimation *enemy = m_course_enemies->at(i).object;
			const Math::Vector3D &pos = enemy->GetPos();

			if( pos.x >= check_area.x && pos.x <= check_area.x+check_area.w &&
				pos.y >= check_area.y && pos.y <= check_area.y+check_area.h )
			{
				if( udonge->DetectCollision( *enemy ) )
				{
					switch( CheckHitDirection( udonge , enemy ) )
					{
					case TOP:
process_for_top_hit:
						// �ォ��Ԃ�����
						if( is_jumping && udonge->GetSpeed().y <= 0 )
						{
							// ���_�Q�b�g
							udonge->IncreasePoint( m_course_enemies->at(i).hit_info.point );
							WinToEnemy( enemy , i );

							if( udonge->GetNowAnimation() != actions.sit_down )
							{
								udonge->SetAnimationToJump();
							}
							is_jumping = true;
							
							if( is_input_jump )
							{
								is_first_input_jump = true;
								udonge->SetSpeedY( JUMP_SPEED );
								udonge->SetAccelY( FIRST_JUMPING_GRAVITY_ACCEL );
							}
							else
							{
								is_first_input_jump = false;
								udonge->SetAccelY( GRAVITY_ACCEL );
								udonge->SetSpeedY( JUMP_SPEED );
							}

							i--;
							enemy_num--;
						}
						continue;

					case RIGHT:
						// �E����Ԃ�����
						if( udonge->GetSpeed().x > 0 && marisa_pos.x >= pos.x && udonge->GetSpeed().y < 0 )
						{
							// ���ǂ񂰂��E�ɐi��ł��āA���ǂ񂰂̕����E�ɂ����瓥�񂾂Ƃ݂Ȃ�
							// �������A�������̏ꍇ�̂�
							goto process_for_top_hit;
						}
						if( !ProcessForHittingEnemy() )
						{
							i = enemy_num;
						}
						break;

					case LEFT:
						// ������Ԃ�����
						if( udonge->GetSpeed().x < 0 && marisa_pos.x <= pos.x && udonge->GetSpeed().y < 0 )
						{
							// ���ǂ񂰂����ɐi��ł��āA���ǂ񂰂̕������ɂ����瓥�񂾂Ƃ݂Ȃ�
							// �������A�������̏ꍇ�̂�
							goto process_for_top_hit;
						}
						if( !ProcessForHittingEnemy() )
						{
							i = enemy_num;
						}
						break;

					case BOTTOM:
						if( !ProcessForHittingEnemy() )
						{
							i = enemy_num;
						}

						break;
					}
				}
			}
		}
	}
}

// �G�ɓ��������Ƃ��̏���
bool GameMain::ProcessForHittingEnemy()
{
	if( !udonge->IsNoEnemyMode() )
	{
		is_first_input_jump = false;
		if( !udonge->ReduceHP() )
		{
			GameOverAnimation();
			return false;
		}
		else
		{
			// ���G���Ԃɓ���
			udonge->SetAnimationToReduceHP();
			GameSound::sound_manager->Stop( GameSound::damage );
			GameSound::sound_manager->Play( GameSound::damage );
		}
	}

	return true;
}

// �G�ƃR�[�X�̓����蔻��
void GameMain::CheckCollisionEnemy( DETAIL_ANIMATION_OBJECT *enemy_object )
{
	int j;
	MultipleAnimation *enemy = enemy_object->object;

	Math::Vector2D direction;
	int object_num;	

	bool is_flying = true;
	float arriving_y;
	Math::Rect2D<Float> col_rc;
	float share_y;


	Math::Rect2D<Float> check_area;
	Math::Vector3D enemy_pos;
	Math::Rect2D<Float> char_col,obj_col;

	enemy->GetCollisionRect( char_col );
				
	enemy_pos = enemy->GetPos();
	// �����蔻����{�G���A
	float width = enemy->GetWidth() , height = enemy->GetHeight();
//	Math::Vector2D changed_pos = enemy->GetSpeed();
//	changed_pos.x = 0;

//	enemy->SetCenterPosition( Math::Vector3D( enemy_pos.x+changed_pos.x , enemy_pos.y+changed_pos.y , enemy_pos.z ) );
	enemy_pos = enemy->GetPos();
	check_area.SetCenter( enemy_pos.x , enemy_pos.y , width*2 , height*2 );

	is_flying = true;
	bool old_direction = enemy->IsDirectionRight();
	
	m_course->SetCheckCollisionRect( check_area );
	if( m_course->DetectCollision( *enemy ) )
	{
		vector<DETAIL_ANIMATION_OBJECT> &col_result = m_course->GetCollisionResult();
		object_num = col_result.size();
		float speed_x = enemy->GetSpeed().x;// , speed_y = enemy->GetSpeed().y;

		share_y = 0;

		for( j=0 ; j<object_num ; j++ )
		{
			DETAIL_ANIMATION_OBJECT &result = col_result.at(j);
			MultipleAnimation *object = result.object;
//			const Math::Vector3D &pos = object->GetPos();
			

			object->GetCollisionRect( obj_col );

			if( result.type == GOAL ||
				result.type == OBJECT_KOUMEI_POINT_BLOCK ||
				result.type == OBJECT_KOUMEI_ITEM_BLOCK ||
				result.type == OBJECT_HEART ||
				result.type == OBJECT_POINT )
			{
				continue;
			}
			
			
			switch( CheckHitDirection( enemy , object ) )
			{
			case TOP:
				arriving_y = enemy_pos.y+obj_col.y+obj_col.h-char_col.y;

				switch( enemy_object->type )
				{
				case OBJECT_BOUNDING_ENEMY:
					// ���˂�G
					enemy->SetSpeedY( enemy->GetIniSpeed().y );
					enemy_pos.y = arriving_y;
					enemy_pos.x = enemy->GetPos().x;
					enemy->SetDirection( old_direction );
					j = object_num;
					break;

				case OBJECT_DO_NOT_FALL_ENEMY:
					// �����Ȃ��G
					share_y += GetShareY( enemy , object );
				case OBJECT_ENEMY:
					// ���ʂ̓G
					is_flying = false;
					break;

				case OBJECT_FLYING_ENEMY:
					// ��ԓG
					enemy_pos.y = arriving_y;
					enemy->SetSpeedY( -enemy->GetSpeed().y/2 );
					break;

				}
				break;

			case BOTTOM:
				enemy_pos.y -= (char_col.y+char_col.h-obj_col.y);
				enemy->SetSpeedY( -enemy->GetSpeed().y/2 );

				break;
			case LEFT:
				// ������Ԃ�����
				// �i�s�����͉E�H
				if( ( speed_x > 0 && enemy->IsDirectionRight() ) ||
					( speed_x < 0 && !enemy->IsDirectionRight() ) )
				{
					enemy_pos.x -= char_col.x+char_col.w-obj_col.x;
					enemy->SetDirection( !enemy->IsDirectionRight() );
					enemy->UpdateVertex();
				}
				break;
			case RIGHT:

				if( ( speed_x < 0 && enemy->IsDirectionRight() ) ||
					( speed_x > 0 && !enemy->IsDirectionRight() ))
				{
					enemy_pos.x += obj_col.x+obj_col.w-char_col.x;
					enemy->SetDirection( !enemy->IsDirectionRight() );
					enemy->UpdateVertex();
				}
				break;
			}
		}
	}

//	enemy_pos.x -= changed_pos.x;
//	enemy_pos.y -= changed_pos.y;
	enemy->SetCenterPosition( enemy_pos );
	if( enemy_object->type != OBJECT_FLYING_ENEMY )
	{
		// �󒆂ɂ��邩�ǂ���
		if( is_flying )
		{
			enemy->SetAccelY(GRAVITY_ACCEL);
		}
		else
		{
			if( enemy_object->type == OBJECT_DO_NOT_FALL_ENEMY )
			{
				if( share_y <= char_col.w*0.7f )
				{
					enemy->SetDirection( !enemy->IsDirectionRight() );
				}
			}
			if( enemy->GetSpeed().y < 0.0f )
			{
				enemy->SetCenterPosition( Math::Vector3D( enemy_pos.x , arriving_y , enemy_pos.z ) );
				enemy->SetSpeedY( 0.0f );
				enemy->SetAccelY( 0.0f );
			}
		}
	}
}


// �Q�[���I�[�o�[��(��������Ƃ�)�̃A�j���[�V������F�X�ݒ�
void GameMain::GameOverAnimation( void )
{
	udonge->SetAccel(Math::Vector3D(0,0,0));
	udonge->SetSpeed(Math::Vector3D(0,0,0));

	StopCountingTime();

	is_beaten = true;
	BeatenTask *got = new BeatenTask( this );
	GameSystem::all_task->AddTask( got , 0xfffd );
}

// �G��|����
void GameMain::WinToEnemy( MultipleAnimation *enemy , int index_of_enemies , float first_y_speed )
{
	// enemies����G���폜
	m_course_enemies->erase( m_course_enemies->begin()+index_of_enemies );

	// �������쒆�̓G���X�g�ɒǉ�
	m_beaten_enemies.push_back( enemy );

	GameSound::sound_manager->Stop( GameSound::beat_enemy );
	GameSound::sound_manager->Play( GameSound::beat_enemy );

	// �����A�j���[�V���������s
	enemy->Stop();
	enemy->Start();
	enemy->SetAccel( Math::Vector3D( 0 , FIRST_JUMPING_GRAVITY_ACCEL , 0 ) );
	enemy->SetSpeed( Math::Vector3D( 0 , first_y_speed , 0 ) );
}

// ���ǂ񂰂��Ƃ߂�
void GameMain::StopUdonge()
{
	m_is_stopped_udonge = true;
	GameSystem::all_object->SetAction( udonge , false );
}

// ���ǂ񂰂𓮂���
void GameMain::StartUdonge()
{
	m_is_stopped_udonge = false;
	GameSystem::all_object->SetAction( udonge , true );
}

// �G���~�߂�
void GameMain::StopAllEnemies()
{
	if( !m_course_enemies )
	{
		return;
	}

	int i,size = m_course_enemies->size();
	m_is_stopped_enemies = true;

	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->SetAction( m_course_enemies->at(i).object , false );
		m_course_enemies->at(i).object->PauseAnimation();
	}

	size = m_beaten_enemies.size();
	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->SetAction( m_beaten_enemies.at(i) , false );
		m_beaten_enemies.at(i)->PauseAnimation();
	}

	size = m_limited_objects.size();
	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->SetAction( m_limited_objects.at(i) , false );
		m_limited_objects.at(i)->PauseAnimation();
	}
}

// �G�𓮂���
void GameMain::StartAllEnemies()
{
	if( !m_course_enemies )
	{
		return;
	}

	int i,size = m_course_enemies->size();
	m_is_stopped_enemies = false;

	for( i=0 ; i<size ; i++ )
	{
//		GameSystem::all_object->SetAction( m_course_enemies->at(i).object , true );
		m_course_enemies->at(i).object->ResumeAnimation();
	}

	size = m_beaten_enemies.size();
	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->SetAction( m_beaten_enemies.at(i) , true );
		m_beaten_enemies.at(i)->PauseAnimation();
	}

	size = m_limited_objects.size();
	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->SetAction( m_limited_objects.at(i) , true );
		m_limited_objects.at(i)->PauseAnimation();
	}
}

// ���Ԍv���J�n
void GameMain::StartCountingTime(int limit_time)
{
	m_rest_time = limit_time;
//	m_time_count_last_time = GameSystem::now_frame_time;
	m_is_counting_time = true;
}

void GameMain::StopCountingTime()
{
	m_is_counting_time = false;
//	m_resume_time = GameSystem::now_frame_time;
}

void GameMain::ResumeCountingTime()
{
	if( m_rest_time > 0 )
	{
		m_is_counting_time = true;
//		m_time_count_last_time += GameSystem::now_frame_time-m_resume_time;
	}
}

void GameMain::ReduceTime( int reduce_time )
{
	m_rest_time -= reduce_time;
	if( m_rest_time/GameSystem::GAME_FPS <= 0 )
	{
		m_rest_time = 0;
	}
}

// BGM�֘A
void GameMain::PlayBGM()
{
	GameSound::sound_manager->Play( m_course_info.bgm_file_name , -1 );
}

void GameMain::StopBGM()
{
	GameSound::sound_manager->Stop( m_course_info.bgm_file_name );
}

void GameMain::PauseBGM()
{
	GameSound::sound_manager->Pause( m_course_info.bgm_file_name );
}

// �R�[�X�̐���
bool GameMain::CreateCourse( int world , int course )
{
	// �R�[�X���擾

	COURSE_INFO ci;
	if( !GameSystem::course_manager->GetCourseInfo( world , course , ci ) )
	{
		return false;
	}
	if( ci.load_info_file_num == 0 ||
		strlen(ci.course_file_name) == 0 )
	{
		return false;
	}

	if( m_course )
	{
		ReleaseCourse();
	}

	int i;
	m_course = new Course( ci.load_info_files[0] , GameSystem::multiple_animation_manager , GameSystem::pFileManager );
	for( i=1 ; i<ci.load_info_file_num ; i++ )
	{
		m_course->AppendCourseInfo( ci.load_info_files[i] );
	}

	GameSystem::all_object->Add( m_course );


	if( m_course != NULL && m_course->IsValiable() )
	{
		ReleaseCourse();
	}
		
	m_course_enemies = m_course->Load( ci.course_file_name , udonge );
		
	if( !m_course_enemies )
	{
		return false;
	}

	// �T�E���h��ǂݍ���
	if( !GameSound::sound_manager->GetSound( ci.bgm_file_name ) )
	{
		GameSystem::pFileManager->SetCurrentPath( "Sound" );
		if( !GameSound::sound_manager->LoadSound( GameSystem::pCore , ci.bgm_file_name , "OggVorbis" ) )
		{
			GameSystem::pFileManager->SetCurrentPath( "Texture" );
			ReleaseCourse();
			return false;
		}
		GameSystem::pFileManager->SetCurrentPath( "Texture" );
	}


	// Objects�ɓo�^
	
	int size = m_course_enemies->size();
	for( i=0 ; i<size ; i++ )
	{
		m_course_enemies->at(i).object->SetWillChangeSpeedAccelWithDirectioni(true);
		GameSystem::all_object->Add( m_course_enemies->at(i).object , true , false );
	}

	m_world_num = world;
	m_course_num = course;

	memcpy( &m_course_info , &ci , sizeof(COURSE_INFO) );
	
	return true;
	
}

// �R�[�X�̉��
void GameMain::ReleaseCourse()
{
	m_course->Release();
	GameSystem::all_object->Delete(m_course);
	m_course = NULL;

	int i;
	int size = m_course_enemies->size();

	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->Delete(m_course_enemies->at(i).object);
		SAFE_RELEASE(m_course_enemies->at(i).object);
	}
	m_course_enemies->clear();
	delete m_course_enemies;
	m_course_enemies = NULL;

	size = m_beaten_enemies.size();
	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->Delete( m_beaten_enemies[i] );
		SAFE_RELEASE(m_beaten_enemies[i]);
	}
	m_beaten_enemies.clear();

	size = m_limited_objects.size();
	for( i=0 ; i<size ; i++ )
	{
		GameSystem::all_object->Delete( m_limited_objects[i] );
		SAFE_RELEASE(m_limited_objects[i]);
	}
	m_limited_objects.clear();

}

// ����������𔻒�
HIT_DIRECTION GameMain::CheckHitDirection( Drawable2DObject *from , Drawable2DObject *to )
{
	HIT_DIRECTION dir_res = NONE;

	Math::Rect2D<Float> from_col,to_col;
	const Math::Vector3D &from_pos = from->GetPos();
	const Math::Vector3D &to_pos = to->GetPos();

	from->GetCollisionRect( from_col );
	to->GetCollisionRect( to_col );
	
	float to_w,to_h,from_w,from_h;
	to_w = to->GetWidth();
	to_h = to->GetHeight();
	from_w = from->GetWidth();
	from_h = from->GetHeight();
	float border_v = (from_col.h+to_col.h)/(from_col.w+to_col.w);
	float tan_v;
	Math::Vector2D direction( (to_col.x+to_col.w/2)-(from_col.x+from_col.w/2) , (to_col.y+to_col.h/2)-(from_col.y+from_col.h/2) );

	// �������L��
	float share;


	if( direction.x > 0.0000001f || direction.x < -0.0000001f )
	{
		tan_v = direction.y/direction.x;
		if( tan_v < 0 )
		{
			tan_v = -tan_v;
		}
	}
	else
	{
		tan_v = 9999999999.0f;
	}
	
	
	if( tan_v < border_v )
	{
		
		if( from_pos.y >= to_pos.y )
		{
			share = to_col.y+to_col.h-from_col.y;
		}
		else
		{
			share = from_col.y+from_col.h-to_col.y;
		}

		// ���L�ʂ��������ق��̍�����10%�𒴂��ĂȂ�������Ԃ������Ƃ݂Ȃ��Ȃ�
		float smaller_y;
		if( to_col.h < from_col.h )
		{
			smaller_y = to_col.h;
		}
		else
		{
			smaller_y = from_col.h;
		}
		if( share < smaller_y*0.25f )
		{
			dir_res = NONE;
		}
		else
		{

			// �� or �E����Ԃ�����
			if( direction.x > 0 )
			{
				dir_res = LEFT;
			}
			else if( direction.x < 0 )
			{
				dir_res = RIGHT;
			}
		}
	}
	else
	{
		if( from_pos.x >= to_pos.x )
		{
			share = to_col.x+to_col.w-from_col.x;
		}
		else
		{
			share = from_col.x+from_col.w-to_col.x;
		}

		// ���L�ʂ��������ق��̍�����20%�𒴂��ĂȂ�������Ԃ������Ƃ݂Ȃ��Ȃ�
		float smaller_x;

		if( to_col.w < from_col.w )
		{
			smaller_x = to_col.w;
		}
		else
		{
			smaller_x = from_col.w;
		}

		if( direction.y <= 0 )
		{
			if( share >= smaller_x*0.15f )
			{
				dir_res = TOP;
			}
		}
		else
		{
			if( share >= smaller_x*0.3f && from->GetSpeed().y > 0 )
			{
				dir_res = BOTTOM;
			}
		}
/*
		if( share < smaller_x*0.2f )
		{
			dir_res = NONE;
		}
		else
		{
			if( direction.y < 0 )
			{
				// �ォ��Ԃ�����
				dir_res = TOP;
			}
			else if( direction.y > 0 && from->GetSpeed().y > 0 )
			{
				dir_res = BOTTOM;
			}
		}*/
	}

	return dir_res;
}

float GameMain::GetShareY( DrawableObject *ob1 , DrawableObject *ob2 )
{
	float share = 0;

	Math::Rect2D<Float> from_col,to_col;
	const Math::Vector3D &from_pos = ob1->GetPos();
	const Math::Vector3D &to_pos = ob2->GetPos();

	ob1->GetCollisionRect( from_col );
	ob2->GetCollisionRect( to_col );

	if( from_pos.x >= to_pos.x )
	{
		share = to_col.x+to_col.w-from_col.x;
	}
	else
	{
		share = from_col.x+from_col.w-to_col.x;
	}

	return share;
}

float GameMain::GetShareX( DrawableObject *ob1 , DrawableObject *ob2 )
{
	float share = 0;

	Math::Rect2D<Float> from_col,to_col;
	const Math::Vector3D &from_pos = ob1->GetPos();
	const Math::Vector3D &to_pos = ob2->GetPos();

	ob1->GetCollisionRect( from_col );
	ob2->GetCollisionRect( to_col );

	if( from_pos.y >= to_pos.y )
	{
		share = to_col.y+to_col.h-from_col.y;
	}
	else
	{
		share = from_col.y+from_col.h-to_col.y;
	}

	return share;
}
