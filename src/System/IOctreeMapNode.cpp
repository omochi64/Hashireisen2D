
#include "IOctreeMapNode.h"

/*

// 八分木の内部ノード
// コイツはマップ生成ツールにも使う
// となると、動的にツリーを変更する操作も入れたい。コイツを継承して作る？
	std::vector<MAP_OBJECT_DATA>	m_mapObjects;
	int				m_objectNum;
	IOctreeMapNode	*m_childrenNode[8];	// 子ノード。必ず8つ以下となる
	Collision::CBox	m_spaceArea;

	// 可視判定
	//	可視判定についてはオライリーの本をもうちょっと詳しく読んでおく
	bool CheckVisible( Scene::ISceneManager *pSceneMgr );

	// 当たり判定
	// 当たった順番でoutに追加されていく
//	bool HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out );

	// ↓こいつら(Octree空間に所属する/しないのチェック関数)は、CheckCollisionあたりを使えば作れるかな？

	// 渡されたボックスがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
//	bool IsBoxtInThisSpacePerfectly( const Collision::CBox &box );
	// 渡されたボックスがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
//	bool IsBoxtInThisSpacePartly( const Collision::CBox &box );
	// 渡されたオブジェクトがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
//	bool IsObjectInThisSpacePerfectly( const IModelActor *pModel );
	// 渡されたオブジェクトがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
//	bool IsObjectInThisSpacePartly( const IModelActor *pModel );
	// 渡されたレイがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
//	bool IsRayInThisSpacePerfectly( const Collision::CLine3D &ray );
	// 渡されたレイがこの空間エリアに入っているかどうかをチェック(一部分だけ入っているか)
//	bool IsRayInThisSpacePartly( const Collision::CLine3D &ray );


	// 可視判定領域の描画
	void DrawVisibleArea( Scene::ISceneManager *pSceneMgr );
	
};

*/

// コンストラクタ
IOctreeMapNode::IOctreeMapNode():m_spaceArea()
{
	m_mapObjects.clear();
	m_objectNum = 0;
	memset( m_childrenNode , 0 , sizeof(IOctreeMapNode*)*8 );
	m_spaceArea.CreateBox( Math::Vector3D() , Math::Vector3D() , Math::Matrix() );
	m_divided_x = m_divided_y = m_divided_z = false;
}

// デスクトラクタ
IOctreeMapNode::~IOctreeMapNode()
{
	Release();
}


// 解放処理
void IOctreeMapNode::Release()
{
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			SAFE_RELEASE(m_childrenNode[i]);
		}
	}

	for( i=0 ; i<m_objectNum ; i++ )
	{
		IMapObjectData *data = m_mapObjects.at(i);
		SAFE_RELEASE(data);
	}
	m_mapObjects.clear();
	m_objectNum = 0;
}

// 八分木を作成
void IOctreeMapNode::CreateTree(int depth_x , int depth_y , int depth_z, const Math::Vector3D &minXYZ, const Math::Vector3D &maxXYZ)
{
	// 空間を設定
	Math::Matrix iniMat;
	iniMat.Identity();
	m_spaceArea.CreateBox( minXYZ , maxXYZ , iniMat );

	// depth==0になったら生成終了
	if( depth_x <= 0 && depth_y <= 0 && depth_z <= 0 )
	{
		return;
	}

	int divide_space_num = 1;
	m_divided_x=false,m_divided_y=false,m_divided_z=false;
	if( depth_x > 0 )
	{
		divide_space_num *= 2;
		m_divided_x = true;
	}
	if( depth_y > 0 )
	{
		divide_space_num *= 2;
		m_divided_y = true;
	}
	if( depth_z > 0 )
	{
		divide_space_num *= 2;
		m_divided_z = true;
	}

	// 空間分割
	Math::Vector3D minPoints[8],maxPoints[8];
	IOctreeMapNode::DivideBoxSpace( minXYZ , maxXYZ , minPoints , maxPoints , m_divided_x , m_divided_y , m_divided_z );

	// 再帰処理
	int i;
	for( i=0 ; i<divide_space_num ; i++ )
	{
		m_childrenNode[i] = NewThisClass();
		m_childrenNode[i]->CreateTree( depth_x-1 , depth_y-1 , depth_z-1 , minPoints[i] , maxPoints[i] );
	}
	for( ; i<8 ; i++ )
	{
		m_childrenNode[i] = NULL;
	}
}

// オブジェクトを追加
// add_node_numberは、どのノードにこのデータを追加するかを表す
//	add_node_number = {1,2,2,0}
// だったら、3段目(ルートは0段目)の2番目のノードに追加する
bool IOctreeMapNode::AddObject( IMapObjectData *add_data , int *add_node_number )
{
	// add_node_numberが0ならこのノードに追加
	if( add_node_number[0] == 0 )
	{
		m_mapObjects.push_back( add_data );
		return true;
	}
	else if( add_node_number[0] < 0 || add_node_number[0] > 7 )
	{
		// 有効でないノード番号
		return false;
	}
	else
	{
		// 子供に追加
		if( m_childrenNode[add_node_number[0]] )
		{
			return m_childrenNode[add_node_number[0]]->AddObject( add_data , ++add_node_number );
		}
		else
		{
			// Compactした後に、元々分解能の範囲内だった部分に追加しようとするとダメ
			// つまり、後に動的に変わるマップや、敵の管理にこれを使う場合はちょっと変える必要がある(別クラスとして継承する？)。
			return false;
		}
	}
}


// ちょっと困ったこと発生・・・
//  この(↓)AddObjectの方式だと、空間内にすっぽり収まらないけどマップに入っているもの(マップの端から半身だけ外に出てるとか)
//  が処理できない。どうしよう

// オブジェクトを追加
//  どこに割り当てられるかは自動的に判別
bool IOctreeMapNode::AddObject( IMapObjectData *add_data )
{
	// 子に追加できるか？
	bool result = false;
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] && m_childrenNode[i]->AddObject( add_data ) )
		{
			result = true;
			break;
		}
	}

	// 子に追加できた。そのまま終了
	if( result )
	{
		return true;
	}

	// 追加できなかった。この空間にすっぽり収まるようならここに追加して終了
//	add_data->m_pActor->TransformUpdateCollisionOnly();
	if( IsObjectInThisSpacePerfectly( add_data->m_pActor ) )
	{
		m_mapObjects.push_back( add_data );
		m_objectNum++;
		return true;
	}
	else
	{
		return false;
	}
}

// オブジェクトの削除
//  ノード指定型
bool IOctreeMapNode::DeleteObject( IMapObjectData *delete_data , int *delete_node_number , bool will_delete_data )
{
	// delete_node_numberが0ならこのノードから削除
	if( delete_node_number[0] == 0 )
	{
		int i,size = m_mapObjects.size();
		for( i=0 ; i<size ; i++ )
		{
			IMapObjectData *del = m_mapObjects.at(i);
			if( delete_data->Compare( del ) )
			{
				// object自体のdeleteはしない？
				if( will_delete_data )
				{
					SAFE_RELEASE(del);
				}
				m_mapObjects.erase( m_mapObjects.begin()+i );
				m_objectNum--;
				return true;
			}
		}
		return false;
	}
	else if( delete_node_number[0] < 0 || delete_node_number[0] > 7 )
	{
		// 有効でないノード番号
		return false;
	}
	else
	{
		// 子供から削除
		if( m_childrenNode[delete_node_number[0]] )
		{
			return m_childrenNode[delete_node_number[0]]->DeleteObject( delete_data , ++delete_node_number , will_delete_data );
		}
		else
		{
			return false;
		}
	}
}

//  自動検索型
//		-親ノード優先
bool IOctreeMapNode::DeleteObjectPrimirilyParent( IMapObjectData *delete_data , bool will_delete_data )
{
	// このノードから検索
	int i,size = m_mapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		IMapObjectData *del = m_mapObjects.at(i);
		if( delete_data->Compare( del ) )
		{
			if( will_delete_data )
			{
				SAFE_RELEASE(del);
			}

			m_mapObjects.erase( m_mapObjects.begin()+i );
			m_objectNum--;
			return true;
		}
	}

	// 子から削除
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			if( m_childrenNode[i]->DeleteObjectPrimirilyParent( delete_data , will_delete_data ) )
			{
				return true;
			}
		}
	}

	return false;
}

//	自動検索型
//		-子ノード優先
bool IOctreeMapNode::DeleteObjectPrimirilyChild( IMapObjectData *delete_data , bool will_delete_data )
{
	// 子から削除
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			if( m_childrenNode[i]->DeleteObjectPrimirilyChild( delete_data , will_delete_data ) )
			{
				return true;
			}
		}
	}

	int size = m_mapObjects.size();
	// このノードから検索
	for( i=0 ; i<size ; i++ )
	{
		IMapObjectData *del = m_mapObjects.at(i);
		if( delete_data->Compare( del ) )
		{
			if( will_delete_data )
			{
				SAFE_RELEASE(del);
			}

			m_mapObjects.erase( m_mapObjects.begin()+i );
			m_objectNum--;
			return true;
		}
	}

	return false;
}


// Octreeをコンパクトにする。オブジェクト数が0なノードは破棄
void IOctreeMapNode::Compact( void )
{
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			// 子供を先に行う
			m_childrenNode[i]->Compact();

			// この子ノードのオブジェクト数は0？
			if( m_childrenNode[i]->GetObjectNum() == 0 )
			{
				SAFE_RELEASE(m_childrenNode[i]);
			}
		}
	}
}

// min,maxの取得
const Math::Vector3D &IOctreeMapNode::GetMinXYZ()
{
	return m_spaceArea.vMin;
}

const Math::Vector3D &IOctreeMapNode::GetMaxXYZ()
{
	return m_spaceArea.vMax;
}

// 当たり判定
// 当たった順番でoutに追加されていく
bool IOctreeMapNode::HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out , std::vector<IMapObjectData *> &out_object )
{
	// レイがこの空間内に入っている？
	if( Collision::Line_Box_3D( ray , m_spaceArea ) )
	{
		// 入っていた。とりあえずこの空間内にあるオブジェクトと判定チェック
		int i;
		Renderer::SCollisionResult result;
		bool ret_v = false;
		for( i=0 ; i<m_objectNum ; i++ )
		{
			IMapObjectData *data = m_mapObjects.at(i);
			data->m_pActor->TransformUpdateCollisionOnly();
			if( data->m_pActor->HitCheckByRay( ray , result ) )
			{
				out.push_back( result );
				out_object.push_back( data );
				ret_v = true;
			}
		}

		// 子供に対してもチェックする
		for( i=0 ; i<8 ; i++ )
		{
			if( m_childrenNode[i] )
			{
				if( m_childrenNode[i]->HitCheckByRay( ray , out , out_object ) && !ret_v )
				{
					ret_v = true;
				}
			}
		}

		return ret_v;
	}
	else
	{
		// 入っていなかった
		return false;
	}
}

// 渡されたレイがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
bool IOctreeMapNode::IsRayInThisSpacePerfectly( const Collision::CLine3D &ray )
{
	// rayの始点と終点両方が空間内にあればよい
	if( ray.vStart.x >= m_spaceArea.vMin.x && ray.vStart.x <= m_spaceArea.vMax.x &&
		ray.vStart.y >= m_spaceArea.vMin.y && ray.vStart.y <= m_spaceArea.vMax.y &&
		ray.vStart.z >= m_spaceArea.vMin.z && ray.vStart.z <= m_spaceArea.vMax.z &&
		ray.vEnd.x >= m_spaceArea.vMin.x && ray.vEnd.x <= m_spaceArea.vMax.x &&
		ray.vEnd.y >= m_spaceArea.vMin.y && ray.vEnd.y <= m_spaceArea.vMax.y &&
		ray.vEnd.z >= m_spaceArea.vMin.z && ray.vEnd.z <= m_spaceArea.vMax.z )
	{
		return true;
	}

	return false;
}

// 渡されたレイがこの空間エリアに入っているかどうかをチェック(一部分だけ入っているか)
bool IOctreeMapNode::IsRayInThisSpacePartly( const Collision::CLine3D &ray )
{
	// rayとこの空間を示すCBoxがCollisionしていれば一部分だけが入っている
	if( Collision_Area_Ray( ray ) )
	{
		return true;
	}

	return false;
}

// 渡されたボックスがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
bool IOctreeMapNode::IsBoxtInThisSpacePerfectly( const Collision::CBox &box )
{
	// 空間エリアに当たっていない && 一点が空間エリアに入っている
	if( !Collision_Area_Box( box ) &&
		box.Points[0].x >= m_spaceArea.vMin.x && box.Points[0].x <= m_spaceArea.vMax.x &&
		box.Points[0].y >= m_spaceArea.vMin.y && box.Points[0].y <= m_spaceArea.vMax.y &&
		box.Points[0].z >= m_spaceArea.vMin.z && box.Points[0].z <= m_spaceArea.vMax.z )
	{
		return true;
	}

	return false;
}

// 渡されたボックスがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
bool IOctreeMapNode::IsBoxtInThisSpacePartly( const Collision::CBox &box )
{
	// 渡されたボックスがこのエリアのボックスとCollisionしていたらtrue
	if( Collision_Area_Box( box ) )
	{
		return true;
	}
	return false;
}

// 渡されたオブジェクトがこの空間エリアに入っているかどうかをチェック(完璧にすっぽり埋まるか)
bool IOctreeMapNode::IsObjectInThisSpacePerfectly( Scene::IModelActor *pModel )
{
	if( !Collision_Area_Model( pModel ) )
	{
		Math::Vector3D pos;
		pModel->GetPosition(pos);
	
		if( pos.x >= m_spaceArea.vMin.x && pos.x <= m_spaceArea.vMax.x &&
			pos.y >= m_spaceArea.vMin.y && pos.y <= m_spaceArea.vMax.y &&
			pos.z >= m_spaceArea.vMin.z && pos.z <= m_spaceArea.vMax.z )
		{
			return true;
		}
	}
	return false;
}

// 渡されたオブジェクトがこの空間エリアに入っているかどうかをチェック(一部分でも入っているか)
bool IOctreeMapNode::IsObjectInThisSpacePartly( Scene::IModelActor *pModel )
{
	if( Collision_Area_Model( pModel ) )
	{
		return true;
	}
	return false;
}

// エリアボックスとの当たり判定
// モデルとの判定
bool IOctreeMapNode::Collision_Area_Model( Scene::IModelActor *pModel )
{
	// 6つの平面に対して当たり判定
	const Math::Vector3D & vMin = m_spaceArea.vMin;
	const Math::Vector3D & vMax = m_spaceArea.vMax;

	Collision::CBox box[6];
	Math::Matrix identity;
	identity.Identity();
	// 当たり判定がなぞ…
	//  SeleneのソースとOBBの当たり判定についての解説を読んで何がどうなっているのか把握したほうがよさそう
	//   分かった…Seleneのサンプルを見る限り、平行移動は行列で指定しないとダメっぽい。つまり、vMinとvMaxでは中央が(0,0,0)似来ないとダメかもしれない
	//	 って本当にそうなのか…？
	// pModel->TransformUpdateCollisionOnlyでうまくいった…
//	identity.Translation( vMax.x-vMin.x , 
	box[0].CreateBox( Math::Vector3D(vMin.x-0.001f,vMax.y,vMax.z) , vMin , identity );
	box[1].CreateBox( Math::Vector3D(vMax.x,vMax.y,vMin.z-0.001f) , vMin , identity );
	box[2].CreateBox( Math::Vector3D(vMax.x,vMin.y-0.001f,vMax.z) , vMin , identity );
	box[3].CreateBox( vMax , Math::Vector3D(vMin.x,vMax.y+0.001f,vMin.z) , identity );
	box[4].CreateBox( vMax , Math::Vector3D(vMin.x,vMin.y,vMax.z+0.001f) , identity );
	box[5].CreateBox( vMax , Math::Vector3D(vMax.x+0.001f,vMin.y,vMin.z) , identity );
//		box[0].CreateBox( vMin+Math::Vector3D(1,1,1) , vMax , identity );

//		pModel->

	int i;
	for( i=0 ; i<6 ; i++ )
	{
		if( pModel->Collision_Check( box[i] ) )
		{
			return true;
		}
	}
	return false;
}

// ボックスとの判定
bool IOctreeMapNode::Collision_Area_Box( const Collision::CBox &box )
{
	// 6つの平面に対して当たり判定
	const Math::Vector3D & vMin = m_spaceArea.vMin;
	const Math::Vector3D & vMax = m_spaceArea.vMax;

	Collision::CBox box_area[6];
	Math::Matrix identity;
	identity.Identity();
	box_area[0].CreateBox( Math::Vector3D(vMin.x,vMax.y,vMax.z) , vMin , identity );
	box_area[1].CreateBox( Math::Vector3D(vMax.x,vMax.y,vMin.z) , vMin , identity );
	box_area[2].CreateBox( Math::Vector3D(vMax.x,vMin.y,vMax.z) , vMin , identity );
	box_area[3].CreateBox( vMax , Math::Vector3D(vMin.x,vMax.y,vMin.z) , identity );
	box_area[4].CreateBox( vMax , Math::Vector3D(vMin.x,vMin.y,vMax.z) , identity );
	box_area[5].CreateBox( vMax , Math::Vector3D(vMax.x,vMin.y,vMin.z) , identity );

	int i;
	for( i=0 ; i<6 ; i++ )
	{
		if( Collision::Box_Box_3D( box , box_area[i] ) )
		{
			return true;
		}
	}
	return false;
}

// レイとの判定
bool IOctreeMapNode::Collision_Area_Ray( const Collision::CLine3D &ray )
{
	// 6つの平面に対して当たり判定
	const Math::Vector3D & vMin = m_spaceArea.vMin;
	const Math::Vector3D & vMax = m_spaceArea.vMax;

	Collision::CBox box[6];
	Math::Matrix identity;
	identity.Identity();
	box[0].CreateBox( Math::Vector3D(vMin.x,vMax.y,vMax.z) , vMin , identity );
	box[1].CreateBox( Math::Vector3D(vMax.x,vMax.y,vMin.z) , vMin , identity );
	box[2].CreateBox( Math::Vector3D(vMax.x,vMin.y,vMax.z) , vMin , identity );
	box[3].CreateBox( vMax , Math::Vector3D(vMin.x,vMax.y,vMin.z) , identity );
	box[4].CreateBox( vMax , Math::Vector3D(vMin.x,vMin.y,vMax.z) , identity );
	box[5].CreateBox( vMax , Math::Vector3D(vMax.x,vMin.y,vMin.z) , identity );

	int i;
	for( i=0 ; i<6 ; i++ )
	{
		if( Collision::Line_Box_3D( ray , box[i] ) )
		{
			return true;
		}
	}
	return false;
}


// 可視判定
bool IOctreeMapNode::CheckVisible( Scene::ISceneManager *pSceneMgr , int window_width , int window_height )
{
	Math::Vector3D transformed;
	bool left=false,right=false,top=false,bottom=false;

	int i;
	for( i=0 ; i<8 ; i++ )
	{
		transformed = pSceneMgr->TransformToScreen( m_spaceArea.Points[i] );
		if( transformed.x >= 0 && transformed.x <= window_width &&
			transformed.y >= 0 && transformed.y <= window_height &&
			transformed.z >= 0 && transformed.z <= 1.0f )
		{
			return true;
		}
		else
		{
			if( transformed.x < 0 )
			{
				left = true;
			}
			else if( transformed.x > window_width )
			{
				right = true;
			}
			if( transformed.y < 0 )
			{
				top = true;
			}
			else if( transformed.y > window_height )
			{
				bottom = true;
			}
		}
	}
	if( left && right && top && bottom )
	{
		return true;
	}

	return false;
}

// 可視判定領域の描画
void IOctreeMapNode::DrawVisibleArea( Scene::ISceneManager *pSceneMgr , int window_width , int window_height )
{
	int i;
	Math::Vector3D pos,rotate,scale;

	// このエリアが可視であれば描画
	if( CheckVisible( pSceneMgr , window_width , window_height ) )
	{
		for( i=0 ; i<m_objectNum ; i++ )
		{
			// このオブジェクト自体は可視判定を行う？
			IMapObjectData *model = m_mapObjects.at(i);
			model->m_pActor->TransformUpdate();
			model->m_pActor->RenderingRequest();
		}
		// 子を描画
		for( i=0 ; i<8 ; i++ )
		{
			if( m_childrenNode[i] )
			{
				m_childrenNode[i]->DrawVisibleArea( pSceneMgr , window_width , window_height );
			}
		}
	}
	
}

// 領域自体の描画
void IOctreeMapNode::DrawOctreeArea( Renderer::Object::ILine3D *pLine , Scene::ISceneManager *pSceneMgr , int window_width , int window_height )
{
	if( CheckVisible( pSceneMgr , window_width , window_height ) )
	{
		// この領域のBoxを描画
		pLine->PushBox( m_spaceArea , CColor(0xff,0xff,0xff,0xff/2) );
		int i;
		const Math::Vector3D & vMin = m_spaceArea.vMin;
		const Math::Vector3D & vMax = m_spaceArea.vMax;
/*		Collision::CBox box[6];
		Math::Matrix identity;
		identity.Identity();
		box[0].CreateBox( Math::Vector3D(vMin.x,vMax.y,vMax.z) , vMin , identity );
		box[1].CreateBox( Math::Vector3D(vMax.x,vMax.y,vMin.z) , vMin , identity );
		box[2].CreateBox( Math::Vector3D(vMax.x,vMin.y,vMax.z) , vMin , identity );
		box[3].CreateBox( vMax , Math::Vector3D(vMin.x,vMax.y,vMin.z) , identity );
		box[4].CreateBox( vMax , Math::Vector3D(vMin.x,vMin.y,vMax.z) , identity );
		box[5].CreateBox( vMax , Math::Vector3D(vMax.x,vMin.y,vMin.z) , identity );

		for( i=0 ; i<6 ; i++ )
		{
			pLine->PushBox( box[i] , CColor(0xff,0xff,0x00) );
		}
		for( i=0 ; i<m_mapObjects.size() ; i++ )
		{
			m_mapObjects.at(i)->m_pActor->CreateCollisionDrawPrimitive( pLine );
		}*/
		// 子を描画
		for( i=0 ; i<8 ; i++ )
		{
			if( m_childrenNode[i] )
			{
				m_childrenNode[i]->DrawOctreeArea( pLine , pSceneMgr , window_width , window_height );
			}
		}
	}
}

// NewThisClass
IOctreeMapNode *IOctreeMapNode::NewThisClass()
{
	return new IOctreeMapNode();
}

// 空間分割を行う
void IOctreeMapNode::DivideBoxSpace( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ ,
		Math::Vector3D minPoints[] , Math::Vector3D maxPoints[] , bool is_divide_x , bool is_divide_y , bool is_divide_z )
{
	int i;
	Math::Vector3D baseMin,baseMax,baseBoxSize;

	// 全部考え直す必要あり
	// (0,0,0)が中心にあることを前提として分割している。これはまずい

	// 基本位置(=0番目の空間)
	baseMin = minXYZ;
	baseBoxSize = (maxXYZ-minXYZ)/2.0f;

	for( i=0 ; i<8 ; i++ )
	{
		minPoints[i] = baseMin;
		maxPoints[i] = baseMin+baseBoxSize;
	}

	// 8パターン網羅する

	// 全分割
	if( is_divide_x && is_divide_y && is_divide_z )
	{
		minPoints[3].x = (minPoints[1].x += baseBoxSize.x);
		maxPoints[3].x = (maxPoints[1].x += baseBoxSize.x);

		minPoints[3].z = (minPoints[2].z += baseBoxSize.z);
		maxPoints[3].z = (maxPoints[2].z += baseBoxSize.z);

		for( i=4 ; i<8 ; i++ )
		{
			minPoints[i] = minPoints[i-4];
			maxPoints[i] = maxPoints[i-4];
			minPoints[i].y += baseBoxSize.y;
			maxPoints[i].y += baseBoxSize.y;
		}

//		minPoints[7].x = (minPoints[5].x += baseBoxSize.x);
//		maxPoints[7].x = (maxPoints[5].x += baseBoxSize.x);

//		minPoints[7].z = (minPoints[6].z += baseBoxSize.z);
//		maxPoints[7].z = (maxPoints[6].z += baseBoxSize.z);
	}
	// Zのみ分割しない
	if( is_divide_x && is_divide_y && !is_divide_z )
	{
		for( i=0 ; i<4 ; i++ )
		{
			maxPoints[i].z += baseBoxSize.z;
		}
		minPoints[3].x = (minPoints[1].x += baseBoxSize.x);
		maxPoints[3].x = (maxPoints[1].x += baseBoxSize.x);
		minPoints[3].y = (minPoints[2].y += baseBoxSize.y);
		maxPoints[3].y = (maxPoints[2].y += baseBoxSize.y);
	}
	// Yのみ分割しない
	if( is_divide_x && !is_divide_y && is_divide_z )
	{
		for( i=0 ; i<4 ; i++ )
		{
			maxPoints[i].y += baseBoxSize.y;
		}
		minPoints[3].x = (minPoints[1].x += baseBoxSize.x);
		maxPoints[3].x = (maxPoints[1].x += baseBoxSize.x);

		minPoints[3].z = (minPoints[2].z += baseBoxSize.z);
		maxPoints[3].z = (maxPoints[2].z += baseBoxSize.z);
		
	}
	// Xのみ分割しない
	if( !is_divide_x && is_divide_y && is_divide_z )
	{
		for( i=0 ; i<4 ; i++ )
		{
			maxPoints[i].x += baseBoxSize.x;
		}

		minPoints[3].z = (minPoints[1].z += baseBoxSize.z);
		maxPoints[3].z = (maxPoints[1].z += baseBoxSize.z);
		minPoints[3].y = (minPoints[2].y += baseBoxSize.y);
		maxPoints[3].y = (maxPoints[2].y += baseBoxSize.y);
	}
	// Xのみ分割
	if( is_divide_x && !is_divide_y && !is_divide_z )
	{
		maxPoints[0].y += baseBoxSize.y;
		maxPoints[0].z += baseBoxSize.z;
		minPoints[1].x += baseBoxSize.x;
		maxPoints[1] += baseBoxSize;
	}
	
	// Yのみ分割
	if( !is_divide_x && is_divide_y && !is_divide_z )
	{
		maxPoints[0].x += baseBoxSize.x;
		maxPoints[0].z += baseBoxSize.z;
		minPoints[1].y += baseBoxSize.y;
		maxPoints[1] += baseBoxSize;
	}
	// zのみ分割
	if( !is_divide_x && !is_divide_y && is_divide_z )
	{
		maxPoints[0].y += baseBoxSize.y;
		maxPoints[0].x += baseBoxSize.x;
		minPoints[1].z += baseBoxSize.z;
		maxPoints[1] += baseBoxSize;
	}
	// 分割なし
	if( !is_divide_x && !is_divide_y && !is_divide_z )
	{
		maxPoints[0] = maxXYZ;
	}

}
