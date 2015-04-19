
#include "Game/UdongeAnimation.h"
#include "UdongeActionConstances.h"
#include "System/FileManagerSupporter.h"

using namespace Udonge;

UdongeAnimation::UdongeAnimation(CTextureManager *tex_manager , int default_hp , int default_rest_life ):MultipleAnimation()
{
	m_tex_manager = tex_manager;
	m_hp = default_hp;
	m_default_hp = default_hp;
	m_rest_life = m_default_rest_life = default_rest_life;
	m_no_enemy_time = 0;
	m_no_enemy_mode_blink_period = 0;
	m_no_enemy_mode_rest_frame = 0;
	m_coin = 0;

	m_will_draw = true;
	m_is_shooting_gun = false;

	m_point = 0;

	m_gun_num = 0;
	m_now_gun = 0;
	m_normal_gun_no = 0;
}

UdongeAnimation::~UdongeAnimation()
{
	Release();
}

void UdongeAnimation::Release()
{
	MultipleAnimation::Release();
	int i;
	for( i=0 ; i<m_gun_num ; i++ )
	{
		delete m_guns[i];
	}
	m_gun_num = 0;
	m_guns.clear();
	ResetAllState();
}

// ���ǂ񂰒�`�t�@�C�����炤�ǂ񂰓ǂݍ���
bool UdongeAnimation::LoadUdongeFromFile( char *file_name , Renderer::IRender *render , File::IFileManager *pFileManager )
{
	char	buf[2048];
	char	*temp1;

	pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = pFileManager->FileOpen( file_name );
	pFileManager->SetCurrentPath( "Texture" );

//	FILE *fp = fopen( file_name , "r" );
	if( fp->IsInvalid() )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	int file_size = fp->GetFileSize();
	//
	// �܂���textures��T��
	//
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		if( strcmp( buf , "textures:" ) == 0 )
		{
			break;
		}
	}
	if( fp->GetFilePosition() == file_size )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	map<int,ATOM>	tex_no_to_fatom;
	int tex_no;

	// textures��ǂݍ���
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		temp1 = strtok( buf , "," );
		if( !temp1 )
		{
			break;
		}
		tex_no = atoi( temp1 );
		temp1 = strtok( NULL , "," );
		if( tex_no <= 0 || !temp1 || !m_tex_manager->AddTexture( temp1 , render ) )
		{
			break;
		}
		
		tex_no_to_fatom[tex_no] = AddAtom(temp1);
	}

	// textures�I���

	// no_enemy_mode�ǂݍ���
	fp->SeekStart(0);
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		if( strcmp( buf , "no_enemy_mode:" ) == 0 )
		{
			break;
		}
	}
	if( fp->GetFilePosition() == file_size )
	{
		return false;
	}
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		if( strlen(buf) == 0 )
		{
			break;
		}
		temp1 = strtok( buf , ":" );
		if( !temp1 )
		{
			break;
		}
		if( strcmp( temp1 , "period" ) == 0 )
		{
			temp1 = strtok( NULL , "\r\n" );
			m_no_enemy_time = atoi(temp1);
		}
		else if( strcmp( temp1 , "blink_period" ) == 0 )
		{
			temp1 = strtok( NULL , "\r\n" );
			m_no_enemy_mode_blink_period = atoi(temp1);
		}
		else
		{
			break;
		}
	}

	//
	// enemies�ǂݍ���
	//
	fp->SeekStart(0);
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		if( strcmp( buf , "definition_of_udonge:" ) == 0 )
		{
			break;
		}
	}
	if( fp->GetFilePosition() == file_size )
	{
		return false;
	}

	// �I�[�܂œǂݍ���
	float width,height,depth;
	Math::Vector3D ini_speed=Math::Vector3D(0,0,0),ini_accel=Math::Vector3D(0,0,0);
	Math::Vector3D ini_rotate_speed=Math::Vector3D(0,0,0);
	int show_span;
	Math::Vector2D uv_lefttop,uv_rightbottom;
	Math::Vector3D speed,accel;
	Math::Vector3D rotate_speed;
	Math::Rect2D<Float> collision(0,0,0,0);
	GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp );
	buf[strlen(buf)-1] = '\0';


	// udonge,width,height,depth
	while( strlen(buf) == 0 )
	{
		if( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) == NULL )
		{
			return true;
		}
		buf[strlen(buf)-1] = '\0';
	}

	temp1 = strtok( buf , "," );
	if( !temp1 )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	if( strcmp( temp1 , "udonge" ) != 0 )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	temp1 = strtok( NULL , "," );
	width = (float)atof(temp1);
	if( width <= 0.0f )
	{
		SAFE_RELEASE(fp);
		return false;
	}
	temp1 = strtok( NULL , "," );
	height = (float)atof( temp1 );
	if( height <= 0.0f )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	// ���̃t�@�C���Ƃ̐���������邽�߁A�ꉞdepth���c���Ă���
	temp1 = strtok( NULL , "," );
	depth = (float)atof( temp1 );
	
	this->SetPolygonSize( width , height );

	GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp );
	buf[strlen(buf)-1] = '\0';
	
	int new_animation;

	while( !(fp->GetFilePosition() == file_size) )
	{
		while( strlen(buf) == 0 )
		{
			if( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) == NULL )
			{
				SAFE_RELEASE(fp);
				return true;
			}
			buf[strlen(buf)-1] = '\0';
		}
		
		ini_speed=Math::Vector3D(0,0,0),ini_accel=Math::Vector3D(0,0,0);


		new_animation = CreateNewAnimation();

		temp1 = strtok( buf , "," );

		if( strcmp( temp1 , "stand" ) == 0 )
		{
			m_udonge_actions.stand = new_animation;
		}
		else if( strcmp( temp1 , "walk" ) == 0 )
		{
			m_udonge_actions.walk = new_animation;
		}
		else if( strcmp( temp1 , "run" ) == 0 )
		{
			m_udonge_actions.run = new_animation;
		}
		else if( strcmp( temp1 , "jump" ) == 0 )
		{
			m_udonge_actions.jump = new_animation;
		}
		else if( strcmp( temp1 , "sit_down" ) == 0 )
		{
			m_udonge_actions.sit_down = new_animation;
		}
		else if( strcmp( temp1 , "game_over" ) == 0 )
		{
			m_udonge_actions.game_over = new_animation;
		}
		else if( strcmp( temp1 , "reduce_hp" ) == 0 )
		{
			m_udonge_actions.reduce_hp = new_animation;
		}
		else if( strcmp( temp1 , "shooting_gun" ) == 0 )
		{
			// shooting_gun�ɂ̓I�v�V��������(�e�̈ʒu�������p�����^)
			// �Ƃ肠�����ŏ��͕��ʂ̃A�j���[�V����
			m_udonge_actions.shooting_gun = new_animation;
		}

		// �������x�����w�肳��Ă���H
		ini_speed.x = ini_speed.y = 0.0f;
		ini_accel.x = ini_accel.y = 0.0f;
		ini_rotate_speed.x = ini_rotate_speed.y = ini_rotate_speed.z = 0.0f;
		temp1 = strtok( NULL , "," );
		if( temp1 )
		{
			ini_speed.x = (float)atof(temp1);
			ini_speed.y = (float)atof(strtok(NULL,","));
			temp1 = strtok( NULL , "," );
			if( temp1 )
			{
				ini_accel.x = (float)atof( temp1 );
				temp1 = strtok( NULL , "," );
				if( strcmp( temp1 , "GRAVITY" ) == 0 )
				{
					ini_accel.y = GRAVITY_ACCEL;
				}
				else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
				{
					ini_accel.y = -GRAVITY_ACCEL;
				}
				else
				{
					ini_accel.y = (float)atof(temp1);
				}
				temp1 = strtok( NULL , "," );
				if( temp1 )
				{
					ini_rotate_speed.z = (float)atof( temp1 );
				}
			}
		}

		SetIniSpeed( ini_speed );
		SetIniAccel( ini_accel );
		ApplyIniSpeed();
		ApplyIniAccel();

		SetRotationSpeed( ini_rotate_speed );

		while(GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ))
		{
			speed=Math::Vector3D(0,0,0),accel=Math::Vector3D(0,0,0);
			buf[strlen(buf)-1] = '\0';
			if( isalpha( buf[0] ) != 0 ||
				strlen(buf) == 0 )
			{
				break;
			}
			temp1 = strtok( buf , "," );
			tex_no = atoi( temp1 );
			if( tex_no <= 0 )
			{
				SAFE_RELEASE(fp);
				return false;
			}

			// �e�N�X�`��
			if( tex_no_to_fatom[tex_no] == 0 )
			{
				SAFE_RELEASE(fp);
				return false;
			}

			// �\���X�p��
			temp1 = strtok( NULL , "," );
			show_span = atoi( temp1 );
			if( show_span <= 0 )
			{
				SAFE_RELEASE(fp);
				return false;
			}

			// UV���W
			// left
			temp1 = strtok( NULL , "," );
			if( !temp1 )
			{
				SAFE_RELEASE(fp);
				return false;
			}
			uv_lefttop.x = (float)atof( temp1 );
			// top
			temp1 = strtok( NULL , "," );
			if( !temp1 )
			{
				SAFE_RELEASE(fp);
				return false;
			}
			uv_lefttop.y = (float)atof( temp1 );
			// right
			temp1 = strtok( NULL , "," );
			if( !temp1 )
			{
				SAFE_RELEASE(fp);
				return false;
			}
			uv_rightbottom.x = (float)atof( temp1 );
			// bottom
			temp1 = strtok( NULL , "," );
			if( !temp1 )
			{
				SAFE_RELEASE(fp);
				return false;
			}
			uv_rightbottom.y = (float)atof( temp1 );

			// �����蔻��
			// �@x,y,width,height,
			// �@�t�@�C���ɂ́A�摜�̍���=(-0.5,-0.5) �E��=(0.5,0.5)�Ƃ����Ƃ��̍��W������B
			// �@����͐����`�łȂ��Ă�����(x,y�Ŗڐ���Ԋu���قȂ��ė���)
			temp1 = strtok( NULL , "," );
			if( !temp1 )
			{
				SAFE_RELEASE(fp);
				return false;
			}
			// NO_CHANGE�Ə����Ă������璼�O�̂��̂Ɠ����f�[�^���g��
			if( strcmp( temp1 , "NO_CHANGE" ) != 0 )
			{
				collision.x = (float)(atof( temp1 )*width);

				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					SAFE_RELEASE(fp);
					return false;
				}
				collision.y = (float)(atof( temp1 )*height);

				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					SAFE_RELEASE(fp);
					return false;
				}
				collision.w = (float)(atof( temp1 )*width);

				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					SAFE_RELEASE(fp);
					return false;
				}
				collision.h = (float)(atof( temp1 )*height);


			}


			// ������͑��x�Ƃ��̐ݒ�B�������͂��Ȃ������炻��ł����܂�
			temp1 = strtok( NULL , "," );
			if( temp1 )
			{
				if( strcmp( temp1 , "GRAVITY" ) == 0 )
				{
					speed.x = GRAVITY_ACCEL;
				}
				else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
				{
					speed.x = -GRAVITY_ACCEL;
				}
				else
				{
					speed.x = (float)atof(temp1);
				}

				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					SAFE_RELEASE(fp);
					return false;
				}
				if( strcmp( temp1 , "GRAVITY" ) == 0 )
				{
					speed.y = GRAVITY_ACCEL;
				}
				else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
				{
					speed.y = -GRAVITY_ACCEL;
				}
				else
				{
					speed.y = (float)atof(temp1);
				}

				temp1 = strtok( NULL , "," );
				if( temp1 )
				{
					if( strcmp( temp1 , "GRAVITY" ) == 0 )
					{
						accel.x = GRAVITY_ACCEL;
					}
					else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
					{
						accel.x = -GRAVITY_ACCEL;
					}
					else
					{
						accel.x = (float)atof(temp1);
					}

					temp1 = strtok( NULL , "," );
					if( !temp1 )
					{
						SAFE_RELEASE(fp);
						return false;
					}
					if( strcmp( temp1 , "GRAVITY" ) == 0 )
					{
						accel.y = GRAVITY_ACCEL;
					}
					else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
					{
						accel.y = -GRAVITY_ACCEL;
					}
					else
					{
						accel.y = (float)atof(temp1);
					}

					temp1 = strtok( NULL , "," );
					if( temp1 )
					{
						rotate_speed.z = (float)atof(temp1);
						rotate_speed.x = 0;
						rotate_speed.y = 0;
					}
					else
					{
						rotate_speed = not_change_param_3d;
					}
				}
				else
				{
					accel = not_change_param_3d;
					rotate_speed = not_change_param_3d;
				}
			}
			else
			{
				accel = speed = not_change_param_3d;
				rotate_speed = not_change_param_3d;
			}
			Add( new_animation , m_tex_manager->GetAt(tex_no_to_fatom[tex_no]) , show_span ,
				uv_lefttop , uv_rightbottom , collision , speed , accel , rotate_speed );
			if( new_animation != m_udonge_actions.game_over && new_animation != m_udonge_actions.reduce_hp &&
				new_animation != m_udonge_actions.shooting_gun )
			{
				SetLoop( new_animation , true );
			}
			else
			{
				SetLoop( new_animation , false );
			}
		}
	}

	SetNormalAnimation(m_udonge_actions.stand);
	UpdateVertex();

	SAFE_RELEASE(fp);
	return true;
}


// �e�̓ǂݍ���
bool UdongeAnimation::LoadGunFromFile( char *file_name , CMultipleAnimationManager *animation_manager , File::IFileManager *pFileManager )
{
//	std::vector<GunController *>	guns;

	pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = pFileManager->FileOpen( file_name );
	pFileManager->SetCurrentPath( "Texture" );

	int file_size = fp->GetFileSize();
	char buf[256];

	// "���O:"�Əo�Ă���܂ŌJ��Ԃ�
	do
	{
		GameSystem::Supporter::fgets_from_fr( buf , 255 , fp );
		buf[strlen(buf)-1] = '\0';
	}while( fp->GetFilePosition() < file_size && strchr( buf , ':' ) == NULL );

	char name[256];
	char gun_animation[256];
	char bullet_name[256];
	int power,pierceful,max_bullet;
	float max_dist;

	char *temp;

	while( !(fp->GetFilePosition() == file_size) )
	{
		// ���O��ۑ�
		temp = strtok( buf , ":" );
		strcpy( name , temp );

		do
		{
			GameSystem::Supporter::fgets_from_fr( buf , 255 , fp );
			buf[strlen(buf)-1] = '\0';
			temp = strtok( buf , "=" );

			if( temp )
			{
				// �p�����^�擾
				if( strcmp( temp , "GunAnimation" ) == 0 )
				{
					temp = strtok( NULL , "\r\n" );
					strcpy( gun_animation , temp );
				}
				else if( strcmp( temp , "Bullet" ) == 0 )
				{
					temp = strtok( NULL , "\r\n" );
					strcpy( bullet_name , temp );
				}
				else if( strcmp( temp , "Power" ) == 0 )
				{
					temp = strtok( NULL , "\r\n" );
					power = atoi( temp );
				}
				else if( strcmp( temp , "Pierceful" ) == 0 )
				{
					temp = strtok( NULL , "\r\n" );
					pierceful = atoi( temp );
				}
				else if( strcmp( temp , "MaxDist" ) == 0 )
				{
					temp = strtok( NULL , "\r\n" );
					max_dist = (float)atof( temp );
				}
				else if( strcmp( temp , "MaxBulletNum" ) == 0 )
				{
					temp = strtok( NULL , "\r\n" );
					max_bullet = atoi( temp );
				}

				// ���̖��O��������I���
				if( strchr( temp , ':' ) != NULL )
				{
					break;
				}
			}

			

		}while( fp->GetFilePosition() < file_size );

		// �e����
		GunController *gun;
		// �������cBullet�̃C���X�^���X�͐����ł��Ȃ����A�A�j���[�V�����������ł�2D��3D�����f�ł��Ȃ�
		// ��������Bullet2D��Bullet3D�����̂��ԈႢ�H
		Bullet *bullet;
		gun = new GunController();
		if( strcmp( name , "normal_gun" ) == 0 )
		{
			// �ʏ�e�B���ł����Ă�
			gun->Initialize( name , -1 , max_bullet );
			m_normal_gun_no = m_guns.size();
			m_now_gun = m_normal_gun_no;
		}
		else
		{
			// ����e�B�e����������
			gun->Initialize( name , 0 , max_bullet );
		}
		gun->SetGunObject( animation_manager->GetCopyMultipleAnimationObject( gun_animation ) );

		bullet = new Bullet( pierceful , max_dist , power );
		bullet->SetBulletObject( animation_manager->GetCopyMultipleAnimationObject( bullet_name ) );
		bullet->GetBulletObject()->SetWillChangeSpeedAccelWithDirectioni( true );
		gun->SetBullet( bullet );

		m_guns.push_back( gun );
	}

	m_gun_num = m_guns.size();

	SAFE_RELEASE(fp);

	return true;
}


// �A�j���[�V�����̃A�b�v�f�[�g
bool UdongeAnimation::UpdateAnimation()
{
	if( m_no_enemy_mode_rest_frame > 0 )
	{
		m_no_enemy_mode_rest_frame--;
		if( m_no_enemy_mode_rest_frame <= 0 )
		{
			m_no_enemy_mode_rest_frame = 0;
			m_will_draw = true;
		}
		else
		{
			if( (m_no_enemy_mode_rest_frame/m_no_enemy_mode_blink_period )%2 == 0 )
			{
				m_will_draw = true;
			}
			else
			{
				m_will_draw = false;
			}
		}
	}
	bool ret_val = MultipleAnimation::UpdateAnimation();
	if( ( !ret_val && m_is_shooting_gun ) || GetNowAnimation() != m_udonge_actions.shooting_gun )
	{
		m_is_shooting_gun = false;
	}
	return ret_val;
}

// �`��
void UdongeAnimation::Draw()
{
	if( m_will_draw )
	{
		MultipleAnimation::Draw();
	}
}

// �A�j���[�V�������e��̂��̂ɐݒ�
void UdongeAnimation::SetAnimationToStand()
{
	SetAnimationNumber( m_udonge_actions.stand );
	m_is_shooting_gun = false;
}

void UdongeAnimation::SetAnimationToRun()
{
	SetAnimationNumber( m_udonge_actions.run );
	m_is_shooting_gun = false;
}

void UdongeAnimation::SetAnimationToWalk()
{
	SetAnimationNumber( m_udonge_actions.walk );
	m_is_shooting_gun = false;
}


void UdongeAnimation::SetAnimationToJump()
{
	SetAnimationNumber( m_udonge_actions.jump );
	m_is_shooting_gun = false;
}

void UdongeAnimation::SetAnimationToSitDown()
{
	SetAnimationNumber( m_udonge_actions.sit_down);
	m_is_shooting_gun = false;
}

void UdongeAnimation::SetAnimationToGameOver()
{
	SetAnimationNumber( m_udonge_actions.game_over );
	m_is_shooting_gun = false;
}

void UdongeAnimation::SetAnimationToShootingGun()
{
	if( m_is_shooting_gun )
	{
		return;
	}
	SetAnimationNumber( m_udonge_actions.shooting_gun );
	m_is_shooting_gun = true;
}

void UdongeAnimation::SetAnimationToReduceHP()
{
//	SetAnimationNumber( m_udonge_actions.reduce_hp );
	m_no_enemy_mode_rest_frame = m_no_enemy_time;
}

const UDONGE_ACTIONS & UdongeAnimation::GetAnimations()
{
	return m_udonge_actions;
}


void UdongeAnimation::IncreaseHP( void )
{
	// HP����
	m_hp++;
	if( m_hp > MAX_HP )
	{
		m_hp = MAX_HP;
	}
}

bool UdongeAnimation::ReduceHP( void )
{
	// false��HP���Ȃ��Ȃ���
	m_hp--;
	if( m_hp <= 0 )
	{
		return false;
	}

	return true;
}

void UdongeAnimation::SetHPToDefault( void )
{
	m_hp = m_default_hp;
}

bool UdongeAnimation::IsNoEnemyMode( void )
{
	if( m_no_enemy_mode_rest_frame == 0 )
	{
		return false;
	}

	return true;
}

bool UdongeAnimation::IsShootingGun( void )
{
	return m_is_shooting_gun;
}

// ���_
void UdongeAnimation::IncreasePoint( int inc_size )
{
	m_point += inc_size;
}

void UdongeAnimation::DecreasePoint( int dec_size )
{
	m_point -= dec_size;
	if( m_point < 0 )
	{
		m_point = 0;
	}
}

void UdongeAnimation::EndNoEnemyMode( void )
{
	m_no_enemy_mode_rest_frame = 0;
}

void UdongeAnimation::ResetAllState()
{
	SetHPToDefault();
	ResetPoint();
	SetRestLifeToDefault();
	SetCoinToDefault();
	EndNoEnemyMode();
}

// �e������
Bullet *UdongeAnimation::ShootTheBullet()
{
	if( m_now_gun >= m_gun_num )
	{
		return NULL;
	}
	GunController *gun = m_guns[m_now_gun];
	if( gun->GetBulletNum() == 0 )
	{
		return NULL;
	}

	return gun->ShootTheBullet();
}

// ��/�O�̏e�ɕύX
void UdongeAnimation::SetNextBullet()
{
	m_now_gun = (m_now_gun+1)%m_gun_num;
}

void UdongeAnimation::SetPreviousBullet()
{
	if( --m_now_gun < 0 )
	{
		m_now_gun = m_gun_num-1;
	}
}