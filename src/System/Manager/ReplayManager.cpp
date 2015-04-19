
#include "ReplayManager.h"

/*
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
	bool SaveToFile( char *file_name , File::IFileManager *pFileManager );

	void Release();
};

*/

ReplayManager::ReplayManager()
{
	m_read_pointer = 0;
	m_key_list.clear();
}

ReplayManager::~ReplayManager()
{
	m_key_list.clear();
}

// 読み込み
bool ReplayManager::LoadFromFile(char *file_name,File::IFileManager *pFileManager)
{
	pFileManager->SetCurrentPath( "Text" );
	File::IResourceFile *fp = pFileManager->FileOpen( file_name );
	pFileManager->SetCurrentPath( "Texture" );
//	File::CFile *fp = new File::CFile( file_name  , FILE_OPEN_TYPE_READ );
	if(!fp || fp->IsInvalid() )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	// 4バイトずつ生バイナリで読み込み
	int size;
	if( ( size = fp->GetFileSize())%4 != 0 )
	{
		SAFE_RELEASE(fp);
		return false;
	}

	int i;
	int read_data;
	for( i=0 ; i*4<size ; i++ )
	{
		fp->Read( &read_data , sizeof(int) );
		m_key_list.push_back(read_data);
	}


	SAFE_RELEASE(fp);
	return true;
}

// 読み込みポインタを先頭に持ってくる
void ReplayManager::SetReadPointerToFirst()
{
	m_read_pointer = 0;
}

// 1つデータを読む
int ReplayManager::GetNextKey()
{
	if( (unsigned)m_read_pointer >= m_key_list.size() )
	{
		return -1;
	}
	return m_key_list[m_read_pointer++];
}

// キーを追加
void ReplayManager::AddKey( int key )
{
	m_key_list.push_back(key);
}

// ファイルに保存
bool ReplayManager::SaveToFile( char *file_name )
{
	File::CFile *fp = new File::CFile( file_name  , FILE_OPEN_TYPE_WRITE );
	if( !fp || !fp->IsOpened() )
	{
		delete fp;
		return false;
	}

	vector<int>::iterator it,end = m_key_list.end();
	for( it = m_key_list.begin() ; it != end ; it++ )
	{
		fp->Write( &(*it) , sizeof(int) );
	}

	delete fp;

	return true;
}

// 資源解放
void ReplayManager::Release()
{
	m_read_pointer = 0;
	m_key_list.clear();
}
