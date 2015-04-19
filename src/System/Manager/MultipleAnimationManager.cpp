
#include "MultipleAnimationManager.h"
#include "Game/SMFObject.h"
#include "Game/UdongeActionConstances.h"
#include "System/FileManagerSupporter.h"
#include "Game/UdongeActionConstances.h"

using namespace Udonge;

/*
bool LoadFromFile( LPCTSTR pFname , bool do_append = true );
	MultipleAnimation *GetCopyMultipleAnimationObject( LPCTSTR pName ){return GetCopyMultipleAnimationObject(AddAtom(pName));}
	MultipleAnimation *GetCopyMultipleAnimationObject( ATOM key );
	MultipleAnimation *GetOriginalMultipleAnimationObject( LPCTSTR pName ){return GetOriginalMultipleAnimationObject(AddAtom(pName));};
	MultipleAnimation *GetOriginalMultipleAnimationObject( ATOM key );

	void DeleteMultipleAnimationObject( LPCTSTR pName ){DeleteMultipleAnimationObject(AddAtom(pName));}
	void DeleteMultipleAnimationObject( ATOM key );
	void Release( void );
*/

#define ENEMY_FILE_VERSION_0_1	0.1f

// Load
bool CMultipleAnimationManager::LoadFromFile(LPCTSTR pFname, char* loadKeyWord , CTextureManager *tex_manager ,
											 CModelManager *model_manager , Renderer::IRender *render ,
											 File::IFileManager *pFileManager , bool do_append )
{
	if( !do_append )
	{
		Release();
	}

	// �����œǂݍ��݁A�ǉ�
	pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = pFileManager->FileOpen( pFname );
	pFileManager->SetCurrentPath( "Texture" );
//	FILE *fp = fopen( pFname , "r" );
	if( fp->IsInvalid() )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	char	buf[2048];
	char	*temp;

	// Version���
	GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp );
	buf[strlen(buf)-1] = '\0';
	temp = strtok( buf , ":" );
	temp = strtok( NULL , ":" );

	float version = (float)atof(temp);
	bool ret_val = true;

	if( version == ENEMY_FILE_VERSION_0_1 )
	{
		ret_val = LoadFromFileVer0_1( fp , loadKeyWord , tex_manager , model_manager , render );
		
	}
	else
	{
		ret_val = false;
	}

	SAFE_RELEASE(fp);
	return ret_val;
}

bool CMultipleAnimationManager::LoadFromFileVer0_1( File::IResourceFile *fp , char* loadKeyWord ,
												   CTextureManager *tex_manager , CModelManager *model_manager , Renderer::IRender *render )
{
	char	buf[2048];
	char	*temp1;

	//
	// �܂���textures��T��
	//
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		if( strcmp( buf , "textures:" ) == 0 )
		{
			break;
		}
	}

	int file_size = fp->GetFileSize();

	if( ( fp->GetFilePosition() == file_size ) )
	{
		return false;
	}

	map<int,ATOM>	tex_no_to_fatom;
	int tex_no;

	// textures��ǂݍ���
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		temp1 = strtok( buf , "," );
		tex_no = atoi( temp1 );
		temp1 = strtok( NULL , "," );
		if( tex_no <= 0 || !temp1 || !tex_manager->AddTexture( temp1 , render ) )
		{
			break;
		}
		
		tex_no_to_fatom[tex_no] = AddAtom(temp1);
	}

	// textures�I���

	//
	// enemies�ǂݍ���
	//
	fp->SeekStart(0);
	while( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) )
	{
		buf[strlen(buf)-1] = '\0';
		if( strcmp( buf , loadKeyWord ) == 0 )
		{
			break;
		}
	}
	if( ( fp->GetFilePosition() == file_size ) )
	{
		return false;
	}

	// �I�[�܂œǂݍ���
	MultipleAnimation *new_enemy;
	float width,height,depth;
	Math::Vector3D ini_speed=Math::Vector3D(0,0,0),ini_accel=Math::Vector3D(0,0,0);
	Math::Vector3D ini_rotate_speed=Math::Vector3D(0,0,0);
	int show_span;
	Math::Vector2D uv_lefttop,uv_rightbottom;
	Math::Vector3D speed=Math::Vector3D(0,0,0),accel=Math::Vector3D(0,0,0);
	Math::Vector3D rotate_speed;
	Math::Rect2D<Float> collision(0,0,0,0);
	ATOM new_atom;
	GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp );
	buf[strlen(buf)-1] = '\0';
	bool is_x_file = false;


	while( !( fp->GetFilePosition() == file_size ) )
	{
		ini_speed=Math::Vector3D(0,0,0),ini_accel=Math::Vector3D(0,0,0);
		speed=Math::Vector3D(0,0,0),accel=Math::Vector3D(0,0,0);
		// �G��,width,height,depth
		while( strlen(buf) == 0 )
		{
			if( GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ) == NULL )
			{
				return true;
			}
			buf[strlen(buf)-1] = '\0';
		}

		temp1 = strtok( buf , "," );
		if( !temp1 )
		{
			return false;
		}

		new_atom = AddAtom( temp1 );
		temp1 = strtok( NULL , "," );
		if( strcmp( temp1 , "XFILE" ) == 0 )
		{
			is_x_file = true;
			temp1 = strtok( NULL , "," );
			if( !temp1 )
			{
				return false;
			}
		}
		else
		{
			is_x_file = false;
		}
		width = (float)atof(temp1);
		if( width <= 0.0f )
		{
			return false;
		}
		temp1 = strtok( NULL , "," );
		height = (float)atof( temp1 );
		if( height <= 0.0f )
		{
			return false;
		}
		temp1 = strtok( NULL , "," );
		depth = (float)atof( temp1 );
		if( is_x_file )
		{
			new_enemy = new SMFObject( model_manager , Math::Vector3D(0,0,0) , width , height , depth );
		}
		else
		{
			if( depth == 0.0f )
			{
				new_enemy = new MultipleAnimation( Math::Vector3D(0,0,0) , width , height );
			}
			else
			{
				new_enemy = new MultipleAnimation3DBox( Math::Vector3D(0,0,0) , width , height , depth );
			}
		}
		
		GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp );
		buf[strlen(buf)-1] = '\0';
		temp1 = strtok( buf , "," );

		
		int new_animation;

		
		if( strcmp( temp1 , "no_animation" ) == 0 )
		{
			// �A�j���[�V��������
			// no_animation,�e�N�X�`���ԍ�,UV���W
			// ���̃e�N�X�`�����\������邾��
			temp1 = strtok( NULL , "," );
			if( temp1 )
			{
				tex_no = atoi( temp1 );
				if( tex_no <= 0 || tex_no_to_fatom[tex_no] == 0 )
				{
					delete new_enemy;
					return false;
				}

				// �����������Ⴞ�߁c
				// UpdateAnimation���\�b�h������Ɏ��s����A������Set����Texture���ύX����Ă��܂�
				// MultipleAnimation���p������3DBox����������Q�������Ɍ���Ă����c
				// no_animation�͂��΂炭�ۗ�
				new_enemy->SetTexture( tex_manager->GetAt(tex_no_to_fatom[tex_no]) );

				// UV���W�擾
				// left
				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					delete new_enemy;
					return false;
				}
				uv_lefttop.x = (float)atof( temp1 );
				// top
				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					delete new_enemy;
					return false;
				}
				uv_lefttop.y = (float)atof( temp1 );
				// right
				temp1 = strtok( NULL , "," );
				if( !temp1 )
				{
					delete new_enemy;
					return false;
				}
				uv_rightbottom.x = (float)atof( temp1 );
				// bottom
				temp1 = strtok( NULL , ",\r\n" );
				if( !temp1 )
				{
					delete new_enemy;
					return false;
				}
				uv_rightbottom.y = (float)atof( temp1 );

				new_enemy->SetUV( uv_lefttop , uv_rightbottom );
			}
			else
			{
				delete new_enemy;
				return false;
			}

			GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp );
			buf[strlen(buf)-1] = '\0';

		}
		else
		{
			if( strcmp( temp1 , "normal_animation" ) == 0 )
			{
				// �������x�����w�肳��Ă���H
				ini_speed.x = ini_speed.y = ini_speed.z = 0.0f;
				ini_accel.x = ini_accel.y = ini_accel.z = 0.0f;
				ini_rotate_speed.x = ini_rotate_speed.y = ini_rotate_speed.z = 0.0f;
				temp1 = strtok( NULL , "," );
				if( temp1 )
				{
					ini_speed.x = (float)atof(temp1);
					ini_speed.y = (float)atof(strtok(NULL,","));
					temp1 = strtok( NULL , "," );
					if( temp1 )
					{
						ini_accel.x = (float)atof( temp1 );
						temp1 = strtok( NULL , "," );
						if( strcmp( temp1 , "GRAVITY" ) == 0 )
						{
							ini_accel.y = GRAVITY_ACCEL;
						}
						else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
						{
							ini_accel.y = -GRAVITY_ACCEL;
						}
						else
						{
							ini_accel.y = (float)atof(temp1);
						}
						temp1 = strtok( NULL , "," );
						if( temp1 )
						{
							ini_rotate_speed.z = (float)atof( temp1 );
						}
					}
				}

				// �A�j���[�V�����쐬
				new_animation = new_enemy->CreateNewAnimation();

	//			new_enemy->SetPolygonSize(width,height);
				new_enemy->SetIniSpeed( ini_speed );
				new_enemy->SetIniAccel( ini_accel );
				new_enemy->ApplyIniSpeed();
				new_enemy->ApplyIniAccel();

				new_enemy->SetRotationSpeed( ini_rotate_speed );

				while(GameSystem::Supporter::fgets_from_fr( buf , 2047 , fp ))
				{
					buf[strlen(buf)-1] = '\0';
					if( strlen(buf) == 0 )
					{
						break;
					}

					temp1 = strtok( buf , "," );
					if( is_x_file )
					{
						// X�t�@�C�����������Ă���
						if( !((SMFObject *)new_enemy)->LoadModel( temp1 ) )
						{
							delete new_enemy;
							return false;
						}
					}
					else
					{
						if( isalpha( buf[0] ) != 0 )
						{
							break;
						}
						// X�t�@�C���ȊO�����AUV���W������
						tex_no = atoi( temp1 );
						if( tex_no <= 0 )
						{
							delete new_enemy;
							return false;
						}

						// �e�N�X�`��
						if( tex_no_to_fatom[tex_no] == 0 )
						{
							delete new_enemy;
							return false;
						}

						// �\���X�p��
						temp1 = strtok( NULL , "," );
						show_span = atoi( temp1 );
						if( show_span <= 0 )
						{
							delete new_enemy;
							return false;
						}

						// UV���W
						// left
						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						uv_lefttop.x = (float)atof( temp1 );
						// top
						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						uv_lefttop.y = (float)atof( temp1 );
						// right
						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						uv_rightbottom.x = (float)atof( temp1 );
						// bottom
						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						uv_rightbottom.y = (float)atof( temp1 );
					}

					// �����蔻��
					// �@x,y,width,height,
					// �@�t�@�C���ɂ́A�摜�̍���=(-0.5,-0.5) �E��=(0.5,0.5)�Ƃ����Ƃ��̍��W������B
					// �@����͐����`�łȂ��Ă�����(x,y�Ŗڐ���Ԋu���قȂ��ė���)
					temp1 = strtok( NULL , "," );
					if( !temp1 )
					{
						delete new_enemy;
						return false;
					}
					// NO_CHANGE�Ə����Ă������璼�O�̂��̂Ɠ����f�[�^���g��
					if( strcmp( temp1 , "NO_CHANGE" ) != 0 )
					{
						collision.x = (float)(atof( temp1 )*width);

						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						collision.y = (float)(atof( temp1 )*height);

						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						collision.w = (float)(atof( temp1 )*width);

						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						collision.h = (float)(atof( temp1 )*height);


					}


					// ������͑��x�Ƃ��̐ݒ�B�������͂��Ȃ������炻��ł����܂�
					temp1 = strtok( NULL , "," );
					if( temp1 )
					{
						if( strcmp( temp1 , "GRAVITY" ) == 0 )
						{
							speed.x = GRAVITY_ACCEL;
						}
						else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
						{
							speed.x = -GRAVITY_ACCEL;
						}
						else
						{
							speed.x = (float)atof(temp1);
						}

						temp1 = strtok( NULL , "," );
						if( !temp1 )
						{
							delete new_enemy;
							return false;
						}
						if( strcmp( temp1 , "GRAVITY" ) == 0 )
						{
							speed.y = GRAVITY_ACCEL;
						}
						else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
						{
							speed.y = -GRAVITY_ACCEL;
						}
						else
						{
							speed.y = (float)atof(temp1);
						}

						temp1 = strtok( NULL , "," );
						if( temp1 )
						{
							if( strcmp( temp1 , "GRAVITY" ) == 0 )
							{
								accel.x = GRAVITY_ACCEL;
							}
							else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
							{
								accel.x = -GRAVITY_ACCEL;
							}
							else
							{
								accel.x = (float)atof(temp1);
							}

							temp1 = strtok( NULL , "," );
							if( !temp1 )
							{
								delete new_enemy;
								return false;
							}
							if( strcmp( temp1 , "GRAVITY" ) == 0 )
							{
								accel.y = GRAVITY_ACCEL;
							}
							else if( strcmp( temp1 , "-GRAVITY" ) == 0 )
							{
								accel.y = -GRAVITY_ACCEL;
							}
							else
							{
								accel.y = (float)atof(temp1);
							}

							temp1 = strtok( NULL , "," );
							if( temp1 )
							{
								rotate_speed.z = (float)atof(temp1);
								rotate_speed.x = 0.0f;
								rotate_speed.y = 0;
							}
							else
							{
								rotate_speed = not_change_param_3d;
							}
						}
						else
						{
							accel = not_change_param_3d;
							rotate_speed = not_change_param_3d;
						}
					}
					else
					{
						accel = speed = not_change_param_3d;
						rotate_speed = not_change_param_3d;
					}
					new_enemy->Add( new_animation , tex_manager->GetAt(tex_no_to_fatom[tex_no]) , show_span ,
						uv_lefttop , uv_rightbottom , collision , speed , accel , rotate_speed );
					new_enemy->SetLoop( new_animation , true );
				}
				new_enemy->SetNormalAnimation( new_animation );
			}
			else
			{
				return false;
			}
		}
		// �o�^
		m_atom_list.push_back( new_atom );
		m_atom_to_animation[new_atom] = new_enemy;

	}
	return true;
}

// GetCopy
MultipleAnimation* CMultipleAnimationManager::GetCopyMultipleAnimationObject(ATOM key)
{
	if( m_atom_to_animation[key] )
	{
		return (MultipleAnimation *)(m_atom_to_animation[key]->Clone());
	}
	else
	{
		return NULL;
	}
}

// GetOriginal
MultipleAnimation* CMultipleAnimationManager::GetOriginalMultipleAnimationObject(ATOM key)
{
	return m_atom_to_animation[key];
}

// Delete
void CMultipleAnimationManager::DeleteMultipleAnimationObject(ATOM key)
{
	MultipleAnimation *enemy = m_atom_to_animation[key];

	if( enemy )
	{
		SAFE_RELEASE(enemy);
		vector<ATOM>::iterator it,end = m_atom_list.end();
		for( it = m_atom_list.begin() ; it != end ; it++ )
		{
			if( *it == key )
			{
				m_atom_list.erase(it);
				break;
			}
		}
		m_atom_to_animation[key] = NULL;
	}
}

// Release
void CMultipleAnimationManager::Release()
{
	vector<ATOM>::iterator it,end = m_atom_list.end();
	for( it = m_atom_list.begin() ; it != end ; it++ )
	{
		SAFE_RELEASE(m_atom_to_animation[*it]);
	}
	m_atom_list.clear();
}
