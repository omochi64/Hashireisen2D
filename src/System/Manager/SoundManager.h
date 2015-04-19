
#pragma once

#include <vector>
#include <map>
#include "Selene.h"

using namespace std;
using namespace Selene;

// ÉTÉEÉìÉhä«óù
class SoundManager
{
	map<ATOM,Sound::IStreamSound *>		m_atom_to_sound;
	vector<ATOM>						m_listed_atoms;

public:
	SoundManager();
	~SoundManager();

	bool LoadSound( ICore *pCore , LPCTSTR sound_name , LPCTSTR pluginName );
	void DeleteSound( LPCTSTR sound_name );
	void DeleteSound( ATOM sound_name );
	void Play( LPCTSTR sound_name , int loop_count = 1 );
	void Play( ATOM sound_name , int loop_count = 1 );
	void Stop( LPCTSTR sound_name );
	void Stop( ATOM sound_name );
	void Pause( LPCTSTR sound_name );
	void Pause( ATOM sound_name );

	void Release();

	Sound::IStreamSound *GetSound( LPCTSTR sound_name );
	Sound::IStreamSound *GetSound( ATOM sound_name_atom );
};
