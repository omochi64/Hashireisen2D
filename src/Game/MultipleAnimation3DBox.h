#pragma once

#include "Game/Objects.h"

class MultipleAnimation3DBox:public MultipleAnimation{
protected:

	float	m_depth;

	Renderer::SVertex3DBase	m_vertex[24];
	Renderer::SVertex3DTexture	m_tex_vertex[24];
	// テクスチャUV座標をどのように扱うべきか…

public:

	// コンストラクタ
	MultipleAnimation3DBox();
	MultipleAnimation3DBox( Math::Vector3D center_pos , float width , float height , float depth ,
		Math::Vector3D speed=Math::Vector3D(0,0,0) , Math::Vector3D accel=Math::Vector3D(0,0,0) );

	// 頂点アップデート
	virtual void UpdateVertex();
	virtual void SetVertexColor( CColor color );

	virtual DrawableObject* Clone( void );

	virtual bool CheckVisible( Scene::ISceneManager *pSceneMgr , int screen_width , int screen_height , DWORD now_time );

	virtual void SetPolygonSize( float width , float height );
	virtual void SetUV( Math::Vector2D & lefttop , Math::Vector2D & rightbottom , bool update_vertex_data );
};
