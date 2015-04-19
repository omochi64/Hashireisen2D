
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "System/Manager/CoursesManager.h"

CoursesManager::CoursesManager()
{
	m_is_valiable = false;
	m_course_num_per_world = 0;
	m_world_num = 0;
	m_objects_file[0] = 0;
	m_gun_animation_file[0] = '\0';
	m_courses_info = NULL;
}

CoursesManager::~CoursesManager()
{
	Release();
}

// ロード
bool CoursesManager::Load(char* pCourseInfoFile , File::IFileManager *pFileManager )
{
	pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = pFileManager->FileOpen( pCourseInfoFile );
	pFileManager->SetCurrentPath( "Texture" );
//	FILE *fp = fopen( pCourseInfoFile , "r" );
	if( fp->IsInvalid() )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	// 最初にcourses_per_world or world_numが来る
	char temp[650]={0};
	char *tok;

	while( m_course_num_per_world == 0 ||
			m_world_num == 0 ||
			m_objects_file[0] == '\0' ||
			m_gun_animation_file[0] == '\0')
	{
		while(1)
		{
			if( GameSystem::Supporter::fgets_from_fr( temp , 649 , fp ) == NULL )
			{
				goto error;
			}
			temp[strlen(temp)-1] = '\0';
			if( strlen(temp) > 0 )
			{
				break;
			}
		}

		tok = strtok( temp , ":" );

		if( strcmp( tok , "courses_per_world" ) == 0 )
		{
			tok = strtok( NULL , "\r\n" );
			m_course_num_per_world = atoi(tok);
			if( m_course_num_per_world == 0 )
			{
				goto error;
			}
			
		}
		else if( strcmp( tok , "world_num" ) == 0 )
		{
			tok = strtok( NULL , "\r\n" );
			m_world_num = atoi(tok);
			if( m_world_num == 0 )
			{
				goto error;
			}
		}
		else if( strcmp( tok , "objects_file" ) == 0 )
		{
			tok = strtok( NULL , "\r\n" );
			strcpy( m_objects_file , tok );
		}
		else if( strcmp( tok , "gun_animation_file" ) == 0 )
		{
			tok = strtok( NULL , "\r\n" );
			strcpy( m_gun_animation_file , tok );
		}
		else
		{
			goto error;
		}
	}

	// コース情報確保
	int i;
	m_courses_info = new COURSE_INFO*[m_world_num];
	for( i=0 ; i<m_world_num ; i++ )
	{
		m_courses_info[i] = new COURSE_INFO[m_course_num_per_world];
	}

	// ここから各コース情報読み込み開始
	int now_world=1,now_course=1;

	do
	{
		COURSE_INFO & now_info = m_courses_info[now_world-1][now_course-1];
		memset( &now_info , 0 , sizeof(COURSE_INFO) );

		// 現在のコース番号
		now_info.world_num = now_world;
		now_info.course_num = now_course;

		// beginのとこまで行く
		memset( temp , 0 , sizeof(char)*648 );
		while(1)
		{
			if( GameSystem::Supporter::fgets_from_fr( temp , 649 , fp ) == NULL )
			{
				goto error2;
			}
			temp[strlen(temp)-1] = '\0';
			if( strcmp( temp , "begin" ) == 0 )
			{
				break;
			}
		}
		// ロード開始
		while(1)
		{
			memset( temp , 0 , sizeof(char)*650 );
			while(1)
			{
				if( GameSystem::Supporter::fgets_from_fr( temp , 649 , fp ) == NULL )
				{
					goto error2;
				}
				temp[strlen(temp)-1] = '\0';
				if( strlen(temp) > 0 )
				{
					break;
				}
			}

			if( strstr( temp , "file" ) != NULL )
			{
				// コース情報ファイル
				strtok( temp , ":" );
				tok = strtok( NULL , "\r\n" );
				strcpy( now_info.course_file_name , tok );
			}
/*			else if( strstr( temp , "delete" ) != NULL )
			{
				// 削除オブジェクト情報
				strtok( temp , ":" );
				tok = strtok( NULL , ",\r\n" );
				while( tok )
				{
					strcpy( now_info.delete_objects[now_info.delete_object_num] , tok );
					now_info.delete_object_num++;
					tok = strtok( NULL , ",\r\n" );
				}
			}
			else if( strstr( temp , "load" ) != NULL )
			{
				// ロードオブジェクトファイル情報
				strtok( temp , ":" );
				tok = strtok( NULL , ",\r\n" );
				while( tok )
				{
					strcpy( now_info.load_object_files[now_info.load_object_file_num] , tok );
					now_info.load_object_file_num++;
					tok = strtok( NULL , ",\r\n" );
				}
			}
*/			else if( strstr( temp , "info" ) != NULL )
			{
				// 関連付けファイル情報
				strtok( temp , ":" );
				tok = strtok( NULL , ",\r\n" );
				while( tok )
				{
					strcpy( now_info.load_info_files[now_info.load_info_file_num] , tok );
					now_info.load_info_file_num++;
					tok = strtok( NULL , ",\r\n" );
				}
			}
			else if( strstr( temp , "background" ) != NULL )
			{
				// 背景情報
				strtok( temp , ":" );
				tok = strtok( NULL , "," );
				strcpy( now_info.background_top_file_name , tok );
				tok = strtok( NULL , "," );
				strcpy( now_info.background_center_file_name , tok );
				tok = strtok( NULL , ",\r\n" );
				strcpy( now_info.background_under_file_name , tok );
			}
			else if( strstr( temp , "bgm" ) != NULL )
			{
				strtok( temp , ":" );
				tok = strtok( NULL , "\r\n" );
				strcpy( now_info.bgm_file_name , tok );
			}
			else if( strstr( temp , "time" ) != NULL )
			{
				strtok( temp , ":" );
				tok = strtok( NULL , "\r\n" );
				now_info.time = atoi(tok);
			}
			else if( strcmp( temp , "end" ) == 0 )
			{
				// 終了
				break;
			}
		}

		// 次のコースへ
		if( now_course == m_course_num_per_world )
		{
			now_world++;
			now_course = 1;
		}else
		{
			now_course++;
		}
	}while( now_world <= m_world_num );


	SAFE_RELEASE(fp);
	m_is_valiable = true;
	return true;

error:
	SAFE_RELEASE(fp);
	m_is_valiable = false;
	return false;

error2:
	SAFE_RELEASE(fp);
	m_is_valiable = true;
	Release();
	return false;
}

// 解放
void CoursesManager::Release()
{
	if( m_is_valiable )
	{
		int i;
		for( i=0 ; i<m_world_num ; i++ )
		{
			delete [] m_courses_info[i];
		}
		delete [] m_courses_info;
		m_course_num_per_world = 0;
		m_world_num = 0;
		m_is_valiable = false;
	}
}

// 取得
bool CoursesManager::GetCourseInfo(int world, int course, COURSE_INFO &info)
{
	if( !m_is_valiable )
	{
		return false;
	}

	memcpy( &info , &m_courses_info[world-1][course-1] , sizeof(COURSE_INFO) );

	return true;
}
