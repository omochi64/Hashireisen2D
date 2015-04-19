
#include "IOctreeMapNode.h"

/*

// �����؂̓����m�[�h
// �R�C�c�̓}�b�v�����c�[���ɂ��g��
// �ƂȂ�ƁA���I�Ƀc���[��ύX���鑀������ꂽ���B�R�C�c���p�����č��H
	std::vector<MAP_OBJECT_DATA>	m_mapObjects;
	int				m_objectNum;
	IOctreeMapNode	*m_childrenNode[8];	// �q�m�[�h�B�K��8�ȉ��ƂȂ�
	Collision::CBox	m_spaceArea;

	// ������
	//	������ɂ��Ă̓I���C���[�̖{������������Əڂ����ǂ�ł���
	bool CheckVisible( Scene::ISceneManager *pSceneMgr );

	// �����蔻��
	// �����������Ԃ�out�ɒǉ�����Ă���
//	bool HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out );

	// ��������(Octree��Ԃɏ�������/���Ȃ��̃`�F�b�N�֐�)�́ACheckCollision��������g���΍��邩�ȁH

	// �n���ꂽ�{�b�N�X�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
//	bool IsBoxtInThisSpacePerfectly( const Collision::CBox &box );
	// �n���ꂽ�{�b�N�X�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
//	bool IsBoxtInThisSpacePartly( const Collision::CBox &box );
	// �n���ꂽ�I�u�W�F�N�g�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
//	bool IsObjectInThisSpacePerfectly( const IModelActor *pModel );
	// �n���ꂽ�I�u�W�F�N�g�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
//	bool IsObjectInThisSpacePartly( const IModelActor *pModel );
	// �n���ꂽ���C�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
//	bool IsRayInThisSpacePerfectly( const Collision::CLine3D &ray );
	// �n���ꂽ���C�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ�����������Ă��邩)
//	bool IsRayInThisSpacePartly( const Collision::CLine3D &ray );


	// ������̈�̕`��
	void DrawVisibleArea( Scene::ISceneManager *pSceneMgr );
	
};

*/

// �R���X�g���N�^
IOctreeMapNode::IOctreeMapNode():m_spaceArea()
{
	m_mapObjects.clear();
	m_objectNum = 0;
	memset( m_childrenNode , 0 , sizeof(IOctreeMapNode*)*8 );
	m_spaceArea.CreateBox( Math::Vector3D() , Math::Vector3D() , Math::Matrix() );
	m_divided_x = m_divided_y = m_divided_z = false;
}

// �f�X�N�g���N�^
IOctreeMapNode::~IOctreeMapNode()
{
	Release();
}


// �������
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

// �����؂��쐬
void IOctreeMapNode::CreateTree(int depth_x , int depth_y , int depth_z, const Math::Vector3D &minXYZ, const Math::Vector3D &maxXYZ)
{
	// ��Ԃ�ݒ�
	Math::Matrix iniMat;
	iniMat.Identity();
	m_spaceArea.CreateBox( minXYZ , maxXYZ , iniMat );

	// depth==0�ɂȂ����琶���I��
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

	// ��ԕ���
	Math::Vector3D minPoints[8],maxPoints[8];
	IOctreeMapNode::DivideBoxSpace( minXYZ , maxXYZ , minPoints , maxPoints , m_divided_x , m_divided_y , m_divided_z );

	// �ċA����
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

// �I�u�W�F�N�g��ǉ�
// add_node_number�́A�ǂ̃m�[�h�ɂ��̃f�[�^��ǉ����邩��\��
//	add_node_number = {1,2,2,0}
// ��������A3�i��(���[�g��0�i��)��2�Ԗڂ̃m�[�h�ɒǉ�����
bool IOctreeMapNode::AddObject( IMapObjectData *add_data , int *add_node_number )
{
	// add_node_number��0�Ȃ炱�̃m�[�h�ɒǉ�
	if( add_node_number[0] == 0 )
	{
		m_mapObjects.push_back( add_data );
		return true;
	}
	else if( add_node_number[0] < 0 || add_node_number[0] > 7 )
	{
		// �L���łȂ��m�[�h�ԍ�
		return false;
	}
	else
	{
		// �q���ɒǉ�
		if( m_childrenNode[add_node_number[0]] )
		{
			return m_childrenNode[add_node_number[0]]->AddObject( add_data , ++add_node_number );
		}
		else
		{
			// Compact������ɁA���X����\�͈͓̔������������ɒǉ����悤�Ƃ���ƃ_��
			// �܂�A��ɓ��I�ɕς��}�b�v��A�G�̊Ǘ��ɂ�����g���ꍇ�͂�����ƕς���K�v������(�ʃN���X�Ƃ��Čp������H)�B
			return false;
		}
	}
}


// ������ƍ��������Ɣ����E�E�E
//  ����(��)AddObject�̕������ƁA��ԓ��ɂ����ۂ���܂�Ȃ����ǃ}�b�v�ɓ����Ă������(�}�b�v�̒[���甼�g�����O�ɏo�Ă�Ƃ�)
//  �������ł��Ȃ��B�ǂ����悤

// �I�u�W�F�N�g��ǉ�
//  �ǂ��Ɋ��蓖�Ă��邩�͎����I�ɔ���
bool IOctreeMapNode::AddObject( IMapObjectData *add_data )
{
	// �q�ɒǉ��ł��邩�H
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

	// �q�ɒǉ��ł����B���̂܂܏I��
	if( result )
	{
		return true;
	}

	// �ǉ��ł��Ȃ������B���̋�Ԃɂ����ۂ���܂�悤�Ȃ炱���ɒǉ����ďI��
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

// �I�u�W�F�N�g�̍폜
//  �m�[�h�w��^
bool IOctreeMapNode::DeleteObject( IMapObjectData *delete_data , int *delete_node_number , bool will_delete_data )
{
	// delete_node_number��0�Ȃ炱�̃m�[�h����폜
	if( delete_node_number[0] == 0 )
	{
		int i,size = m_mapObjects.size();
		for( i=0 ; i<size ; i++ )
		{
			IMapObjectData *del = m_mapObjects.at(i);
			if( delete_data->Compare( del ) )
			{
				// object���̂�delete�͂��Ȃ��H
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
		// �L���łȂ��m�[�h�ԍ�
		return false;
	}
	else
	{
		// �q������폜
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

//  ���������^
//		-�e�m�[�h�D��
bool IOctreeMapNode::DeleteObjectPrimirilyParent( IMapObjectData *delete_data , bool will_delete_data )
{
	// ���̃m�[�h���猟��
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

	// �q����폜
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

//	���������^
//		-�q�m�[�h�D��
bool IOctreeMapNode::DeleteObjectPrimirilyChild( IMapObjectData *delete_data , bool will_delete_data )
{
	// �q����폜
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
	// ���̃m�[�h���猟��
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


// Octree���R���p�N�g�ɂ���B�I�u�W�F�N�g����0�ȃm�[�h�͔j��
void IOctreeMapNode::Compact( void )
{
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			// �q�����ɍs��
			m_childrenNode[i]->Compact();

			// ���̎q�m�[�h�̃I�u�W�F�N�g����0�H
			if( m_childrenNode[i]->GetObjectNum() == 0 )
			{
				SAFE_RELEASE(m_childrenNode[i]);
			}
		}
	}
}

// min,max�̎擾
const Math::Vector3D &IOctreeMapNode::GetMinXYZ()
{
	return m_spaceArea.vMin;
}

const Math::Vector3D &IOctreeMapNode::GetMaxXYZ()
{
	return m_spaceArea.vMax;
}

// �����蔻��
// �����������Ԃ�out�ɒǉ�����Ă���
bool IOctreeMapNode::HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out , std::vector<IMapObjectData *> &out_object )
{
	// ���C�����̋�ԓ��ɓ����Ă���H
	if( Collision::Line_Box_3D( ray , m_spaceArea ) )
	{
		// �����Ă����B�Ƃ肠�������̋�ԓ��ɂ���I�u�W�F�N�g�Ɣ���`�F�b�N
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

		// �q���ɑ΂��Ă��`�F�b�N����
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
		// �����Ă��Ȃ�����
		return false;
	}
}

// �n���ꂽ���C�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
bool IOctreeMapNode::IsRayInThisSpacePerfectly( const Collision::CLine3D &ray )
{
	// ray�̎n�_�ƏI�_��������ԓ��ɂ���΂悢
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

// �n���ꂽ���C�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ�����������Ă��邩)
bool IOctreeMapNode::IsRayInThisSpacePartly( const Collision::CLine3D &ray )
{
	// ray�Ƃ��̋�Ԃ�����CBox��Collision���Ă���Έꕔ�������������Ă���
	if( Collision_Area_Ray( ray ) )
	{
		return true;
	}

	return false;
}

// �n���ꂽ�{�b�N�X�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
bool IOctreeMapNode::IsBoxtInThisSpacePerfectly( const Collision::CBox &box )
{
	// ��ԃG���A�ɓ������Ă��Ȃ� && ��_����ԃG���A�ɓ����Ă���
	if( !Collision_Area_Box( box ) &&
		box.Points[0].x >= m_spaceArea.vMin.x && box.Points[0].x <= m_spaceArea.vMax.x &&
		box.Points[0].y >= m_spaceArea.vMin.y && box.Points[0].y <= m_spaceArea.vMax.y &&
		box.Points[0].z >= m_spaceArea.vMin.z && box.Points[0].z <= m_spaceArea.vMax.z )
	{
		return true;
	}

	return false;
}

// �n���ꂽ�{�b�N�X�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
bool IOctreeMapNode::IsBoxtInThisSpacePartly( const Collision::CBox &box )
{
	// �n���ꂽ�{�b�N�X�����̃G���A�̃{�b�N�X��Collision���Ă�����true
	if( Collision_Area_Box( box ) )
	{
		return true;
	}
	return false;
}

// �n���ꂽ�I�u�W�F�N�g�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
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

// �n���ꂽ�I�u�W�F�N�g�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
bool IOctreeMapNode::IsObjectInThisSpacePartly( Scene::IModelActor *pModel )
{
	if( Collision_Area_Model( pModel ) )
	{
		return true;
	}
	return false;
}

// �G���A�{�b�N�X�Ƃ̓����蔻��
// ���f���Ƃ̔���
bool IOctreeMapNode::Collision_Area_Model( Scene::IModelActor *pModel )
{
	// 6�̕��ʂɑ΂��ē����蔻��
	const Math::Vector3D & vMin = m_spaceArea.vMin;
	const Math::Vector3D & vMax = m_spaceArea.vMax;

	Collision::CBox box[6];
	Math::Matrix identity;
	identity.Identity();
	// �����蔻�肪�Ȃ��c
	//  Selene�̃\�[�X��OBB�̓����蔻��ɂ��Ẳ����ǂ�ŉ����ǂ��Ȃ��Ă���̂��c�������ق����悳����
	//   ���������cSelene�̃T���v�����������A���s�ړ��͍s��Ŏw�肵�Ȃ��ƃ_�����ۂ��B�܂�AvMin��vMax�ł͒�����(0,0,0)�����Ȃ��ƃ_����������Ȃ�
	//	 ���Ė{���ɂ����Ȃ̂��c�H
	// pModel->TransformUpdateCollisionOnly�ł��܂��������c
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

// �{�b�N�X�Ƃ̔���
bool IOctreeMapNode::Collision_Area_Box( const Collision::CBox &box )
{
	// 6�̕��ʂɑ΂��ē����蔻��
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

// ���C�Ƃ̔���
bool IOctreeMapNode::Collision_Area_Ray( const Collision::CLine3D &ray )
{
	// 6�̕��ʂɑ΂��ē����蔻��
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


// ������
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

// ������̈�̕`��
void IOctreeMapNode::DrawVisibleArea( Scene::ISceneManager *pSceneMgr , int window_width , int window_height )
{
	int i;
	Math::Vector3D pos,rotate,scale;

	// ���̃G���A�����ł���Ε`��
	if( CheckVisible( pSceneMgr , window_width , window_height ) )
	{
		for( i=0 ; i<m_objectNum ; i++ )
		{
			// ���̃I�u�W�F�N�g���͉̂�������s���H
			IMapObjectData *model = m_mapObjects.at(i);
			model->m_pActor->TransformUpdate();
			model->m_pActor->RenderingRequest();
		}
		// �q��`��
		for( i=0 ; i<8 ; i++ )
		{
			if( m_childrenNode[i] )
			{
				m_childrenNode[i]->DrawVisibleArea( pSceneMgr , window_width , window_height );
			}
		}
	}
	
}

// �̈掩�̂̕`��
void IOctreeMapNode::DrawOctreeArea( Renderer::Object::ILine3D *pLine , Scene::ISceneManager *pSceneMgr , int window_width , int window_height )
{
	if( CheckVisible( pSceneMgr , window_width , window_height ) )
	{
		// ���̗̈��Box��`��
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
		// �q��`��
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

// ��ԕ������s��
void IOctreeMapNode::DivideBoxSpace( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ ,
		Math::Vector3D minPoints[] , Math::Vector3D maxPoints[] , bool is_divide_x , bool is_divide_y , bool is_divide_z )
{
	int i;
	Math::Vector3D baseMin,baseMax,baseBoxSize;

	// �S���l�������K�v����
	// (0,0,0)�����S�ɂ��邱�Ƃ�O��Ƃ��ĕ������Ă���B����͂܂���

	// ��{�ʒu(=0�Ԗڂ̋��)
	baseMin = minXYZ;
	baseBoxSize = (maxXYZ-minXYZ)/2.0f;

	for( i=0 ; i<8 ; i++ )
	{
		minPoints[i] = baseMin;
		maxPoints[i] = baseMin+baseBoxSize;
	}

	// 8�p�^�[���ԗ�����

	// �S����
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
	// Z�̂ݕ������Ȃ�
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
	// Y�̂ݕ������Ȃ�
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
	// X�̂ݕ������Ȃ�
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
	// X�̂ݕ���
	if( is_divide_x && !is_divide_y && !is_divide_z )
	{
		maxPoints[0].y += baseBoxSize.y;
		maxPoints[0].z += baseBoxSize.z;
		minPoints[1].x += baseBoxSize.x;
		maxPoints[1] += baseBoxSize;
	}
	
	// Y�̂ݕ���
	if( !is_divide_x && is_divide_y && !is_divide_z )
	{
		maxPoints[0].x += baseBoxSize.x;
		maxPoints[0].z += baseBoxSize.z;
		minPoints[1].y += baseBoxSize.y;
		maxPoints[1] += baseBoxSize;
	}
	// z�̂ݕ���
	if( !is_divide_x && !is_divide_y && is_divide_z )
	{
		maxPoints[0].y += baseBoxSize.y;
		maxPoints[0].x += baseBoxSize.x;
		minPoints[1].z += baseBoxSize.z;
		maxPoints[1] += baseBoxSize;
	}
	// �����Ȃ�
	if( !is_divide_x && !is_divide_y && !is_divide_z )
	{
		maxPoints[0] = maxXYZ;
	}

}
