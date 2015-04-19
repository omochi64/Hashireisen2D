
#include "Selene.h"
#pragma comment(lib,"Selene.lib")

using namespace Selene;


#include "Game/Objects.h"

//std::vector<Renderer::ITexture*>	g_animation_textures;
//std::vector<bool>				g_is_enable_texture;
extern Renderer::IRender				*g_pRender;
extern Scene::ISceneManager			*g_pSceneMgr;


/*SimpleAnimation2DObject*/
SimpleAnimation2DObject::SimpleAnimation2DObject(void):Drawable2DObject()
{
	m_past_frame = 0;
	m_animation_num = 0;
	m_is_loop = false;
	m_did_start = false;
	m_animation_duration=0;
	m_normal_no = 0;
//	m_pause_time = 0;
}

SimpleAnimation2DObject::SimpleAnimation2DObject(Math::Vector3D center_pos, float width, float height,
												 Math::Vector3D speed , Math::Vector3D accel )
												 :Drawable2DObject(center_pos,width,height,speed,accel)
{
	m_past_frame = 0;
	m_animation_num = 0;
	m_is_loop = false;
	m_did_start = false;
	m_animation_duration=0;
	m_normal_no = 0;
//	m_pause_time = 0;
}

SimpleAnimation2DObject::~SimpleAnimation2DObject()
{
	Release();
	m_texture = NULL;
}

DrawableObject *SimpleAnimation2DObject::Clone()
{
	SimpleAnimation2DObject *clone_object = new SimpleAnimation2DObject(
		m_pos , m_width , m_height , m_speed , m_accel );

	if( clone_object )
	{
		clone_object->SetIniAccel( m_ini_accel );
		clone_object->SetIniSpeed( m_ini_speed );
		clone_object->SetDirection( m_is_right_direction );
		clone_object->SetRotation( m_rotate );
		clone_object->SetRotationSpeed( this->m_rotation_speed );
		
		int i;
		for( i=0 ; i<m_animation_num ; i++ )
		{
			clone_object->Add( m_show_texture[i] , m_animation_span[i] , m_uv_lefttops[i] , m_uv_rightbottoms[i] ,
				m_col_rects[i] , m_new_speed[i] , m_new_accel[i] , m_new_rotate_speed[i] );
		}
		clone_object->SetLoop( m_is_loop );
		clone_object->SetNormalTextureByAnimationNo( m_normal_no );
		if( m_is_billboard )
		{
			clone_object->SetBillBoard();
		}
		
		clone_object->UpdateVertex();
	}

	return clone_object;
}

// アニメーションをAdd
int SimpleAnimation2DObject::Add( Renderer::ITexture *texture , DWORD show_tex_span ,
								 Math::Vector2D uv_lefttop , Math::Vector2D uv_rightbottom , Math::Rect2D<Float> &collision_rect ,
								Math::Vector3D new_speed , Math::Vector3D new_accel , Math::Vector3D new_rotate_speed )
{
	m_show_texture.push_back( texture );
	m_animation_span.push_back( show_tex_span );
	m_uv_lefttops.push_back( uv_lefttop );
	m_uv_rightbottoms.push_back( uv_rightbottom );
	m_col_rects.push_back( collision_rect );
	m_new_speed.push_back( new_speed );
	m_new_accel.push_back( new_accel );
	m_new_rotate_speed.push_back( new_rotate_speed );

	m_animation_duration += show_tex_span;

	m_animation_num++;

	if( m_normal_no >= 0 && m_normal_no < m_animation_num )
	{
		m_texture = m_show_texture[m_normal_no];
		SetUV(m_uv_lefttops[m_normal_no],m_uv_rightbottoms[m_normal_no]);

		if( m_new_speed[m_normal_no] != not_change_param_3d )
		{
			m_speed = m_new_speed[m_normal_no];
		}
		if( m_new_accel[m_normal_no] != not_change_param_3d )
		{
			m_accel = m_new_accel[m_normal_no];
		}
		if( m_new_rotate_speed[m_normal_no] != not_change_param_3d )
		{
			m_rotation_speed = m_new_rotate_speed[m_normal_no];
		}
	}

	m_is_valiable = true;

	return m_animation_num;
}

// アニメーションをDelete
int SimpleAnimation2DObject::Delete( int animation_no )
{
	if( animation_no < 0 || animation_no >= m_animation_num )
	{
		return -1;
	}

	std::vector<Renderer::ITexture *>::iterator it1;
	std::vector<DWORD>::iterator it2;
	std::vector<Math::Vector2D>::iterator it3;
	std::vector<Math::Vector3D>::iterator it4;
	std::vector<Math::Rect2D<Float>>::iterator it5;
	int i;

	// 色々と消去
	for( it1 = m_show_texture.begin() , i=0 ; i<animation_no  ; i++ , it1++ );
	m_show_texture.erase( it1 );
	for( it2 = m_animation_span.begin() , i=0 ; i<animation_no  ; i++ , it2++ );
	m_animation_duration -= *it2;
	m_animation_span.erase( it2 );
	for( it3 = m_uv_lefttops.begin() , i=0 ; i<animation_no  ; i++ , it3++ );
	m_uv_lefttops.erase( it3 );
	for( it3 = m_uv_rightbottoms.begin() , i=0 ; i<animation_no  ; i++ , it3++ );
	m_uv_rightbottoms.erase( it3 );
	m_col_rects.erase( m_col_rects.begin()+animation_no );
	for( it4 = m_new_speed.begin() , i=0 ; i<animation_no ; i++ , it4++ );
	m_new_speed.erase(it4);
	for( it4 = m_new_accel.begin() , i=0 ; i<animation_no ; i++ , it4++ );
	m_new_accel.erase(it4);
	for( it4 = m_new_rotate_speed.begin() , i=0 ; i<animation_no ; i++ , it4++ );
	m_new_rotate_speed.erase(it4);

	--m_animation_num;
	if( m_normal_no < 0 || m_normal_no >= m_animation_num )
	{
		m_texture = NULL;
	}

	return m_animation_num;
}

// 全部Clear
void SimpleAnimation2DObject::Release( void )
{
	m_animation_num=0;
	m_did_start = false;
	m_past_frame = 0;
	m_animation_duration = 0;
	m_animation_span.clear();
	m_uv_lefttops.clear();
	m_col_rects.clear();
	m_new_speed.clear();
	m_new_accel.clear();
	m_new_rotate_speed.clear();
	m_uv_rightbottoms.clear();
	m_show_texture.clear();

	m_texture = NULL;

	m_is_valiable = false;

	Drawable2DObject::Release();
}

// アニメーションスタート
void SimpleAnimation2DObject::Start()
{
	m_past_frame = 0;
	m_did_start = true;
	m_executing_animation_no=0;
	UpdateVertex();
}


// アニメーションアップデート
bool SimpleAnimation2DObject::Update()
{
	if( !m_did_start )
	{
		return false;
	}

	m_past_frame++;
	if( m_is_loop )
	{
		if( m_past_frame >= m_animation_duration )
		{
			m_past_frame -= m_animation_duration;
		}
	}
	else
	{
		if( m_past_frame >= m_animation_duration )
		{
			Stop();
			return false;
		}
	}

	DWORD accumulate_time=0;
	int i;
	for( i=0 ; i<m_animation_num ; i++ )
	{
		accumulate_time += m_animation_span[i];
		if( m_past_frame < accumulate_time )
		{
			if( i != m_executing_animation_no )
			{
				m_texture = m_show_texture[i];
				SetUV(m_uv_lefttops[i],m_uv_rightbottoms[i]);
				m_executing_animation_no = i;
				m_collision_rect = m_col_rects[i];
				if( m_new_speed[i] != not_change_param_3d )
				{
					m_speed = m_new_speed[i];
				}
				if( m_new_accel[i] != not_change_param_3d )
				{
					m_accel = m_new_accel[i];
				}
				if( m_new_rotate_speed[i] != not_change_param_3d )
				{
					m_rotation_speed = m_new_rotate_speed[i];
				}
				UpdateVertex();
			}
			break;
		}
	}

	return true;
}

// アニメーション停止
void SimpleAnimation2DObject::Stop()
{
	if( !m_did_start )
	{
		return;
	}
	m_did_start = false;
	m_past_frame = 0;

	if( m_normal_no < 0 || m_normal_no >= m_animation_num )
	{
		m_texture = NULL;
	}
	else
	{
		m_texture = m_show_texture[m_normal_no];
		SetUV(m_uv_lefttops[m_normal_no],m_uv_rightbottoms[m_normal_no]);
//		m_uv_lefttop = ;
//		m_uv_rightbottom = ;
		m_collision_rect = m_col_rects[m_normal_no];
		if( m_new_speed[m_normal_no] != not_change_param_3d )
		{
			m_speed = m_new_speed[m_normal_no];
		}
		if( m_new_accel[m_normal_no] != not_change_param_3d )
		{
			m_accel = m_new_accel[m_normal_no];
		}
		if( m_new_rotate_speed[m_normal_no] != not_change_param_3d )
		{
			m_rotation_speed = m_new_rotate_speed[m_normal_no];
		}
		UpdateVertex();
	}
}

// アニメーションを一時停止
void SimpleAnimation2DObject::PauseAnimation()
{
	if( !m_did_start )
	{
		return;
	}
	m_did_start = false;
}

// アニメーションを再開
void SimpleAnimation2DObject::ResumeAnimation()
{
	if( m_did_start )
	{
		return;
	}

//	m_start_time += now_time-m_pause_time;
//	m_last_visible_time += now_time-m_pause_time;

	m_did_start = true;
}

// デフォルト表示テクスチャをアニメーション番号で設定
void SimpleAnimation2DObject::SetNormalTextureByAnimationNo(int animation_no)
{
	m_normal_no = animation_no;
	if( m_normal_no < 0 || m_normal_no >= m_animation_num )
	{
		m_texture = NULL;
	}
	else
	{
		m_texture = m_show_texture[m_normal_no];
		SetUV( m_uv_lefttops[m_normal_no] , m_uv_rightbottoms[m_normal_no] );
		m_collision_rect = m_col_rects[m_normal_no];
		if( m_new_speed[m_normal_no] != not_change_param_3d )
		{
			m_speed = m_new_speed[m_normal_no];
		}
		if( m_new_accel[m_normal_no] != not_change_param_3d )
		{
			m_accel = m_new_accel[m_normal_no];
		}
		if( m_new_rotate_speed[m_normal_no] != not_change_param_3d )
		{
			m_rotation_speed = m_new_rotate_speed[m_normal_no];
		}
	
	}
}


/*MultipleAnimation*/
/*
DWORD				m_start_time;
	bool				m_did_start;
	int					m_normal_animation;
	int					m_now_status;		// 現在の状態(どのアニメーションをするか、というのを表す)
	int					m_excecuting_count;
	int					m_animation_num;
	std::vector<DWORD>	m_animation_durations;
	std::vector<DWORD>	m_animation_counts;
	std::vector<bool>	m_is_bools;
	std::vector<int>	m_default_tex_no;
	std::vector<std::vector<DOWRD>>		m_animation_spans;
	std::vector<std::vector<int>>		m_show_texture_no;
	std::vector<std::vector<Math::Vector2D>>		m_uv_lefttops;
	std::vector<std::vector<Math::Vector2D>>		m_uv_rightbottoms;
	int m_normal_animation;
	bool m_is_doing_animation;

public:
	// 設定されてるアニメーションを開始
	void Start( DWORD start_time );
	bool SetNormalTexture( int animation_no , int normal_tex_by_animation_count );

	int GetAnimationNum(){return m_animation_num;};

	void Draw( void );
	void Action( void );

	void Release();
*/

MultipleAnimation::MultipleAnimation()
:Drawable2DObject()
{
	m_animation_num = 0;
	m_did_start = false;
	m_now_status = 0;
	m_executing_count = 0;
	m_normal_animation = 0;
}

MultipleAnimation::MultipleAnimation( Math::Vector3D center_pos , float width , float height , Math::Vector3D speed ,
									 Math::Vector3D accel )
									 :Drawable2DObject( center_pos , width , height , speed , accel )
{
	m_animation_num = 0;
	m_did_start = false;
	m_now_status = 0;
	m_executing_count = 0;
	m_normal_animation = 0;
}

MultipleAnimation::~MultipleAnimation()
{
	Release();
}

// アニメーションを作成
int MultipleAnimation::CreateNewAnimation()
{
	m_animation_num++;
	m_animation_durations.push_back(0);
	m_animation_counts.push_back(0);
	m_is_loops.push_back(0);
	m_default_tex_no.push_back(0);
	m_animation_spans.push_back( new std::vector<DWORD> );
	m_show_texture.push_back( new std::vector<Renderer::ITexture*> );
	m_uv_lefttops.push_back( new std::vector<Math::Vector2D> );
	m_uv_rightbottoms.push_back( new std::vector<Math::Vector2D> );
	m_collision_rects.push_back( new std::vector<Math::Rect2D<Float>> );
	m_new_speeds.push_back( new std::vector<Math::Vector3D> );
	m_new_accels.push_back( new std::vector<Math::Vector3D> );
	m_new_rotate_speeds.push_back( new std::vector<Math::Vector3D> );

	m_is_valiable = true;

	return m_animation_num-1;
}

void MultipleAnimation::Release()
{
	/*
	bool				m_did_start;
	int					m_normal_animation;
	int					m_now_status;		// 現在の状態(どのアニメーションをするか、というのを表す)
	int					m_excecuting_count;
	int					m_animation_num;
	std::vector<DWORD>	m_animation_durations;
	std::vector<DWORD>	m_animation_counts;
	std::vector<bool>	m_is_bools;
	std::vector<int>	m_default_tex_no;
	std::vector<std::vector<DOWRD>>		m_animation_spans;
	std::vector<std::vector<int>>		m_show_texture_no;
	std::vector<std::vector<Math::Vector2D>>		m_uv_lefttops;
	std::vector<std::vector<Math::Vector2D>>		m_uv_rightbottoms;
	*/
	m_did_start = false;
	m_normal_animation = 0;
	m_now_status = 0;
	m_executing_count = 0;
	
	int i;
	for( i=0 ; i<m_animation_num ; i++ )
	{
		delete m_animation_spans[i];
		delete m_show_texture[i];
		delete m_uv_lefttops[i];
		delete m_uv_rightbottoms[i];
		delete m_collision_rects[i];
		delete m_new_speeds[i];
		delete m_new_accels[i];
		delete m_new_rotate_speeds[i];
	}

	m_animation_num = 0;

	Drawable2DObject::Release();
}

int MultipleAnimation::Add( int animation_no , Renderer::ITexture *texture , DWORD show_tex_span ,
						   Math::Vector2D uv_lefttop , Math::Vector2D uv_rightbottom , Math::Rect2D<Float> &collision_rect ,
						   Math::Vector3D speed , Math::Vector3D accel , Math::Vector3D rotate_speed )
{
	if( animation_no < 0 || animation_no >= m_animation_num )
	{
		return 0;
	}

	int count = m_animation_counts[animation_no]+1;
	m_animation_counts[animation_no] = count;
	m_animation_durations[animation_no] = m_animation_durations[animation_no] + show_tex_span;
	m_animation_spans[animation_no]->push_back(show_tex_span);
	m_show_texture[animation_no]->push_back(texture);
	m_uv_lefttops[animation_no]->push_back(uv_lefttop);
	m_uv_rightbottoms[animation_no]->push_back(uv_rightbottom);
	m_collision_rects[animation_no]->push_back(collision_rect);
	m_new_speeds[animation_no]->push_back(speed);
	m_new_accels[animation_no]->push_back(accel);
	m_new_rotate_speeds[animation_no]->push_back(rotate_speed);

	return count;
}

// 削除
//	int DeletePartOfAnimation( int animation_no , int count );
int MultipleAnimation::Delete( int animation_no )
{
	if( animation_no < 0 || animation_no >= m_animation_num )
	{
		return -1;
	}

	// normal_animationやnow_statusの設定
	if( animation_no == m_normal_animation )
	{
		m_normal_animation = 0;
	}
	else if( animation_no < m_normal_animation )
	{
		m_normal_animation--;
	}

	if( m_now_status == animation_no )
	{
		m_now_status = m_normal_animation;
	}
	else if( m_now_status > animation_no )
	{
		m_now_status--;
	}

	m_animation_durations.erase( m_animation_durations.begin()+animation_no );
	m_animation_counts.erase( m_animation_counts.begin()+animation_no );
	m_is_loops.erase( m_is_loops.begin()+animation_no );
	m_default_tex_no.erase( m_default_tex_no.begin()+animation_no );

	delete m_animation_spans[animation_no];
	delete m_show_texture[animation_no];
	delete m_uv_lefttops[animation_no];
	delete m_uv_rightbottoms[animation_no];
	delete m_collision_rects[animation_no];
	delete m_new_speeds[animation_no];
	delete m_new_accels[animation_no];
	delete m_new_rotate_speeds[animation_no];

	m_animation_spans.erase( m_animation_spans.begin()+animation_no );
	m_show_texture.erase( m_show_texture.begin()+animation_no );
	m_uv_lefttops.erase( m_uv_lefttops.begin()+animation_no );
	m_uv_rightbottoms.erase( m_uv_rightbottoms.begin()+animation_no );
	m_collision_rects.erase( m_collision_rects.begin()+animation_no );
	m_new_speeds.erase( m_new_speeds.begin()+animation_no );
	m_new_accels.erase( m_new_accels.begin()+animation_no );
	m_new_rotate_speeds.erase( m_new_rotate_speeds.begin()+animation_no );

	m_animation_num--;
	if( m_animation_num <= 0 )
	{
		m_did_start = false;
		m_executing_count=0;
	}

	return m_animation_num;
}

int MultipleAnimation::DeletePartOfAnimation( int animation_no , int count )
{
	if( animation_no < 0 || animation_no >= m_animation_num )
	{
		return -1;
	}

	int total_count = m_animation_counts[animation_no];
	
	if( count < 0 || count >= total_count )
	{
		return -1;
	}

	/*bool				m_did_start;
	int					m_normal_animation;
	int					m_now_status;		// 現在の状態(どのアニメーションをするか、というのを表す)
	int					m_executing_count;
	int					m_animation_num;
	std::vector<DWORD>	m_animation_durations;
	std::vector<DWORD>	m_animation_counts;
	std::vector<bool>	m_is_bools;
	std::vector<int>	m_default_tex_no;
	std::vector<std::vector<DOWRD>*>		m_animation_spans;
	std::vector<std::vector<int>*>		m_show_texture_no;
	std::vector<std::vector<Math::Vector2D>*>		m_uv_lefttops;
	std::vector<std::vector<Math::Vector2D>*>		m_uv_rightbottoms;
	int m_normal_animation;
	bool m_is_doing_animation;*/

	m_animation_counts[animation_no] = m_animation_counts[animation_no]-1;
	m_animation_durations[animation_no] = m_animation_durations[animation_no]-m_animation_spans[animation_no]->at(count);
	if( m_default_tex_no[animation_no] > count )
	{
		m_default_tex_no[animation_no] = m_default_tex_no[animation_no]-1;
	}
	else if( m_default_tex_no[animation_no] == count )
	{
		m_default_tex_no[animation_no] = 0;
	}

	m_animation_spans[animation_no]->erase(m_animation_spans[animation_no]->begin()+count);
	m_show_texture[animation_no]->erase(m_show_texture[animation_no]->begin()+count);
	m_uv_lefttops[animation_no]->erase(m_uv_lefttops[animation_no]->begin()+count);
	m_uv_rightbottoms[animation_no]->erase(m_uv_rightbottoms[animation_no]->begin()+count);
	m_collision_rects[animation_no]->erase(m_collision_rects[animation_no]->begin()+count);
	m_new_speeds[animation_no]->erase(m_new_speeds[animation_no]->begin()+count);
	m_new_accels[animation_no]->erase(m_new_accels[animation_no]->begin()+count);
	m_new_rotate_speeds[animation_no]->erase(m_new_rotate_speeds[animation_no]->begin()+count);


	return total_count-1;
}

DrawableObject *MultipleAnimation::Clone()
{
	MultipleAnimation *clone_object = new MultipleAnimation(
		m_pos , m_width , m_height , m_speed , m_accel );

	if( clone_object )
	{
		clone_object->SetDirection( m_is_right_direction );
		clone_object->SetRotation( m_rotate );
		clone_object->SetRotationSpeed( m_rotation_speed );
		clone_object->SetIniAccel( m_ini_accel );
		clone_object->SetIniSpeed( m_ini_speed );
		
		int i;
		unsigned int j;
		int new_animation;
		for( i=0 ; i<m_animation_num ; i++ )
		{
			new_animation = clone_object->CreateNewAnimation();
			for( j=0 ; j<m_animation_counts[i] ; j++ )
			{
				clone_object->Add( new_animation , m_show_texture[i]->at(j) , m_animation_spans[i]->at(j) , m_uv_lefttops[i]->at(j) ,
					m_uv_rightbottoms[i]->at(j) , m_collision_rects[i]->at(j) , m_new_speeds[i]->at(j) , m_new_accels[i]->at(j) , m_new_rotate_speeds[i]->at(j) );
			}
			clone_object->SetLoop( new_animation , m_is_loops[i] );
			clone_object->SetNormalTexture( new_animation , m_default_tex_no[i] );
			clone_object->SetNormalAnimation( m_normal_animation );
		}
		
		if( m_is_billboard )
		{
			clone_object->SetBillBoard();
		}
		clone_object->UpdateVertex();
	}

	return clone_object;
}

// 通常時アニメーションセット
bool MultipleAnimation::SetNormalAnimation(int number)
{
	if( number >= m_animation_num || number < 0 )
	{
		return false;
	}

	m_normal_animation = number;
	if( !m_did_start )
	{
		m_now_status = m_normal_animation;

		m_texture = m_show_texture[number]->at(m_default_tex_no[number]);
		SetUV( m_uv_lefttops[number]->at(m_default_tex_no[number]) , m_uv_rightbottoms[number]->at(m_default_tex_no[number]) );
		m_collision_rect = m_collision_rects[number]->at(m_default_tex_no[number]);

		if( m_new_speeds[number]->at(m_default_tex_no[number]) != not_change_param_3d )
		{
			m_speed = m_new_speeds[number]->at(m_default_tex_no[number]);
		}
		if( m_new_accels[number]->at(m_default_tex_no[number]) != not_change_param_3d )
		{
			m_accel = m_new_accels[number]->at(m_default_tex_no[number]);
		}
		if( m_new_rotate_speeds[number]->at(m_default_tex_no[number]) != not_change_param_3d )
		{
			m_rotation_speed = m_new_rotate_speeds[number]->at(m_default_tex_no[number]);
		}
	}

	return true;
}

// アニメーションセット
bool MultipleAnimation::SetAnimationNumber(int number)
{
	if( number >= m_animation_num || number < 0 )
	{
		return false;
	}

	if( m_did_start )
	{
		Stop();
		m_now_status = number;
		m_texture = m_show_texture[m_now_status]->at(m_default_tex_no[m_now_status]);
		SetUV( m_uv_lefttops[m_now_status]->at(m_default_tex_no[m_now_status]) , m_uv_rightbottoms[m_now_status]->at(m_default_tex_no[m_now_status]));
		m_collision_rect = m_collision_rects[m_now_status]->at(m_default_tex_no[m_now_status]);

		if( m_new_speeds[number]->at(m_default_tex_no[number]) != not_change_param_3d )
		{
			m_speed = m_new_speeds[number]->at(m_default_tex_no[number]);
		}
		if( m_new_accels[number]->at(m_default_tex_no[number]) != not_change_param_3d )
		{
			m_accel = m_new_accels[number]->at(m_default_tex_no[number]);
		}
		if( m_new_rotate_speeds[number]->at(m_default_tex_no[number]) != not_change_param_3d )
		{
			m_rotation_speed = m_new_rotate_speeds[number]->at(m_default_tex_no[number]);
		}
		Start();
	}
	else
	{
		m_now_status = number;
	}


	return true;
}

// デフォルトテクスチャ番号の設定
bool MultipleAnimation::SetNormalTexture(int animation_no, int normal_tex_by_animation_count)
{
	if( animation_no < 0 || animation_no >= m_animation_num )
	{
		return false;
	}

	m_default_tex_no[animation_no] = normal_tex_by_animation_count;

	return true;
}

// アニメーション強制停止
void MultipleAnimation::Stop( void )
{
	if( !m_did_start )
	{
		return;
	}

	m_did_start = false;
	m_now_status = m_normal_animation;
	m_executing_count = m_default_tex_no[m_now_status];
}

// アニメーションをアップデート
bool MultipleAnimation::UpdateAnimation()
{
	if( !m_did_start )
	{
		return false;
	}

	m_past_frame++;
	if( m_is_loops[m_now_status] )
	{
		if( m_past_frame >= m_animation_durations[m_now_status] )
		{
			m_past_frame -= m_animation_durations[m_now_status];
		}
	}
	else
	{
		if( m_past_frame >= m_animation_durations[m_now_status] )
		{
			Stop();
			return false;
		}
	}

	DWORD accumulate_time=0;
	unsigned int i;
	for( i=0 ; i<m_animation_counts[m_now_status] ; i++ )
	{
		accumulate_time += m_animation_spans[m_now_status]->at(i);
		if( m_past_frame < accumulate_time )
		{
			if( (signed)i != m_executing_count )
			{
				m_texture = m_show_texture[m_now_status]->at(i);
				SetUV(m_uv_lefttops[m_now_status]->at(i) , m_uv_rightbottoms[m_now_status]->at(i));
				m_collision_rect = m_collision_rects[m_now_status]->at(i);
				if( m_new_speeds[m_now_status]->at(i) != not_change_param_3d )
				{
					m_speed = m_new_speeds[m_now_status]->at(i);
				}
				if( m_new_accels[m_now_status]->at(i) != not_change_param_3d )
				{
					m_accel = m_new_accels[m_now_status]->at(i);
				}
				if( m_new_rotate_speeds[m_now_status]->at(i) != not_change_param_3d )
				{
					m_rotation_speed = m_new_rotate_speeds[m_now_status]->at(i);
				}
				m_executing_count = i;
				UpdateVertex();
			}
			break;
		}
	}

	return true;
}

// アニメーションスタート
void MultipleAnimation::Start()
{
	if( m_animation_num <= 0 || m_animation_counts[m_now_status] <= 0 )
	{
		return;
	}

	m_past_frame = 0;
	m_did_start = true;
	m_executing_count=0;
	
	m_texture = m_show_texture[m_now_status]->at(0);
	SetUV(m_uv_lefttops[m_now_status]->at(0),m_uv_rightbottoms[m_now_status]->at(0));
//	fttop = ;
//	m_uv_rightbottom = ;
	m_collision_rect = m_collision_rects[m_now_status]->at(0);
	if( m_new_speeds[m_now_status]->at(0) != not_change_param_3d )
	{
		m_speed = m_new_speeds[m_now_status]->at(0);
	}
	if( m_new_accels[m_now_status]->at(0) != not_change_param_3d )
	{
		m_accel = m_new_accels[m_now_status]->at(0);
	}
	if( m_new_rotate_speeds[m_now_status]->at(0) != not_change_param_3d )
	{
		m_rotation_speed = m_new_rotate_speeds[m_now_status]->at(0);
	}


	UpdateVertex();
}
// アニメーションを一時停止
void MultipleAnimation::PauseAnimation()
{
	if( !m_did_start )
	{
		return;
	}
	m_did_start = false;
}

// アニメーションを再開
void MultipleAnimation::ResumeAnimation()
{
	if( m_did_start )
	{
		return;
	}

//	m_start_time += now_time-m_animation_pause_time;
//	m_last_visible_time += now_time-m_animation_pause_time;

	m_did_start = true;
}


// loop設定
bool MultipleAnimation::SetLoop( int animation_no , bool is_loop )
{
	if( animation_no < 0 || animation_no >= m_animation_num )
	{
		return false;
	}

	m_is_loops[animation_no] = is_loop;

	return true;
}

// 可視判定MultipleAnimationバージョン
bool MultipleAnimation::CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time )
{
	if( !Drawable2DObject::CheckVisible( pSceneMgr , screen_width , screen_height , now_time ) )
	{
		if( !m_did_start )
		{
			m_invisible_period--;
		}
		return false;
	}
	return true;
}
