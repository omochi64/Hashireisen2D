
#include "MultipleAnimation3DBox.h"
#include "Game/UdongeAnimation.h"

// �R���X�g���N�^
// �����i�V�o�[�W����
MultipleAnimation3DBox::MultipleAnimation3DBox():MultipleAnimation()
{
	m_depth = 0;
	int i;
	for( i=0 ; i<24 ; i++ )
	{
		m_vertex[i].Col = m_vertex_color;
	}
}
// �����A���o�[�W����
MultipleAnimation3DBox::MultipleAnimation3DBox(Math::Vector3D center_pos,
											   float width, float height, float depth,
											   Math::Vector3D speed,Math::Vector3D accel)
											   :MultipleAnimation(center_pos , width , height , speed , accel)
{
	m_depth = depth;

	int i;
	for( i=0 ; i<24 ; i++ )
	{
		m_vertex[i].Col = m_vertex_color;
	}

	

	// ����
	m_vertex[0].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[1].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[2].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , -m_depth/2 );
	m_vertex[3].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , -m_depth/2 );
	
	// �E
	m_vertex[4].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[5].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[6].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , +m_depth/2 );
	m_vertex[7].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , -m_depth/2 );

	// ��
	m_vertex[8].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[9].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[10].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , -m_depth/2 );
	m_vertex[11].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , +m_depth/2 );
	
	// ��
	m_vertex[12].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[13].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[14].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[15].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , -m_depth/2 );

	// ��
	m_vertex[16].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , +m_depth/2 );
	m_vertex[17].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , +m_depth/2 );
	m_vertex[18].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , -m_depth/2 );
	m_vertex[19].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , -m_depth/2 );

	
//	UpdateVertex();

	
}

// ���_���W�A�b�v�f�[�g
void MultipleAnimation3DBox::UpdateVertex()
{
	/*
	SAFE_RELEASE( m_primitive );

	m_primitive = g_pRender->CreatePrimitive3D( 4 , 6 , VERTEX_ELEMENT_3DBASE | VERTEX_ELEMENT_3DTEXTURE , false , false );

	Renderer::SVertex3DBase	vertex[4];
	Renderer::SVertex3DTexture	tex_vertex[4];

	m_primitive->Begin();

	int i;
	for( i=0 ; i<4 ; i++ )
	{
		vertex[i].Col = CColor( 0xff , 0xff , 0xff );
	}
	vertex[0].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , m_pos.z );
	vertex[1].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , m_pos.z );
	vertex[2].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , m_pos.z );
	vertex[3].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , m_pos.z );

	m_primitive->Push( vertex , 4 );

	if( m_is_right_direction )
	{
		tex_vertex[0].TexLight = tex_vertex[0].TexColor = m_uv_lefttop;
		tex_vertex[1].TexLight = tex_vertex[1].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		tex_vertex[2].TexLight = tex_vertex[2].TexColor = m_uv_rightbottom;
		tex_vertex[3].TexLight = tex_vertex[3].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
	}
	else
	{
		tex_vertex[1].TexLight = tex_vertex[1].TexColor = m_uv_lefttop;
		tex_vertex[0].TexLight = tex_vertex[0].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		tex_vertex[2].TexLight = tex_vertex[3].TexColor = m_uv_rightbottom;
		tex_vertex[3].TexLight = tex_vertex[2].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
	}
	m_primitive->Push( tex_vertex , 4 );

	Uint16 Idx[] = {
		0 , 1 , 2 , 0 , 2 , 3
	};
	m_primitive->Push( Idx , 6 );

	m_primitive->End();

	SAFE_RELEASE(m_actor);
	m_actor = g_pSceneMgr->CreateActor( m_primitive );
	*/

	// Selene��CBox�g�����A���O�Œ��_���`�����ق����悳����
	// �Ȃ��Ȃ�A���̂ق���UV���W�̈�����������₷��(�Ƃ�����Selene�ł̂������킩��Ȃ�)����


	SAFE_RELEASE( m_primitive );

	m_primitive = g_pRender->CreatePrimitive3D( 24 , 36 , VERTEX_ELEMENT_3DBASE | VERTEX_ELEMENT_3DTEXTURE , false , false );

	

	m_primitive->Begin();

	

	m_primitive->Push( m_vertex , 24 );

	
	m_primitive->Push( m_tex_vertex , 20 );

	Uint16 Idx[] = {
		0 , 1 , 2 , 0 , 2 , 3 , 4 , 5 , 6 ,
		4 , 6 , 7 , 8 , 9 , 10 , 8 , 10 , 11 ,
		12 , 13 , 14 , 12 , 14 , 15 , 16 , 17 , 18 ,
		16 , 18, 19//, 20, 21, 22, 20, 22, 23
	};
	m_primitive->Push( Idx , 30 );

	m_primitive->End();

	SAFE_RELEASE(m_actor);
	m_actor = g_pSceneMgr->CreateActor( m_primitive );


	/*for debug*
	for( i=0 ; i<4 ; i++ )
	{
		SAFE_RELEASE(m_debug_line_primitive[i]);
		SAFE_RELEASE(m_debug_line_actor[i]);
		m_debug_line_primitive[i] = g_pRender->CreatePrimitive3D( 4 , 6 , VERTEX_ELEMENT_3DBASE , false , false );
		m_debug_line_primitive[i]->Begin();

		vertex[i].Col = CColor( 0xff , 0 , 0 );
	}

	vertex[0].Pos = Math::Vector3D( m_collision_rect.x , (m_collision_rect.y+m_collision_rect.h) , -1.1f );
	vertex[1].Pos = Math::Vector3D( m_collision_rect.x+0.05f , (m_collision_rect.y+m_collision_rect.h) , -1.1f );
	vertex[2].Pos = Math::Vector3D( m_collision_rect.x+0.05f , m_collision_rect.y , -1.1f );
	vertex[3].Pos = Math::Vector3D( m_collision_rect.x , m_collision_rect.y , -1.1f );
	
	m_debug_line_primitive[0]->Push( vertex , 4 );
	m_debug_line_primitive[0]->Push( Idx , 6 );

	vertex[0].Pos = Math::Vector3D( m_collision_rect.x , (m_collision_rect.y+m_collision_rect.h) , -1.1f );
	vertex[1].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , (m_collision_rect.y+m_collision_rect.h) , -1.1f );
	vertex[2].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , (m_collision_rect.y+m_collision_rect.h)-0.05f , -1.1f );
	vertex[3].Pos = Math::Vector3D( m_collision_rect.x , (m_collision_rect.y+m_collision_rect.h)-0.05f , -1.1f );
	
	m_debug_line_primitive[1]->Push( vertex , 4 );
	m_debug_line_primitive[1]->Push( Idx , 6 );

	vertex[0].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w)-0.05f , (m_collision_rect.y+m_collision_rect.h) , -1.1f );
	vertex[1].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , (m_collision_rect.y+m_collision_rect.h) , -1.1f );
	vertex[2].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , m_collision_rect.y ,-1.1f );
	vertex[3].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w)-0.05f , m_collision_rect.y , -1.1f );
	
	m_debug_line_primitive[2]->Push( vertex , 4 );
	m_debug_line_primitive[2]->Push( Idx , 6 );

	vertex[0].Pos = Math::Vector3D( m_collision_rect.x , m_collision_rect.y+0.05f , -1.1f );
	vertex[1].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , m_collision_rect.y+0.05f , -1.1f );
	vertex[2].Pos = Math::Vector3D( (m_collision_rect.x+m_collision_rect.w) , m_collision_rect.y , -1.1f );
	vertex[3].Pos = Math::Vector3D( m_collision_rect.x , m_collision_rect.y , -1.1f );
	
	m_debug_line_primitive[3]->Push( vertex , 4 );
	m_debug_line_primitive[3]->Push( Idx , 6 );

	for( i=0 ; i<4 ; i++ )
	{
		m_debug_line_primitive[i]->End();
		m_debug_line_actor[i] = g_pSceneMgr->CreateActor( m_debug_line_primitive[i] );
	}


	/*for debug*/

}

// ���_�F�ݒ�
void MultipleAnimation3DBox::SetVertexColor(Selene::CColor color)
{
	m_vertex_color = color;
	int i;
	for( i=0 ; i<24 ; i++ )
	{
		m_vertex[i].Col = m_vertex_color;
	}
}

// �`��
//void MultipleAnimation3DBox::Draw( void )
//{
//}

DrawableObject* MultipleAnimation3DBox::Clone( void )
{
	MultipleAnimation3DBox *clone_object = new MultipleAnimation3DBox(
		m_pos , m_width , m_height , m_depth , m_speed , m_accel );

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
		
		clone_object->UpdateVertex();
	}

	return clone_object;
}

// ������
bool MultipleAnimation3DBox::CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time )
{
	// ���ꂼ��̍��W��ϊ�
	Math::Vector3D vertex[8];
	vertex[0].x = - m_width/2;
	vertex[0].y = + m_height/2;
	vertex[0].z = - m_depth/2;
	vertex[1].x = + m_width/2;
	vertex[1].y = + m_height/2;
	vertex[1].z = - m_depth/2;
	vertex[2].x = + m_width/2;
	vertex[2].y = - m_height/2;
	vertex[2].z = - m_depth/2;
	vertex[3].x = - m_width/2;
	vertex[3].y = - m_height/2;
	vertex[3].z = - m_depth/2;
	vertex[4].x = - m_width/2;
	vertex[4].y = + m_height/2;
	vertex[4].z = + m_depth/2;
	vertex[5].x = + m_width/2;
	vertex[5].y = + m_height/2;
	vertex[5].z = + m_depth/2;
	vertex[6].x = + m_width/2;
	vertex[6].y = - m_height/2;
	vertex[6].z = - m_depth/2;
	vertex[7].x = - m_width/2;
	vertex[7].y = - m_height/2;
	vertex[7].z = + m_depth/2;
	
	// �S���_�ɑ΂��ă`�F�b�N
	Math::Vector3D transformed[8];
	Math::Matrix translation;
	translation.Translation( m_pos.x , m_pos.y , m_pos.z );
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		transformed[i] = pSceneMgr->TransformToScreen( vertex[i].TransformCoord( translation ) );
//		D3DXVec3TransformCoord( &transformed[i] , &vertex[i] , &trans_mat );
		// ���_�������ɂ������疳�����ŕ`��
		if( transformed[i].z <= 1.0f && transformed[i].z >=0 &&
			transformed[i].x <= screen_width && transformed[i].x >= 0.0f &&
			transformed[i].y <= screen_height && transformed[i].y >= 0.0f )
		{
			m_last_visible_time = now_time;
			m_invisible_period = 0;
			return true;
		}
	}

	// ���_�ԂŒ��������сA��_�����o
	// �l�p�`��̑S�Ă̒����A�����đΊp���̂ǂꂩ�������ɐi�����Ă�����`��

	m_invisible_period++;
	
	return false;
}

void MultipleAnimation3DBox::SetPolygonSize( float width , float height )
{
	m_width = width;
	m_height = height;

	
	// ����
	m_vertex[0].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[1].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[2].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , -m_depth/2 );
	m_vertex[3].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , -m_depth/2 );
	
	// �E
	m_vertex[4].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[5].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[6].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , +m_depth/2 );
	m_vertex[7].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , -m_depth/2 );

	// ��
	m_vertex[8].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[9].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[10].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , -m_depth/2 );
	m_vertex[11].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , +m_depth/2 );
	
	// ��
	m_vertex[12].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[13].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , +m_depth/2 );
	m_vertex[14].Pos = Math::Vector3D( +m_width/2 , +m_height/2 , -m_depth/2 );
	m_vertex[15].Pos = Math::Vector3D( -m_width/2 , +m_height/2 , -m_depth/2 );

	// ��
	m_vertex[16].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , +m_depth/2 );
	m_vertex[17].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , +m_depth/2 );
	m_vertex[18].Pos = Math::Vector3D( -m_width/2 , -m_height/2 , -m_depth/2 );
	m_vertex[19].Pos = Math::Vector3D( +m_width/2 , -m_height/2 , -m_depth/2 );
}

void MultipleAnimation3DBox::SetUV( Math::Vector2D & lefttop , Math::Vector2D & rightbottom , bool update_vertex_data )
{
	MultipleAnimation::SetUV( lefttop , rightbottom , false );

	// right_direction�Ƃ�m_tex_vertex�Ɋ֌W�Ȃ��ˁc�H
	if( update_vertex_data )
	{
		m_tex_vertex[0].TexLight = m_tex_vertex[0].TexColor = m_uv_lefttop;
		m_tex_vertex[1].TexLight = m_tex_vertex[1].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		m_tex_vertex[2].TexLight = m_tex_vertex[2].TexColor = m_uv_rightbottom;
		m_tex_vertex[3].TexLight = m_tex_vertex[3].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
		m_tex_vertex[4].TexLight = m_tex_vertex[4].TexColor = m_uv_lefttop;
		m_tex_vertex[5].TexLight = m_tex_vertex[5].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		m_tex_vertex[6].TexLight = m_tex_vertex[6].TexColor = m_uv_rightbottom;
		m_tex_vertex[7].TexLight = m_tex_vertex[7].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
		m_tex_vertex[8].TexLight = m_tex_vertex[8].TexColor = m_uv_lefttop;
		m_tex_vertex[9].TexLight = m_tex_vertex[9].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		m_tex_vertex[10].TexLight = m_tex_vertex[10].TexColor = m_uv_rightbottom;
		m_tex_vertex[11].TexLight = m_tex_vertex[11].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
		m_tex_vertex[12].TexLight = m_tex_vertex[12].TexColor = m_uv_lefttop;
		m_tex_vertex[13].TexLight = m_tex_vertex[13].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		m_tex_vertex[14].TexLight = m_tex_vertex[14].TexColor = m_uv_rightbottom;
		m_tex_vertex[15].TexLight = m_tex_vertex[15].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
		m_tex_vertex[16].TexLight = m_tex_vertex[16].TexColor = m_uv_lefttop;
		m_tex_vertex[17].TexLight = m_tex_vertex[17].TexColor = Math::Vector2D( m_uv_rightbottom.x , m_uv_lefttop.y );
		m_tex_vertex[18].TexLight = m_tex_vertex[18].TexColor = m_uv_rightbottom;
		m_tex_vertex[19].TexLight = m_tex_vertex[19].TexColor = Math::Vector2D( m_uv_lefttop.x , m_uv_rightbottom.y );
	}

}
