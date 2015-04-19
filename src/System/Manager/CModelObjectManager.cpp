
#include <iostream>
#include <map>
#include "CModelObjectManager.h"
#include "System/GameSystem.h"
#include "System/FileManagerSupporter.h"

namespace Manager
{

/*	CModelObjectManager::CModelObjectManager()
	{
	}

	CModelObjectManager::CModelObjectManager(const Manager::CModelObjectManager &)
	{
	}*/

	CModelObjectManager::~CModelObjectManager()
	{
		Release();
	}
/*
	CModelObjectManager & CModelObjectManager::operator =( const CModelObjectManager &a )
	{
		return (CModelObjectManager &)a;
	}*/

	// �C���X�^���X�̎擾
	CModelObjectManager &CModelObjectManager::getInstance()
	{
		static CModelObjectManager only_object;
		return only_object;
	}

	// ���
	void CModelObjectManager::Release()
	{
		std::vector<ATOM>::iterator it,end;

		for( it=m_addedObject.begin(), end=m_addedObject.end() ;
			it != end ; it++ )
		{
			delete m_XMLParser[*it];
		}
		m_addedObject.clear();
		m_XMLParser.clear();

		for( it=m_readObject.begin(), end=m_readObject.end() ;
			it != end ; it++ )
		{
			SAFE_RELEASE(m_objectMaster[*it]);
		}
		m_readObject.clear();
		m_objectMaster.clear();
	}

	// �t�@�C����ǉ�
	bool CModelObjectManager::AddDefinitionFile(LPCTSTR pFileName)
	{
		File::IFileManager *pFileMgr = GameSystem::SystemGetter::GetFileManager();
		File::IResourceFile *pFile = pFileMgr->FileOpen( pFileName );
		if( !pFile || pFile->IsInvalid() )
		{
			SAFE_RELEASE(pFile);
			return false;
		}

		unsigned int size = pFile->GetFileSize();
		char *data = new char[size];
		if( !data )
		{
			SAFE_RELEASE(pFile);
			return false;
		}
		if( pFile->Read( data , size ) != size )
		{
			SAFE_RELEASE(pFile);
			delete [] data;
			return false;
		}

		TiXmlDocument *new_doc = new TiXmlDocument();
		new_doc->Parse( data );

		// ����delete��������đ��v���ȁH
		// ����ς肱��̓_���L���Ȃ��c
		delete [] data;
		// !!check!!!!check!!!!check!!
		// !!check!!!!check!!!!check!!

		TiXmlElement *el = new_doc->RootElement();
		const char *name = el->Attribute("object");
		if( strlen(name) == 0 )
		{
			SAFE_RELEASE(pFile);
			delete new_doc;
			return false;
		}

		ATOM new_name = AddAtom(name);

		// �ǉ�
		m_XMLParser[new_name] = new_doc;

		
		return true;
	}

	

	// XML����́A�I�u�W�F�N�g���[�h
	bool CModelObjectManager::LoadObject( ATOM name )
	{
		TiXmlDocument *pDoc = m_XMLParser[name];

		if( !pDoc )
		{
			return false;
		}

		// ��͊J�n
		CModelObject *new_object = new CModelObject(name);
		if( !new_object )
		{
			return false;
		}
		// type �����͂ǂ��������˂�

		TiXmlElement *root = pDoc->FirstChildElement();
		TiXmlElement *data_scheme = root->FirstChildElement();

		

		while( data_scheme )
		{
			const char *value = data_scheme->Value();

			if( !strcmp( value , "model" ) )
			{
				// ���f���t�@�C���̒�`
				string name = data_scheme->Attribute( "file" );
//				const char *fname = att->VValue();

				// model_manager����ǂݍ���
				//  �ق�Ƃ͂�����SetCurrentPath�����e�����Ŏw�肵�Ȃ��I�I
				GameSystem::SystemGetter::GetFileManager()->SetCurrentPath( "Model" );
				if( !new_object->LoadModel( name.c_str() ) )
				{
					SAFE_RELEASE(new_object);
					return false;
				}
			}
			else if( !strcmp( value , "animation" ) )
			{
				if( !AnalysisForAnimation( data_scheme , new_object ) )
				{
					SAFE_RELEASE(new_object);
					return false;
				}
			}

			data_scheme = data_scheme->NextSiblingElement();
		}


		return true;
	}

	bool CModelObjectManager::AnalysisForAnimation( TiXmlElement *animation_root , CModelObject *object )
	{
		std::vector<std::string>	labels;
		std::vector<int>			label_to_lines;
		// �A�j���[�V�����̒ǉ�
		// �A�j���[�V������
		const char *new_animation_name = animation_root->Attribute( "name" );
		if( strlen(new_animation_name) == 0 )
		{
			return false;
		}

		int now_line=0;
		CAnimation *add_animation = new CAnimation();
		if( !add_animation )
		{
			return false;
		}
		CAnimation::SCRIPT_ACTION sa={0};
		sa.tm_animation.will_call = false;
		sa.jump_to_line_number = -1;
		TiXmlElement *animation_element = animation_root->FirstChildElement();

		while(1)
		{
			const char *value = animation_element->Value();
			if( !value || strlen(value)==0 || !strcmp(value,"stop") )
			{
				// �I���t���O
				break;
			}
			if( !strcmp( value , "label" ) )
			{
				// ���x���ݒ�
				string label = animation_element->Attribute("name");
				labels.push_back(label);
				label_to_lines.push_back( now_line );
			}
			else if( !strcmp( value , "motion" ) )
			{
				// ��O�̃��C���̃X�N���v�g��ǉ�
				if( now_line > 0 )
				{
					if( !add_animation->AddScriptLine( sa ) )
					{
						break;
					}
					memset( &sa , 0 , sizeof(CAnimation::SCRIPT_ACTION) );
					sa.tm_animation.delay = false;
					sa.jump_to_line_number = -1;
				}

				// motion�̏ڍ׃f�[�^���
				if( !AnalysisForMotionOfAnimation( animation_element , sa ) )
				{
					delete add_animation;
					return false;
				}

				now_line++;
			}
			else if( !strcmp( value , "goto" ) )
			{
				string goto_dest = animation_element->Attribute( "dest" );
				int size = labels.size();
				// ���x����T��
				int i;
				for( i=0 ; i<size ; i++ )
				{
					if( goto_dest.compare( labels[i] ) )
					{
						break;
					}
				}
				if( i!=size )
				{
					// ���x���ւ̃W�����v�o�^
					sa.jump_to_line_number = label_to_lines[i];
				}
			}

			animation_element = animation_element->NextSiblingElement();
		}
		if( now_line > 0 )
		{
			add_animation->AddScriptLine( sa );
		}

		object->AddAnimation( AddAtom(new_animation_name) , add_animation );

		return true;
	}

	bool CModelObjectManager::AnalysisForMotionOfAnimation(TiXmlElement *motion_root , CAnimation::SCRIPT_ACTION &sa )
	{
		// ���̃��C���̃X�N���v�g��ݒ�

		// �Y��Ă����I
		//  motion�Ŏw�肳��Ă��Ȃ������́A0�ɂ���̂ł͂Ȃ��A�ȑO�̃f�[�^�����̂܂ܕێ�����K�v������I
		//  �܂�A���̂܂܂�CAnimation�̎�������܂���!!

		// ���ƁAcalllocal�͂ǂ����悤���˂�
		// <motion></motion>�̊O�H����Ƃ����H����ɂ����Delay�̑��������邩���Ȃ����ς���ė���B
		//  �����Agoto�Ƃ̏��ԓ��̖�肪���邩��A�ł���Β��������B�����āA�A�j���[�V�����͕�����ŌĂяo����̂�����B

		const char *t_frame = motion_root->Attribute( "time" );
		sa.frame_to_next_action = atoi(t_frame);

		// ���[�V��������
		TiXmlElement *motion_element = motion_root->FirstChildElement();
		while( motion_element )
		{
			const char *action_name = motion_element->Value();

			if( !strcmp( action_name , "move" ) )
			{
				// move�̏���

				// �߂�ǂ������̂Ń��[�v�ł܂킹��悤��
				float *set_elements[] = {
					&sa.pos_action.speed.x , &sa.pos_action.speed.y , &sa.pos_action.speed.z ,
					&sa.pos_action.accel.x , &sa.pos_action.accel.y , &sa.pos_action.accel.z ,
					&sa.rot_action.speed.x , &sa.rot_action.speed.y , &sa.rot_action.speed.y ,
					&sa.rot_action.accel.x , &sa.rot_action.accel.y , &sa.rot_action.accel.y ,
				};
				bool *set_valiable_elements[] = {
					&sa.pos_action.valiable_vx , &sa.pos_action.valiable_vy , &sa.pos_action.valiable_vz , 
					&sa.pos_action.valiable_ax , &sa.pos_action.valiable_ay , &sa.pos_action.valiable_az , 
					&sa.rot_action.valiable_vx , &sa.rot_action.valiable_vy , &sa.rot_action.valiable_vz , 
					&sa.rot_action.valiable_ax , &sa.rot_action.valiable_ay , &sa.rot_action.valiable_az
				};
				char *key_strs[] = {
					"vx" , "vy" , "vz" , "ax" , "ay" , "az" ,
					"rvx" , "rvy" , "rvz" , "rax" , "ray" , "raz" , NULL
				};
				// vx,vy,vz,rvx,rvy,rvz�̏����B�L�q���Ȃ���ΕύX����
				int index=0;
				while(key_strs[index])
				{
					const char *v = motion_element->Attribute( key_strs[index] );
					if( v && strlen(v) )
					{
						*set_valiable_elements[index] = true;
						*set_elements[index] = (float)atof(v);
					}
					else
					{
						*set_valiable_elements[index] = false;
					}
					index++;
				}
			}
			else if( !strcmp( action_name , "calllocal" ) )
			{
				// ���[�J���A�j���[�V�����̃R�[��
				// �Ƃ肠�����A�j���[�V�����͔ԍ��Ƃ��Ē�`���Ă���
				// �ꉞ"name"�����Ƃ��Ă���
				const char *v = motion_element->Attribute("name");
				if( v && strlen(v) )
				{
					sa.tm_animation.call_animation = atoi(v);
					v = motion_element->Attribute("delay");
					sa.tm_animation.delay = v?atoi(v):0;
				}
				else
				{
					sa.tm_animation.call_animation = -1;
				}
			}


			motion_element = motion_element->NextSiblingElement();
		}
		return true;
	}

	CModelObject *CModelObjectManager::GetOriginal(ATOM name)
	{
		return m_objectMaster[name];
	}
	CModelObject *CModelObjectManager::GetCopy(ATOM name)
	{
		CModelObject *master = m_objectMaster[name];
		if( master )
		{
			return (CModelObject *)master->Clone();
		}
		return NULL;
	}
};
