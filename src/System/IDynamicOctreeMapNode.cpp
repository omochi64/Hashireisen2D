
#include "IMapObjectData.h"
#include "IDynamicOctreeMapNode.h"
/*
IDynamicOctreeMapNode();
	virtual ~IDynamicOctreeMapNode();

	// ��ԃG���A�T�C�Y��ύX
	void ChangeSpaceSize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// ��������ύX
	void ChangeDivideDepth( int depth );*/

// �R���X�g���N�^�A�f�X�g���N�^
IDynamicOctreeMapNode::IDynamicOctreeMapNode():IOctreeMapNode()
{
}
IDynamicOctreeMapNode::~IDynamicOctreeMapNode()
{
}

// ���̃N���X��New
IOctreeMapNode *IDynamicOctreeMapNode::NewThisClass()
{
	return new IDynamicOctreeMapNode();
}

// ��ԃG���A�T�C�Y��ύX
void IDynamicOctreeMapNode::ChangeSpaceSize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ )
{
	ChangeOnlySize( minXYZ , maxXYZ );
	std::vector<IMapObjectData *> outOfMapObjects;
	Re_PlaceObjects( outOfMapObjects );
	// ���ӂꂽ���͍폜
	int i,size = outOfMapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(outOfMapObjects.at(i));
	}
}

// �T�C�Y�ύX(�ύX���邾��)
void IDynamicOctreeMapNode::ChangeOnlySize( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ )
{
	Math::Matrix identity;
	identity.Identity();
	m_spaceArea.CreateBox( minXYZ , maxXYZ , identity );
	Math::Vector3D minPoints[8],maxPoints[8];
	IOctreeMapNode::DivideBoxSpace( minXYZ , maxXYZ , minPoints , maxPoints , m_divided_x , m_divided_y , m_divided_z );

	// �q�̕ύX
	int i;
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IDynamicOctreeMapNode *)m_childrenNode[i])->ChangeOnlySize( minPoints[i] , maxPoints[i] );
		}
	}

}

// �I�u�W�F�N�g�̍Ĕz�u
void IDynamicOctreeMapNode::Re_PlaceObjects( std::vector<IMapObjectData *> &outOfMapObjects )
{
	int i;

	// �q�����ɍĔz�u���Ă���
	for( i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IDynamicOctreeMapNode *)m_childrenNode[i])->Re_PlaceObjects( outOfMapObjects );
		}
	}
	
//	int size = m_mapObjects.size();

	std::vector<IMapObjectData *> temp_outOfMapObjects;

	for( i=0 ; i<m_objectNum ; i++ )
	{
		IMapObjectData *object = m_mapObjects.at(i);
		object->m_pActor->TransformUpdateCollisionOnly();

		// ���̋�ԓ��ɂ����Ɠ����Ă�H
		if( !IsObjectInThisSpacePerfectly( object->m_pActor ) )
		{
			// �����Ă��Ȃ�
			temp_outOfMapObjects.push_back( object );
			m_mapObjects.erase( m_mapObjects.begin()+i );
			m_objectNum--;
			i--;
			continue;
		}
		else
		{
			int j;
			for( j=0 ; j<8 ; j++ )
			{
				// �q��Add���Ă݂�
				if( m_childrenNode[j] )
				{
					if( m_childrenNode[j]->AddObject( object ) )
					{
						// Add�ł���
						// �����������
						m_mapObjects.erase( m_mapObjects.begin()+i );
						m_objectNum--;
						i--;
						break;
					}
				}
			}
		}
	}

	// �q���炠�ӂ�Ă���Objects��������Add�ł��邩�H
	int size = outOfMapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		IMapObjectData *object = outOfMapObjects.at(i);
		if( IsObjectInThisSpacePerfectly( object->m_pActor ) )
		{
			// ������Add
			m_mapObjects.push_back( object );
			m_objectNum++;
			outOfMapObjects.erase( outOfMapObjects.begin()+i );
			i--,size--;
			continue;
		}
	}

	// temp��ǉ�
	size = temp_outOfMapObjects.size();
	for( i=0 ; i<size ; i++ )
	{
		outOfMapObjects.push_back( temp_outOfMapObjects.at(i) );
	}
}

// ��Ԃ̍ĕ���
void IDynamicOctreeMapNode::ChangeDivideDepth( int depth_x , int depth_y , int depth_z )
{
	// �I�u�W�F�N�g�̎��W�A�c���[�̍폜
	Math::Vector3D min = m_spaceArea.vMin , max = m_spaceArea.vMax;
	std::vector<IMapObjectData *> objects;
	CollectObjectAndEraseTree( objects );

	// �c���[�̍č\�z
	CreateTree( depth_x , depth_y , depth_z , min , max );

	// �I�u�W�F�N�g�̍Ĕz�u
	std::vector<IMapObjectData *>::iterator it=objects.begin(),end=objects.end();

	while( it != end )
	{
		(*it)->m_pActor->TransformUpdateCollisionOnly();
		AddObject( *it );
		it++;
	}

}

// �I�u�W�F�N�g�̎��W
void IDynamicOctreeMapNode::CollectObjectAndEraseTree(std::vector<IMapObjectData*> &collection)
{
	std::vector<IMapObjectData *>::iterator it = m_mapObjects.begin(),end = m_mapObjects.end();

	while( it != end )
	{
		collection.push_back(*it);
		it++;
	}
	m_mapObjects.clear();
	m_objectNum = 0;

	for( int i=0 ; i<8 ; i++ )
	{
		if( m_childrenNode[i] )
		{
			((IDynamicOctreeMapNode *)m_childrenNode[i])->CollectObjectAndEraseTree( collection );
			SAFE_RELEASE(m_childrenNode[i]);
		}
	}

}
