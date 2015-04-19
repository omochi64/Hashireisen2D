
#pragma once

#include "Selene.h"
using namespace Selene;

#include "System/FileManagerSupporter.h"

#define	MAX_LOAD_OBJECT_FILE_NUM	10
#define	MAX_DELETE_OBJECT_NUM		20
#define MAX_LOAD_INFO_FILE_NUM		5

typedef struct _course_info{
	char	world_num,course_num;
	char	course_file_name[650];
	char	background_top_file_name[256];
	char	background_center_file_name[256];
	char	background_under_file_name[256];
	char	bgm_file_name[256];

	int		time;
//	int		load_object_file_num;
//	int		delete_object_num;
	int		load_info_file_num;
//	char	load_object_files[MAX_DELETE_OBJECT_NUM][650];
//	char	delete_objects[MAX_DELETE_OBJECT_NUM][20];
	char	load_info_files[5][650];
}COURSE_INFO;

class CoursesManager
{
	int		m_course_num_per_world;
	int		m_world_num;
	char	m_objects_file[650];
	char	m_gun_animation_file[650];
	bool	m_is_valiable;

	COURSE_INFO	**m_courses_info;
public:
	CoursesManager();
	~CoursesManager();

	bool Load( char* pCourseInfoFile , File::IFileManager *pFileManager );
	void Release();

	bool GetCourseInfo( int world , int course , COURSE_INFO & info );
	int GetCourseNumPerWorld(){return m_course_num_per_world;}
	int GetWorldNum(){return m_world_num;}
	const char *GetObjectsFileName(){return m_objects_file;}
	const char *GetGunAnimationFileName(){return m_gun_animation_file;}
};
