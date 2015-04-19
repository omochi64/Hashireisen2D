
#pragma once

#include <stdio.h>
#include <vector>
#include <map>

#include "Selene.h"

#include "Game/Objects.h"
#include "System/Manager/MultipleAnimationManager.h"
#include "System/Manager/CoursesManager.h"

using namespace std;
using namespace Selene;

#ifdef MAIN_COURSE_SOURCE
#define EXTERN_COURSE /*extern*/

// #HEART_BLOCKのように、アイテムに限定してブロックを作るのはコインだけ
const char *kind_of_object_identity_str[] = {
	"#BLOCK" , "#BROKEN_BLOCK" ,  "#WALL" , "#ENEMY" , "#BOUNDING_ENEMY" , "#FLYING_ENEMY" , "#DO_NOT_FALL_ENEMY" ,
	"#ITEM_BLOCK" , "#POINT_BLOCK" , "#KOUMEI_ITEM_BLOCK" , "#KOUMEI_POINT_BLOCK" , "#HEART" , "#POINT" , "#ONE_UP" , 
	"#UDONGE" , "#GOAL" , "#BACKGROUND" , NULL
};
#else
#define EXTERN_COURSE extern
extern const char *kind_of_object_identity_str[];
#endif

// オブジェクトの種類とその識別子の定義
typedef enum KIND_OF_OBJECT_IDENTITY{
	NONE_OF_OBJECT , OBJECT_BLOCK , OBJECT_BROKEN_BLOCK , OBJECT_WALL , OBJECT_ENEMY , OBJECT_BOUNDING_ENEMY ,
	OBJECT_FLYING_ENEMY , OBJECT_DO_NOT_FALL_ENEMY , 
	OBJECT_ITEM_BLOCK , OBJECT_POINT_BLOCK , OBJECT_KOUMEI_ITEM_BLOCK , OBJECT_KOUMEI_POINT_BLOCK ,
	OBJECT_HEART , OBJECT_POINT , OBJECT_ONE_UP , UDONGE_FIRST_POS , GOAL , BACKGROUND
}KIND_OF_OBJECT_IDENTITY;

typedef union{
	int		point;		// 得点
	struct _item_block_info{
		TCHAR	object_name[64];
		KIND_OF_OBJECT_IDENTITY	object_type;
		int	object_point;
		TCHAR	after_object[64];
		KIND_OF_OBJECT_IDENTITY	after_object_type;
		int	after_object_point;
	}item_block_info;
}HIT_INFO;

// typeの定義が必要
typedef struct _object_info{
	ATOM	object_identity;
	KIND_OF_OBJECT_IDENTITY		type;
	HIT_INFO hit_info;
	bool	is_direction_right;
}OBJECT_INFO;

typedef struct _detail_object{
	MultipleAnimation	*object;
	KIND_OF_OBJECT_IDENTITY		type;
	HIT_INFO hit_info;
	bool	is_direction_right;
}DETAIL_ANIMATION_OBJECT;

class ObjectRelationManager{
	// このDrawableObjectのマスタをどうやって持ってくるかが問題
	// 敵、壁、アイテムボックス等の区別をなくして管理したほうが良いのだろうか？
	// Course::Loadで、enemyは実際に操作するコピーデータを返し、管理は呼び出し元に任せる。コース情報はCourseで管理
	// Enemy、WallやBlockのMaster管理はMultipleAnimationManagerに任せる。というのが良いんじゃないだろうか。
	// それならこのObjectRelationManagerにも、MultipleAnimationManagerを1つ渡せばよくなる

	// 問題発生…
	//	3DのObjectを扱う場合、MultipleAnimationだけじゃだめだ…
	//	取る引数を2つにするか…？

	File::IFileManager *m_pFileManager;
	vector<int>				m_registered_colors;
	map<DWORD,OBJECT_INFO>	m_objects_relations;
	char	m_relation_info_file[512];
	CMultipleAnimationManager	*m_object_manager;
public:
	
	ObjectRelationManager( char *object_info_file , CMultipleAnimationManager *manager , File::IFileManager *pFileManager );
	ObjectRelationManager(File::IFileManager *pFileManager);
	~ObjectRelationManager();

	bool Load( char *object_info_file = NULL );
	void DeleteAll();

	void SetAnimationManager( CMultipleAnimationManager *manager ){m_object_manager = manager;}

	MultipleAnimation * GetCopyObject( DWORD color );
	MultipleAnimation * GetOriginalObject( DWORD color );
	bool GetIsDirectionRight( DWORD color );
	KIND_OF_OBJECT_IDENTITY GetType( DWORD color );
	bool GetHitInfo( DWORD color , HIT_INFO &info );
	bool GetObjectInfo( DWORD color , DETAIL_ANIMATION_OBJECT &info );

	CMultipleAnimationManager *GetAnimationManager(void){return m_object_manager;}
//	char *GetRelationInfoFileName(void){return m_relation_info_file;}
};

class Course:public DrawableObject{
protected:

	int		m_height,m_width;
	DETAIL_ANIMATION_OBJECT **m_course;
	bool	**m_draw_flags;
	char	m_course_file_name[256];

	char	m_course_info_files[MAX_LOAD_INFO_FILE_NUM][650];
	int		m_course_info_file_num;

	Math::Rect2D<Float> m_collision_rect;
	vector<DETAIL_ANIMATION_OBJECT>	m_collision_objects;

	File::IFileManager *m_pFileManager;
	

	Math::Rect2DI	m_check_visible_area;

	ObjectRelationManager *m_relation_manager;
	
public:

	Course( char *couse_info_file , CMultipleAnimationManager *animation_manager , File::IFileManager *pFileManager );
	Course( File::IFileManager *pFileManager );
	virtual ~Course();

	// このLoadが呼ばれる前に、コンストラクタで渡したanimation_managerのLoadが完了していないとこの関数は失敗する
	vector<DETAIL_ANIMATION_OBJECT>* Load( char *course_file_name , MultipleAnimation *udonge );
	void Release();

	bool SetCourseInfo( char *course_info_file , CMultipleAnimationManager *animation_manager );
	bool AppendCourseInfo( char *course_info_file );

	virtual void Draw( void );
	virtual void Action( void );
	virtual bool CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time );

	virtual DrawableObject* Clone( void );

	bool DeleteObject( MultipleAnimation *object , Math::Rect2DF search_area = Math::Rect2DF(0,0,0,0) );
	void DeleteObject( Math::Rect2DF delete_area );
	bool ChangeObject( MultipleAnimation *old_object , DETAIL_ANIMATION_OBJECT *new_object , Math::Rect2DF search_area = Math::Rect2DF(0,0,0,0) );

	bool AddObject( DETAIL_ANIMATION_OBJECT *new_object , Math::Point2DF add_point );

	int GetCourseWidth(){return m_width;}
	int GetCourseHeight(){return m_height;}

	void SetCheckVisibleArea( Math::Rect2DF &area );
	void SetCheckVisibleArea( Math::Rect2DI &area );

	virtual bool DetectCollision( DrawableObject &object );
	void SetCheckCollisionRect( Math::Rect2D<Float> col_rect );
	vector<DETAIL_ANIMATION_OBJECT> & GetCollisionResult( void );
	virtual void GetCollisionRect( Math::Rect2D<Float> &rect ){};

private:

//	bool get_bitmap_file_name( char *course_name , char file_name[] );
};
