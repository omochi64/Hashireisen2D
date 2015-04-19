
#pragma once

// ���v���C�t�@�C���̓ǂݍ��݁C���̓L�[�擾
// ���̓L�[�ۑ��C���v���C�t�@�C���ւ̏������݂�S��
// ���łɃX�R�A���Ǘ�

#include "Selene.h"
#include <vector>

using namespace std;
using namespace Selene;

class ReplayManager
{
	int m_read_pointer;
	vector<int>	m_key_list;

public:
	ReplayManager();
	~ReplayManager();

	// �ǂݍ��݊֘A
	bool LoadFromFile( char *file_name , File::IFileManager *pFileManager );
	void SetReadPointerToFirst();
	int GetNextKey();

	// �ۑ��֘A
	void AddKey( int key );
	bool SaveToFile( char *file_name );

	void Release();
};
