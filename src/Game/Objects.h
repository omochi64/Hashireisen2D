
#pragma once

#include "Selene.h"
#include <vector>

using namespace Selene;
using namespace std;

// �`��\�I�u�W�F�N�g(2D�A3D�͖��Ȃ��B�������z�֐����܂ޒ��ۃN���X)
class DrawableObject{
protected:
	bool				m_is_valiable;
	bool				m_is_needed_collision_detection;	// �����蔻�肪�K�v�ɂȂ邩�H �� �s�v�Ȃ����蔻������炷����
	Math::Vector3D		m_pos;
	Math::Rect2D<Float>	m_collision_rect;		// (0,0)�𒆐S�Ƃ����Ƃ��̓����蔻��

	DWORD				m_last_visible_time;	// �Ō�ɉ��ɂȂ�������( = 0 �ň�x�����ɂȂ��Ă��Ȃ�)
	DWORD				m_invisible_period;		// ���̃I�u�W�F�N�g�������Ă��Ȃ�����(�P�ʂ̓t���[��)

	Renderer::SVertex3DBase	m_vertex[4];
	Renderer::SVertex3DTexture	m_tex_vertex[4];
	CColor				m_vertex_color;

	float				m_width,m_height;
	
	

public:
	static Math::Vector2D	not_change_param_2d;
	static Math::Vector3D	not_change_param_3d;

	bool	IsValiable(){return m_is_valiable;}

	DrawableObject();
	virtual ~DrawableObject();

	const Math::Vector3D &GetPos( void ){return m_pos;};
	
	static void Initialize( Renderer::IRender *renderer , Scene::ISceneManager *SceneMgr );

	virtual void Draw( void )=0;
	virtual void Action( void )=0;
	virtual bool CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time )=0;
	virtual void SetVertexColor( CColor color );

	virtual DrawableObject* Clone( void )=0;
	// �߂�l�͓������Ă��邩�ۂ��B
	// hit_direction�́A�������Ă���ꍇ�Athis��object�ւ̕����x�N�g��(�񐳋K��)
	virtual bool DetectCollision( DrawableObject &object ) = 0;
	virtual void GetCollisionRect( Math::Rect2D<Float> & rect ) = 0;

	DWORD GetLastVisibleTime( void ){return m_last_visible_time;}
	DWORD GetInvisiblePeriod( void ){return m_invisible_period;}
	void SetInvisibleThisFrame( void ){m_invisible_period++;}

	const float GetWidth( void ){return m_width;};
	const float GetHeight( void ){return m_height;};


};

// �����蔻��p��`�ƁA�`��I�u�W�F�N�g��`�͕ʁX�ɂ���I�I
// �o�Ȃ��ƁAUV���W�̐ݒ肪�ƂĂ��Ȃ��ʓ|�ɂȂ�

// �`��\�P��2D�I�u�W�F�N�g(�G�̐؂�ւ��A�j���[�V�������s��Ȃ�)
class Drawable2DObject:public DrawableObject{
protected:
	Scene::IPrimitiveActor			*m_actor;
	Renderer::Object::IPrimitive3D	*m_primitive;
	Renderer::ITexture				*m_texture;
	int								m_texture_num;

	bool				m_will_change_speed_and_accel_with_direction;
	bool				m_do_detail_check_visible;
	bool				m_is_billboard;

	Math::Vector3D		m_speed;
	Math::Vector3D		m_accel;
	Math::Vector3D		m_ini_speed;
	Math::Vector3D		m_ini_accel;
	Math::Vector2D		m_uv_lefttop,m_uv_rightbottom;
	Math::Vector3D		m_rotate;
	Math::Vector3D		m_rotation_speed;
	bool				m_is_right_direction;


	// �f�o�b�O�p
	Scene::IPrimitiveActor			*m_debug_line_actor[4];
	Renderer::Object::IPrimitive3D	*m_debug_line_primitive[4];

public:

	Drawable2DObject();
	Drawable2DObject( Math::Vector3D center_pos , float width , float height ,
		Math::Vector3D speed=Math::Vector3D(0,0,0) , Math::Vector3D accel=Math::Vector3D(0,0,0) ,
		Math::Vector2D uv_lefttop=Math::Vector2D(0,0) , Math::Vector2D uv_rightbottom=Math::Vector2D(0,0) );
	virtual ~Drawable2DObject();

	void SetTexture( Renderer::ITexture *texture ){m_texture = texture;}
	void SetCenterPosition( const Math::Vector3D & pos );
	virtual void SetPolygonSize( float width , float height );
	void SetCollisionRectOnCenter( const Math::Rect2D<Float> &rect );
	void SetSpeed( Math::Vector3D & speed );
	void SetIniSpeed( const Math::Vector3D & ini_speed ){m_ini_speed = ini_speed;};
	void SetIniAccel( const Math::Vector3D & ini_accel ){m_ini_accel = ini_accel;};
	void SetSpeedX( float speed_x );
	void SetSpeedY( float speed_y );
	void SetSpeedZ( float speed_z );
	void SetAccel( Math::Vector3D & accel );
	void SetAccelX( float accel_x );
	void SetAccelY( float accel_y );
	void SetAccelZ( float accel_z );
	// Rotation��DEGREE�Ŏw�肷��
	void SetRotation( Math::Vector3D & rotate );
	void SetRotationX( float rotate_x );
	void SetRotationY( float rotate_y );
	void SetRotationZ( float rotate_z );
	void SetRotationSpeed( Math::Vector3D & rotate_speed );
	void SetRotationSpeedX( float rotate_speed_x );
	void SetRotationSpeedY( float rotate_speed_y );
	void SetRotationSpeedZ( float rotate_speed_z );
	virtual void SetUV( Math::Vector2D & lefttop , Math::Vector2D & rightbottom , bool update_vertex_data = true );
	virtual void UpdateVertex( void );
	void SetDirection( bool is_right );
	bool IsDirectionRight( void ){return m_is_right_direction;};

	void SetWillChangeSpeedAccelWithDirectioni( bool will_change ){m_will_change_speed_and_accel_with_direction = will_change;}
	void SetDoDetailCheckVisible( bool do_detailly ){m_do_detail_check_visible = do_detailly;}
	Math::Vector3D GetSpeed( void ){return m_speed;}
	Math::Vector3D GetAccel( void ){return m_accel;}
	Math::Vector3D GetRotation( void ){return m_rotate;}
	Math::Vector3D GetIniSpeed( void ){return m_ini_speed;}
	Math::Vector3D GetIniAccel( void ){return m_ini_accel;}
	void ApplyIniSpeed( void ){m_speed = m_ini_speed;}
	void ApplyIniAccel( void ){m_accel = m_ini_accel;}

	void SetBillBoard( void ){m_is_billboard = true;}
	void ResetBillBoard( void ){m_is_billboard = false;}
	

	virtual void Release();

	virtual void Action( void );
	virtual void Draw( void );
	virtual bool CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time );
	virtual DrawableObject* Clone( void );
	virtual bool DetectCollision( DrawableObject &object );
	virtual void GetCollisionRect( Math::Rect2D<Float> & rect );

protected:

	bool Rect_Rect_Collision( const Math::Rect2D<Float> &rect1 , const Math::Rect2D<Float> &rect2 );
};

#define not_change_param_2d	Math::Vector2D(-9999,-9999)
#define not_change_param_3d Math::Vector3D(-9999,-9999,-9999)

// (�P���ȃe�N�X�`���̐؂�ւ��ɂ��)�A�j���[�V�����t��2D�I�u�W�F�N�g
// �A�j���[�V�����̎w��͂ǂ����邩�H
//	���@1.Duration��ݒ聨�e�摜��Duration�ɑ΂���\�����Ԃ̊����Ŏ���
//	���@2.Duration�Ȃǐݒ肵�Ȃ����\�����Ԃ��e�摜�ɐݒ肷��(���ǂ��̍��v����Duration���o����B�{���I�ɂ�1�Ɠ���)

class SimpleAnimation2DObject:public Drawable2DObject{
protected:
	DWORD	m_animation_duration;
	DWORD	m_past_frame;
//	DWORD	m_pause_time;
	int		m_animation_num;
	bool	m_is_loop;
	bool	m_did_start;
	int		m_normal_no;
	int		m_executing_animation_no;
	std::vector<DWORD>					m_animation_span;
	std::vector<Renderer::ITexture*>		m_show_texture;
	std::vector<Math::Vector2D>			m_uv_lefttops;
	std::vector<Math::Vector2D>			m_uv_rightbottoms;
	std::vector<Math::Vector3D>			m_new_speed;
	std::vector<Math::Vector3D>			m_new_accel;
	std::vector<Math::Vector3D>			m_new_rotate_speed;
	std::vector<Math::Rect2D<Float>>	m_col_rects;

public:
	SimpleAnimation2DObject();
	SimpleAnimation2DObject(Math::Vector3D center_pos , float width , float height ,
		Math::Vector3D speed=Math::Vector3D(0,0,0) , Math::Vector3D accel=Math::Vector3D(0,0,0) );
	virtual ~SimpleAnimation2DObject();

	// �A�j���[�V�����i���o�[�����^�[��
	int Add( Renderer::ITexture *texture , DWORD show_tex_span , Math::Vector2D uv_lefttop , Math::Vector2D uv_rightbottom ,
		Math::Rect2D<Float> &collision_rect , Math::Vector3D speed = not_change_param_3d , Math::Vector3D accel = not_change_param_3d ,
		Math::Vector3D rotate_speed = not_change_param_3d );

	int Delete( int animation_no );
	virtual void Release( void );

	// �A�j���[�V�������X�^�[�g
	void Start();
	// �A�j���[�V�����A�b�v�f�[�g(�A�j���[�V�������I��������false���A���Ă���)
	bool Update();
	// �A�j���[�V�����������I��
	void Stop( void );
	// �A�j���[�V�������ꎞ��~
	void PauseAnimation();
	// �A�j���[�V�������ĊJ
	void ResumeAnimation();

	// �A�j���[�V�������s��Ȃ��Ƃ��̕\���e�N�X�`����ݒ�
	void SetNormalTextureByAnimationNo( int animation_no );
	// �J��Ԃ��ݒ�
	void SetLoop( bool is_loop ){m_is_loop = is_loop;};

	virtual DrawableObject* Clone( void );

	// Action��Draw���㏑������K�v�͂Ȃ��BUpdate�ŕK�v�ȏ���SuperClass���������o�ɓ���Ă�������


};

// �}���`�A�j���[�V�����t��2D�I�u�W�F�N�g
// ���s�ړ����x�Ȃǂɂ���ăe�N�X�`���̐؂�ւ�鑬�x�ɉe�����o��

// ���̂����͂܂��������c
// is-a�֌W�ł��Ȃ��ƁAdirection��pos��speed��animation_object�ɂ���Ă܂��܂��ɂȂ��Ă��܂��c
class MultipleAnimation:public Drawable2DObject{
protected:
	DWORD				m_past_frame;
	bool				m_did_start;
	int					m_normal_animation;
	int					m_now_status;		// ���݂̏��(�ǂ̃A�j���[�V���������邩�A�Ƃ����̂�\��)
	int					m_executing_count;
	int					m_animation_num;
//	DWORD				m_animation_pause_time;
	std::vector<DWORD>	m_animation_durations;
	std::vector<DWORD>	m_animation_counts;
	std::vector<bool>	m_is_loops;
	std::vector<int>	m_default_tex_no;
	std::vector<std::vector<DWORD>*>		m_animation_spans;
	std::vector<std::vector<Renderer::ITexture*>*>	m_show_texture;
	std::vector<std::vector<Math::Vector2D>*>		m_uv_lefttops;
	std::vector<std::vector<Math::Vector2D>*>		m_uv_rightbottoms;
	std::vector<std::vector<Math::Vector3D>*>		m_new_speeds;
	std::vector<std::vector<Math::Vector3D>*>		m_new_accels;
	std::vector<std::vector<Math::Vector3D>*>		m_new_rotate_speeds;
	std::vector<std::vector<Math::Rect2D<Float>>*>	m_collision_rects;
	

public:

	MultipleAnimation();
	MultipleAnimation( Math::Vector3D center_pos , float width , float height ,
		Math::Vector3D speed=Math::Vector3D(0,0,0) , Math::Vector3D accel=Math::Vector3D(0,0,0) );
	virtual ~MultipleAnimation();

	// �ǉ�
	int CreateNewAnimation();
	int Add( int animation_no , Renderer::ITexture *texture , DWORD show_tex_span ,
		Math::Vector2D uv_lefttop ,Math::Vector2D uv_rightbottom , Math::Rect2D<Float> &collision_rect , 
		Math::Vector3D speed = not_change_param_3d , Math::Vector3D accel = not_change_param_3d ,
		Math::Vector3D rotate_speed = not_change_param_3d );
	// �폜
	int Delete( int animation_no );
	int DeletePartOfAnimation( int animation_no , int count );

	// �ݒ肳��Ă�A�j���[�V�������J�n
	void Start();
	// �A�j���[�V����������~
	void Stop( void );
	// �A�j���[�V�������ꎞ��~
	void PauseAnimation();
	// �A�j���[�V�������ĊJ
	void ResumeAnimation();

	// �A�j���[�V�������A�b�v�f�[�g
	virtual bool UpdateAnimation();
	// �A�j���[�V�����̐؂�ւ�
	bool SetAnimationNumber( int number );


	// �ʏ펞�̃A�j���[�V�����ݒ�
	bool SetNormalAnimation( int number );
	bool SetNormalTexture( int animation_no , int normal_tex_by_animation_count );

	int GetAnimationNum(){return m_animation_num;};
	int GetNowAnimation(){return m_now_status;};

	bool SetLoop( int animation_no , bool is_loop );

	virtual void Release();

	virtual DrawableObject* Clone( void );
	virtual bool CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time );

};

class CObjects{
private:
	std::vector<DrawableObject *>		objects;

	std::vector<bool>	m_do_draw;
	std::vector<bool>	m_do_action;

public:
	CObjects();
	~CObjects();

	void Add( DrawableObject *object , bool do_draw = true , bool do_action = true );
	
	bool Delete( DrawableObject *object );

	void ActAll( void );
	void DrawAll( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height );

	void SetDraw( DrawableObject *object , bool do_draw );
	void SetAction( DrawableObject *object , bool do_action );
};


#ifdef MAIN_OBJECT_SOURCE
#define EXTERN_2	/*extern*/
#else
#define EXTERN_2	extern
#endif

EXTERN_2 Renderer::IRender				*g_pRender;
EXTERN_2 Scene::ISceneManager			*g_pSceneMgr;
