/////////////////////////////////////////////////////////////////
// BmpIO.h
// �r�b�g�}�b�v�t�@�C������͂���֐���錾
/////////////////////////////////////////////////////////////////

#if !defined( BMPIO_H_INCLUDED_ )
#define BMPIO_H_INCLUDED_

#include "Selene.h"

using namespace Selene;

#ifdef __cplusplus
extern "C" {
#endif

// �F��ێ�����\����
typedef struct tagInternalColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} ICOLOR;

// �摜�f�[�^��ێ�����\����
typedef struct tagInternalBMP
{
	int width;
	int height;
	int BitPerPix;	// �P�s�N�Z��������̃r�b�g��
	ICOLOR *pColor;	// �J���[�e�[�u���������̓s�N�Z���̃f�[�^
	unsigned char *pPix;	// �s�N�Z���̃f�[�^
} IBMP;

// ���ʂ̃^�X�N
IBMP* BmpIO_CreateBitmap( int width, int height, int BitPerPixcel );
IBMP* BmpIO_Load( File::IResourceFile *infile );
int BmpIO_Save( FILE *outfile, const IBMP *pBmp );
void BmpIO_DeleteBitmap( IBMP *pBmp );
int BmpIO_GetWidth( const IBMP *pBmp );
int BmpIO_GetHeight( const IBMP *pBmp );
int BmpIO_GetBitPerPixcel( const IBMP *pBmp );
unsigned char BmpIO_GetR( int x, int y, const IBMP *pBmp );
unsigned char BmpIO_GetG( int x, int y, const IBMP *pBmp );
unsigned char BmpIO_GetB( int x, int y, const IBMP *pBmp );

// 24bit�r�b�g�}�b�v�p
void BmpIO_SetR( int x, int y, IBMP *pBmp, unsigned char v );
void BmpIO_SetG( int x, int y, IBMP *pBmp, unsigned char v );
void BmpIO_SetB( int x, int y, IBMP *pBmp, unsigned char v );

// 1,4,8bit�r�b�g�}�b�v�p
unsigned char BmpIO_GetColorTableR( int idx, const IBMP *pBmp );
unsigned char BmpIO_GetColorTableG( int idx, const IBMP *pBmp );
unsigned char BmpIO_GetColorTableB( int idx, const IBMP *pBmp );
void BmpIO_SetColorTableR( int idx, const IBMP *pBmp, unsigned char v );
void BmpIO_SetColorTableG( int idx, const IBMP *pBmp, unsigned char v );
void BmpIO_SetColorTableB( int idx, const IBMP *pBmp, unsigned char v );
unsigned char BmpIO_GetPixcel( int x, int y, const IBMP *pBmp );
void BmpIO_SetPixcel( int x, int y, const IBMP *pBmp, unsigned char v );
int BmpIO_TranseTo24BitColor( IBMP *pBmp );

#ifdef __cplusplus
}
#endif


#endif // BMPIO_H_INCLUDED_
