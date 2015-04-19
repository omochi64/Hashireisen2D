
#include "Game/Objects.h"
#include "DrawBackgroundTask.h"
#include "System/GlobalNamespace.h"

DrawBackgroundTask::DrawBackgroundTask( Renderer::ITexture *pTopTex , Renderer::ITexture *pCenterTex , Renderer::ITexture *pUnderTex , Math::Point2DI course_size )
{
//	BACKGROUND_Z = ;
	m_pTopTex = pTopTex;
	m_pUnderTex = pUnderTex;
	m_pCenterTex = pCenterTex;


	// 通常バージョンの生成
	Drawable2DObject *background;
	int normal_center_y = 10;
	int width_height = BACKGROUND_Z*1.2f;
	int start_x = 0;
//	int start_y = -course_size.y;
	float width_stretch = 2.0f;
	int polygon_num_x = (course_size.x-(start_x-width_height/2)+width_height/2)/(width_height*width_stretch)+2;
	int i;
	int j;
	// 真ん中
	for( i=0 ; i<polygon_num_x ; i++ )
	{
		background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
			width_height*width_stretch , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(width_stretch,1.0f) );
		background->SetTexture( m_pCenterTex );
		background->SetCenterPosition( Math::Vector3D( start_x+i*width_height*width_stretch , normal_center_y , BACKGROUND_Z ) );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );

	}
	// 左
	background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
		width_height , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(1.0f,1.0f) );
	background->SetCenterPosition( Math::Vector3D( start_x-width_height*width_stretch/2 , normal_center_y , BACKGROUND_Z-width_height/2 ) );
	background->SetTexture( m_pCenterTex );
	background->SetRotationY( -90 );
	background->SetDoDetailCheckVisible(true);
	background->UpdateVertex();
	GameSystem::all_object->Add( background , true , false );
	m_backgrounds.push_back( background );

	// 右
	background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
		width_height , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(1.0f,1.0f) );
	background->SetCenterPosition( Math::Vector3D( start_x+(polygon_num_x-0.5f)*width_height*width_stretch , normal_center_y , BACKGROUND_Z-width_height/2 ) );
	background->SetTexture( m_pCenterTex );
	background->SetRotationY( 90 );
	background->SetDoDetailCheckVisible(true);
	background->UpdateVertex();
	GameSystem::all_object->Add( background , true , false );
	m_backgrounds.push_back( background );


	// 上バージョンの生成
	for( j=0 ; normal_center_y+(j+0.5f)*width_height<course_size.y ; j++ )
	{
		for( i=0 ; i<polygon_num_x ; i++ )
		{
			background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
				width_height*width_stretch , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(width_stretch,1.0f) );
			background->SetTexture( m_pTopTex );
			background->SetCenterPosition( Math::Vector3D( start_x+i*width_height*width_stretch , normal_center_y+width_height*(j+1) , BACKGROUND_Z) );
			background->SetDoDetailCheckVisible(true);
			background->UpdateVertex();
			GameSystem::all_object->Add( background , true , false );
			m_backgrounds.push_back( background );
		}

		// 左
		background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
			width_height , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(1.0f,1.0f) );
		background->SetCenterPosition( Math::Vector3D( start_x-width_height*width_stretch/2 , normal_center_y+width_height*(j+1) , BACKGROUND_Z-width_height/2 ) );
		background->SetTexture( m_pTopTex );
		background->SetRotationY( -90 );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );

		// 右
		background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
			width_height , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(1.0f,1.0f) );
		background->SetCenterPosition( Math::Vector3D( start_x+(polygon_num_x-0.5f)*width_height*width_stretch , normal_center_y+width_height*(j+1) , BACKGROUND_Z-width_height/2 ) );
		background->SetTexture( m_pTopTex );
		background->SetRotationY( 90 );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );
	}
	// 天蓋
	int top = normal_center_y+(j+0.5f)*width_height;
	for( i=0 ; i<polygon_num_x ; i++ )
	{
		background = new Drawable2DObject( Math::Vector3D(0,0,0) , 
			width_height*width_stretch , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) ,
			Math::Vector2D(0,0) , Math::Vector2D( width_stretch , 1.0f ) );
		background->SetCenterPosition( Math::Vector3D(start_x+i*width_height*width_stretch , top , BACKGROUND_Z/2) );
		background->SetTexture( m_pTopTex );
		background->SetRotationX( -90 );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );
	}

	// 下バージョンの生成
	for( j=0 ; normal_center_y-(j+0.5f)*width_height>-course_size.y ; j++ )
	{
		for( i=0 ; i<polygon_num_x ; i++ )
		{
			background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
				width_height*width_stretch , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(width_stretch,1.0f) );
			background->SetTexture( m_pUnderTex );
			background->SetCenterPosition( Math::Vector3D( start_x+i*width_height*width_stretch , normal_center_y-width_height*(j+1) , BACKGROUND_Z) );
			background->SetDoDetailCheckVisible(true);
			background->UpdateVertex();
			GameSystem::all_object->Add( background , true , false );
			m_backgrounds.push_back( background );
		}

		// 左
		background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
			width_height , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(1.0f,1.0f) );
		background->SetCenterPosition( Math::Vector3D( start_x-width_height*width_stretch/2 , normal_center_y-width_height*(j+1) , BACKGROUND_Z-width_height/2 ) );
		background->SetTexture( m_pUnderTex );
		background->SetRotationY( -90 );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );

		// 右
		background = new Drawable2DObject( Math::Vector3D(0,0,0) ,
			width_height , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) , Math::Vector2D(0,0) , Math::Vector2D(1.0f,1.0f) );
		background->SetCenterPosition( Math::Vector3D( start_x+(polygon_num_x-0.5f)*width_height*width_stretch , normal_center_y-width_height*(j+1) , BACKGROUND_Z-width_height/2 ) );
		background->SetTexture( m_pUnderTex );
		background->SetRotationY( 90 );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );
	}
	int under = normal_center_y-(j+0.5f)*width_height;
	for( i=0 ; i<polygon_num_x ; i++ )
	{
		background = new Drawable2DObject( Math::Vector3D(0,0,0) , 
			width_height*width_stretch , width_height , Math::Vector3D(0,0,0) , Math::Vector3D(0,0,0) ,
			Math::Vector2D(0,0) , Math::Vector2D( width_stretch , 1.0f ) );
		background->SetCenterPosition( Math::Vector3D(start_x+i*width_height*width_stretch , under , BACKGROUND_Z/2) );
		background->SetTexture( m_pUnderTex );
		background->SetRotationX( 90 );
		background->SetDoDetailCheckVisible(true);
		background->UpdateVertex();
		GameSystem::all_object->Add( background , true , false );
		m_backgrounds.push_back( background );
	}

}

DrawBackgroundTask::~DrawBackgroundTask()
{
	std::vector<Drawable2DObject *>::iterator it,end;
	it = m_backgrounds.begin();
	end = m_backgrounds.end();

	while( it != end )
	{
		GameSystem::all_object->Delete( *it );
		SAFE_RELEASE(*it);
		it++;
	}

//	SAFE_RELEASE(m_pTopTex);
//	SAFE_RELEASE(m_pCenterTex);
//	SAFE_RELEASE(m_pUnderTex);
}


void DrawBackgroundTask::run()
{
/*
	Renderer::SPrimitiveVertex2D v[2]={0};

	int left_x;
	int top_y;
	left_x = -Character::udonge->GetPos().x*calibration_x;
	left_x %= tex_w;
	top_y = Character::udonge->GetPos().y*calibration_y;
	if( GameSystem::is_searching_udonge )
	{
		last_start_x = left_x;
		last_start_y = top_y;
	}
	else
	{
		left_x = last_start_x;
		top_y = last_start_y;
	}
	int i=0;

	m_pPrimitive->Begin();

	int j;

	v[0].v1.Col = v[0].v2.Col = v[0].v3.Col = CColor(0xff,0xff,0xff);
	v[1].v1.Col = v[1].v2.Col = v[1].v3.Col = CColor(0xff,0xff,0xff);

	// なぜだ？

	while( left_x+i*tex_w < SCREEN_WIDTH )
	{
		// 真ん中部分
		if( top_y < SCREEN_HEIGHT && top_y + tex_h > 0 )
		{
			v[0].v1.Pos = Math::Vector4D( left_x+tex_w*i , top_y , 0 , 1.0f );
			v[0].v2.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y , 0 , 1.0f );
			v[0].v3.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y+tex_h , 0 , 1.0f );
			v[0].v1.Tex[1] = v[0].v1.Tex[0] = Math::Vector2D( 0,0.5f );
			v[0].v2.Tex[1] = v[0].v2.Tex[0] = Math::Vector2D( 0.5f,0.5f );
			v[0].v3.Tex[1] = v[0].v3.Tex[0] = Math::Vector2D( 0.5,1.0f );
			
			v[1].v1.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y+tex_h , 0 , 1.0f );
			v[1].v2.Pos = Math::Vector4D( left_x+tex_w*i , top_y+tex_h , 0 , 1.0f );
			v[1].v3.Pos = Math::Vector4D( left_x+tex_w*i , top_y , 0 , 1.0f );
			v[1].v1.Tex[1] = v[1].v1.Tex[0] = Math::Vector2D( 0.5f,1.0f );
			v[1].v2.Tex[1] = v[1].v2.Tex[0] = Math::Vector2D( 0.0f,1.0f );
			v[1].v3.Tex[1] = v[1].v3.Tex[0] = Math::Vector2D( 0.0,0.5f );
			
			m_pPrimitive->Push( v , 2 );
		}

		// 上部分
		j = 1;
		while( top_y-(j-1)*tex_h > 0 )
		{
			v[0].v1.Pos = Math::Vector4D( left_x+tex_w*i , top_y-tex_h*j , 0 , 1.0f );
			v[0].v2.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y-tex_h*j , 0 , 1.0f );
			v[0].v3.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y-tex_h*(j-1) , 0 , 1.0f );
			v[0].v1.Tex[1] = v[0].v1.Tex[0] = Math::Vector2D( 0,0.0f );
			v[0].v2.Tex[1] = v[0].v2.Tex[0] = Math::Vector2D( 0.5f,0.0f );
			v[0].v3.Tex[1] = v[0].v3.Tex[0] = Math::Vector2D( 0.5,0.5f );
			v[0].v1.Col = v[0].v2.Col = v[0].v3.Col = CColor(0xff,0xff,0xff);

			v[1].v1.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y-tex_h*(j-1) , 0 , 1.0f );
			v[1].v2.Pos = Math::Vector4D( left_x+tex_w*i , top_y-tex_h*(j-1) , 0 , 1.0f );
			v[1].v3.Pos = Math::Vector4D( left_x+tex_w*i , top_y-tex_h*j , 0 , 1.0f );
			v[1].v1.Tex[1] = v[1].v1.Tex[0] = Math::Vector2D( 0.5f,0.5f );
			v[1].v2.Tex[1] = v[1].v2.Tex[0] = Math::Vector2D( 0.0f,0.5f );
			v[1].v3.Tex[1] = v[1].v3.Tex[0] = Math::Vector2D( 0.0,0.0f );
			v[1].v1.Col = v[1].v2.Col = v[1].v3.Col = CColor(0xff,0xff,0xff);

			m_pPrimitive->Push( v , 2 );

			j++;
		}

		// 下部分
		j = 1;
		while( top_y+j*tex_h < SCREEN_HEIGHT )
		{
			v[0].v1.Pos = Math::Vector4D( left_x+tex_w*i , top_y+tex_h*j , 0 , 1.0f );
			v[0].v2.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y+tex_h*j , 0 , 1.0f );
			v[0].v3.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y+tex_h*(j+1) , 0 , 1.0f );
			v[0].v1.Tex[1] = v[0].v1.Tex[0] = Math::Vector2D( 0.5f,0.5f );
			v[0].v2.Tex[1] = v[0].v2.Tex[0] = Math::Vector2D( 1.0f,0.5f );
			v[0].v3.Tex[1] = v[0].v3.Tex[0] = Math::Vector2D( 1.0f,1.0f );
			v[0].v1.Col = v[0].v2.Col = v[0].v3.Col = CColor(0xff,0xff,0xff);

			v[1].v1.Pos = Math::Vector4D( left_x+tex_w*(i+1) , top_y+tex_h*(j+1) , 0 , 1.0f );
			v[1].v2.Pos = Math::Vector4D( left_x+tex_w*i , top_y+tex_h*(j+1) , 0 , 1.0f );
			v[1].v3.Pos = Math::Vector4D( left_x+tex_w*i , top_y+tex_h*j , 0 , 1.0f );
			v[1].v1.Tex[1] = v[1].v1.Tex[0] = Math::Vector2D( 1.0f,1.0f );
			v[1].v2.Tex[1] = v[1].v2.Tex[0] = Math::Vector2D( 0.5f,1.0f );
			v[1].v3.Tex[1] = v[1].v3.Tex[0] = Math::Vector2D( 0.5,0.5f );
			v[1].v1.Col = v[1].v2.Col = v[1].v3.Col = CColor(0xff,0xff,0xff);

			m_pPrimitive->Push( v , 2 );

			j++;
		}

		i++;
	}

//	GameSystem::pTextureForPrimitive2D = m_texture;

	m_pPrimitive->End();

	PRIMITIVE2D_SET set;
	set.pPrimitive = m_pPrimitive;
	set.pTargetTexture = m_render_texture;
	set.setTexture = m_texture;

	GameSystem::pPrimitive2D.push_back( set );*/

//	GameSystem::pScene->SetParameter_BGTexture( m_texture );

//	GameSystem::pScene->CreateActor( 
//	GameSystem::pRender->SetRenderTarget
//	GameSystem::pRender->SetTexture( TEXTURE_STAGE_COLOR , m_texture );
//	m_actor->Translation( Math::Vector3D(Character::udonge->GetPos().x,0,0) );
//	m_actor->Translation( Character::udonge->GetPos() );
//	for( int i=0 ; i<9 ; i++ )
//	{
//		m_actor[i]->RenderingRequest();
//	}
}
