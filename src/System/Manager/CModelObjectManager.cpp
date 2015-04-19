
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

	// インスタンスの取得
	CModelObjectManager &CModelObjectManager::getInstance()
	{
		static CModelObjectManager only_object;
		return only_object;
	}

	// 解放
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

	// ファイルを追加
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

		// これdeleteしちゃって大丈夫かな？
		// やっぱりこれはダメ臭いなぁ…
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

		// 追加
		m_XMLParser[new_name] = new_doc;

		
		return true;
	}

	

	// XMLを解析、オブジェクトロード
	bool CModelObjectManager::LoadObject( ATOM name )
	{
		TiXmlDocument *pDoc = m_XMLParser[name];

		if( !pDoc )
		{
			return false;
		}

		// 解析開始
		CModelObject *new_object = new CModelObject(name);
		if( !new_object )
		{
			return false;
		}
		// type 属性はどう扱うかねぇ

		TiXmlElement *root = pDoc->FirstChildElement();
		TiXmlElement *data_scheme = root->FirstChildElement();

		

		while( data_scheme )
		{
			const char *value = data_scheme->Value();

			if( !strcmp( value , "model" ) )
			{
				// モデルファイルの定義
				string name = data_scheme->Attribute( "file" );
//				const char *fname = att->VValue();

				// model_managerから読み込む
				//  ほんとはここのSetCurrentPathもリテラルで指定しない！！
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
		// アニメーションの追加
		// アニメーション名
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
				// 終了フラグ
				break;
			}
			if( !strcmp( value , "label" ) )
			{
				// ラベル設定
				string label = animation_element->Attribute("name");
				labels.push_back(label);
				label_to_lines.push_back( now_line );
			}
			else if( !strcmp( value , "motion" ) )
			{
				// 一つ前のラインのスクリプトを追加
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

				// motionの詳細データ解析
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
				// ラベルを探す
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
					// ラベルへのジャンプ登録
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
		// 次のラインのスクリプトを設定

		// 忘れていた！
		//  motionで指定されていない属性は、0にするのではなく、以前のデータをそのまま保持する必要がある！
		//  つまり、今のままのCAnimationの実装じゃまずい!!

		// あと、calllocalはどうしようかねぇ
		// <motion></motion>の外？それとも中？それによってDelayの属性をつけるかつけないか変わって来る。
		//  ただ、gotoとの順番等の問題があるから、できれば中がいい。そして、アニメーションは文字列で呼び出せるのかが謎。

		const char *t_frame = motion_root->Attribute( "time" );
		sa.frame_to_next_action = atoi(t_frame);

		// モーション処理
		TiXmlElement *motion_element = motion_root->FirstChildElement();
		while( motion_element )
		{
			const char *action_name = motion_element->Value();

			if( !strcmp( action_name , "move" ) )
			{
				// moveの処理

				// めんどくさいのでループでまわせるように
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
				// vx,vy,vz,rvx,rvy,rvzの処理。記述がなければ変更無し
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
				// ローカルアニメーションのコール
				// とりあえずアニメーションは番号として定義しておく
				// 一応"name"属性としておく
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
