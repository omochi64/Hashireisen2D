
#include "FileManagerSupporter.h"
#include "System/GameSystem.h"

namespace GameSystem
{

namespace Supporter
{

char *fgets_from_fr( char *buf , int max_count , File::IResourceFile *pFileResource )
{
	// ファイルの終端か？
	if( pFileResource->GetFileSize() == pFileResource->GetFilePosition() )
	{
		return NULL;
	}

	int i;
	char read_char;
	for( i=0 ; i<max_count-1 ; i++ )
	{
		if( pFileResource->Read( &buf[i] , sizeof(char) ) < sizeof(char) )
		{
			break;
		}

		if( buf[i] == '\n' || buf[i] == '\r' )
		{
			int pos = pFileResource->GetFilePosition();
			// さらに改行コードが続くか？
			if( pFileResource->Read( &read_char , sizeof(char) ) == sizeof(char) )
			{
				if( read_char == '\r' || read_char == '\n' )
				{
					break;
				}
			}
			pFileResource->Seek( pos );
			break;
		}

	}
	buf[i+1] = '\0';

	return buf;
}

};
};
