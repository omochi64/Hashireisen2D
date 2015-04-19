
#include "Selene.h"
#pragma comment(lib,"Selene.lib")

using namespace Selene;

#define MAIN_OBJECT_SOURCE

#include "Game/Objects.h"
#include "System/GlobalNamespace.h"

//extern std::vector<Renderer::ITexture*>	g_animation_textures;
//extern std::vector<bool>				g_is_enable_texture;

/*
	void SetCenterPosition( Math::Vector3D pos );
	void SetPolygonSize( float width , float height );
	void SetSpeed( Math::Vector2D speed );
	void SetAccel( Math::Vector2D accel );

	void DeleteTexture();

	virtual void Action();
	virtual void Draw();
};

*/

/* ------------------------
	DrawableObject実装
   ------------------------ */
void DrawableObject::Initialize( Renderer::IRender *renderer , Scene::ISceneManager *SceneMgr )
{
	g_pRender = renderer;
	g_pSceneMgr = SceneMgr;
}

DrawableObject::DrawableObject(  )
{
	m_is_valiable = false;
	m_is_needed_collision_detection = true;
	m_collision_rect.Set(0,0,0,0);
	m_last_visible_time = 0;
	m_invisible_period = 0;
	m_vertex_color = CColor( 0xff , 0xff , 0xff );
}
DrawableObject::~DrawableObject()
{
}

void DrawableObject::SetVertexColor( CColor color )
{
	m_vertex_color = color;
	for( int i = 0 ; i<4 ; i++ )
	{
		m_vertex[i].Col = color;
	}
}

Drawable2DObject::Drawable2DObject():DrawableObject()
{
	m_pos = Math::Vector3D( 0 , 0 , 0 );
	m_speed = m_accel = Math::Vector3D( 0,0,0 );
	m_ini_speed = Math::Vector3D( 0 , 0 , 0 );
	m_ini_accel = Math::Vector3D( 0 , 0 , 0 );
	m_speed = Math::Vector3D( 0,0,0 );
	m_accel = Math::Vector3D( 0,0,0 );
	m_actor = NULL;
	m_primitive = NULL;
	m_texture = NULL;
	m_uv_lefttop = Math::Vector2D(0,0);
	m_uv_rightbottom = Math::Vector2D(1.0f,1.0f);
	m_width = 0;
	m_height = 0;
	m_is_right_direction = true;
	m_rotate = Math::Vector3D(0,0,0);
	m_rotation_speed = Math::Vector3D(0,0,0);
	m_is_billboard = false;
	m_will_change_speed_and_accel_with_direction = false;
	m_do_detail_check_visible = false;

	memset( m_debug_line_primitive , 0 , sizeof(Renderer::Object::IPrimitive3D*)*4 );
	memset( m_debug_line_actor , 0 , sizeof(Scene::IPrimitiveActor*)*4 );

	int i;
	for( i=0 ; i<4 ; i++ )
	{
		m_vertex[i].Col = m_vertex_color;
	}
}

// 2Dオブジェクトの生成
Drawable2DObject::Drawable2DObject( Math::Vector3D center_pos, float width, float height,
							   Math::Vector3D speed , Math::Vector3D accel , Selene::Math::Vector2D uv_lefttop, Selene::Math::Vector2D uv_rightbottom)
{
	m_pos.x = center_pos.x;
	m_pos.y = center_pos.y;
	m_pos.z = center_pos.z;
	m_width = width;
	m_height = height;

	m_uv_lefttop.x = uv_lefttop.x;
	m_uv_lefttop.y = uv_lefttop.x;
	m_uv_rightbottom.x = uv_rightbottom.x;
	m_uv_rightbottom.y = uv_rightbottom.y;

	m_speed.x = speed.x;
	m_speed.y = speed.y;
	m_speed.z = speed.z;
	m_accel.x = accel.x;
	m_accel.y = accel.y;
	m_accel.z = accel.z;

	m_rotate = Math::Vector3D(0,0,0);
	m_rotation_speed = Math::Vector3D(0,0,0);

	m_texture = NULL;
	m_is_right_direction = true;

	m_primitive = NULL;
	m_actor = NULL;
	m_do_detail_check_visible = false;

	int i;
	for( i=0 ; i<4 ; i++ )
	{
		m_vertex[i].Col = m_vertex_color;
	}
	m_vertex[0].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , 0 );
	m_vertex[1].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , 0 );
	m_vertex[2].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , 0 );
	m_vertex[3].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , 0 );

	m_tex_vertex[0].TexLight.x = m_tex_vertex[0].TexColor.x = m_uv_lefttop.x;
	m_tex_vertex[0].TexLight.y = m_tex_vertex[0].TexColor.y = m_uv_lefttop.y;
	m_tex_vertex[1].TexLight.x = m_tex_vertex[1].TexColor.x = m_uv_rightbottom.x;
	m_tex_vertex[1].TexLight.y = m_tex_vertex[1].TexColor.y = m_uv_lefttop.y;
	m_tex_vertex[2].TexLight.x = m_tex_vertex[2].TexColor.x = m_uv_rightbottom.x;
	m_tex_vertex[2].TexLight.y = m_tex_vertex[2].TexColor.y = m_uv_rightbottom.y;
	m_tex_vertex[3].TexLight.x = m_tex_vertex[3].TexColor.x = m_uv_lefttop.x;
	m_tex_vertex[3].TexLight.y = m_tex_vertex[3].TexColor.y = m_uv_rightbottom.y;

	for( i=0 ; i<4 ; i++ )
	{
		m_debug_line_primitive[i] = NULL;
		m_debug_line_actor[i] = NULL;
	}

//	UpdateVertex();

	m_is_valiable = true;

	m_is_billboard = false;
	m_will_change_speed_and_accel_with_direction = false;
}

// デストラクタ
Drawable2DObject::~Drawable2DObject()
{
	Release();
}

DrawableObject *Drawable2DObject::Clone()
{
	Drawable2DObject *clone_object = new Drawable2DObject(
		m_pos , m_width , m_height , m_speed , m_accel , m_uv_lefttop , m_uv_rightbottom );

	if( clone_object )
	{
		clone_object->SetIniAccel( m_ini_accel );
		clone_object->SetIniSpeed( m_ini_speed );
		clone_object->SetDirection( m_is_right_direction );
		clone_object->SetRotation( m_rotate );
		Math::Rect2D<Float> r;
		clone_object->GetCollisionRect(r);
		clone_object->SetCollisionRectOnCenter( r );
		clone_object->SetRotationSpeed( m_rotation_speed );
		clone_object->SetTexture( m_texture );
		clone_object->SetVertexColor( m_vertex_color );
		if( m_is_billboard )
		{
			clone_object->SetBillBoard();
		}
		clone_object->UpdateVertex();
	}

	return clone_object;
}

// 頂点の更新
void Drawable2DObject::UpdateVertex()
{
	SAFE_RELEASE( m_primitive );

	m_primitive = g_pRender->CreatePrimitive3D( 4 , 6 , VERTEX_ELEMENT_3DBASE | VERTEX_ELEMENT_3DTEXTURE , false , false );

	m_primitive->Begin();

	m_primitive->Push( m_vertex , 4 );
	
	m_primitive->Push( m_tex_vertex , 4 );

	Uint16 Idx[] = {
		0 , 1 , 2 , 0 , 2 , 3
	};
	m_primitive->Push( Idx , 6 );

	m_primitive->End();

	SAFE_RELEASE(m_actor);
	m_actor = g_pSceneMgr->CreateActor( m_primitive );

	/*for debug*
	int i;
	Renderer::SVertex3DBase vertex[4];
	for( i=0 ; i<4 ; i++ )
	{
		SAFE_RELEASE(m_debug_line_primitive[i]);
		SAFE_RELEASE(m_debug_line_actor[i]);
		m_debug_line_primitive[i] = g_pRender->CreatePrimitive3D( 4 , 6 , VERTEX_ELEMENT_3DBASE , false , false );
		m_debug_line_primitive[i]->Begin();

		vertex[i].Col = CColor( 0xff , 0 , 0 );
	}

	vertex[0].Pos = Math::Vector3D( m_collision_rect.x , (m_collision_rect.y+m_collision_rect.h) , 0 );
	vertex[1].Pos = Math::Vector3D( m_collision_rect.x+0.05f , (m_collision_rect.y+m_collision_rect.h) , 0 );
	vertex[2].Pos = Math::Vector3D( m_collision_rect.x+0.05f , m_collision_rect.y , 0 );
	vertex[3].Pos = Math::Vector3D( m_collision_rect.x , m_collision_rect.y , 0 );
	
	m_debug_line_primitive[0]->Push( vertex , 4 );
	m_debug_line_primitive[0]->Push( Idx , 6 );

	vertex[0].Pos = Math::Vector3D( m_collision_rect.x , (m_collision_rect.y+m_collision_rect.h) , 0 );
	vertex[1].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , (m_collision_rect.y+m_collision_rect.h) , 0 );
	vertex[2].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , (m_collision_rect.y+m_collision_rect.h)-0.05f , 0 );
	vertex[3].Pos = Math::Vector3D( m_collision_rect.x , (m_collision_rect.y+m_collision_rect.h)-0.05f , 0 );
	
	m_debug_line_primitive[1]->Push( vertex , 4 );
	m_debug_line_primitive[1]->Push( Idx , 6 );

	vertex[0].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w)-0.05f , (m_collision_rect.y+m_collision_rect.h) , 0 );
	vertex[1].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , (m_collision_rect.y+m_collision_rect.h) , 0 );
	vertex[2].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , m_collision_rect.y ,0 );
	vertex[3].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w)-0.05f , m_collision_rect.y , 0 );
	
	m_debug_line_primitive[2]->Push( vertex , 4 );
	m_debug_line_primitive[2]->Push( Idx , 6 );

	vertex[0].Pos = Math::Vector3D( m_collision_rect.x , m_collision_rect.y+0.05f , 0 );
	vertex[1].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , m_collision_rect.y+0.05f , 0 );
	vertex[2].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , m_collision_rect.y , 0 );
	vertex[3].Pos = Math::Vector3D( m_collision_rect.x , m_collision_rect.y , 0 );
	
	m_debug_line_primitive[3]->Push( vertex , 4 );
	m_debug_line_primitive[3]->Push( Idx , 6 );

	for( i=0 ; i<4 ; i++ )
	{
		m_debug_line_primitive[i]->End();
		m_debug_line_actor[i] = g_pSceneMgr->CreateActor( m_debug_line_primitive[i] );
	}


	/*for debug*/

		
}

void Drawable2DObject::SetCenterPosition( const Math::Vector3D & pos )
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;
}

void Drawable2DObject::SetDirection( bool is_right )
{
	m_is_right_direction = is_right;
	if( m_is_right_direction )
	{
		m_tex_vertex[0].TexLight.x = m_tex_vertex[0].TexColor.x = m_uv_lefttop.x;
		m_tex_vertex[0].TexLight.y = m_tex_vertex[0].TexColor.y = m_uv_lefttop.y;
		m_tex_vertex[1].TexLight.x = m_tex_vertex[1].TexColor.x = m_uv_rightbottom.x;
		m_tex_vertex[1].TexLight.y = m_tex_vertex[1].TexColor.y = m_uv_lefttop.y;
		m_tex_vertex[2].TexLight.x = m_tex_vertex[2].TexColor.x = m_uv_rightbottom.x;
		m_tex_vertex[2].TexLight.y = m_tex_vertex[2].TexColor.y = m_uv_rightbottom.y;
		m_tex_vertex[3].TexLight.x = m_tex_vertex[3].TexColor.x = m_uv_lefttop.x;
		m_tex_vertex[3].TexLight.y = m_tex_vertex[3].TexColor.y = m_uv_rightbottom.y;
	}
	else
	{
		m_tex_vertex[1].TexLight.x = m_tex_vertex[1].TexColor.x = m_uv_lefttop.x;
		m_tex_vertex[1].TexLight.y = m_tex_vertex[1].TexColor.y = m_uv_lefttop.y;
		m_tex_vertex[0].TexLight.x = m_tex_vertex[0].TexColor.x = m_uv_rightbottom.x;
		m_tex_vertex[0].TexLight.y = m_tex_vertex[0].TexColor.y = m_uv_lefttop.y;
		m_tex_vertex[2].TexLight.x = m_tex_vertex[3].TexColor.x = m_uv_rightbottom.x;
		m_tex_vertex[2].TexLight.y = m_tex_vertex[3].TexColor.y = m_uv_rightbottom.y;
		m_tex_vertex[3].TexLight.x = m_tex_vertex[2].TexColor.x = m_uv_lefttop.x;
		m_tex_vertex[3].TexLight.y = m_tex_vertex[2].TexColor.y = m_uv_rightbottom.y;
	}
}


void Drawable2DObject::SetPolygonSize( float width , float height )
{
	m_width = width;
	m_height = height;

	m_vertex[0].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , 0 );
	m_vertex[1].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , 0 );
	m_vertex[2].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , 0 );
	m_vertex[3].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , 0 );
}

void Drawable2DObject::SetCollisionRectOnCenter( const Math::Rect2D<Float> &rect )
{
	m_collision_rect.x = rect.x;
	m_collision_rect.y = rect.y;
	m_collision_rect.w = rect.w;
	m_collision_rect.h = rect.h;
}

void Drawable2DObject::SetSpeed( Math::Vector3D & speed )
{
	m_speed.x = speed.x;
	m_speed.y = speed.y;
	m_speed.z = speed.z;
}

void Drawable2DObject::SetSpeedX( float speed_x )
{
	m_speed.x = speed_x;
}

void Drawable2DObject::SetSpeedY( float speed_y )
{
	m_speed.y = speed_y;
}
void Drawable2DObject::SetSpeedZ(float speed_z)
{
	m_speed.z = speed_z;
}

void Drawable2DObject::SetAccel( Math::Vector3D & accel )
{
	m_accel.x = accel.x;
	m_accel.y = accel.y;
	m_accel.z = accel.z;
}
void Drawable2DObject::SetAccelX( float accel_x )
{
	m_accel.x = accel_x;
}
void Drawable2DObject::SetAccelY( float accel_y )
{
	m_accel.y = accel_y;
}
void Drawable2DObject::SetAccelZ( float accel_z )
{
	m_accel.z = accel_z;
}

void Drawable2DObject::SetRotation( Math::Vector3D & rotate )
{
	m_rotate.x = rotate.x;
	m_rotate.y = rotate.y;
	m_rotate.z = rotate.z;
}

void Drawable2DObject::SetRotationX( float rotate_x )
{
	m_rotate.x = rotate_x;
}

void Drawable2DObject::SetRotationY( float rotate_y )
{
	m_rotate.y = rotate_y;
}

void Drawable2DObject::SetRotationZ( float rotate_z )
{
	m_rotate.z = rotate_z;
}

void Drawable2DObject::SetRotationSpeed( Math::Vector3D & rotate_speed )
{
	m_rotation_speed = rotate_speed;
}

void Drawable2DObject::SetRotationSpeedX( float rotate_speed_x )
{
	m_rotation_speed.x = rotate_speed_x;
}

void Drawable2DObject::SetRotationSpeedY( float rotate_speed_y )
{
	m_rotation_speed.y = rotate_speed_y;
}

void Drawable2DObject::SetRotationSpeedZ( float rotate_speed_z )
{
	m_rotation_speed.z = rotate_speed_z;
}



void Drawable2DObject::SetUV(Selene::Math::Vector2D & lefttop, Selene::Math::Vector2D & rightbottom , bool update_vertex_data )
{
	m_uv_lefttop.x = lefttop.x;
	m_uv_lefttop.y = lefttop.y;
	m_uv_rightbottom.x = rightbottom.x;
	m_uv_rightbottom.y = rightbottom.y;

	if( update_vertex_data )
	{
		if( m_is_right_direction )
		{
			m_tex_vertex[0].TexLight.x = m_tex_vertex[0].TexColor.x = m_uv_lefttop.x;
			m_tex_vertex[0].TexLight.y = m_tex_vertex[0].TexColor.y = m_uv_lefttop.y;
			m_tex_vertex[1].TexLight.x = m_tex_vertex[1].TexColor.x = m_uv_rightbottom.x;
			m_tex_vertex[1].TexLight.y = m_tex_vertex[1].TexColor.y = m_uv_lefttop.y;
			m_tex_vertex[2].TexLight.x = m_tex_vertex[2].TexColor.x = m_uv_rightbottom.x;
			m_tex_vertex[2].TexLight.y = m_tex_vertex[2].TexColor.y = m_uv_rightbottom.y;
			m_tex_vertex[3].TexLight.x = m_tex_vertex[3].TexColor.x = m_uv_lefttop.x;
			m_tex_vertex[3].TexLight.y = m_tex_vertex[3].TexColor.y = m_uv_rightbottom.y;
		}
		else
		{
			m_tex_vertex[1].TexLight.x = m_tex_vertex[1].TexColor.x = m_uv_lefttop.x;
			m_tex_vertex[1].TexLight.y = m_tex_vertex[1].TexColor.y = m_uv_lefttop.y;
			m_tex_vertex[0].TexLight.x = m_tex_vertex[0].TexColor.x = m_uv_rightbottom.x;
			m_tex_vertex[0].TexLight.y = m_tex_vertex[0].TexColor.y = m_uv_lefttop.y;
			m_tex_vertex[2].TexLight.x = m_tex_vertex[3].TexColor.x = m_uv_rightbottom.x;
			m_tex_vertex[2].TexLight.y = m_tex_vertex[3].TexColor.y = m_uv_rightbottom.y;
			m_tex_vertex[3].TexLight.x = m_tex_vertex[2].TexColor.x = m_uv_lefttop.x;
			m_tex_vertex[3].TexLight.y = m_tex_vertex[2].TexColor.y = m_uv_rightbottom.y;
		}
	}
}

void Drawable2DObject::Release()
{
	SAFE_RELEASE( m_primitive );
	SAFE_RELEASE( m_actor );
	m_is_valiable = false;
}

void Drawable2DObject::Action( void )
{
	if( m_will_change_speed_and_accel_with_direction &&
		!m_is_right_direction )
	{
		m_pos.x -= m_speed.x;
		m_pos.y += m_speed.y;
		m_pos.z += m_speed.z;
		m_speed.x -= m_accel.x;
		m_speed.y += m_accel.y;
		m_speed.z += m_accel.z;
	}
	else
	{
		m_pos.x += m_speed.x;
		m_pos.y += m_speed.y;
		m_pos.z += m_speed.z;
		m_speed.x += m_accel.x;
		m_speed.y += m_accel.y;
		m_speed.z += m_accel.z;
	}

	if( m_is_right_direction )
	{
		m_rotate.x += m_rotation_speed.x;
		m_rotate.y += m_rotation_speed.y;
		m_rotate.z += m_rotation_speed.z;
	}
	else
	{
		m_rotate.x += m_rotation_speed.x;
		m_rotate.y -= m_rotation_speed.y;
		m_rotate.z -= m_rotation_speed.z;
	}
	
	if( m_rotate.x > 360 )
	{
		m_rotate.x -= 360;
	}
	else if( m_rotate.x < -360 )
	{
		m_rotate.x += 360;
	}

	if( m_rotate.y > 360 )
	{
		m_rotate.y -= 360;
	}
	else if( m_rotate.y < -360 )
	{
		m_rotate.y += 360;
	}

	if( m_rotate.z > 360 )
	{
		m_rotate.z -= 360;
	}
	else if( m_rotate.z < -360 )
	{
		m_rotate.z += 360;
	}
}

// 描画
void Drawable2DObject::Draw( void )
{
	if( !m_actor )
	{
		return;
	}

	g_pRender->SetTexture( TEXTURE_STAGE_COLOR , m_texture );


	
	m_actor->TransformReset();
//	rotate.RotationZXY( DEG_TO_ANGLE(m_rotate.x) , DEG_TO_ANGLE(m_rotate.y) , DEG_TO_ANGLE(m_rotate.z) );
//	translation.Translation( m_pos.x , m_pos.y , m_pos.z );
//	world = rotate.Multiply( translation );

	// FixedPositionの場合、ここでさらに特別処理
	if( m_is_billboard )
	{
		const Math::Matrix & world_to_view = g_pSceneMgr->GetCamera()->WorldToView();
		Math::Matrix rotate,translation,world;

//		Math::Vector3D coord = g_pSceneMgr->TransformFromScreen( m_pos );

		world.Translation( m_pos.x , m_pos.y , m_pos.z );

		world.x.x = world_to_view.x.x;
		world.x.y = world_to_view.y.x;
		world.x.z = world_to_view.z.x;
		world.y.x = world_to_view.x.y;
		world.y.y = world_to_view.y.y;
		world.y.z = world_to_view.z.y;
		world.z.x = world_to_view.x.z;
		world.z.y = world_to_view.y.z;
		world.z.z = world_to_view.z.z;
		m_actor->FromMatrix( world );
	}
	else
	{
		m_actor->RotationX( DEG_TO_ANGLE(m_rotate.x) );
		m_actor->RotationY( DEG_TO_ANGLE(m_rotate.y) );
		m_actor->RotationZ( DEG_TO_ANGLE(m_rotate.z) );
		m_actor->Translation( m_pos );
	}

	m_actor->TransformUpdate();
	m_actor->RenderingRequest();

//	g_pRender->SetTextureFilterType

	/*for debug*
	int i;
	Math::Vector3D p;
	Math::Rect2D<Float> r;
	GetCollisionRect( r );
	p.x = r.x+r.w/2;
	p.y = r.y+r.h/2;
	p.z = m_pos.z;
		
	for( i=0 ; i<4 ; i++ )
	{
		if( !m_debug_line_actor[i] )
		{
			continue;
		}
		g_pRender->SetTexture( TEXTURE_STAGE_COLOR , NULL );
		m_debug_line_actor[i]->TransformReset();

//		m_debug_line_actor[i]->RotationX( DEG_TO_ANGLE(m_rotate.x) );
//		m_debug_line_actor[i]->RotationY( DEG_TO_ANGLE(m_rotate.y) );
//		m_debug_line_actor[i]->RotationZ( DEG_TO_ANGLE(m_rotate.z) );
		m_debug_line_actor[i]->Translation( m_pos );
		m_debug_line_actor[i]->TransformUpdate();

		m_debug_line_actor[i]->RenderingRequest();
	}

	/*for Debug*/
}

// 可視判定
bool Drawable2DObject::CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time )
{
	// それぞれの座標を変換
	Math::Vector3D vertex[4];
	vertex[0].x = - m_width/2;
	vertex[0].y = + m_height/2;
	vertex[0].z = 0;
	vertex[1].x = + m_width/2;
	vertex[1].y = + m_height/2;
	vertex[1].z = 0;
	vertex[2].x = + m_width/2;
	vertex[2].y = - m_height/2;
	vertex[2].z = 0;
	vertex[3].x = - m_width/2;
	vertex[3].y = - m_height/2;
	vertex[3].z = 0;

	
	// 全頂点に対してチェック
	Math::Vector3D transformed[4];
	Math::Matrix rotate,move;
	rotate.RotationZXY( DEG_TO_ANGLE(m_rotate.x) , DEG_TO_ANGLE(m_rotate.y) , DEG_TO_ANGLE(m_rotate.z) );
	move.Translation( m_pos.x , m_pos.y , m_pos.z );
	int i;
	for( i=0 ; i<4 ; i++ )
	{
		vertex[i].TransformCoord( rotate );
		vertex[i].TransformCoord( move );
		transformed[i] = pSceneMgr->TransformToScreen( vertex[i] );
//		D3DXVec3TransformCoord( &transformed[i] , &vertex[i] , &trans_mat );
		// 頂点が内部にあったら無条件で描画
		if(// transformed[i].z <= 1.0f && transformed[i].z >=0 &&
			transformed[i].x <= screen_width && transformed[i].x >= 0.0f &&
			transformed[i].y <= screen_height && transformed[i].y >= 0.0f )
		{
			m_last_visible_time = now_time;
			m_invisible_period = 0;
			return true;
		}
	}

	if( !m_do_detail_check_visible )
	{
		m_invisible_period++;
		return false;
	}

	float x,y;
	int next;
	for( i=0 ; i<4 ; i++ )
	{
		for( next=i+1 ; next<4 ; next++ )
		{
			// x = 0 に対する評価
			if( ( transformed[i].x > 0 && transformed[next].x < 0 ) ||
				( transformed[i].x < 0 && transformed[next].x > 0 ) )
			{
				y = -(transformed[next].y-transformed[i].y)/(transformed[next].x-transformed[i].x)*transformed[i].x+transformed[i].y;
				if( y>=0 && y<= screen_height )
				{
					m_last_visible_time = now_time;
					m_invisible_period = 0;
					return true;
				}
			}

			// x = screen_widthに対する評価
			if( ( transformed[i].x > screen_width && transformed[next].x < screen_width ) ||
				( transformed[i].x < screen_width && transformed[next].x > screen_width ) )
			{
				y = (transformed[next].y-transformed[i].y)/(transformed[next].x-transformed[i].x)*(screen_width-transformed[i].x)+transformed[i].y;
				if( y>=0 && y<= screen_height )
				{
					m_last_visible_time = now_time;
					m_invisible_period = 0;
					return true;
				}
			}

			// y = 0に対する評価
			if( ( transformed[i].y > 0 && transformed[next].y < 0 ) ||
				( transformed[i].y < 0 && transformed[next].y > 0 ) )
			{
				x = transformed[i].x - transformed[i].y*(transformed[next].x-transformed[i].x)/(transformed[next].y-transformed[i].y);
				if( x>=0 && x<=screen_width )
				{
					m_last_visible_time = now_time;
					m_invisible_period = 0;
					return true;
				}
			}

			// y = screen_heightに対する評価
			if( ( transformed[i].y > screen_height && transformed[next].y < screen_height ) ||
				( transformed[i].y < screen_height && transformed[next].y > screen_height ) )
			{
				x = transformed[i].x + (screen_height-transformed[i].y)*(transformed[next].x-transformed[i].x)/(transformed[next].y-transformed[i].y);
				if( x>=0 && x<=screen_width )
				{
					m_last_visible_time = now_time;
					m_invisible_period = 0;
					return true;
				}
			}
		}
	}

	m_invisible_period++;
	return false;
}

bool Drawable2DObject::DetectCollision( DrawableObject &object )
{
	Math::Rect2D<Float> target,this_rect;
	GetCollisionRect( this_rect );
	object.GetCollisionRect( target );

	// 判定

	// Collision::Rect_Rectが何故かうまく動いていない！！
	// すきやきのアルゴリズムを見て自前で書いたほうがよさそう

	if( Rect_Rect_Collision( this_rect , target ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// RectとRectのコリジョン
bool Drawable2DObject::Rect_Rect_Collision( const Math::Rect2D<Float> &rect1 , const Math::Rect2D<Float> &rect2 )
{
//	if(X0 < X3 && X2 < X1 && Y0 < Y3 && Y2 < Y1)

	if( rect1.x <= rect2.x+rect2.w && rect2.x <= rect1.x+rect1.w &&
		rect1.y <= rect2.y+rect2.h && rect2.y <= rect1.y+rect1.h )
	{
		return true;
	}
	return false;
}

void Drawable2DObject::GetCollisionRect( Math::Rect2D<Float> & rect )
{
	if( m_is_right_direction )
	{
		rect.x = m_pos.x+m_collision_rect.x;
		rect.y = m_pos.y+m_collision_rect.y;
		rect.w = m_collision_rect.w;
		rect.h = m_collision_rect.h;
	}
	else
	{
		rect.x = m_pos.x-(m_collision_rect.x+m_collision_rect.w);
		rect.y = m_pos.y+m_collision_rect.y;
		rect.w = m_collision_rect.w;
		rect.h = m_collision_rect.h;
	}
}


/* --------------
	Objects実装
   -------------- */
CObjects::CObjects()
{
}
CObjects::~CObjects()
{
}

void CObjects::Add( DrawableObject *object , bool do_draw , bool do_action )
{
	objects.push_back( object );
	m_do_draw.push_back( do_draw );
	m_do_action.push_back( do_action );
}

bool CObjects::Delete( DrawableObject *object )
{
	int size = objects.size();
	int i;
	for( i=0 ; i<size ; i++ )
	{
		if( objects.at(i) == object )
		{
			objects.erase( objects.begin()+i );
			m_do_draw.erase( m_do_draw.begin()+i );
			m_do_action.erase( m_do_action.begin()+i );
			return true;
		}
	}

	return true;
}

void CObjects::ActAll( void )
{
	int i;
	int size = objects.size();
	for( i=0 ; i<size ; i++ )
	{
		if( m_do_action.at(i) )
		{
			objects.at(i)->Action();
		}
	}
}

void CObjects::DrawAll( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height )
{
	int i;
	int size = objects.size();
	for( i=0 ; i<size ; i++ )
	{
		DrawableObject *object = objects.at(i);
		if( m_do_draw.at(i) )
		{
			if( object->CheckVisible( pSceneMgr , screen_width , screen_width , GameSystem::now_frame_time ) )
			{
				object->Draw();
			}
		}
		else
		{
			object->SetInvisibleThisFrame();
		}
	}
}

void CObjects::SetDraw( DrawableObject *object , bool do_draw )
{
	std::vector<DrawableObject*>::iterator it = objects.begin() , end = objects.end();
	int i=0;

	while( it != end )
	{
		if( *it == object )
		{
			m_do_draw[i] = do_draw;
			break;
		}
		i++;
		it++;
	}
}

void CObjects::SetAction( DrawableObject *object , bool do_action )
{
	std::vector<DrawableObject*>::iterator it = objects.begin() , end = objects.end();
	int i=0;

	while( it != end )
	{
		if( *it == object )
		{
			m_do_action[i] = do_action;
			break;
		}
		i++;
		it++;
	}
}
