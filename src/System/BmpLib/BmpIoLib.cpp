#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

#if !defined( _MSC_VER )
#include <alloca.h>
#endif

#include "BmpIoLib.h"

// ���g���G���f�B�A�����ۂ�
#define ISLITTLEENDIAN 1

// �e��f�[�^�^

#define WORD unsigned short
#define DWORD unsigned int
#define LONG int
#define BYTE unsigned char
#define BOOL int
#define FALSE 0
#define TRUE 1
/*
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} BITMAPINFOHEADER;
*/

// �s�N�Z���̃f�[�^��ǂނ��߂̃o�b�t�@�̍\����
typedef struct tagBuf
{
	union {
		unsigned int buf : 32;
		unsigned char vbuf[4];
	} BufU;
	int buflen;
} BUF;

#define BITMAPFILEHEADER_SIZE ( sizeof( WORD ) * 3 + sizeof( DWORD ) * 2 )
#define BITMAPINFOHEADER_SIZE ( sizeof( WORD ) * 2 + sizeof( DWORD ) * 5 + sizeof( LONG ) * 4 )


static int PixIdx( int w, int h, const IBMP *pBmp );


///////////////////////////////////////////////////////////////////////////////////
// �\�z�E�j��

// �\�z
IBMP* BmpIO_CreateBitmap( int width, int height, int BitPerPixcel )
{
	IBMP *p = NULL;

	// �r�b�g���̎w����m�F����B
	assert( BitPerPixcel == 24 ||
		BitPerPixcel == 8 ||
		BitPerPixcel == 4 ||
		BitPerPixcel == 1 );
	if ( BitPerPixcel != 24 && BitPerPixcel != 8 &&
			BitPerPixcel != 4 && BitPerPixcel != 1 )
		return NULL;

	p = (IBMP*)malloc( sizeof( IBMP ) );
	if ( NULL == p ) return NULL;
	p->pColor = NULL;
	p->pPix = NULL;

	// 24Bit�J���[�̏ꍇ
	if ( 24 == BitPerPixcel ) {
		// p->pPix�͎g�p�����Ap->pColor�Ƀs�N�Z���f�[�^���i�[����B
		p->pColor = (ICOLOR*)calloc( width * height, sizeof( ICOLOR ) );
		if ( NULL == p->pColor ) goto ERR_EXIT;
	}
	else {
		// p->pColor�ɂ̓J���[�e�[�u�����i�[����B
		p->pColor = (ICOLOR*)calloc( ( 1 << BitPerPixcel ), sizeof( ICOLOR ) );
		p->pPix = (unsigned char*)calloc( width * height, sizeof( unsigned char ) );
		if ( NULL == p->pColor || NULL == p->pPix ) goto ERR_EXIT;
	}

	p->width = width;
	p->height = height;
	p->BitPerPix = BitPerPixcel;

	return p;

	// ���s�����ꍇ
ERR_EXIT:
	free( p->pColor );
	free( p->pPix );
	free( p );
	return NULL;
}

// �j��
void BmpIO_DeleteBitmap( IBMP *pBmp )
{
	if ( NULL == pBmp ) return ;
	if ( NULL != pBmp->pPix ) free( pBmp->pPix );
	if ( NULL != pBmp->pColor ) free( pBmp->pColor );
	free( pBmp );
}


int BmpIO_GetWidth( const IBMP *pBmp )
{
	assert( NULL != pBmp );
	return pBmp->width;
}

int BmpIO_GetHeight( const IBMP *pBmp )
{
	assert( NULL != pBmp );
	return pBmp->height;
}

int BmpIO_GetBitPerPixcel( const IBMP *pBmp )
{
	assert( NULL != pBmp );
	return pBmp->BitPerPix;
}

unsigned char BmpIO_GetR( int x, int y, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );

	if ( pBmp->BitPerPix == 24 )
		return pBmp->pColor[ PixIdx( x, y, pBmp ) ].r;
	else
		return BmpIO_GetColorTableR( BmpIO_GetPixcel( x, y, pBmp ), pBmp );
}

unsigned char BmpIO_GetG( int x, int y, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );

	if ( pBmp->BitPerPix == 24 )
		return pBmp->pColor[ PixIdx( x, y, pBmp ) ].g;
	else
		return BmpIO_GetColorTableG( BmpIO_GetPixcel( x, y, pBmp ), pBmp );
}

unsigned char BmpIO_GetB( int x, int y, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );

	if ( pBmp->BitPerPix == 24 )
		return pBmp->pColor[ PixIdx( x, y, pBmp ) ].b;
	else
		return BmpIO_GetColorTableB( BmpIO_GetPixcel( x, y, pBmp ), pBmp );
}


///////////////////////////////////////////////////////////////////////////////////
// 24bit�r�b�g�}�b�v�p

void BmpIO_SetR( int x, int y, IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor && pBmp->BitPerPix == 24 );
	pBmp->pColor[ PixIdx( x, y, pBmp ) ].r = v;
}

void BmpIO_SetG( int x, int y, IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor && pBmp->BitPerPix == 24 );
	pBmp->pColor[ PixIdx( x, y, pBmp ) ].g = v;
}

void BmpIO_SetB( int x, int y, IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor && pBmp->BitPerPix == 24 );
	pBmp->pColor[ PixIdx( x, y, pBmp ) ].b = v;
}


///////////////////////////////////////////////////////////////////////////////////
// 1,4,8bit�r�b�g�}�b�v�p

unsigned char BmpIO_GetColorTableR( int idx, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( idx >= 0 && idx < ( 1 << pBmp->BitPerPix ) );
	idx = idx % ( 1 << pBmp->BitPerPix );
	return pBmp->pColor[ idx ].r;
}

unsigned char BmpIO_GetColorTableG( int idx, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( idx >= 0 && idx < ( 1 << pBmp->BitPerPix ) );
	idx = idx % ( 1 << pBmp->BitPerPix );
	return pBmp->pColor[ idx ].g;
}

unsigned char BmpIO_GetColorTableB( int idx, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( idx >= 0 && idx < ( 1 << pBmp->BitPerPix ) );
	idx = idx % ( 1 << pBmp->BitPerPix );
	return pBmp->pColor[ idx ].b;
}

void BmpIO_SetColorTableR( int idx, const IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( idx >= 0 && idx < ( 1 << pBmp->BitPerPix ) );
	idx = idx % ( 1 << pBmp->BitPerPix );
	pBmp->pColor[ idx ].r = v;
}

void BmpIO_SetColorTableG( int idx, const IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( idx >= 0 && idx < ( 1 << pBmp->BitPerPix ) );
	idx = idx % ( 1 << pBmp->BitPerPix );
	pBmp->pColor[ idx ].g = v;
}

void BmpIO_SetColorTableB( int idx, const IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( idx >= 0 && idx < ( 1 << pBmp->BitPerPix ) );
	idx = idx % ( 1 << pBmp->BitPerPix );
	pBmp->pColor[ idx ].b = v;
}

unsigned char BmpIO_GetPixcel( int x, int y, const IBMP *pBmp )
{
	assert( NULL != pBmp && NULL != pBmp->pColor && NULL != pBmp->pPix );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	return pBmp->pPix[ PixIdx( x, y, pBmp ) ];
}

void BmpIO_SetPixcel( int x, int y, const IBMP *pBmp, unsigned char v )
{
	assert( NULL != pBmp && NULL != pBmp->pColor && NULL != pBmp->pPix );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );
	assert( v < ( 1 << pBmp->BitPerPix ) );
	v = v % ( 1 << pBmp->BitPerPix );
	pBmp->pPix[ PixIdx( x, y, pBmp ) ] = v;
}

int BmpIO_TranseTo24BitColor( IBMP *pBmp )
{
	ICOLOR *wpColor;
	int PixCount;	// ���s�N�Z����
	int i;

	assert( NULL != pBmp );

	// 24Bit�J���[�̃f�[�^���n���ꂽ��A�����������ɐ^��Ԃ�
	if ( 24 == pBmp->BitPerPix ) return TRUE;

	assert( NULL != pBmp->pColor && NULL != pBmp->pPix );
	assert( 1 == pBmp->BitPerPix ||
			4 == pBmp->BitPerPix ||
			8 == pBmp->BitPerPix );

	PixCount = pBmp->width * pBmp->height;

	// 24Bit�J���[�̉摜���i�[���邽�߂̗̈���m��
	wpColor = (ICOLOR*)calloc( PixCount, sizeof( ICOLOR ) );
	if ( NULL == wpColor ) return FALSE;

	// �e�s�N�Z���ɐF��ݒ肷��
	for ( i = 0; i < PixCount; i++ )
		wpColor[i] = pBmp->pColor[ pBmp->pPix[i] ];

	// �\���̂̒��g�������ւ���
	free( pBmp->pColor );
	free( pBmp->pPix );
	pBmp->pColor = wpColor;
	pBmp->pPix = NULL;
	pBmp->BitPerPix = 24;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// ��������

// �C���f�b�N�X�𐶐�
static int PixIdx( int w, int h, const IBMP *pBmp )
{
	assert( w >= 0 && w < pBmp->width && h >= 0 && h <= pBmp->height );
	w = w % pBmp->width;
	h = h % pBmp->height;
	return h * pBmp->width + w;
}


///////////////////////////////////////////////////////////////////////////////////
// ���͗p���[�`��

static BOOL LoadHeader(
	File::IResourceFile *infile, unsigned int *ctsize, int *blen, int *pWidth, int *pHeight
);
static BOOL LoadBody1( File::IResourceFile *infile, int BitLen, IBMP *pBmp );
static BOOL LoadBody24( File::IResourceFile *infile, IBMP *pBmp );
static void FrushBuf_ipt( BUF *pBuf, File::IResourceFile *infile );
static int ReadBuf( BUF *pBuf, int len, File::IResourceFile *infile );
static size_t int_fread( void *buffer, size_t size, size_t count, File::IResourceFile *stream );

// �C���[�W�ǂݍ��ݎw��
IBMP* BmpIO_Load( File::IResourceFile *infile )
{
	unsigned int ctsize;	// �J���[�e�[�u���̃G���g����
	int blen;		// �P�s�N�Z��������r�b�g��
	IBMP *pBmp;
	int w, h;
	BOOL r;

	// �t�@�C���w�b�_�̓��́A�J���[�e�[�u�����A
	// pixels�̗̈撷�A�s�N�Z��������̃r�b�g�����擾
	if ( !LoadHeader( infile, &ctsize, &blen, &w, &h ) ) return FALSE;

	// �������̈���m��
	pBmp = BmpIO_CreateBitmap( w, h, blen );
	if ( NULL == pBmp ) return NULL;

	// �s�N�Z��������r�b�g���ʂɃt�@�C���{�f�B����ǂݍ���
	if ( 24 != blen )
		r = LoadBody1( infile, blen, pBmp );
	else
		r = LoadBody24( infile, pBmp );

	if ( !r ) {
		// ���s
		BmpIO_DeleteBitmap( pBmp );
		return NULL;
	}

	return pBmp;
}

// �w�b�_����ǂ�
static BOOL LoadHeader(
	File::IResourceFile *infile,
	unsigned int *ctsize,
	int *blen,
	int *pWidth,
	int *pHeight
)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bi;

	// �e�\���̂����
	if ( int_fread( &( bfh.bfType ), sizeof( bfh.bfType ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bfh.bfSize ), sizeof( bfh.bfSize ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bfh.bfReserved1 ), sizeof( bfh.bfReserved1 ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bfh.bfReserved2 ), sizeof( bfh.bfReserved2 ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bfh.bfOffBits ), sizeof( bfh.bfOffBits ), 1, infile ) < 1 )
		return FALSE;

	if ( int_fread( &( bi.biSize ), sizeof( bi.biSize ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biWidth ), sizeof( bi.biWidth ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biHeight ), sizeof( bi.biHeight ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biPlanes ), sizeof( bi.biPlanes ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biBitCount ), sizeof( bi.biBitCount ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biCompression ), sizeof( bi.biCompression ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biSizeImage ), sizeof( bi.biSizeImage ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biXPelsPerMeter ), sizeof( bi.biXPelsPerMeter ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biYPelsPerMeter ), sizeof( bi.biYPelsPerMeter ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biClrUsed ), sizeof( bi.biClrUsed ), 1, infile ) < 1 )
		return FALSE;
	if ( int_fread( &( bi.biClrImportant ), sizeof( bi.biClrImportant ), 1, infile ) < 1 )
		return FALSE;
	// �t�@�C���^�C�v���m�F
	if ( bfh.bfType != 0x4D42 ) return FALSE;

	// �ǂݍ��񂾏���Ԃ�
	(*blen) = bi.biBitCount;	// �P�s�N�Z������̃r�b�g��
	(*ctsize) = bi.biClrUsed;	// �g�p�����F�̐�
	(*pWidth) = bi.biWidth;		// ��
	(*pHeight) = bi.biHeight;	// ����

	return TRUE;
}

// �J���[�e�[�u�����g�p����r�b�g�}�b�v��ǂݍ���
static BOOL LoadBody1( File::IResourceFile *infile, int BitLen, IBMP *pBmp )
{
	int i, j;
	BUF buf;	// �ǂݍ��ނ��߂̃o�b�t�@
	int ctsize = ( 1 << BitLen );	// �J���[�e�[�u���̐�
	char temp;

	// �J���[�e�[�u����ǂ�
	int file_size = infile->GetFileSize();
	for ( i = 0; i < ctsize && !(infile->GetFilePosition() > file_size) /*&& !feof( infile )*/; i++ ) {
		infile->Read( &pBmp->pColor[i].b , sizeof(char) );
		infile->Read( &pBmp->pColor[i].g , sizeof(char) );
		infile->Read( &pBmp->pColor[i].r , sizeof(char) );
//		pBmp->pColor[i].g = fgetc( infile );
//		pBmp->pColor[i].r = fgetc( infile );
		infile->Read( &temp , sizeof(char) );	// rgbReserved
	}
	if ( infile->GetFilePosition() > file_size ) return FALSE;

	// �o�b�t�@�̓��e������������
	buf.BufU.buf = 0;
	buf.buflen = 0;
	FrushBuf_ipt( &buf, infile );

	for ( i = 0; i < pBmp->height && !(infile->GetFilePosition() > file_size) /*&& !ferror( infile )*/; i++ ) {
		for ( j = 0; j < pBmp->width && !(infile->GetFilePosition() > file_size) /*&& !ferror( infile )*/; j++ ) {
			int wIdx = PixIdx( j, i, pBmp );
			// �w�肳�ꂽ�r�b�g���̃f�[�^���擾
			pBmp->pPix[ wIdx ] = ReadBuf( &buf, BitLen, infile );
		}
		FrushBuf_ipt( &buf, infile );
	}

	return ( i == pBmp->height && j == pBmp->width );
}

// �Q�S�r�b�g�J���[�r�b�g�}�b�v��ǂ�
static BOOL LoadBody24( File::IResourceFile *infile, IBMP *pBmp )
{
//	int PixCnt = pBmp->width * pBmp->height;	// �s�N�Z����
	int i, j;
	BUF buf;

	// �o�b�t�@�̓��e������������
	buf.buflen = 0;
	buf.BufU.buf = 0;
	FrushBuf_ipt( &buf, infile );

	int file_size = infile->GetFileSize();

	for ( i = 0; i < pBmp->height && !(file_size<infile->GetFilePosition()) /*&& !ferror( infile )*/; i++ ) {
		for ( j = 0; j < pBmp->width && !(file_size<infile->GetFilePosition()) /*&& !ferror( infile )*/; j++ ) {
			// �F��BGR�̏��ɋL�^����Ă���
			BmpIO_SetB( j, i, pBmp, (unsigned char)ReadBuf( &buf, 8, infile ) );
			BmpIO_SetG( j, i, pBmp, (unsigned char)ReadBuf( &buf, 8, infile ) );
			BmpIO_SetR( j, i, pBmp, (unsigned char)ReadBuf( &buf, 8, infile ) );
		}
		FrushBuf_ipt( &buf, infile );
	}
	return ( i == pBmp->height && j == pBmp->width );
}

// �o�b�t�@�Ƀf�[�^��ǂݍ���
static void FrushBuf_ipt( BUF *pBuf, File::IResourceFile *infile )
{
	int i;

#if ISLITTLEENDIAN
	for ( i = 3; i >= 0; i-- )
#else
	for ( i = 0; i < 4; i++ )
#endif
		infile->Read( &pBuf->BufU.vbuf[ i ] , sizeof(char) );
//		pBuf->BufU.vbuf[ i ] = getc( infile );
	pBuf->buflen = 32;
}

// �o�b�t�@����w�肵���r�b�g�����f�[�^���擾����
static int ReadBuf( BUF *pBuf, int len, File::IResourceFile *infile )
{
	int r;
	if ( pBuf->buflen < len ) FrushBuf_ipt( pBuf, infile );
	r = ( ( ( ( 1 << len ) - 1 ) << ( 32 - len ) ) & pBuf->BufU.buf ) >> ( 32 - len );
	pBuf->BufU.buf <<= len;
	pBuf->buflen -= len;
	return r;
}

// �G���f�B�A���l�X���z�����āA�t�@�C����ǂݍ���
size_t int_fread( void *buffer, size_t size, size_t count, File::IResourceFile *stream )
{
#if ISLITTLEENDIAN
	return stream->Read( buffer, size*count );
#else
	size_t i, j;
	size_t r;
	char *cbuf = (char*)buffer;

	// �ǂݍ���
	r = stream->Read( buffer, size*count );
	if ( 1 == size ) return r;

	// ���ڂ��ƂɃo�C�g�I�[�_�[�𔽓]
	for ( i = 0; i < count; i++ ) {
		for ( j = 0; j < size / 2; j++ ) {
			int idx1 = i * size + j;
			int idx2 = i * size + ( size - j - 1 );
			char c = cbuf[ idx1 ];
			cbuf[ idx1 ] = cbuf[ idx2 ];
			cbuf[ idx2 ] = c;
		}
	}
	return r;
#endif
}

///////////////////////////////////////////////////////////////////////////////////
// �o�͗p���[�`��

static BOOL WriteHeader( FILE *outfile, const IBMP *pBmp );
static BOOL WriteBody1( FILE *outfile, const IBMP *pBmp );
static BOOL WriteBody24( FILE *outfile, const IBMP *pBmp );
static void FrushBuf_opt( BUF *pBuf, FILE *outfile );
static void WriteBuf( BUF *pBuf, int BitLen, unsigned char c, FILE *outfile );
static size_t int_fwrite( const void *buffer, size_t size, size_t count, FILE *stream );

// �C���[�W���o�͂���
BOOL BmpIO_Save( FILE *outfile, const IBMP *pBmp )
{
	// �w�b�_���o�͂���
	if ( !WriteHeader( outfile, pBmp ) ) return FALSE;

	// �C���[�W�̃f�[�^���o�͂���
	if ( 24 == pBmp->BitPerPix )
		return WriteBody24( outfile, pBmp );
	else
		return WriteBody1( outfile, pBmp );
}

// �w�b�_�����o�͂���
static BOOL WriteHeader( FILE *outfile, const IBMP *pBmp )
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bi;

	// �\���̂ɒl��ݒ�
	bfh.bfType = 0x4D42;
	bfh.bfSize = 0;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE;
      if ( pBmp->BitPerPix <= 8 )
		bfh.bfOffBits += 4 * ( 1 << pBmp->BitPerPix );
	bi.biSize = BITMAPINFOHEADER_SIZE;
	bi.biWidth = pBmp->width;
	bi.biHeight = pBmp->height;
	bi.biPlanes = 1;
	bi.biBitCount = pBmp->BitPerPix;
	bi.biCompression = 0L;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 1;
	bi.biYPelsPerMeter = 1;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// �e�\���̂��o��
	if ( int_fwrite( &(bfh.bfType), sizeof( bfh.bfType ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bfh.bfSize), sizeof( bfh.bfSize ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bfh.bfReserved1), sizeof( bfh.bfReserved1 ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bfh.bfReserved2), sizeof( bfh.bfReserved2 ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bfh.bfOffBits), sizeof( bfh.bfOffBits ), 1, outfile ) < 1 )
		return FALSE;

	if ( int_fwrite( &(bi.biSize), sizeof( bi.biSize ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biWidth), sizeof( bi.biWidth ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biHeight), sizeof( bi.biHeight ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biPlanes), sizeof( bi.biPlanes ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biBitCount), sizeof( bi.biBitCount ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biCompression), sizeof( bi.biCompression ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biSizeImage), sizeof( bi.biSizeImage ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biXPelsPerMeter), sizeof( bi.biXPelsPerMeter ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biYPelsPerMeter), sizeof( bi.biYPelsPerMeter ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biClrUsed), sizeof( bi.biClrUsed ), 1, outfile ) < 1 )
		return FALSE;
	if ( int_fwrite( &(bi.biClrImportant), sizeof( bi.biClrImportant ), 1, outfile ) < 1 )
		return FALSE;

	return TRUE;
}

// 1,4,8bit�r�b�g�}�b�v���o�͂���
BOOL WriteBody1( FILE *outfile, const IBMP *pBmp )
{
	int i, j;
	int ctcnt = 1 << pBmp->BitPerPix;	// �J���[�e�[�u���̐�
	BUF buf;

	// �J���[�e�[�u�����o�͂���
	for ( i = 0; i < ctcnt && !ferror( outfile ); i++ ) {
		fputc( pBmp->pColor[i].b, outfile );
		fputc( pBmp->pColor[i].g, outfile );
		fputc( pBmp->pColor[i].r, outfile );
		fputc( 0, outfile );
	}

	buf.BufU.buf = 0;
	buf.buflen = 0;

	// �s�N�Z���f�[�^���o��
	for ( i = 0; i < pBmp->height; i++ ) {
		for ( j = 0; j < pBmp->width; j++ ) {
			WriteBuf( &buf, pBmp->BitPerPix, BmpIO_GetPixcel( j, i, pBmp ), outfile );
		}
		FrushBuf_opt( &buf, outfile );
	}
	return TRUE;
}

// �Q�S�r�b�g�J���[�r�b�g�}�b�v���o�͂���
static BOOL WriteBody24( FILE *outfile, const IBMP *pBmp )
{
	int i, j;
	BUF buf;

	buf.BufU.buf = 0;
	buf.buflen = 0;

	// �R�o�C�g���o�͂���
	for ( i = 0; i < pBmp->height; i++ ){
		for ( j = 0; j < pBmp->width; j++ ) {
			WriteBuf( &buf, 8, BmpIO_GetB( j, i, pBmp ), outfile );
			WriteBuf( &buf, 8, BmpIO_GetG( j, i, pBmp ), outfile );
			WriteBuf( &buf, 8, BmpIO_GetR( j, i, pBmp ), outfile );
		}
		FrushBuf_opt( &buf, outfile );
	}
	return TRUE;
}

// �o�b�t�@�̃f�[�^��S�ďo�͂���
static void FrushBuf_opt( BUF *pBuf, FILE *outfile )
{
	int i;
	if ( 0 == pBuf->buflen ) return ;

#if ISLITTLEENDIAN
	for ( i = 3; i >= 0; i-- )
#else
	for ( i = 0; i < 4; i++ )
#endif
		putc( pBuf->BufU.vbuf[ i ], outfile );

	pBuf->buflen = 0;
	pBuf->BufU.buf = 0;
}

// �o�b�t�@�Ɏw�肵���r�b�g�����f�[�^���o�͂���
static void WriteBuf( BUF *pBuf, int BitLen, unsigned char c, FILE *outfile )
{
	const unsigned long MASK = ( 0x1 << ( BitLen + 1 ) ) - 1;
	pBuf->BufU.buf |= ( ( (unsigned long)c ) & MASK ) << ( 32 - ( pBuf->buflen + BitLen ) );
	pBuf->buflen += BitLen;
	if ( pBuf->buflen >= 32 )
		FrushBuf_opt( pBuf, outfile );
}

// �G���f�B�A���l�X���z�����āA�t�@�C�����o�͂���
size_t int_fwrite( const void *buffer, size_t size, size_t count, FILE *stream )
{
#if ISLITTLEENDIAN
	return fwrite( buffer, size, count, stream );
#else
	size_t i, j;
	char *cbuf = (char*)buffer;
	char *wbuf = (char*)alloca( size );
	size_t r;

	// 1�o�C�g�P�ʂȂ�΁A���]����K�v�͂Ȃ�
	if ( 1 == size )
		return fwrite( buffer, size, count, stream );

	// ���ڂ��ƂɃo�C�g�I�[�_�[�𔽓]���ďo�͂���
	r = 0;
	for ( i = 0; i < count; i++ ) {
		for ( j = 0; j < size; j++ ) {
			int idx1 = i * size + j;
			int idx2 = i * size + ( size - j - 1 );
			wbuf[ idx2 ] = cbuf[ idx1 ];
		}
		if ( fwrite( wbuf, size, 1, stream ) < 1 )
			return r;
		r++;
	}
	return r;
#endif
}
