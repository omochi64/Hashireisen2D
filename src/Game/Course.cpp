
#define	MAIN_COURSE_SOURCE

#include "Game/Course.h"
#include "System/BmpLib/BmpIoLib.h"
#include "System/FileManagerSupporter.h"

#include <string.h>


//------------------------------------------------------------------------
// ObjectRelationManager�N���X����
//	�R�[�X���ǂݍ��݂̂��߂̏����Ǘ�����N���X
//	RGB�F(int�ŊǗ�)�ƁA�I�u�W�F�N�g���A�I�u�W�F�N�g��ނ̊֘A�t�����s��
//	�F�����̃N���X�ɓn���ƁA����Ɋ֘A�t�����Ă���I�u�W�F�N�g��񂪎擾�\
//------------------------------------------------------------------------
ObjectRelationManager::ObjectRelationManager( char *object_info_file , CMultipleAnimationManager *manager , File::IFileManager *pFileManager )
{
	// �֘A�t���̓ǂݍ���
	strcpy( m_relation_info_file , object_info_file );
	m_object_manager = manager;
	m_pFileManager = pFileManager;
}
ObjectRelationManager::ObjectRelationManager(File::IFileManager *pFileManager)
{
	m_relation_info_file[0] = '\0';
	m_object_manager = NULL;
	m_pFileManager = pFileManager;
}

ObjectRelationManager::~ObjectRelationManager()
{
	DeleteAll();
}

bool ObjectRelationManager::Load( char *object_info_file )
{
	if( object_info_file != NULL )
	{
		strcpy( m_relation_info_file , object_info_file );
	}

	// �����œǂݍ��݁B�������s������false��Ԃ�
	m_pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = m_pFileManager->FileOpen( m_relation_info_file );
	m_pFileManager->SetCurrentPath( "Texture" );
	if( fp->IsInvalid() )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	// version���̓i�V�ɂ��悤�c�߂�ǂ�
	// ���̃t�@�C���͑��̏��Ƌ����ł��Ȃ�
	char	line[1024];
	char	*temp;
	int r,g,b;
	DWORD	color;
	KIND_OF_OBJECT_IDENTITY type;
	ATOM	object_identity;
	HIT_INFO	hit_info;
	bool	is_direction_right;
	int		i;

	while( GameSystem::Supporter::fgets_from_fr( line , 1023 , fp ) != NULL )
	{
		if( strlen(line) == 0 )
		{
			continue;
		}
		is_direction_right = true;
		// �F(R,G,B�ŋL��)
		// �G���[�̏ꍇ�A�����I��0�ɂȂ�B����0���o�^�ς݂��ƃG���[�ɂȂ�
		temp = strtok( line , "," );
		r = atoi(temp);
		temp = strtok( NULL , "," );
		g = atoi(temp);
		temp = strtok( NULL , ":" );
		b = atoi(temp);
		color = RGB(r,g,b);
		if( m_objects_relations[color].object_identity != NULL )
		{
			DeleteAll();
			SAFE_RELEASE(fp);
			return false;
		}

		// �I�u�W�F�N�g��
		temp = strtok( NULL , ":" );
		if( !temp )
		{
			DeleteAll();
			SAFE_RELEASE(fp);
			return false;
		}
		object_identity = AddAtom(temp);
		// ���̒i�K�ł́AMultipleAnimationManager��Load���������Ă���K�v�͂Ȃ�

		// �I�u�W�F�N�g�̎��
		temp = strtok( NULL , ":" );
		if( !temp )
		{
			DeleteAll();
			SAFE_RELEASE(fp);
			return false;
		}

		// ��ނŕ�����
		for( i=0 ; kind_of_object_identity_str[i] ; i++ )
		{
			if( strcmp( temp , kind_of_object_identity_str[i] ) == 0 )
			{
				type = (KIND_OF_OBJECT_IDENTITY)(i+1);
				break;
			}
		}
		if( !kind_of_object_identity_str[i] )
		{
			DeleteAll();
			SAFE_RELEASE(fp);
			return false;
		}

		// ���̎����_ or �q�b�g��֘A�I�u�W�F�N�g
		temp = strtok( NULL , ":\r\n" );
		if( !temp )
		{
			DeleteAll();
			SAFE_RELEASE(fp);
			return false;
		}
		if( isdigit(temp[0]) )
		{
			// ���_���
			hit_info.point = atoi(temp);
			// ���̌�ALEFT or RIGHT�Ƒ����΁A�������`�\(���������Ȃ� or LEFT��RIGHT�ȊO�̂��̂�������Ă��� �� RIGHT�ɂȂ�)
			temp = strtok( NULL , ":\r\n" );
			if( temp && strcmp( temp , "LEFT" ) == 0 )
			{
				is_direction_right = false;
			}
			else
			{
				is_direction_right = true;
			}
		}
		else
		{
			// �q�b�g��֘A�I�u�W�F�N�g
			// �I�u�W�F�N�g��
			strcpy( hit_info.item_block_info.object_name , temp );
			// �I�u�W�F�N�g���
			temp = strtok( NULL , ":\r\n" );
			KIND_OF_OBJECT_IDENTITY type_temp;
			for( i=0 ; kind_of_object_identity_str[i] ; i++ )
			{
				if( strcmp( temp , kind_of_object_identity_str[i] ) == 0 )
				{
					type_temp = (KIND_OF_OBJECT_IDENTITY)(i+1);
					break;
				}
			}
			if( !kind_of_object_identity_str[i] )
			{
				DeleteAll();
				SAFE_RELEASE(fp);
				return false;
			}
			hit_info.item_block_info.object_type = type_temp;
			// �I�u�W�F�N�g���_
			temp = strtok( NULL , ":\r\n" );
			hit_info.item_block_info.object_point = atoi(temp);

			// �q�b�g��̕ω��I�u�W�F�N�g
			temp = strtok( NULL , ":\r\n" );
			strcpy( hit_info.item_block_info.after_object , temp );
			temp = strtok( NULL , ":\r\n" );
			for( i=0 ; kind_of_object_identity_str[i] ; i++ )
			{
				if( strcmp( temp , kind_of_object_identity_str[i] ) == 0 )
				{
					type_temp = (KIND_OF_OBJECT_IDENTITY)(i+1);
					break;
				}
			}
			if( !kind_of_object_identity_str[i] )
			{
				DeleteAll();
				SAFE_RELEASE(fp);
				return false;
			}
			hit_info.item_block_info.after_object_type = type_temp;
			temp = strtok( NULL , ":\r\n" );
			hit_info.item_block_info.after_object_point = atoi(temp);
		}

		// �ݒ�
		m_objects_relations[color].object_identity = object_identity;
		m_objects_relations[color].type = type;
		m_objects_relations[color].is_direction_right = is_direction_right;
		memcpy( &m_objects_relations[color].hit_info , &hit_info , sizeof(HIT_INFO) );

		m_registered_colors.push_back(color);

	}

	SAFE_RELEASE(fp);


	return true;
}

// �S�폜
void ObjectRelationManager::DeleteAll()
{
	vector<int>::iterator it = m_registered_colors.begin();
	vector<int>::iterator end = m_registered_colors.end();

	while( it != end )
	{
		m_objects_relations[*it].object_identity = 0;
		m_objects_relations[*it].type = NONE_OF_OBJECT;
		it++;
	}
}

// �J���[����擾
MultipleAnimation * ObjectRelationManager::GetCopyObject( DWORD color )
{
	if( m_objects_relations[color].object_identity != 0 && m_object_manager )
	{
		return m_object_manager->GetCopyMultipleAnimationObject( m_objects_relations[color].object_identity );
	}
	else
	{
		return NULL;
	}
}

MultipleAnimation * ObjectRelationManager::GetOriginalObject( DWORD color )
{
	if( m_objects_relations[color].object_identity != 0 && m_object_manager )
	{
		return m_object_manager->GetOriginalMultipleAnimationObject( m_objects_relations[color].object_identity );
	}
	else
	{
		return NULL;
	}
}

// �^�C�v���擾
KIND_OF_OBJECT_IDENTITY ObjectRelationManager::GetType( DWORD color )
{
	if( m_objects_relations[color].object_identity != 0 )
	{
		return m_objects_relations[color].type;
	}
	else
	{
		return NONE_OF_OBJECT;
	}
}

// ���_���擾
bool ObjectRelationManager::GetHitInfo( DWORD color , HIT_INFO &info )
{
	if( m_objects_relations[color].object_identity != 0 )
	{
		memcpy( &info , &m_objects_relations[color].hit_info , sizeof( HIT_INFO ) );
		return true;
	}
	else
	{
		return false;
	}
}

// �E�������ǂ������擾
bool ObjectRelationManager::GetIsDirectionRight( DWORD color )
{
	if( m_objects_relations[color].object_identity != 0 )
	{
		return m_objects_relations[color].is_direction_right;
	}
	else
	{
		return true;
	}
}

// �S�����擾

// ���������[�[�[�I�I
// ������GetCopyMultipleAnimationObject�������Ȃ��񂶂�[�I�I

bool ObjectRelationManager::GetObjectInfo( DWORD color , DETAIL_ANIMATION_OBJECT &info )
{

	if( m_objects_relations[color].object_identity == 0 || !m_object_manager )
	{
		return false;
	}

	OBJECT_INFO &object_info = m_objects_relations[color];
	memcpy( &info.hit_info , &object_info.hit_info , sizeof(HIT_INFO) );
	info.object = m_object_manager->GetCopyMultipleAnimationObject( object_info.object_identity );
	info.type = object_info.type;
	info.is_direction_right = object_info.is_direction_right;

	return true;
}

//------------------------------------------------------------------------------------------
// Course�N���X����
//	Course�N���X�́A�Q�[�������s����Ƃ��̃R�[�X���Ǘ�����N���X
//	�I�u�W�F�N�g��`�t�@�C���ƃr�b�g�}�b�v�t�@�C������f�[�^��ǂݎ��R�[�X���\�z����
//	�G�ȊO�̃R�[�X��̃I�u�W�F�N�g�̊Ǘ����s���Ă���
//------------------------------------------------------------------------------------------

Course::Course( char *course_info_file , CMultipleAnimationManager *animation_manager , File::IFileManager *pFileManager )
{
	m_relation_manager = NULL;
	m_course = NULL;
	m_draw_flags = NULL;
	m_is_valiable = false;
	m_course_info_file_num = 0;
	m_pFileManager = pFileManager;
	SetCourseInfo( course_info_file , animation_manager );
	memset( m_course_file_name , 0 , sizeof(char)*256 );
	m_collision_rect.Set(0,0,0,0);

	m_check_visible_area.Set(0,0,0,0);

}
Course::Course( File::IFileManager *pFileManager )
{
	m_relation_manager = NULL;
	m_course = NULL;
	m_draw_flags = NULL;
	m_is_valiable = false;
	m_course_info_file_num = 0;
	memset( m_course_file_name , 0 , sizeof(char)*256 );
	m_collision_rect.Set(0,0,0,0);
	m_pFileManager = pFileManager;
	m_check_visible_area.Set(0,0,0,0);
}

Course::~Course()
{
	Release();
}

// �R�[�X���̃��[�h
vector<DETAIL_ANIMATION_OBJECT>* Course::Load( char *course_file_name , MultipleAnimation *udonge )
{
	if( !m_relation_manager )
	{
		return NULL;
	}

	// course_info_file_name�Ƃ����t�@�C�����̃t�@�C������Acourse_name�Ƃ����R�[�X����`����Ă���t�@�C�������擾
/*	char file_name[650] = {0};
	if( !get_bitmap_file_name( course_name , file_name ) )
	{
		return NULL;
	}
*/	
	// �킴�킴�R�[�X���ƃR�[�X�t�@�C�����֘A�t����t�@�C�������K�v�͂Ȃ��Ǝv���c
	// stageXX.bmp�Ƃ����߂����ł��������I�I

	// �R�[�X�t�@�C�����I�[�v��
	m_pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = m_pFileManager->FileOpen( course_file_name );
	m_pFileManager->SetCurrentPath( "Texture" );
//	FILE *fp = fopen( course_file_name , "r" );
	if( fp->IsInvalid() )
	{
		SAFE_RELEASE(fp);
		return NULL;
	}

	
	// �r�b�g�}�b�v���T�[�`�Bm_relation_manager->GetCopyObject��p���A�ǂ�ǂ�I�u�W�F�N�g�����[�h���Ă���
	// �������Am_relation_manager->GetType�Ŏ�ނ����ʂ���B�G�̏ꍇ��enemies�ɒǉ�����
	// �������Aanimation_manager->Load�����ɐ������Ă��Ȃ��Ƃ��͎��s����

	IBMP *bmp = BmpIO_Load( fp );
	SAFE_RELEASE(fp);

	if( !bmp ){
		return NULL;
	}

	m_width = BmpIO_GetWidth(bmp);
	m_height = BmpIO_GetHeight(bmp);

	m_course = new DETAIL_ANIMATION_OBJECT*[m_height];
	m_draw_flags = new bool*[m_height];
	vector<DETAIL_ANIMATION_OBJECT>* enemies = new vector<DETAIL_ANIMATION_OBJECT>();

	// ���ォ�珇�ɐF��ǂ�ł���
	// ����̍��W��(0,height)�Ƃ���
	int i,j;
	DWORD color;
	
//	MultipleAnimation *next_object;
//	KIND_OF_OBJECT_IDENTITY next_type;
	DETAIL_ANIMATION_OBJECT /*next_enemy,*/next_info;
//	HIT_INFO next_hit_info;

	Math::Rect2D<Float> col;

	for( i=0 ; i<m_height ; i++ )
	{
		m_course[i] = new DETAIL_ANIMATION_OBJECT[m_width];
		m_draw_flags[i] = new bool[m_width];

		for( j=0 ; j<m_width ; j++ )
		{
			// (j,i)�̃f�[�^ = (j,i)�̃s�N�Z���l
			color = RGB(BmpIO_GetR( j , i , bmp ),BmpIO_GetG( j , i , bmp ),BmpIO_GetB( j , i , bmp ));
//			next_object = m_relation_manager->GetCopyObject( color );
//			next_type = m_relation_manager->GetType( color );
//			m_relation_manager->GetHitInfo( color , next_hit_info );
//			is_direction_right = m_relation_manager->GetIsDirectionRight( color );

			if( !m_relation_manager->GetObjectInfo( color , next_info ) )
			{
				m_course[i][j].object = NULL;
				m_draw_flags[i][j] = false;
				continue;
			}
			
			
//			memcpy( &m_course[i][j].hit_info , &next_hit_info , sizeof(HIT_INFO) );

			Math::Rect2D<Float> col;

//			m_course[i][j].type = next_info.type;
//			m_course[i][j].is_direction_right = next_info.is_direction_right;

			if( next_info.type == UDONGE_FIRST_POS )
			{
				// ���ǂ񂰂̏����ʒu
				udonge->GetCollisionRect(col);
				udonge->SetCenterPosition( Math::Vector3D(j+col.w/2 , i+col.h/2 , udonge->GetPos().z) );

				SAFE_RELEASE(next_info.object);
				m_course[i][j].object = NULL;
				m_draw_flags[i][j] = false;

			}
				

			if( next_info.object )
			{
				// ���ʂ̃I�u�W�F�N�g
				if( next_info.type == BACKGROUND )
				{
					next_info.object->SetCenterPosition(Math::Vector3D(j+next_info.object->GetWidth()/2,i+next_info.object->GetHeight()/2,next_info.hit_info.point));
					next_info.object->SetDoDetailCheckVisible(true);
				}
				else
				{
					next_info.object->SetCenterPosition(Math::Vector3D(j+next_info.object->GetWidth()/2,i+next_info.object->GetHeight()/2,0));
				}
				next_info.object->SetDirection( next_info.is_direction_right );

				switch( next_info.type )
				{
				case OBJECT_ENEMY:
				case OBJECT_BOUNDING_ENEMY:
				case OBJECT_FLYING_ENEMY:
				case OBJECT_DO_NOT_FALL_ENEMY:
					// �G�̏ꍇ
//					next_enemy.object = next_object;
//					next_enemy.type = next_type;
//					memcpy( &next_enemy.hit_info , &m_course[i][j].hit_info , sizeof(HIT_INFO) );
					enemies->push_back( next_info );
					m_course[i][j].object = NULL;
					break;

				default:
					memcpy( &m_course[i][j] , &next_info , sizeof(DETAIL_ANIMATION_OBJECT) );
//					m_course[i][j].object = next_object;
					break;
				}

				next_info.object->UpdateVertex();
				next_info.object->Start();
				m_draw_flags[i][j] = true;
			}
			else
			{
				m_course[i][j].object = NULL;
				m_draw_flags[i][j] = false;
			}
		}
	}

	m_check_visible_area.Set(0,0,m_width,m_height);

	BmpIO_DeleteBitmap( bmp );

	strcpy( m_course_file_name , course_file_name );

	m_is_valiable = true;

	return enemies;
}

// �I�u�W�F�N�g������(search_area�Ȃ��ɂ���I�u�W�F�N�g)
bool Course::DeleteObject( MultipleAnimation *object , Math::Rect2DF search_area )
{
	int i,j;
	float half_width = object->GetWidth()/2;
	float half_height = object->GetHeight()/2;
	int start_x = search_area.x-half_width;
	int end_x = search_area.x+search_area.w-half_width;
	int start_y = search_area.y-half_height;
	int end_y = search_area.y+search_area.h-half_height;

	if( search_area.x == 0 &&
		search_area.y == 0&&
		search_area.w == 0 &&
		search_area.h == 0 )
	{
		start_x = 0;
		end_x = m_width-1;
		start_y = 0;
		end_y = m_height-1;
	}
	if( start_x < 0 )
	{
		start_x = 0;
	}
	if( end_x >= m_width )
	{
		end_x = m_width-1;
	}
	if( start_y < 0 )
	{
		start_y = 0;
	}
	if( end_y >= m_height )
	{
		end_y = m_height-1;
	}

	for( i=start_y ; i<=end_y ; i++ )
	{
		for( j=start_x ; j<=end_x ; j++ )
		{
			if( m_course[i][j].object == object && m_course[i][j].type != BACKGROUND )
			{
				SAFE_RELEASE(m_course[i][j].object);
				m_draw_flags[i][j] = false;
				return true;
			}
		}
	}

	return false;
}

// �I�u�W�F�N�g������(delete_area���ɂ���I�u�W�F�N�g)
void Course::DeleteObject( Math::Rect2DF delete_area )
{
	int i,j;
	int start_x = delete_area.x;
	int end_x = delete_area.x+delete_area.w;
	int start_y = delete_area.y;
	int end_y = delete_area.y+delete_area.h;

	if( start_x < 0 )
	{
		start_x = 0;
	}
	if( end_x >= m_width )
	{
		end_x = m_width-1;
	}
	if( start_y < 0 )
	{
		start_y = 0;
	}
	if( end_y >= m_height )
	{
		end_y = m_height-1;
	}


	for( i=start_y ; i<=end_y ; i++ )
	{
		for( j=start_x ; j<=end_x ; j++ )
		{
			if( m_course[i][j].object )
			{
				const Math::Vector3D &pos = m_course[i][j].object->GetPos();
				if( pos.x >= delete_area.x && pos.x <= delete_area.x+delete_area.w &&
					pos.y >= delete_area.y && pos.y <= delete_area.y+delete_area.h && m_course[i][j].type != BACKGROUND )
				{
					SAFE_RELEASE(m_course[i][j].object);
//					delete m_course[i][j].object;
//					m_course[i][j].object = NULL;
					m_draw_flags[i][j] = false;
				}
			}
		}
	}

}

// �I�u�W�F�N�g������
bool Course::ChangeObject( MultipleAnimation *old_object , DETAIL_ANIMATION_OBJECT *new_object , Math::Rect2DF search_area )
{
	int i,j;

	float half_width = old_object->GetWidth()/2;
	float half_height = old_object->GetHeight()/2;
	int start_x = search_area.x-half_width;
	int end_x = search_area.x+search_area.w-half_width;
	int start_y = search_area.y-half_height;
	int end_y = search_area.y+search_area.h-half_height;

	if( search_area.x == 0 &&
		search_area.y == 0&&
		search_area.w == 0 &&
		search_area.h == 0 )
	{
		start_x = 0;
		end_x = m_width-1;
		start_y = 0;
		end_y = m_height-1;
	}

	if( start_x < 0 )
	{
		start_x = 0;
	}
	if( end_x >= m_width )
	{
		end_x = m_width-1;
	}
	if( start_y < 0 )
	{
		start_y = 0;
	}
	if( end_y >= m_height )
	{
		end_y = m_height-1;
	}

	for( i=start_y ; i<=end_y ; i++ )
	{
		for( j=start_x ; j<=end_x ; j++ )
		{
			if( m_course[i][j].object == old_object )
			{
				SAFE_RELEASE(m_course[i][j].object);
				memcpy( &m_course[i][j] , new_object , sizeof(DETAIL_ANIMATION_OBJECT) );
				new_object->object->SetCenterPosition( Math::Vector3D( j + new_object->object->GetWidth()/2 ,
					i + new_object->object->GetHeight()/2 , 0 ) );
				return true;
			}
		}
	}

	return false;
}

// �I�u�W�F�N�g��ǉ�
bool Course::AddObject( DETAIL_ANIMATION_OBJECT *new_object , Math::Point2DF add_point )
{
	int point_x = add_point.x-new_object->object->GetWidth()/2;
	int point_y = add_point.y-new_object->object->GetHeight()/2;

	if( m_course[point_y][point_x].object )
	{
		return false;
	}

	memcpy( &m_course[point_y][point_x] , new_object , sizeof(DETAIL_ANIMATION_OBJECT) );
	m_draw_flags[point_y][point_x] = true;
	new_object->object->SetCenterPosition( Math::Vector3D( point_x + new_object->object->GetWidth()/2 ,
		point_y + new_object->object->GetHeight()/2 , 0 ) );

	return true;
}

void Course::Release()
{
	// ���[�h�����S�ẴI�u�W�F�N�g��j��
	int i,j;
	for( i=0 ; i<m_height ; i++ )
	{
		for( j=0 ; j<m_width ; j++ )
		{
			SAFE_RELEASE(m_course[i][j].object);
		}
		delete [] m_course[i];
		delete [] m_draw_flags[i];
	}
	delete [] m_course;
	delete [] m_draw_flags;
	m_course = NULL;
	m_draw_flags = NULL;

	m_width = m_height = 0;
	m_course_info_file_num = 0;

	delete m_relation_manager;
	m_relation_manager = NULL;

	m_is_valiable = false;
}

bool Course::SetCourseInfo( char *course_info_file , CMultipleAnimationManager *animation_manager )
{
	if( m_relation_manager )
	{
		m_relation_manager->DeleteAll();
		delete m_relation_manager;
	}
	strcpy( m_course_info_files[0] , course_info_file );
	m_course_info_file_num = 1;
	m_relation_manager = new ObjectRelationManager( course_info_file , animation_manager , m_pFileManager );
	return m_relation_manager->Load();
}

bool Course::AppendCourseInfo( char *course_info_file )
{
	if( m_relation_manager == NULL )
	{
		return false;
	}

	strcpy( m_course_info_files[m_course_info_file_num++] , course_info_file );
	return m_relation_manager->Load( course_info_file );
}

void Course::Draw( void )
{
	// �S�ẴI�u�W�F�N�g�̕`��
	int i,j;
	
	int last_y = m_check_visible_area.y+m_check_visible_area.h;
	int last_x = m_check_visible_area.x+m_check_visible_area.w;
	for( i=m_check_visible_area.y ; i<last_y ; i++ )
	{
		for( j=m_check_visible_area.x ; j<last_x ; j++ )
		{
			if( m_course[i][j].object && m_draw_flags[i][j] )
			{
				m_course[i][j].object->Draw();
			}
		}
	}
}

void Course::Action( void )
{
	// �S�ẴI�u�W�F�N�g�̈ړ�
	int i,j;

	for( i=0 ; i<m_height ; i++ )
	{
		for( j=0 ; j<m_width ; j++ )
		{
			if( m_course[i][j].object )
			{
				m_course[i][j].object->UpdateAnimation();
				m_course[i][j].object->Action();
			}
		}
	}
}

// �N���[�������
DrawableObject* Course::Clone( void )
{
	Course* copy = new Course( m_course_info_files[0] , m_relation_manager->GetAnimationManager() , m_pFileManager );
	if( copy == NULL )
	{
		return NULL;
	}
	int i;
	for( i=1 ; i<m_course_info_file_num ; i++ )
	{
		copy->AppendCourseInfo( m_course_info_files[i] );
	}

	MultipleAnimation *temp = new MultipleAnimation();

	vector<DETAIL_ANIMATION_OBJECT>*enemies = copy->Load( m_course_file_name , temp );

	SAFE_RELEASE(temp);

	if( enemies == NULL )
	{
		copy->Release();
		delete copy;

		return NULL;
	}

	vector<DETAIL_ANIMATION_OBJECT>::iterator it = enemies->begin(),end = enemies->end();
	while( it != end )
	{
		SAFE_RELEASE((*it).object);
	}

	delete enemies;

	return copy;
}

// ������
bool Course::CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time )
{
	if( !m_is_valiable )
	{
		return false;
	}

	int i,j;
	int last_y = m_check_visible_area.y+m_check_visible_area.h;
	int last_x = m_check_visible_area.x+m_check_visible_area.w;
	for( i=m_check_visible_area.y ; i<last_y ; i++ )
	{
		for( j=m_check_visible_area.x ; j<last_x ; j++ )
		{
			if( m_course[i][j].object )
			{
				m_draw_flags[i][j] = m_course[i][j].object->CheckVisible( pSceneMgr , screen_width , screen_height , now_time );
			}
			else
			{
				m_draw_flags[i][j] = false;
			}
		}
	}
	return true;
}

// object�ƃR�[�X�̏Փ˂����m
bool Course::DetectCollision( DrawableObject &object )
{
	m_collision_objects.clear();

	int i,j;

	int start_x = m_collision_rect.x;
	int end_x = m_collision_rect.x+m_collision_rect.w;
	int start_y = m_collision_rect.y;
	int end_y = m_collision_rect.y+m_collision_rect.h;

	if( m_collision_rect.x == 0 &&
		m_collision_rect.y == 0&&
		m_collision_rect.w == 0 &&
		m_collision_rect.h == 0 )
	{
		start_x = 0;
		end_x = m_width-1;
		start_y = 0;
		end_y = m_height-1;
	}

	if( start_x < 0 )
	{
		start_x = 0;
	}
	if( end_x >= m_width )
	{
		end_x = m_width-1;
	}
	if( start_y < 0 )
	{
		start_y = 0;
	}
	if( end_y >= m_height )
	{
		end_y = m_height-1;
	}

	for( i=start_y ; i<=end_y ; i++ )
	{
		for( j=start_x ; j<=end_x ; j++ )
		{
			if( m_course[i][j].object )
			{
				const Math::Vector3D & pos = m_course[i][j].object->GetPos();
				if( pos.x >= m_collision_rect.x && pos.x <= m_collision_rect.x+m_collision_rect.w &&
					pos.y >= m_collision_rect.y && pos.y <= m_collision_rect.y+m_collision_rect.h && m_course[i][j].type != BACKGROUND )
				{
					if( object.DetectCollision(*m_course[i][j].object) )
					{
						m_collision_objects.push_back( m_course[i][j] );
					}
				}
			}
		}
	}

	if( m_collision_objects.size() > 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// �R���W�������o�͈͂�ݒ�
void Course::SetCheckCollisionRect( Math::Rect2D<Float> col_rect )
{
	m_collision_rect = col_rect;
}

// �Փˌ��o���ʂ��擾
vector<DETAIL_ANIMATION_OBJECT> & Course::GetCollisionResult()
{
	return m_collision_objects;
}

// �����茟�o�G���A��ݒ�
void Course::SetCheckVisibleArea( Math::Rect2DF &area )
{
	SetCheckVisibleArea( Math::Rect2DI( area.x , area.y , area.w , area.h ) );
}

// �����茟�o�G���A��ݒ�
void Course::SetCheckVisibleArea( Math::Rect2DI &area )
{
	if( area.x < 0 )
	{
		m_check_visible_area.x = 0;
	}
	else
	{
		m_check_visible_area.x = area.x;
	}
	if( area.y < 0 )
	{
		m_check_visible_area.y = 0;
	}
	else
	{
		m_check_visible_area.y = area.y;
	}
	if( m_check_visible_area.x+area.w >= m_width )
	{
		m_check_visible_area.w = m_width-m_check_visible_area.x;
	}
	else
	{
		m_check_visible_area.w = area.w;
	}
	if( m_check_visible_area.y+area.h >= m_height )
	{
		m_check_visible_area.h = m_height-m_check_visible_area.y;
	}
	else
	{
		m_check_visible_area.h = area.h;
	}
}
