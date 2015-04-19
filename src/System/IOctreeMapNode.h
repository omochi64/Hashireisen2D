#pragma once

#include <vector>
#include "Selene.h"

using namespace Selene;

#include "AllObject.h"
#include "IMapObjectData.h"


/*
// �}�b�v�f�[�^����̃��[�h
OctreeMapNode* LoadFromFile( char *file_name , Renderer::IRender *pRender , File::IFileManager *pFileManager );
���̂悤�ȊO���֐�������Ă����B���������OctreeMap�N���X�͂���Ȃ�
*/

// �}�b�v���i�f�[�^�\����
//  �����͍\���̂ɂ����A�}�b�v�f�[�^�C���^�[�t�F�[�X�N���X������Ă����B�����āA�c����A�܂āA������Ȃ񂩂ւ񂾁c
//	���[��AID�̕ۑ��Ƃ��͂ǂ����悤�c
//	����A����ς��ŏ������悤�Ƀf�[�^�͌p�����Ȃ��c�ŏ�ʂ̐e��ModelActor�����������ARelease��Copy���Œ���̃��\�b�h�Ƃ��Ď��c���[��c
//typedef struct _map_object_data{
//	Scene::IModelActor *pModel;
//	Math::Vector3D	vPosition;
//	Math::Vector3D	vRotation;
//}MAP_OBJECT_DATA;

// �����؂̓����m�[�h
// �R�C�c�̓}�b�v�����c�[���ɂ��g��
// �ƂȂ�ƁA���I�Ƀc���[��ύX���鑀������ꂽ���B�R�C�c���p�����č��H
class IOctreeMapNode:public AllObject
{
protected:
	

	std::vector<IMapObjectData *>	m_mapObjects;
	int				m_objectNum;
	IOctreeMapNode	*m_childrenNode[8];	// �q�m�[�h�B�K��8�ȉ��ƂȂ�
	Collision::CBox	m_spaceArea;
	//		int				visibleArea;	�܂��d�l������
	bool			m_divided_x,m_divided_y,m_divided_z;

	// ������
	//	������ɂ��Ă̓I���C���[�̖{������������Əڂ����ǂ�ł���
	bool CheckVisible( Scene::ISceneManager *pSceneMgr , int window_width , int window_height );

	// ���g��new�B�p�����ď㏑������p
	virtual IOctreeMapNode *NewThisClass();

	// �G���A�{�b�N�X�Ƃ̓����蔻��
	bool Collision_Area_Model( Scene::IModelActor *pModel );
	bool Collision_Area_Box( const Collision::CBox &box );
	bool Collision_Area_Ray( const Collision::CLine3D &ray );

public:
	IOctreeMapNode();
	virtual ~IOctreeMapNode();
	virtual void Release();

	// ��������쐬�B�ċA�����p
	virtual void CreateTree( int depth_x , int depth_y , int depth_z , const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ );

	// �I�u�W�F�N�g��ǉ�
	// add_node_number�́A�ǂ̃m�[�h�ɂ��̃f�[�^��ǉ����邩��\��
	//	add_node_number = {1,2,2,0}
	// ��������A3�i��(���[�g��0�i��)��2�Ԗڂ̃m�[�h�ɒǉ�����
	bool AddObject( IMapObjectData *add_data , int *add_node_number );
	// �I�u�W�F�N�g��ǉ�
	//  �ǂ��Ɋ��蓖�Ă��邩�͎����I�ɔ���
	bool AddObject( IMapObjectData *add_data );

	// �I�u�W�F�N�g�̍폜
	//  �m�[�h�w��^
	bool DeleteObject( IMapObjectData *delete_data , int *delete_node_number , bool will_delete_data = true );
	//  ���������^
	//		-�e�m�[�h�D��
	bool DeleteObjectPrimirilyParent( IMapObjectData *delete_data , bool will_delete_data = true );
	//	���������^
	//		-�q�m�[�h�D��
	bool DeleteObjectPrimirilyChild( IMapObjectData *delete_data , bool will_delete_data = true );

	int GetObjectNum( void ){return m_objectNum;}

	// Octree���R���p�N�g�ɂ���B�I�u�W�F�N�g����0�ȃm�[�h�͔j��
	void Compact( void );

	// max,min�̎擾
	const Math::Vector3D &GetMinXYZ();
	const Math::Vector3D &GetMaxXYZ();

	// �����蔻��
	// �����������Ԃ�out�ɒǉ�����Ă���
	bool HitCheckByRay( const Collision::CLine3D &ray , std::vector<Renderer::SCollisionResult> &out , std::vector<IMapObjectData *> &out_object );

	// ��������(Octree��Ԃɏ�������/���Ȃ��̃`�F�b�N�֐�)�́ACheckCollision��������g���΍��邩�ȁH

	// �n���ꂽ�{�b�N�X�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
	bool IsBoxtInThisSpacePerfectly( const Collision::CBox &box );
	// �n���ꂽ�{�b�N�X�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
	bool IsBoxtInThisSpacePartly( const Collision::CBox &box );
	// �n���ꂽ�I�u�W�F�N�g�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
	bool IsObjectInThisSpacePerfectly( Scene::IModelActor *pModel );
	// �n���ꂽ�I�u�W�F�N�g�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
	bool IsObjectInThisSpacePartly( Scene::IModelActor *pModel );
	// �n���ꂽ���C�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�����ɂ����ۂ薄�܂邩)
	bool IsRayInThisSpacePerfectly( const Collision::CLine3D &ray );
	// �n���ꂽ���C�����̋�ԃG���A�ɓ����Ă��邩�ǂ������`�F�b�N(�ꕔ���ł������Ă��邩)
	bool IsRayInThisSpacePartly( const Collision::CLine3D &ray );


	// ������̈�̕`��
	void DrawVisibleArea( Scene::ISceneManager *pSceneMgr , int window_width , int window_height );

	// �̈掩�̂̕`��
	void DrawOctreeArea( Renderer::Object::ILine3D *pLine , Scene::ISceneManager *pSceneMgr , int window_width , int window_height );


	// ��ԕ����֐�
	static void DivideBoxSpace( const Math::Vector3D &minXYZ , const Math::Vector3D &maxXYZ ,
		Math::Vector3D minPoints[] , Math::Vector3D maxPoints[] , bool is_divide_x , bool is_divide_y , bool is_divide_z );
};
