
#pragma once

// リプレイファイルの読み込み，入力キー取得
// 入力キー保存，リプレイファイルへの書き込みを担当
// ついでにスコアも管理

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

	// 読み込み関連
	bool LoadFromFile( char *file_name , File::IFileManager *pFileManager );
	void SetReadPointerToFirst();
	int GetNextKey();

	// 保存関連
	void AddKey( int key );
	bool SaveToFile( char *file_name );

	void Release();
};
