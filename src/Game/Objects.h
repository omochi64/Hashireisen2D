
#pragma once

#include "Selene.h"
#include <vector>

using namespace Selene;
using namespace std;

// 描画可能オブジェクト(2D、3Dは問わない。純粋仮想関数を含む抽象クラス)
class DrawableObject{
protected:
	bool				m_is_valiable;
	bool				m_is_needed_collision_detection;	// 当たり判定が必要になるか？ → 不要なあたり判定を減らすため
	Math::Vector3D		m_pos;
	Math::Rect2D<Float>	m_collision_rect;		// (0,0)を中心としたときの当たり判定

	DWORD				m_last_visible_time;	// 最後に可視になった時間( = 0 で一度も可視になっていない)
	DWORD				m_invisible_period;		// このオブジェクトが見えていない期間(単位はフレーム)

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
	// 戻り値は当たっているか否か。
	// hit_directionは、当たっている場合、this→objectへの方向ベクトル(非正規化)
	virtual bool DetectCollision( DrawableObject &object ) = 0;
	virtual void GetCollisionRect( Math::Rect2D<Float> & rect ) = 0;

	DWORD GetLastVisibleTime( void ){return m_last_visible_time;}
	DWORD GetInvisiblePeriod( void ){return m_invisible_period;}
	void SetInvisibleThisFrame( void ){m_invisible_period++;}

	const float GetWidth( void ){return m_width;};
	const float GetHeight( void ){return m_height;};


};

// あたり判定用矩形と、描画オブジェクト矩形は別々にする！！
// 出ないと、UV座標の設定がとてつもなく面倒になる

// 描画可能単純2Dオブジェクト(絵の切り替えアニメーションを行わない)
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


	// デバッグ用
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
	// RotationはDEGREEで指定する
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

// (単純なテクスチャの切り替えによる)アニメーション付き2Dオブジェクト
// アニメーションの指定はどうするか？
//	方法1.Durationを設定→各画像をDurationに対する表示時間の割合で示す
//	方法2.Durationなど設定しない→表示時間を各画像に設定する(結局その合計からDurationが出せる。本質的には1と同じ)

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

	// アニメーションナンバーをリターン
	int Add( Renderer::ITexture *texture , DWORD show_tex_span , Math::Vector2D uv_lefttop , Math::Vector2D uv_rightbottom ,
		Math::Rect2D<Float> &collision_rect , Math::Vector3D speed = not_change_param_3d , Math::Vector3D accel = not_change_param_3d ,
		Math::Vector3D rotate_speed = not_change_param_3d );

	int Delete( int animation_no );
	virtual void Release( void );

	// アニメーションをスタート
	void Start();
	// アニメーションアップデート(アニメーションが終了したらfalseが帰ってくる)
	bool Update();
	// アニメーションを強制終了
	void Stop( void );
	// アニメーションを一時停止
	void PauseAnimation();
	// アニメーションを再開
	void ResumeAnimation();

	// アニメーションを行わないときの表示テクスチャを設定
	void SetNormalTextureByAnimationNo( int animation_no );
	// 繰り返し設定
	void SetLoop( bool is_loop ){m_is_loop = is_loop;};

	virtual DrawableObject* Clone( void );

	// ActionもDrawも上書きする必要はない。Updateで必要な情報をSuperClassが持つメンバに入れておくから


};

// マルチアニメーション付き2Dオブジェクト
// 平行移動速度などによってテクスチャの切り替わる速度に影響が出る

// このやり方はまずかった…
// is-a関係でやらないと、directionやposやspeedがanimation_objectによってまちまちになってしまう…
class MultipleAnimation:public Drawable2DObject{
protected:
	DWORD				m_past_frame;
	bool				m_did_start;
	int					m_normal_animation;
	int					m_now_status;		// 現在の状態(どのアニメーションをするか、というのを表す)
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

	// 追加
	int CreateNewAnimation();
	int Add( int animation_no , Renderer::ITexture *texture , DWORD show_tex_span ,
		Math::Vector2D uv_lefttop ,Math::Vector2D uv_rightbottom , Math::Rect2D<Float> &collision_rect , 
		Math::Vector3D speed = not_change_param_3d , Math::Vector3D accel = not_change_param_3d ,
		Math::Vector3D rotate_speed = not_change_param_3d );
	// 削除
	int Delete( int animation_no );
	int DeletePartOfAnimation( int animation_no , int count );

	// 設定されてるアニメーションを開始
	void Start();
	// アニメーション強制停止
	void Stop( void );
	// アニメーションを一時停止
	void PauseAnimation();
	// アニメーションを再開
	void ResumeAnimation();

	// アニメーションをアップデート
	virtual bool UpdateAnimation();
	// アニメーションの切り替え
	bool SetAnimationNumber( int number );


	// 通常時のアニメーション設定
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
