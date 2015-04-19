
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	Release();
}

// サウンドロード
bool SoundManager::LoadSound( ICore *pCore , LPCTSTR sound_name , LPCTSTR pluginName )
{
	Sound::IStreamSound *sound = pCore->CreateStreamSoundFromFile( sound_name , false , pluginName );
	if( sound == NULL || sound->IsInvalid() )
	{
		return false;
	}

	ATOM new_atom = AddAtom( sound_name );
	m_atom_to_sound[new_atom] = sound;
	m_listed_atoms.push_back(new_atom);

	return true;
}

// サウンド削除
void SoundManager::DeleteSound(LPCTSTR sound_name)
{
	DeleteSound(AddAtom(sound_name));
}
void SoundManager::DeleteSound(ATOM sound_name)
{
	Sound::IStreamSound *sound = m_atom_to_sound[sound_name];

	SAFE_RELEASE(sound);

	m_atom_to_sound[sound_name] = NULL;
	vector<ATOM>::iterator it,end=m_listed_atoms.end();

	for( it = m_listed_atoms.begin() ; it != end ; it++ )
	{
		if( *it == sound_name )
		{
			m_listed_atoms.erase(it);
			return;
		}
	}
}

// サウンド再生
void SoundManager::Play(LPCTSTR sound_name, int loop_count )
{
	Play( AddAtom(sound_name) , loop_count );
}
void SoundManager::Play(ATOM sound_name, int loop_count)
{
	Sound::IStreamSound *sound = m_atom_to_sound[sound_name];
	if( !sound )
	{
		return;
	}
	sound->Play( loop_count );
}
// サウンド停止
void SoundManager::Stop(LPCTSTR sound_name)
{
	Stop( AddAtom(sound_name) );
}
void SoundManager::Stop(ATOM sound_name)
{
	Sound::IStreamSound *sound = m_atom_to_sound[sound_name];
	if( !sound )
	{
		return;
	}
	sound->Stop();

}
// サウンド一時停止
void SoundManager::Pause(LPCTSTR sound_name)
{
	Pause(AddAtom(sound_name));
}
void SoundManager::Pause(ATOM sound_name)
{
	Sound::IStreamSound *sound = m_atom_to_sound[sound_name];
	if( !sound )
	{
		return;
	}
	sound->Pause();
}

// サウンドインターフェース取得
Sound::IStreamSound *SoundManager::GetSound(LPCTSTR sound_name)
{
	return GetSound(AddAtom(sound_name));
}
Sound::IStreamSound *SoundManager::GetSound(ATOM sound_name_atom)
{
	return m_atom_to_sound[sound_name_atom];
}

// 解放
void SoundManager::Release()
{
	int i,size = m_listed_atoms.size();

	for( i=0 ; i<size ; i++ )
	{
		SAFE_RELEASE(m_atom_to_sound[m_listed_atoms[i]]);
	}
	m_atom_to_sound.clear();
	m_listed_atoms.clear();
}
