
#include "System/GlobalNamespace.h"
#include "CreateBackgroundTexture.h"
#include "System/GameSystem.h"

CreateBackgroundTexture::CreateBackgroundTexture(  Renderer::ITexture *pBackTex , Renderer::ITexture *pUnderTexture ,
								Renderer::ITexture *pCenterTexture , Renderer::ITexture *pTopTexture )
{
	m_pUnderTex = pUnderTexture;
	m_pCenterTex = pCenterTexture;
	m_pTopTex = pTopTexture;
	m_pBackTex = pBackTex;
	m_is_first = true;

	m_pUnderPrimitive = GameSystem::pRender->CreatePrimitive2D( 1024 );
	m_pCenterPrimitive = GameSystem::pRender->CreatePrimitive2D( 1024 );
	m_pTopPrimitive = GameSystem::pRender->CreatePrimitive2D( 1024 );
}

CreateBackgroundTexture::~CreateBackgroundTexture()
{
	SAFE_RELEASE(m_pUnderPrimitive);
	SAFE_RELEASE(m_pCenterPrimitive);
	SAFE_RELEASE(m_pTopPrimitive);
}

void CreateBackgroundTexture::run()
{
	if( m_is_first )
	{
		GameSystem::PRIMITIVE2D_SET set;
		int i,j;

		// テクスチャ構築用のプリミティブ生成
		Math::Point2DI size = m_pBackTex->GetSize();
		Renderer::SPrimitiveVertex2D vertex[2];

		vertex[0].v1.Col = vertex[0].v2.Col = vertex[0].v3.Col =
			vertex[1].v1.Col = vertex[1].v2.Col = vertex[1].v3.Col = CColor( 0xff,0xff,0xff );
		
		// 上
		vertex[0].v1.Tex[0] = vertex[1].v3.Tex[0] = Math::Vector2D( 0 , 0 );
		vertex[1].v1.Tex[0] = vertex[0].v3.Tex[0] = Math::Vector2D( 0.5f , 0.5f );
		vertex[0].v2.Tex[0] = Math::Vector2D( 0.5f , 0.0f );
		vertex[1].v2.Tex[0] = Math::Vector2D( 0.0f , 0.5f );
		

		int target_size_x = m_pTopTex->GetSize().x;
		int target_size_y = m_pTopTex->GetSize().y;

		m_pTopPrimitive->Begin();
		for( i=0 ; i<target_size_y ; i+= size.y/2 )
		{
			for( j=0 ; j<target_size_x ; j += size.x/2 )
			{
				vertex[0].v1.Pos = vertex[1].v3.Pos = Math::Vector4D( j , i , 0 , 1.0f );
				vertex[1].v1.Pos = vertex[0].v3.Pos = Math::Vector4D( j+size.x*0.5f , i+size.y*0.5f , 0 , 1.0f );
				vertex[0].v2.Pos = Math::Vector4D( j+size.x*0.5f , i , 0 , 1.0f );
				vertex[1].v2.Pos = Math::Vector4D( j , i+size.y*0.5f , 0 , 1.0f );
				
				m_pTopPrimitive->Push( vertex , 2 );
			}
		}
		m_pTopPrimitive->End();
		set.pPrimitive = m_pTopPrimitive;
		set.pTargetTexture = m_pTopTex;
		set.setTexture = m_pBackTex;
		GameSystem::pPrimitive2D.push_back( set );


		// 下
		vertex[0].v1.Tex[0] = vertex[1].v3.Tex[0] = Math::Vector2D( 0.5f , 0.5f );
		vertex[1].v1.Tex[0] = vertex[0].v3.Tex[0] = Math::Vector2D( 1.0f , 1.0f );
		vertex[0].v2.Tex[0] = Math::Vector2D( 1.0f , 0.5f );
		vertex[1].v2.Tex[0] = Math::Vector2D( 0.5f , 1.0f );

		target_size_x = m_pUnderTex->GetSize().x;
		target_size_y = m_pUnderTex->GetSize().y;

		m_pUnderPrimitive->Begin();
		for( i=0 ; i<target_size_y ; i+= size.y/2 )
		{
			for( j=0 ; j<target_size_x ; j += size.x/2 )
			{
				vertex[0].v1.Pos = vertex[1].v3.Pos = Math::Vector4D( j , i , 0 , 1.0f );
				vertex[1].v1.Pos = vertex[0].v3.Pos = Math::Vector4D( j+size.x*0.5f , i+size.y*0.5f , 0 , 1.0f );
				vertex[0].v2.Pos = Math::Vector4D( j+size.x*0.5f , i , 0 , 1.0f );
				vertex[1].v2.Pos = Math::Vector4D( j , i+size.y*0.5f , 0 , 1.0f );
				
				m_pUnderPrimitive->Push( vertex , 2 );
			}
		}
		m_pUnderPrimitive->End();
		set.pPrimitive = m_pUnderPrimitive;
		set.pTargetTexture = m_pUnderTex;
		set.setTexture = m_pBackTex;
		GameSystem::pPrimitive2D.push_back( set );

		// 真ん中
		vertex[0].v1.Tex[1] = vertex[1].v3.Tex[1] = vertex[0].v1.Tex[0] = vertex[1].v3.Tex[0] = Math::Vector2D( 0.0f , 0.5f );
		vertex[1].v1.Tex[1] = vertex[0].v3.Tex[1] = vertex[1].v1.Tex[0] = vertex[0].v3.Tex[0] = Math::Vector2D( 0.5f , 1.0f );
		vertex[0].v2.Tex[1] = vertex[0].v2.Tex[0] = Math::Vector2D( 0.5f , 0.5f );
		vertex[1].v2.Tex[1] = vertex[1].v2.Tex[0] = Math::Vector2D( 0.0f , 1.0f );

		target_size_x = m_pCenterTex->GetSize().x;

		m_pCenterPrimitive->Begin();
		for( j=0 ; j<target_size_x ; j += size.x/2 )
		{
			vertex[0].v1.Pos = vertex[1].v3.Pos = Math::Vector4D( j , 0 , 0 , 1.0f );
			vertex[1].v1.Pos = vertex[0].v3.Pos = Math::Vector4D( j+size.x*0.5f , size.y*0.5f , 0 , 1.0f );
			vertex[0].v2.Pos = Math::Vector4D( j+size.x*0.5f , 0 , 0 , 1.0f );
			vertex[1].v2.Pos = Math::Vector4D( j , size.y*0.5f , 0 , 1.0f );
			
			m_pCenterPrimitive->Push( vertex , 2 );
		}

		m_pCenterPrimitive->End();
		set.pPrimitive = m_pCenterPrimitive;
		set.pTargetTexture = m_pCenterTex;
		set.setTexture = m_pBackTex;
		GameSystem::pPrimitive2D.push_back( set );

		m_is_first = false;
	}
	else
	{
		GameSystem::draw_task->DeleteTask( m_id );
	}
}
