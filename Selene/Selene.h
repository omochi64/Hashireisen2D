#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------
#include <windows.h>


//-----------------------------------------------------------------------------------
// WARNING
//-----------------------------------------------------------------------------------
#pragma inline_depth ( 255 )
#pragma inline_recursion ( on )

#pragma warning ( disable: 4100 )
#pragma warning ( disable: 4819 )
#pragma warning ( disable: 4201 )
#pragma warning ( disable: 4239 )
#pragma warning ( disable: 4995 )
#pragma warning ( disable: 4996 )
#pragma warning ( disable: 4324 )


//-----------------------------------------------------------------------------------
// LIBRARY
//-----------------------------------------------------------------------------------
#pragma comment( lib, "ole32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "comdlg32.lib" )
#pragma comment( lib, "advapi32.lib" )


//-----------------------------------------------------------------------------------
// TYPEDEF
//-----------------------------------------------------------------------------------
typedef char					Sint8;					///< signed char 型の別定義
typedef short					Sint16;					///< signed short 型の別定義
typedef long					Sint32;					///< signed long 型の別定義
typedef long long				Sint64;					///< signed long 型の別定義
typedef unsigned char			Uint8;					///< unsigned char 型の別定義
typedef unsigned short			Uint16;					///< unsigned short 型の別定義
typedef unsigned long			Uint32;					///< unsigned long 型の別定義
typedef unsigned long long		Uint64;					///< unsigned long long 型の別定義
typedef float					Float;					///< float 型の別定義
typedef double					Double;					///< double 型の別定義
typedef long					Bool;					///< bool 型の別定義


//-----------------------------------------------------------------------------------
// DEFINE
//-----------------------------------------------------------------------------------
#define ef									else if															///< else if 簡略用マクロ

#define PI									(3.141592653589793238462643383279f)								///< π
#define PI2									(6.283185307179586476925286766559f)								///< 2π
#define REV(val)							(1.0f/toF(val))													///< 逆数算出マクロ

#define AXIS_RANGE_MAX						(4096)															///< ジョイスティックの軸の最大値

#define toF(val)							((Float)(val))													///< floatへのキャスト
#define toI(val)							((Sint32)(val))													///< intへのキャスト

#define MIN(N1,N2)							((N1 < N2) ? (N1) : (N2))										///< 最小値取得マクロ
#define MAX(N1,N2)							((N1 > N2) ? (N1) : (N2))										///< 最大値取得マクロ
#define SWAP(N1,N2)							{ N1 = N2 - N1; N2 -= N1; N1 += N2; }							///< 値交換マクロ

#define SAFE_ADDREF(val)					if ( (val) != NULL ) { (val)->AddRef(); }						///< COM安全AddRefマクロ
#define SAFE_RELEASE(val)					if ( (val) != NULL ) { (val)->Release(); (val) = NULL; }		///< COM安全Releaseマクロ
#define SAFE_DELETE(val)					if ( (val) != NULL ) { delete (val); (val) = NULL; }			///< newメモリ安全解放
#define SAFE_DELETE_ARRAY(val)				if ( (val) != NULL ) { delete [] (val); (val) = NULL; }			///< new[]メモリ安全解放

#define ANGLE_TABLE_BIT						(16)															///< サインテーブルのビット数
#define ANGLE_MAX							(1 << ANGLE_TABLE_BIT)											///< サインテーブルのサイズ
#define ANGLE_TABLE_MASK					(ANGLE_MAX - 1)													///< サインテーブルのアクセス用マスク
#define NORMALIZE_ANGLE(val)				((val) & ANGLE_TABLE_MASK)										///< 角度の正規化
#define DEG_TO_ANGLE(val)					toI(toF(val) * toF(ANGLE_MAX) / 360.0f)							///< 度数法から専用角度
#define ANGLE_TO_DEG(val)					((val) * 360 / ANGLE_MAX)										///< 度数法から専用角度
#define PI_TO_ANGLE(val)					toI(toF(val) * toF(ANGLE_MAX) / PI2)							///< 弧度法から専用角度
#define ANGLE_TO_PI(val)					(toF(val) * PI2 / toF(ANGLE_MAX))								///< 専用角度から弧度法

#define MemoryClear(PD,S)					::FillMemory((PD),(S),0x00)										///< メモリクリア
#define MemoryFill(PD,PS,S)					::FillMemory((PD),(S),(PS))										///< メモリ塗りつぶし
#define MemoryMove(PD,PS,S)					::MoveMemory((PD),(PS),(S))										///< メモリ移動
#define MemoryCopy(PD,PS,S)					::CopyMemory((PD),(PS),(S))										///< メモリコピー


//-----------------------------------------------------------------------------------
// DEFINE
//-----------------------------------------------------------------------------------
#ifdef LIB_SELENE
#	define SELENE_DLL_API
#else // LIB_SELENE
#	ifdef DLL_SELENE
#		define SELENE_DLL_API	__declspec(dllexport)
#	else // DLL_SELENE
#		define SELENE_DLL_API	__declspec(dllimport)
#	endif // DLL_SELENE
#endif // LIB_SELENE


//-----------------------------------------------------------------------------------
// ENUM
//-----------------------------------------------------------------------------------
/**
	@brief ライブラリ名前空間
*/
namespace Selene
{
	/**
		@brief 頂点定義
		@author 葉迩倭

		頂点定義クラスの作成時に指定するフラグです。<BR>
		なお VERTEX_ELEMENT_PRIMITIVE、VERTEX_ELEMENT_SPRITE、VERTEX_ELEMENT_3DBASE は<BR>
		独立しており排他です。
	*/
	enum eVertexElement
	{
		VERTEX_ELEMENT_PRIMITIVE	= 1 << 0,			///< 位置と色だけを含む最も単純な2D用頂点
		VERTEX_ELEMENT_SPRITE		= 1 << 1,			///< 位置と色とテクスチャUVを含む2D用頂点
		VERTEX_ELEMENT_3DBASE		= 1 << 2,			///< 位置と色を含むもっとも単純な3D頂点
		VERTEX_ELEMENT_3DTEXTURE	= 1 << 3,			///< テクスチャUVを含む基本的な3D用頂点
		VERTEX_ELEMENT_3DLIGHT		= 1 << 4,			///< ライティングに関する法線を含む3D頂点
		VERTEX_ELEMENT_3DBUMP		= 1 << 5,			///< バンプマップに関する接線を含む3D頂点
		VERTEX_ELEMENT_3DANIMATION	= 1 << 6,			///< スキニングに関するボーンのインデックス＆ウェイトを含む3D頂点
	};

	/**
		@brief IParticle用タイプ
		@author 葉迩倭
	*/
	enum eParticleType
	{
		PARTICLE_TYPE_NORMAL,				///< 通常パーティクル
		PARTICLE_TYPE_VOLUME,				///< 疑似ボリュームパーティクル
	};

	/**
		@brief フレームレート定義
		@author 葉迩倭
	*/
	enum eFrameRate
	{
		FRAME_RATE_60,					///< 1フレーム1/60秒
		FRAME_RATE_30,					///< 1フレーム1/30秒
		FRAME_RATE_20,					///< 1フレーム1/20秒
		FRAME_RATE_15,					///< 1フレーム1/15秒
	};

	/**
		@brief アダプタータイプ
		@author 葉迩倭
	*/
	enum eGraphicCardNo
	{
		GRAPHIC_CARD_NO1,									///< １番目に接続されたビデオカード
		GRAPHIC_CARD_NO2,									///< ２番目に接続されたビデオカード

		GRAPHIC_CARD_DEFAULT_NO		= GRAPHIC_CARD_NO1,		///< デフォルトのビデオカード
		GRAPHIC_CARD_NV_PERF_HUD	= 0xFFFFFFFF,			///< NVPerfHUD専用のビデオカード
	};

	/**
		@brief サーフェイスフォーマット定義
		@author 葉迩倭
	*/
	enum eSurfaceFormat
	{
		FORMAT_INVALID,						///< 無効

		FORMAT_TEXTURE_32BIT,				///< 32Bitテクスチャフォーマットに準拠
		FORMAT_TEXTURE_16BIT,				///< 16Bitテクスチャフォーマットに準拠
		FORMAT_TEXTURE_DXT,					///< DXT圧縮フォーマットテクスチャ

		FORMAT_TARGET_UCHAR4,				///< unsnged char x4 レンダリングターゲット
		FORMAT_TARGET_FLOAT1,				///< float x1 レンダリングターゲット
		FORMAT_TARGET_FLOAT2,				///< float x2 レンダリングターゲット
		FORMAT_TARGET_FLOAT4,				///< float x4 レンダリングターゲット
		FORMAT_TARGET_FLOAT4_ALPHA,			///< float x4 アルファ可能レンダリングターゲット

		FORMAT_DEPTHBUFFER_SURFACE,			///< 深度バッファ
		FORMAT_DEPTHBUFFER_TEXTURE,			///< 深度バッファ

		FORMAT_MAX,

		FORMAT_TARGET_32BIT		= FORMAT_TARGET_UCHAR4,
		FORMAT_TARGET_HDR		= FORMAT_TARGET_FLOAT4_ALPHA,
	};

	/**
		@brief テクスチャステージ定義
		@author 葉迩倭

		テクスチャステージに関しての定義です。<BR>
		3Dオブジェクトに関しては基本的にシェーダー内で<BR>
		UVを計算するようになっているので、<BR>
		頂点データとして保持するUVはカラーマップとライトマップだけです。
	*/
	enum eTextureStage
	{
		TEXTURE_STAGE_COLOR			= 0,	///< カラーマップ
		TEXTURE_STAGE_LIGHT,				///< ライトマップ
		TEXTURE_STAGE_ENVIRONMENT,			///< キューブ環境
		TEXTURE_STAGE_SPECULAR,				///< スペキュラマップ
		TEXTURE_STAGE_NORMAL,				///< バンプor視差マップ用法線マップ

		TEXTURE_STAGE_SHADOW,				///< シャドウマップ
		TEXTURE_STAGE_DEPTH,				///< 深度マップ

		TEXTURE_STAGE_TOON,					///< トゥーン用特殊マップ

		TEXTURE_STAGE_MAX,					///< テクスチャステージ数

		MATERIAL_TEXTURE_MAX = TEXTURE_STAGE_NORMAL + 1,
	};

	/**
		@brief 描画タイプ定義
		@author 葉迩倭

		各種描画タイプに関しての定義です。
	*/
	enum eDrawType
	{
		DRAW_TYPE_NORMAL,					///< 通常描画
		DRAW_TYPE_BLEND,					///< 半透明描画
		DRAW_TYPE_ADD,						///< 加算描画
		DRAW_TYPE_ADD_NOALPHA,				///< 加算描画（アルファ値無効）
		DRAW_TYPE_SUB,						///< 減算描画
		DRAW_TYPE_SUB_NOALPHA,				///< 減算描画（アルファ値無効）
		DRAW_TYPE_MULTIPLE,					///< 乗算描画
	};

	/**
		@brief カリングタイプ定義
		@author 葉迩倭

		ポリゴンの表裏に描画に関しての定義です。
	*/
	enum eCullType
	{
		CULL_FRONT,							///< 表の場合のみ描画
		CULL_BACK,							///< 裏の場合のみ描画
		CULL_NONE,							///< 裏表両面描画
	};

	/**
		@brief テクスチャフィルタタイプ定義
		@author 葉迩倭

		ポリゴンの表裏に描画に関しての定義です。
	*/
	enum eTextureFilterType
	{
		TEXTURE_FILTER_DISABLE,		///< フィルタリング無し
		TEXTURE_FILTER_2D,			///< バイリニアフィルタリング
		TEXTURE_FILTER_3D_LOW,		///< バイリニアフィルタリング
		TEXTURE_FILTER_3D_MIDDLE,	///< 異方性フィルタリング/低画質
		TEXTURE_FILTER_3D_HIGH,		///< 異方性フィルタリング/高画質
	};

	/**
		@brief ファイルシーク定義
		@author 葉迩倭
	*/
	enum eSeekFlag
	{
		SEEK_FILE_CURRENT,					///< カレント位置
		SEEK_FILE_START,					///< 先頭位置
		SEEK_FILE_END,						///< 終端位置
	};

	/**
		@brief 頂点変換タイプ
		@author 葉迩倭
	*/
	enum eTransformType
	{
		TRANSFORM_TYPE_MODEL,				///< モデル用頂点変換
		TRANSFORM_TYPE_BILLBOARD,			///< ビルボード用頂点変換
	};

	/**
		@brief バンプタイプ
		@author 葉迩倭
	*/
	enum eBumpType
	{
		BUMP_TYPE_NONE,						///< バンプ処理なし
		BUMP_TYPE_ENABLE,					///< バンプ処理あり
	};

	/**
		@brief スペキュラタイプ
		@author 葉迩倭
	*/
	enum eSpecularType
	{
		SPECULAR_TYPE_NONE,					///< スペキュラ処理なし
		SPECULAR_TYPE_ENABLE,				///< スペキュラ処理あり
	};

	/**
		@brief モーションブラーレベル
		@author 葉迩倭
	*/
	enum eSceneMotionBlurQuarity
	{
		MOTION_BLUR_LIGHT,						///< モデル引き延ばしを行わない軽量なブラー
		MOTION_BLUR_FULL,						///< モデル引き延ばしを行うブラー

		MOTION_BLUR_MAX
	};

	/**
		@brief 被写界深度タイプ
		@author 葉迩倭
	*/
	enum eSceneDepthOfFieldType
	{
		DOF_TYPE_DISABLE,						///< 被写界深度なし
		DOF_TYPE_ENABLE,						///< 被写界深度あり
	};

	/**
		@brief フォグタイプ
		@author 葉迩倭
	*/
	enum eSceneFogType
	{
		FOG_TYPE_DISABLE,						///< フォグなし
		FOG_TYPE_ENABLE,						///< フォグあり
	};

	/**
		@brief アンチエイリアスタイプ
		@author 葉迩倭
	*/
	enum eSceneAntiAliasType
	{
		AA_TYPE_DISABLE,						///< アンチエイリアスなし
		AA_TYPE_ENABLE,							///< アンチエイリアスあり
	};

	/**
		@brief フォグエフェクト
		@author 葉迩倭
	*/
	enum eSceneFogEffect
	{
		FOG_EFFECT_LINEAR,						///< 線形フォグ
//		FOG_EFFECT_SCATTERING,					///< スキャッタリング
	};

	/**
		@brief シェーディングタイプ
		@author 葉迩倭
	*/
	enum eSceneShadingType
	{
		SHADING_TYPE_NORMAL,					///< 通常のシェーディング
		SHADING_TYPE_TOON,						///< トゥーン風シェーディング
		SHADING_TYPE_HATCHING,					///< ハッチング風シェーディング
	};

	/**
		@brief シーン用モーションブラータイプ
		@author 葉迩倭
	*/
	enum eSceneMotionBlurType
	{
		MOTION_BLUR_TYPE_DISABLE,				///< なし
		MOTION_BLUR_TYPE_LOW,					///< クオリティ低：４サンプリング
		MOTION_BLUR_TYPE_HIGH,					///< クオリティ高：８サンプリング
	};

	/**
		@brief シーン用シャドウマップタイプ
		@author 葉迩倭
	*/
	enum eSceneShadowType
	{
		SHADOW_TYPE_DISABLE,				///< 影なし
		SHADOW_TYPE_PROJECTION,				///< プロジェクションシャドウ
		SHADOW_TYPE_SHADOWMAP,				///< シャドウマッピング
	};

	/**
		@brief シーン用シャドウクオリティタイプ
		@author 葉迩倭
	*/
	enum eSceneShadowQuarity
	{
		SHADOW_QUARITY_LOW,					///< 低解像度(512x512)
		SHADOW_QUARITY_MIDDLE,				///< 低解像度(1024x1024)
		SHADOW_QUARITY_HIGH,				///< 低解像度(2048x2048)
		SHADOW_QUARITY_BEST,				///< 低解像度(4096x4096)
	};

	/**
		@brief HDR処理
		@author 葉迩倭
	*/
	enum eSceneHighDynamicRangeType
	{
		HDR_TYPE_DISABLE,						///< HDR処理なし
		HDR_TYPE_ENABLE,						///< HDR処理あり
	};

	/**
		@brief HDRエフェクト
		@author 葉迩倭
	*/
	enum eSceneHighDynamicRangeEffect
	{
		HDR_EFFECT_BLOOM,						///< ブルーム処理
		HDR_EFFECT_CROSS,						///< クロスフィルタ
	};

	/**
		@brief 投影シャドウ用種類
		@author 葉迩倭
	*/
	enum eProjectionShadowType
	{
		PROJECTION_SHADOW_DISABLE,				///< 影を落とさず、影の影響もない
		PROJECTION_SHADOW_DROP_SIMPLE,			///< 丸影を落とし、影の影響は受けない
		PROJECTION_SHADOW_DROP_SHAPE,			///< モデルの形状の影を落とし、影の影響は受けない
	};

	/**
		@brief 投影シャドウ用プライオリティ
		@author 葉迩倭
	*/
	enum eProjectionShadowPriority
	{
		PROJECTION_SHADOW_PRIORITY_0,			///< 影を落とさない
		PROJECTION_SHADOW_PRIORITY_1,			///< PROJECTION_SHADOW_PRIORITY_0以下に影を落とす
		PROJECTION_SHADOW_PRIORITY_2,			///< PROJECTION_SHADOW_PRIORITY_1以下に影を落とす
		PROJECTION_SHADOW_PRIORITY_3,			///< PROJECTION_SHADOW_PRIORITY_2以下に影を落とす
		PROJECTION_SHADOW_PRIORITY_4,			///< PROJECTION_SHADOW_PRIORITY_3以下に影を落とす

		PROJECTION_SHADOW_PRIORITY_MAX,
	};

	/**
		@brief ボタン状態
		@author 葉迩倭
	*/
	enum eInputButtonState
	{
		BUTTON_STATE_FREE,					///< 押していない状態取得用
		BUTTON_STATE_HOLD,					///< 押し続けている状態取得用
		BUTTON_STATE_PUSH,					///< 押した瞬間取得用
		BUTTON_STATE_PULL,					///< 離した瞬間取得用

		BUTTON_STATE_MAX,
	};

	/**
		@brief 軸定義
		@author 葉迩倭
	*/
	enum eInputAxisType
	{
		AXIS_TYPE_01,						///< 1番目の軸
		AXIS_TYPE_02,						///< 2番目の軸
		AXIS_TYPE_03,						///< 3番目の軸
		AXIS_TYPE_04,						///< 4番目の軸

		AXIS_TYPE_MAX,
	};

	/**
		@brief ボタン種類
		@author 葉迩倭
	*/
	enum eInputButtonType
	{
		BUTTON_AXIS1_UP,					///< 上方向指定
		BUTTON_AXIS1_DOWN,					///< 下方向指定
		BUTTON_AXIS1_LEFT,					///< 左方向指定
		BUTTON_AXIS1_RIGHT,					///< 右方向指定

		BUTTON_AXIS2_UP,					///< 上方向指定
		BUTTON_AXIS2_DOWN,					///< 下方向指定
		BUTTON_AXIS2_LEFT,					///< 左方向指定
		BUTTON_AXIS2_RIGHT,					///< 右方向指定

		BUTTON_AXIS3_UP,					///< 上方向指定
		BUTTON_AXIS3_DOWN,					///< 下方向指定
		BUTTON_AXIS3_LEFT,					///< 左方向指定
		BUTTON_AXIS3_RIGHT,					///< 右方向指定

		BUTTON_AXIS4_UP,					///< 上方向指定
		BUTTON_AXIS4_DOWN,					///< 下方向指定
		BUTTON_AXIS4_LEFT,					///< 左方向指定
		BUTTON_AXIS4_RIGHT,					///< 右方向指定

		BUTTON_01,							///< ボタン1指定
		BUTTON_02,							///< ボタン2指定
		BUTTON_03,							///< ボタン3指定
		BUTTON_04,							///< ボタン4指定
		BUTTON_05,							///< ボタン5指定
		BUTTON_06,							///< ボタン6指定
		BUTTON_07,							///< ボタン7指定
		BUTTON_08,							///< ボタン8指定
		BUTTON_09,							///< ボタン9指定
		BUTTON_10,							///< ボタン10指定
		BUTTON_11,							///< ボタン11指定
		BUTTON_12,							///< ボタン12指定
		BUTTON_13,							///< ボタン13指定
		BUTTON_14,							///< ボタン14指定
		BUTTON_15,							///< ボタン15指定
		BUTTON_16,							///< ボタン16指定

		BUTTON_AXIS_MAX,
		BUTTON_MAX		= BUTTON_16 - BUTTON_01,
		AXIS_MAX		= BUTTON_AXIS4_RIGHT - BUTTON_AXIS1_UP,

		BUTTON_DISABLE,						///< 無効
	};

	/**
		@brief ジョイスティック軸方向定義
		@author 葉迩倭
	*/
	enum ePadAxisDirection
	{
		PAD_AXIS_X,							///< X軸
		PAD_AXIS_Y,							///< Y軸
		PAD_AXIS_Z,							///< Z軸

		PAD_AXIS_DIRECTION_MAX,
	};

	/**
		@brief ジョイスティック軸種類定義
		@author 葉迩倭
	*/
	enum ePadAxisType
	{
		PAD_AXIS_POSITION,
		PAD_AXIS_POSITION_ROTATE,
		PAD_AXIS_VELOCITY,
		PAD_AXIS_VELOCITY_ROTATE,
		PAD_AXIS_ACCEL,
		PAD_AXIS_ACCEL_ROTATE,
		PAD_AXIS_FORCE,
		PAD_AXIS_FORCE_ROTATE,
		PAD_AXIS_POV,

		PAD_AXIS_MAX,
	};

	/**
		@brief ジョイスティックスライダー定義
		@author 葉迩倭
	*/
	enum ePadSliderType
	{
		PAD_SLIDER_0,							///< 1番目のスライダー
		PAD_SLIDER_1,							///< 2番目のスライダー
		PAD_SLIDER_2,							///< 3番目のスライダー
		PAD_SLIDER_3,							///< 4番目のスライダー

		PAD_SLIDER_MAX,
	};

	/**
		@brief ジョイスティックボタン定義
		@author 葉迩倭
	*/
	enum ePadButtonType
	{
		PAD_BUTTON_01,
		PAD_BUTTON_02,
		PAD_BUTTON_03,
		PAD_BUTTON_04,
		PAD_BUTTON_05,
		PAD_BUTTON_06,
		PAD_BUTTON_07,
		PAD_BUTTON_08,
		PAD_BUTTON_09,
		PAD_BUTTON_10,
		PAD_BUTTON_11,
		PAD_BUTTON_12,
		PAD_BUTTON_13,
		PAD_BUTTON_14,
		PAD_BUTTON_15,
		PAD_BUTTON_16,

		PAD_BUTTON_MAX,
	};

	/**
		@brief 仮想キーコード
		@author 葉迩倭
	*/
	enum eVirtualKeyCode
	{
		SELENE_VK_ESCAPE          = 0x01,
		SELENE_VK_1               = 0x02,
		SELENE_VK_2               = 0x03,
		SELENE_VK_3               = 0x04,
		SELENE_VK_4               = 0x05,
		SELENE_VK_5               = 0x06,
		SELENE_VK_6               = 0x07,
		SELENE_VK_7               = 0x08,
		SELENE_VK_8               = 0x09,
		SELENE_VK_9               = 0x0A,
		SELENE_VK_0               = 0x0B,
		SELENE_VK_MINUS           = 0x0C,
		SELENE_VK_EQUALS          = 0x0D,
		SELENE_VK_BACK            = 0x0E,
		SELENE_VK_TAB             = 0x0F,
		SELENE_VK_Q               = 0x10,
		SELENE_VK_W               = 0x11,
		SELENE_VK_E               = 0x12,
		SELENE_VK_R               = 0x13,
		SELENE_VK_T               = 0x14,
		SELENE_VK_Y               = 0x15,
		SELENE_VK_U               = 0x16,
		SELENE_VK_I               = 0x17,
		SELENE_VK_O               = 0x18,
		SELENE_VK_P               = 0x19,
		SELENE_VK_LBRACKET        = 0x1A,
		SELENE_VK_RBRACKET        = 0x1B,
		SELENE_VK_RETURN          = 0x1C,
		SELENE_VK_LCONTROL        = 0x1D,
		SELENE_VK_A               = 0x1E,
		SELENE_VK_S               = 0x1F,
		SELENE_VK_D               = 0x20,
		SELENE_VK_F               = 0x21,
		SELENE_VK_G               = 0x22,
		SELENE_VK_H               = 0x23,
		SELENE_VK_J               = 0x24,
		SELENE_VK_K               = 0x25,
		SELENE_VK_L               = 0x26,
		SELENE_VK_SEMICOLON       = 0x27,
		SELENE_VK_APOSTROPHE      = 0x28,
		SELENE_VK_GRAVE           = 0x29,
		SELENE_VK_LSHIFT          = 0x2A,
		SELENE_VK_BACKSLASH       = 0x2B,
		SELENE_VK_Z               = 0x2C,
		SELENE_VK_X               = 0x2D,
		SELENE_VK_C               = 0x2E,
		SELENE_VK_V               = 0x2F,
		SELENE_VK_B               = 0x30,
		SELENE_VK_N               = 0x31,
		SELENE_VK_M               = 0x32,
		SELENE_VK_COMMA           = 0x33,
		SELENE_VK_PERIOD          = 0x34,
		SELENE_VK_SLASH           = 0x35,
		SELENE_VK_RSHIFT          = 0x36,
		SELENE_VK_MULTIPLY        = 0x37,
		SELENE_VK_LMENU           = 0x38,
		SELENE_VK_SPACE           = 0x39,
		SELENE_VK_CAPITAL         = 0x3A,
		SELENE_VK_F1              = 0x3B,
		SELENE_VK_F2              = 0x3C,
		SELENE_VK_F3              = 0x3D,
		SELENE_VK_F4              = 0x3E,
		SELENE_VK_F5              = 0x3F,
		SELENE_VK_F6              = 0x40,
		SELENE_VK_F7              = 0x41,
		SELENE_VK_F8              = 0x42,
		SELENE_VK_F9              = 0x43,
		SELENE_VK_F10             = 0x44,
		SELENE_VK_NUMLOCK         = 0x45,
		SELENE_VK_SCROLL          = 0x46,
		SELENE_VK_NUMPAD7         = 0x47,
		SELENE_VK_NUMPAD8         = 0x48,
		SELENE_VK_NUMPAD9         = 0x49,
		SELENE_VK_SUBTRACT        = 0x4A,
		SELENE_VK_NUMPAD4         = 0x4B,
		SELENE_VK_NUMPAD5         = 0x4C,
		SELENE_VK_NUMPAD6         = 0x4D,
		SELENE_VK_ADD             = 0x4E,
		SELENE_VK_NUMPAD1         = 0x4F,
		SELENE_VK_NUMPAD2         = 0x50,
		SELENE_VK_NUMPAD3         = 0x51,
		SELENE_VK_NUMPAD0         = 0x52,
		SELENE_VK_DECIMAL         = 0x53,
		SELENE_VK_OEM_102         = 0x56,
		SELENE_VK_F11             = 0x57,
		SELENE_VK_F12             = 0x58,
		SELENE_VK_KANA            = 0x70,
		SELENE_VK_ABNT_C1         = 0x73,
		SELENE_VK_CONVERT         = 0x79,
		SELENE_VK_NOCONVERT       = 0x7B,
		SELENE_VK_YEN             = 0x7D,
		SELENE_VK_ABNT_C2         = 0x7E,
		SELENE_VK_NUMPADEQUALS    = 0x8D,
		SELENE_VK_PREVTRACK       = 0x90,
		SELENE_VK_NUMPADENTER     = 0x9C,
		SELENE_VK_RCONTROL        = 0x9D,
		SELENE_VK_NUMPADCOMMA     = 0xB3,
		SELENE_VK_DIVIDE          = 0xB5,
		SELENE_VK_SYSRQ           = 0xB7,
		SELENE_VK_RMENU           = 0xB8,
		SELENE_VK_PAUSE           = 0xC5,
		SELENE_VK_HOME            = 0xC7,
		SELENE_VK_UP              = 0xC8,
		SELENE_VK_PRIOR           = 0xC9,
		SELENE_VK_LEFT            = 0xCB,
		SELENE_VK_RIGHT           = 0xCD,
		SELENE_VK_END             = 0xCF,
		SELENE_VK_DOWN            = 0xD0,
		SELENE_VK_NEXT            = 0xD1,
		SELENE_VK_INSERT          = 0xD2,
		SELENE_VK_DELETE          = 0xD3,
		SELENE_VK_LWIN            = 0xDB,
		SELENE_VK_RWIN            = 0xDC,

		SELENE_VK_DISABLE         = 0x00
	};

	/**
		@brief マウスボタン状態定義
		@author 葉迩倭
	*/
	enum eMouseState
	{
		MOUSE_FREE,							///< マウスボタンは押されていない
		MOUSE_PULL,							///< マウスボタンは離された
		MOUSE_PUSH,							///< マウスボタンは押された
		MOUSE_HOLD,							///< マウスボタンは押されつづけている
	};

	/**
		@brief 物理シミュレーション用接触点定義
		@author 葉迩倭
	*/
	enum eContactMode
	{
		ContactMu2			= 0x001,
		ContactFDir1		= 0x002,
		ContactBounce		= 0x004,
		ContactSoftERP		= 0x008,
		ContactSoftCFM		= 0x010,
		ContactMotion1		= 0x020,
		ContactMotion2		= 0x040,
		ContactSlip1		= 0x080,
		ContactSlip2		= 0x100,

		ContactApprox0		= 0x0000,
		ContactApprox1_1	= 0x1000,
		ContactApprox1_2	= 0x2000,
		ContactApprox1		= 0x3000,
	};

	/**
		@brief 描画系
	*/
	namespace Renderer
	{
		/**
			@brief シェーダー系
		*/
		namespace Shader
		{
		}

		/**
			@brief 描画オブジェクト系
		*/
		namespace Object
		{
		}
	}

	/**
		@brief ファイル系
	*/
	namespace File
	{
	}

	namespace Scene
	{
	}

	/**
		@brief 入力デバイス系
	*/
	namespace Peripheral
	{
	}

	/**
		@brief サウンド系
	*/
	namespace Sound
	{
	}

	/**
		@brief システム系
	*/
	namespace System
	{
	}

	/**
		@brief 算術演算系
	*/
	namespace Math
	{
	}

	/**
		@brief 数値補間系
	*/
	namespace Interpolation
	{
	}

	/**
		@brief コリジョン系
	*/
	namespace Collision
	{
	}

	/**
		@brief 物理シミュレーション系
	*/
	namespace Dynamics
	{
		class IRigidModel;
		class IRigidBody;
		class IContactPoint;
		class ISimulationWorld;
	}

	/**
		@brief スレッド系
	*/
	namespace Thread
	{
	}

	/**
		@brief シーン系
	*/
	namespace Scene
	{
	}

	/**
		@brief ファイル系
	*/
	namespace File
	{
	}

	/**
		@brief ファイバー系
	*/
	namespace Fiber
	{
	}

	/**
		@brief ネットワーク系
	*/
	namespace Network
	{
		class INetworkManager;
		class ISocketTCP;
		class IServerTCP;
		class IServerClientTCP;
	}
}


//-----------------------------------------------------------------------------------
// INTERFACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	class ICore;
	class IGraphicCard;

	namespace Renderer
	{
		class IRender;
		class ITexture;
		class ITexture;

		namespace Shader
		{
			class IShader;
		}

		namespace Object
		{
			class IPoint2D;
			class ILine2D;
			class IPrimitive2D;
			class ISprite2D;
			class IFontSprite2D;
			class IPoint3D;
			class ILine3D;
			class IPrimitive3D;
			class ISprite3D;
			class IFontSprite3D;
			class IParticle;
			class IModel;
			class IMapModel;
		}
	}

	namespace Fiber
	{
		class IFiber;
		class IFiberController;
	};

	namespace Peripheral
	{
		class IMouse;
		class IKeyboard;
		class IJoystick;
		class IInputController;
	}

	namespace File
	{
		class IFileManager;
		class IResourceFile;
	}

	namespace Sound
	{
		class ISound;
		class IStreamSound;
	}

	namespace Scene
	{
		class IPrimitiveActor;
		class ISpriteActor;
		class IParticleActor;
		class IModelActor;
		class IMapActor;
		class ICamera;
		class ISceneManager;
		class ICustomizedSceneManager;
		class ICustomizedSceneRenderMaterial;
		class ICustomizedSceneRenderObject;
	}

	namespace Dynamics
	{
		class ISimulationWorld;
		class IRigidBody;
		class IRigidModel;
		class IContactPoint;
	}

	namespace Math
	{
		class Vector2D;
		class Vector3D;
		class Vector4D;
		class Quaternion;
		class Matrix;
		class Style;
	}
}


//-----------------------------------------------------------------------------------
// INLINE
//-----------------------------------------------------------------------------------
namespace Selene
{
	inline void Float2Int( Sint32 &iVal, Float fVal )
	{
		*((Uint32*)&iVal) = *((Uint32*)&fVal);
	}

	inline void Int2Float( Float &fVal, Sint32 iVal )
	{
		*((Uint32*)&fVal) = *((Uint32*)&iVal);
	}

	inline Float FastSqrt( Float fVal )
	{
		Float fRetVal;
		Sint32 iVal;

		Float2Int( iVal, fVal );

		iVal &= 0x7FFFFFFF;
		iVal -= 0x3F800000;
		iVal >>= 1;
		iVal += 0x3F800000;

		Int2Float( fRetVal, iVal );

		return fRetVal;
	}

	inline float InvSqrt( float fVal )
	{
		Sint32 iVal;
		Float fValHalf = fVal * 0.5f;

		Float2Int( iVal, fVal );

		iVal = 0x5F375A86 - (iVal >> 1);

		Int2Float( fVal, iVal );

		return fVal * (1.5f - fValHalf * fVal * fVal);
	}

	inline Float Abs( Float fVal )
	{
		Sint32 iVal;

		Float2Int( iVal, fVal );

		iVal &= 0x7FFFFFFF;

		Int2Float( fVal, iVal );

		return fVal;
	}

	inline Sint32 Abs( Sint32 iVal )
	{
		return iVal & 0x7FFFFFFF;
	}

	inline Bool Is2ByteChara( char Src )
	{
		unsigned char Code = *((unsigned char*)&Src);

		if ( Code <= 0x80 ) return false;	// 制御コード＆文字
		if ( Code <= 0x9F ) return true;
		if ( Code <= 0xDF ) return false;	// 半角カナ
		if ( Code <= 0xFE ) return true;

		return false;
	}

}

#pragma once


/**
	@file
	@brief 色表現用クラス
	@author 葉迩倭
*/

//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	/**
		@brief 色情報管理クラス
		@author 葉迩倭

		@note
			各種データの色を定義するためのクラスです。<BR>
			ライブラリ内で色を指定する場合はすべてこのクラスを使います。
	*/
	class CColor
	{
	public:
		union {
			struct {
				Uint8 b;		///< 色の青成分
				Uint8 g;		///< 色の緑成分
				Uint8 r;		///< 色の赤成分
				Uint8 a;		///< 色の透明度成分
			};
			Sint32 Color;		///< 色の32Bit整数表記
		};

	public:
/**
@brief コンストラクタ
@author 葉迩倭
*/
		CColor()
		{
		}

/**
@brief コンストラクタ
@author 葉迩倭
@param r		[in] 赤成分（０～２５５）
@param g		[in] 緑成分（０～２５５）
@param b		[in] 青成分（０～２５５）
@param a		[in] アルファ成分（０～２５５）
*/
		CColor( Sint32 r, Sint32 g, Sint32 b, Sint32 a = 255 )
		{
			this->a = (Uint8)a;
			this->r = (Uint8)r;
			this->g = (Uint8)g;
			this->b = (Uint8)b;
		}

/**
@brief コンストラクタ
@author 葉迩倭
@param Color	[in] 色成分
*/
		CColor( Sint32 Color )
		{
			this->Color = Color;
		}

/**
@brief 色ブレンド
@author 葉迩倭
@param SrcA		[in] 色成分
@param SrcB		[in] 色成分
@param fRate	[in] ブレンド率

@note
2つの色をブレンドします。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Selene::CColor ColorA( 255, 255, 255, 255 );
  Selene::CColor ColorB( 128, 128, 128, 128 );

  // ColorAとColorBを50%でブレンド
  Selene::CColor Ret;
  Ret.Blend( ColorA, ColorB, 0.5f );

  return 0;
}
@endcode
*/
		CColor &Blend( const CColor &SrcA, const CColor &SrcB, Float fRate )
		{
			if ( fRate < 0.0f ) fRate = 0.0f;
			if ( fRate > 1.0f ) fRate = 1.0f;
			r = (Uint8)(toF(SrcA.r) + (toF(SrcB.r - SrcA.r) * fRate));
			g = (Uint8)(toF(SrcA.g) + (toF(SrcB.g - SrcA.g) * fRate));
			b = (Uint8)(toF(SrcA.b) + (toF(SrcB.b - SrcA.b) * fRate));
			a = (Uint8)(toF(SrcA.a) + (toF(SrcB.a - SrcA.a) * fRate));

			return *this;
		}

/**
@brief オペレーター=演算子
@author 葉迩倭
*/
		CColor operator = ( const CColor &Color )
		{
			this->a = Color.a;
			this->r = Color.r;
			this->g = Color.g;
			this->b = Color.b;

			return *this;
		}

/**
@brief オペレーター=演算子
@author 葉迩倭
*/
		CColor operator = ( Sint32 Color )
		{
			this->Color = Color;

			return *this;
		}

/**
@brief 型変換
@author 葉迩倭
*/
		operator Sint32 () const
		{
			return Color;
		}

	public:
/**
@brief 色取得
@author 葉迩倭

@note
RGBAカラー取得

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Selene::CColor Color = Selene::CColor::RGBA( 255, 255, 255, 255 );

  return 0;
}
@endcode
*/
		static CColor RGBA( Sint32 r, Sint32 g, Sint32 b, Sint32 a )
		{
			return CColor( r, g, b, a );
		}

/**
@brief 色取得
@author 葉迩倭

@note
BGRAカラー取得

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Selene::CColor Color = Selene::CColor::BGRA( 255, 255, 255, 255 );

  return 0;
}
@endcode
*/
		static CColor BGRA( Sint32 b, Sint32 g, Sint32 r, Sint32 a )
		{
			return CColor( r, g, b, a );
		}

/**
@brief 色取得
@author 葉迩倭

@note
ARGBカラー取得

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Selene::CColor Color = Selene::CColor::ARGB( 255, 255, 255, 255 );

  return 0;
}
@endcode
*/
		static CColor ARGB( Sint32 a, Sint32 r, Sint32 g, Sint32 b )
		{
			return CColor( r, g, b, a );
		}

/**
@brief 色取得
@author 葉迩倭

@note
XRGBカラー取得

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Selene::CColor Color = Selene::CColor::XRGB( 255, 255, 255 );

  return 0;
}
@endcode
*/
		static CColor XRGB( Sint32 r, Sint32 g, Sint32 b )
		{
			return CColor( r, g, b, 255 );
		}
	};
}
#pragma once


/**
	@file
	@brief システム系関数群
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace System
{
/**
@brief システム初期化
@author 葉迩倭
@param IsSetExePath		[in] カレントディレクトリを実行ファイルの場所に強制的に変更するかの有無
@param IsCreateLogFile	[in] ログファイルを生成するかどうか
@param IsSSE			[in] SSEの使用有無
@retval true	成功
@retval false	失敗

@note
Seleneシステムの初期化を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
  }

  System::Finalize();            // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API Bool Initialize( Bool IsSetExePath = true, Bool IsCreateLogFile = true, Bool IsSSE = true );

/**
@brief システム終了
@author 葉迩倭

@note
Seleneシステムの終了をします。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
  }

  System::Finalize();            // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API void Finalize( void );

/**
@brief 多重起動ブロック
@author 葉迩倭
@param pApplicationName	[in] アプリケーション名
@retval false	既に同名のアプリケーションが起動している
@retval true	同名のアプリケーションは存在しない

@note
返り値false時にアプリケーションを終了させることで<BR>
多重起動を防止することができます。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
    // 多重起動チェック
    if ( System::BlockDualBoot( "Application Name" ) )
    {
    }
  }

  System::Finalize();            // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API Bool BlockDualBoot( const char *pApplicationName );

/**
@brief カレントディレクトリリセット
@author 葉迩倭

@note
カレントディレクトリをアプリケーションの実行フォルダに設定します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
    // カレントディレクトリを実行ファイルのフォルダにする
    System::ResetCurrentDirectory();
  }

  System::Finalize();            // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API void ResetCurrentDirectory( void );

/**
@brief ディレクトリ生成
@author 葉迩倭
@param pPath	[in] フォルダパス

@note
指定パスへディレクトリを生成します。<BR>
途中のフォルダが存在しない場合、全ての階層に対してフォルダを作成していきます。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
    // カレントディレクトリを実行ファイルのフォルダにする
    System::ResetCurrentDirectory();

    // カレントディレクトリの場所以下にフォルダ生成
    // ※実行ファイルの場所にフォルダを掘る
    System::DirectoryCreate( "Save\\Config" );
  }

  System::Finalize();            // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API void DirectoryCreate( const char *pPath );

/**
@brief Coreの取得
@author 葉迩倭
@return ICoreインターフェイス

@note
ウィンドウを管理するICoreクラスを生成/取得します。<BR>
現在、１つのICoreしか生成できません。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore      *pCore      = NULL;

  // システムの起動
  if ( System::Initialize() )
  {
    // ICoreの生成
    ICore *pCore = System::CreateCore();
    if ( pCore == NULL ) goto EXIT;      // ICoreにはNullDeviceがないので失敗するとNULLが返る  
  }

EXIT:
  SAFE_RELEASE( pCore );            // コアの解放

  System::Finalize();              // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API ICore *CreateCore( void );

/**
@brief SSEのサポートの有無
@author 葉迩倭
@return	SSEのサポートの有無

@note
使用しているCPUがSSEをサポートしているかを調べます。<BR>
この関数はInitialize()のIsSSEにfalseを指定するとかならずfalseを返します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
    // SSEサポートしてる？
    if ( System::IsSupportSSE() )
    {
      // SSEでSIMD演算
    }
    else
    {
      // FPUで普通に演算
    }
  }

  System::Finalize();        // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API Bool IsSupportSSE( void );

/**
@brief 使用するCPU番号を設定
@author 葉迩倭
@param No	[in] CPU番号

@note
現在のスレッドで使用するCPUの番号を設定します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // システムの起動
  if ( System::Initialize() )
  {  
    // 現在のスレッドをCPU1で動かす
    System::SetThreadCPUNo( 1 );
  }

  System::Finalize();        // システムの終了処理を行う

  return 0;
}
@endcode
*/
SELENE_DLL_API void SetThreadCPUNo( Sint32 No );
}
}
#pragma once


/**
	@file
	@brief システム系関数群
	@author 葉迩倭
*/

//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Dynamics
{
/**
@brief ダイナミクス用ワールド生成
@author 葉迩倭
@return ワールドのインターフェイス

@note
Dynamics(ODE)のワールドの生成を行います。

@code
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	// ワールドの生成
	ISimulationWorld *pWorld = CreateWorld();

	// 略
	…

	// ワールドの解放
	SAFE_RELEASE( pWorld );

	return 0;
}
@endcode
*/
SELENE_DLL_API ISimulationWorld *CreateWorld( Scene::ISceneManager *pScene = NULL );
}
}

#pragma once


/**
	@file
	@brief システム系関数群
	@author 葉迩倭
*/

//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Network
{
/**
@brief ネットワークマネージャーインターフェイス生成
@author 葉迩倭
@return ネットワークマネージャーインターフェイス

@note
新規のネットワークマネージャーインターフェイスを生成します。<BR>
ネットワークのパスなどの設定は全てINetworkManagerを経由して行います。
取得したネットワークマネージャーインターフェイスは使用終了後には必ずRelease()して下さい。。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Network::INetworkManager *pNetworkMgr = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
SELENE_DLL_API INetworkManager *CreateManager( void );
}
}

#pragma once


/**
	@file
	@brief クリティカルセクション操作
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Thread
{
	/**
		@brief クリティカルセクション管理クラス
		@author 葉迩倭
	*/
	class SELENE_DLL_API CCriticalSection
	{
	private:
		CRITICAL_SECTION m_CriticalSection;		///< クリティカルセクション

	public:
/**
@brief コンストラクタ
@author 葉迩倭
*/
		CCriticalSection();

/**
@brief デストラクタ
@author 葉迩倭
*/
		virtual ~CCriticalSection();

/**
@brief クリティカルセクションに入る
@author 葉迩倭

@note
クリティカルセクションに入ります。

@code

using namespace Selene;

CriticalSection cs;
Sint32 g_Result = 0;

void CThreadProc( void *pData )
{
  // 変数にアクセス前にロック
  cs.Enter();

  // 適当
  for ( Sint32 i = 0; i < 100000; )
  {
    g_Result += i * i;
  }

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // 適当に待ってる
  Sleep( 1000 );

  // 変数にアクセス前にロック
  cs.Enter();

  // 結果をゲット
  Sint32 ret = g_Result;

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();

  // スレッド終わってる？
  while ( !CThread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		void Enter( void ) const;

/**
@brief クリティカルセクションから抜ける
@author 葉迩倭

@note
クリティカルセクションから抜けます。

@code

using namespace Selene;

CriticalSection cs;
Sint32 g_Result = 0;

void CThreadProc( void *pData )
{
  // 変数にアクセス前にロック
  cs.Enter();

  // 適当
  for ( Sint32 i = 0; i < 100000; )
  {
    g_Result += i * i;
  }

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // 適当に待ってる
  Sleep( 1000 );

  // 変数にアクセス前にロック
  cs.Enter();

  // 結果をゲット
  Sint32 ret = g_Result;

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();

  // スレッド終わってる？
  while ( !CThread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		void Leave( void ) const;
	};

	/**
		@brief 自動クリティカルセクション管理クラス
		@author 葉迩倭
	*/
	class CAutoLock
	{
	private:
		const CCriticalSection	&m_cs;			///< 使用するクリティカルセクション

	private:
/**
@brief = オペレーター
@author 葉迩倭
@param	Lock [in] 入力
@return	入力と同じもの

コンパイルを通すためのダミー用。
*/
		CAutoLock & operator = ( CAutoLock &Lock ) {
			return Lock;
		}

	public:
/**
@brief コンストラクタ
@author 葉迩倭
@param cs	[in] クリティカルセクション

@note
コンストラクタ内で自動的にクリティカルセクションに入ります。

@code

using namespace Selene;

CriticalSection cs;
Sint32 g_Result = 0;

void CThreadProc( void *pData )
{
  // Lockのコンストラクタ内で
  // csがクリティカルセクションに入る
  CAutoLock Lock( cs );

  // 適当
  for ( Sint32 i = 0; i < 100000; )
  {
    g_Result += i * i;
  }

  // スコープから抜けるときにLockのデストラクタがコールされ
  // csがクリティカルセクションから抜ける
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // 適当に待ってる
  Sleep( 1000 );

  // 変数にアクセス前にロック
  cs.Enter();

  // 結果をゲット
  Sint32 ret = g_Result;

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();

  // スレッド終わってる？
  while ( !CThread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		CAutoLock( const CCriticalSection &cs )
			: m_cs	( cs )
		{
			m_cs.Enter();
		}

/**
@brief コンストラクタ
@author 葉迩倭
@param cs	[in] クリティカルセクション

@note
コンストラクタ内で自動的にクリティカルセクションに入ります。

@code

using namespace Selene;

CriticalSection cs;
Sint32 g_Result = 0;

void CThreadProc( void *pData )
{
  // Lockのコンストラクタ内で
  // csがクリティカルセクションに入る
  CAutoLock Lock( &cs );

  // 適当
  for ( Sint32 i = 0; i < 100000; )
  {
    g_Result += i * i;
  }

  // スコープから抜けるときにLockのデストラクタがコールされ
  // csがクリティカルセクションから抜ける
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // 適当に待ってる
  Sleep( 1000 );

  // 変数にアクセス前にロック
  cs.Enter();

  // 結果をゲット
  Sint32 ret = g_Result;

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();

  // スレッド終わってる？
  while ( !CThread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		CAutoLock( const CCriticalSection *cs )
			: m_cs	( *cs )
		{
			m_cs.Enter();
		}

/**
@brief デストラクタ
@author 葉迩倭

@note
デストラクタが呼ばれる時に自動的にクリティカルセクションから出ます。

@code

using namespace Selene;

CriticalSection cs;
Sint32 g_Result = 0;

void CThreadProc( void *pData )
{
  // Lockのコンストラクタ内で
  // csがクリティカルセクションに入る
  CAutoLock Lock( &cs );

  // 適当
  for ( Sint32 i = 0; i < 100000; )
  {
    g_Result += i * i;
  }

  // スコープから抜けるときにLockのデストラクタがコールされ
  // csがクリティカルセクションから抜ける
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // 適当に待ってる
  Sleep( 1000 );

  // 変数にアクセス前にロック
  cs.Enter();

  // 結果をゲット
  Sint32 ret = g_Result;

  // 変数使い終わったのでアクセスロック解除
  cs.Leave();

  // スレッド終わってる？
  while ( !CThread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		~CAutoLock()
		{
			m_cs.Leave();
		}
	};
}
}

#pragma once


/**
	@file
	@brief イベント操作
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Thread
{
	/**
		@brief スレッド用イベントオブジェクト
		@author 葉迩倭
	*/
	class SELENE_DLL_API CEvent
	{
	private:
		HANDLE m_EventHandle[MAXIMUM_WAIT_OBJECTS];		///< イベントハンドル
		Sint32 m_HandleCount;							///< イベント数

	public:
/**
@brief コンストラクタ
@author 葉迩倭
@param IsSignal		[in] シグナル状態で初期化する場合はtrue
@param Count		[in] イベントのカウント（MAXIMUM_WAIT_OBJECTS以下）
*/
		CEvent( Bool IsSignal = false, Sint32 Count = 1 );

/**
@brief デストラクタ
@author 葉迩倭
*/
		virtual ~CEvent();

/**
@brief 指定されたイベントをシグナル状態に設定
@author 葉迩倭
@param No	[in] シグナル状態するイベントの番号

@note
指定した番号のイベントをシグナル状態に設定します。

@code

using namespace Selene;

// 5個のイベント生成
Event Event( false, 5 );

void ThreadProc( void *pData )
{
  // No=3のイベントがシグナル？
  while ( !Event.IsSignal( 3 ) )
  {
    // しばし待つ
    Sleep( 1000 );
  }

  // No=3のイベントを非シグナルにする
  Event.Reset( 3 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Thread Thread;

  // スレッド生成
  Thread.Create( ThreadProc, NULL );

  // 適当に待ってるふり
  Sleep( 1000 );

  // No=3のイベントをシグナルにする
  Event.Set( 3 );

  // スレッド終わってる？
  while ( !Thread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		virtual void Set( Sint32 No = 0 );

/**
@brief 指定されたイベントを非シグナル状態に設定
@author 葉迩倭
@param No	[in] 非シグナル状態するイベントの番号

@note
指定した番号のイベンとぉ非シグナル状態に設定します。

@code

using namespace Selene;

// 5個のイベント生成
Event Event( false, 5 );

void ThreadProc( void *pData )
{
  // No=3のイベントがシグナル？
  while ( !Event.IsSignal( 3 ) )
  {
    // しばし待つ
    Sleep( 1000 );
  }

  // No=3のイベントを非シグナルにする
  Event.Reset( 3 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Thread Thread;

  // スレッド生成
  Thread.Create( ThreadProc, NULL );

  // 適当に待ってるふり
  Sleep( 1000 );

  // No=3のイベントをシグナルにする
  Event.Set( 3 );

  // スレッド終わってる？
  while ( !Thread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		virtual void Reset( Sint32 No = 0 );

/**
@brief 指定したイベントがシグナル状態かチェック
@author 葉迩倭
@param No	[in] チェックするイベントの番号
@retval false	非シグナル状態
@retval true	シグナル状態

@note
指定したイベントがシグナル状態になっているかをチェックします。

@code

using namespace Selene;

// 5個のイベント生成
Event Event( false, 5 );

void ThreadProc( void *pData )
{
  // No=3のイベントがシグナル？
  while ( !Event.IsSignal( 3 ) )
  {
    // しばし待つ
    Sleep( 1000 );
  }

  // No=3のイベントを非シグナルにする
  Event.Reset( 3 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Thread Thread;

  // スレッド生成
  Thread.Create( ThreadProc, NULL );

  // 適当に待ってるふり
  Sleep( 1000 );

  // No=3のイベントをシグナルにする
  Event.Set( 3 );

  // スレッド終わってる？
  while ( !Thread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		virtual Bool IsSignal( Sint32 No = 0 );

/**
@brief 全てのイベントがシグナル状態かチェック
@author 葉迩倭
@retval false	非シグナル状態
@retval true	シグナル状態

@note
全てのイベントがシグナル状態になっているかをチェックします。

@code

using namespace Selene;

// 3個のイベント生成
Event Event( false, 3 );

void ThreadProc( void *pData )
{
  // 全部シグナルになるまで待つ
  while ( !Event.IsSignalAll() )
  {
    // まだなのでしばし待つ
    Sleep( 1000 );
  }
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Thread Thread;

  // スレッド生成
  Thread.Create( ThreadProc, NULL );

  // 適当に待ってるふり
  Sleep( 1000 );

  // イベントをシグナルにする
  Event.Set( 0 );
  Event.Set( 1 );
  Event.Set( 2 );

  // スレッド終わってる？
  while ( !Thread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		virtual Bool IsSignalAll( void );

/**
@brief シグナル状態になったイベントを取得
@author 葉迩倭
@param TimeOut		[in] タイムアウトms時間（-1で無限）
@retval -1		タイムアウト
@retval 0以上	シグナル状態になったイベント番号

@note
指定時間待機し１つでもシグナル状態になったらその瞬間にそのイベントの番号を返します。<BR>
指定時間経過してもシグナル状態になったイベントがなければ-1を返します。

@code

using namespace Selene;

// 3個のイベント生成
Event Event( false, 3 );

void ThreadProc( void *pData )
{
  // 何かがシグナルになるまで1秒ほど待つ
  Sint32 No = Event.Wait( 1000 );
  switch ( No )
  {
  case 0:
  case 1:
  case 2:
    break;
  default:
    break;
  }
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Thread Thread;

  // スレッド生成
  Thread.Create( ThreadProc, NULL );

  // 適当に待ってるふり
  Sleep( 1000 );

  // イベントをシグナルにする
  Event.Set( 1 );

  // スレッド終わってる？
  while ( !Thread.WaitForExit() )
  {
    Sleep( 1000 );
  }
  return 0;
}
@endcode
*/
		virtual Sint32 Wait( Sint32 TimeOut = -1 );

/**
@brief シグナル状態になったイベントを取得
@author 葉迩倭
@param No		[in] ハンドル番号
@return		ハンドル

@note
指定番号のハンドルを取得します。
*/
		virtual HANDLE GetHandle( Sint32 No );
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Thread
{
	/**
		@brief スレッド実行優先度
		@author 葉迩倭
	*/
	enum eCThreadPriority
	{
		THREAD_PRIORITY_LOW		= -2,		/// 低い
		THREAD_PRIORITY_MIDDLE	= -1,		/// やや低い
		THREAD_PRIORITY_DEFAULT	=  0,		/// 通常
		THREAD_PRIORITY_HIGH	= +1,		/// 高い
	};

	/**
		@brief スレッド管理操作クラス
		@author 葉迩倭
	*/
	class SELENE_DLL_API CThread
	{
	private:
/**
@brief スレッド用関数
@author 葉迩倭
@param pArguments	[in] CThreadのポインタ

@note
クラスの関数を直接は指定できないので<BR>
staticな関数を経由して呼び出す。
*/
		static Uint32 __stdcall CThreadFunc( void* pArguments );

	private:
		HANDLE m_hThread;					///< スレッドハンドル
		Uint32 m_ThreadId;					///< スレッドID
		void *m_pData;						///< スレッドデータ伝達用ポインタ
		void (*m_pMainProc)( void* );		///< スレッド関数
		CCriticalSection m_cs;				///< クリティカルセクション

	public:
/**
@brief コンストラクタ
@author 葉迩倭
*/
		CThread();

/**
@brief デストラクタ
@author 葉迩倭
*/
		virtual ~CThread();

	public:
/**
@brief スレッド生成
@author 葉迩倭
@param pCThreadFunc	[in] スレッド関数
@param pData		[in] スレッドに引き渡すデータポインタ
@retval false	生成失敗or既にスレッドが生成されている
@retval true	生成完了

@note
スレッドの生成を行います。

@code

using namespace Selene;

void CThreadProc( void *pData )
{
  // 10秒待機
  Sleep( 10 * 1000 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // スレッド終わるまで待つ
  while ( !CThread.WaitForExit() )
  {
    // 適当
    Sleep( 100 );
  }
  return 0;
}
@endcode
*/
		virtual Bool Create( void (*pCThreadFunc)( void* ), void *pData );

/**
@brief スレッド終了待ち
@author 葉迩倭
@retval false	スレッドは終了していない
@retval true	スレッドは終了した

@note
スレッドが終了するのを待ちます

@code
void CThreadProc( void *pData )
{
  // 10秒待機
  Sleep( 10 * 1000 );
}

void hogehoge( void )
{
	CThread CThread;

	// スレッド生成
	CThread.Create( CThreadProc, NULL );

	// スレッド終わるまで待つ
	while ( !CThread.WaitForExit() )
	{
		// 適当
		Sleep( 100 );
	}
}
@endcode
*/
		virtual Bool WaitForExit( void );

/**
@brief スレッドの優先度変更
@author 葉迩倭
@param Priority		[in] スレッド処理の優先度

@note
スレッドの優先度を変更します。<BR>
デフォルトの優先度はTHREAD_PRIORITY_DEFAULTです。

@code

using namespace Selene;

void CThreadProc( void *pData )
{
  // 10秒待機
  Sleep( 10 * 1000 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // スレッドの優先度を1ランク下げる
  CThread.SetPriority( THREAD_PRIORITY_MIDDLE );

  // スレッド終わるまで待つ
  while ( !CThread.WaitForExit() )
  {
    // 適当
    Sleep( 100 );
  }
  return 0;
}
@endcode
*/
		virtual Bool SetPriority( Sint32 Priority );

/**
@brief スレッドの中断を解除
@author 葉迩倭

@note
スレッドのサスペンドカウントが 1 減ります。<BR>
カウントが 0 になった時点でスレッドの中断が解除されます。

@code

using namespace Selene;

void CThreadProc( void *pData )
{
  // 10秒待機
  Sleep( 10 * 1000 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // スレッド一時停止
  CThread.Suspend();

  // 適当に待ってるふり
  Sleep( 1000 );

  // スレッド再開
  CThread.Resume();

  // スレッド終わるまで待つ
  while ( !CThread.WaitForExit() )
  {
    // 適当
    Sleep( 100 );
  }
  return 0;
}
@endcode
*/
		virtual void Resume( void );

/**
@brief スレッドの実行を中断
@author 葉迩倭

@note
指定されたスレッドの実行が中断され、<BR>
そのスレッドのサスペンドカウントが 1 増えます。

@code
void CThreadProc( void *pData  )
{
}

void hogehoge( void )
{
	Selene::CThread CThread;

	// スレッド生成
	CThread.Create( CThreadProc );

	// スレッド一時停止
	CThread.Suspend();

	// 略
	…

	// スレッド再開
	CThread.Resume();

	// 略
	…
}
@endcode
*/
		virtual void Suspend( void );

/**
@brief スレッドの実行を終了
@author 葉迩倭

@note
指定されたスレッドの実行を強制終了します。<BR>
この関数で終了させる場合はスタックの解放などが行われないので<BR>
通常は使用しないでください。<BR>
強制終了を行わざるを得ない状況でのみ使用して下さい。

@code

using namespace Selene;

void CThreadProc( void *pData )
{
  // 10秒待機
  Sleep( 10 * 1000 );
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  CThread CThread;

  // スレッド生成
  CThread.Create( CThreadProc, NULL );

  // 適当に待ってるふり
  Sleep( 1000 );

  // スレッド終わってる？
  if ( !CThread.WaitForExit() )
  {
    // 強制終了
    CThread.Terminate();
  }
  return 0;
}
@endcode
*/
		virtual void Terminate( void );
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	/**
		@brief ファイルオープンモード
		@author 葉迩倭
	*/
	enum eFileOpenType
	{
		FILE_OPEN_TYPE_WRITE,			///< 書き込み専用
		FILE_OPEN_TYPE_READ,			///< 読み込み専用
		FILE_OPEN_TYPE_READ_WRITE,		///< 読み書き用

		FILE_OPEN_TYPE_INVALID,			///< 無効
	};

namespace File
{
	/**
		@brief ファイル操作
		@author 葉迩倭
	*/
	class SELENE_DLL_API CFile
	{
	private:
		HANDLE m_hFile;				///< ファイルハンドル
		eFileOpenType m_Type;		///< ファイルオープンモード
		FILETIME m_TimeCreate;		///< ファイル作成時間
		FILETIME m_TimeAccess;		///< ファイルアクセス時間
		FILETIME m_TimeLastWrite;	///< ファイル書き込み時間

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CFile( const char *pFileName, eFileOpenType Type );

		/**
			@brief デストラクタ
			@author 葉迩倭
		*/
		virtual ~CFile();

		/**
			@brief ファイルオープンチェック
			@author 葉迩倭
			@retval false	ファイルは開かれていない
			@retval true	ファイルは開かれている
		*/
		virtual Bool IsOpened( void );

		/**
			@brief ファイル書き込み
			@author 葉迩倭
			@param pData	[in] 書き込みデータ
			@param Size		[in] データサイズ
			@return 実際の書き込みサイズ
		*/
		virtual Sint32 Write( const void *pData, Sint32 Size );

		/**
			@brief ファイル読み込み
			@author 葉迩倭
			@param pData	[in] 読み込みデータ
			@param Size		[in] データサイズ
			@return 実際の読み込みサイズ
		*/
		virtual Sint32 Read( void *pData, Sint32 Size );

		/**
			@brief ファイルサイズ取得
			@author 葉迩倭
		*/
		virtual Sint32 GetFileSize( void );

		/**
			@brief ファイル位置取得
			@author 葉迩倭
			preturn ファイル位置
		*/
		virtual Sint32 GetFilePosition( void );

		/**
			@brief 先頭からシーク
			@author 葉迩倭
			@param Offset	[in] 先頭からのオフセット
			@param ファイル位置
		*/
		virtual Sint32 SeekStart( Sint32 Offset );

		/**
			@brief 終端からシーク
			@author 葉迩倭
			@param Offset	[in] 終端からのオフセット
			@param ファイル位置
		*/
		virtual Sint32 SeekEnd( Sint32 Offset );

		/**
			@brief シーク
			@author 葉迩倭
			@param Offset	[in] 現在位置からのオフセット
			@param ファイル位置
		*/
		virtual Sint32 Seek( Sint32 Offset );
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace File
{
	/**
		@brief HTMLログ用ファイル操作
		@author 葉迩倭
	*/
	class SELENE_DLL_API CFileHtmlLog : public CFile
	{
	private:
		Thread::CCriticalSection m_CS;

	public:
		CFileHtmlLog( const char *pFileName, const char *pTitle );
		virtual ~CFileHtmlLog();

		virtual Sint32 Print( Sint32 Color, const char *pStr,... );
		virtual Sint32 PrintStrong( Sint32 Color, const char *pStr,... );
		virtual Sint32 PrintLine( Sint32 Color, const char *pStr,... );
		virtual Sint32 PrintStrongLine( Sint32 Color, const char *pStr,... );

		virtual Sint32 TableBegin( void );
		virtual Sint32 TableEnd( void );
		virtual Sint32 TableLine( Sint32 Bold );

		virtual Sint32 CellBegin( Sint32 Width );
		virtual Sint32 CellEnd( void );
	};
}
}


#pragma once


/**
	@file
	@brief 2次元ベクトル
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 2次元ベクトル演算クラス
	*/
	class SELENE_DLL_API Vector2D
	{
	public:
		Float x;
		Float y;

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Vector2D();
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param v	[in] 入力
		*/
		Vector2D( const Vector2D &v );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param fPx	[in] x初期値
			@param fPy	[in] y初期値
		*/
		Vector2D( Float fPx, Float fPy );
		/**
			@brief 加算
			@author 葉迩倭
			@param In1	[in] 加算用入力ベクトル
			@param In2	[in] 加算用入力ベクトル
			@return 計算結果後の*this

			@note
			this = In + In2
		*/
		Vector2D& Add( const Vector2D &In1, const Vector2D &In2 );
		/**
			@brief 減算
			@author 葉迩倭
			@param In1	[in] 加算用入力ベクトル
			@param In2	[in] 加算用入力ベクトル
			@return 計算結果後の*this

			@note
			this = In - In2
		*/
		Vector2D& Sub( const Vector2D &In1, const Vector2D &In2 );
		/**
			@brief 値設定
			@author 葉迩倭
			@param fPx	[in] x設定値
			@param fPy	[in] y設定値
			@return 計算結果後の*this
		*/
		Vector2D& Set( Float fPx, Float fPy );
		/**
			@brief 長さの二乗取得
			@author 葉迩倭
			@return 長さの二乗
		*/
		Float LengthSq( void ) const;
		/**
			@brief 長さ取得
			@author 葉迩倭
			@return 長さ
		*/
		Float Length( void ) const;
		/**
			@brief 長さ取得
			@author 葉迩倭
			@return 長さ
		*/
		Float LengthFast( void ) const;
		/**
			@brief 内積
			@author 葉迩倭
			@param In	[in] 内積算出用入力ベクトル
			@return 内積
		*/
		Float Dot( const Vector2D &In ) const;
		/**
			@brief 正規化
			@author 葉迩倭
			@return 計算結果後の*this

			@note
			長さを1.0に正規化します。
		*/
		Vector2D& Normalize( void );
		/**
			@brief 正規化
			@author 葉迩倭
			@return 計算結果後の*this

			@note
			長さを1.0に正規化します。
		*/
		Vector2D& NormalizeFast( void );
		/**
			@brief 反射ベクトルの取得
			@author 葉迩倭
			@param In1	[in] 反射対象の法線
			@param In2	[in] 入射ベクトル
			@return 計算結果後の*this
		*/
		Vector2D& Reflect( const Vector2D &In1, const Vector2D &In2 );
		/**
			@brief 正規化
			@author 葉迩倭
			@param In	[in] 正規化元のベクトル
			@return 計算結果後の*this
		*/
		Vector2D& Normalize( const Vector2D &In );
		/**
			@brief 正規化
			@author 葉迩倭
			@param In	[in] 正規化元のベクトル
			@return 計算結果後の*this
		*/
		Vector2D& NormalizeFast( const Vector2D &In );
		/**
			@brief 外積
			@author 葉迩倭
			@param In	[in] 外積用のベクトル
			@return z=0.0として計算したベクトルの外積
		*/
		Float Cross( const Vector2D &In ) const;
		/**
			@brief 反射
			@author 葉迩倭
			@param In	[in] 反射用法線
			@return 計算結果後の*this
		*/
		Vector2D& Reflect( const Vector2D &In );
		/**
			@brief Z軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector2D& RotationZ( Sint32 Angle );
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator + () const
		{
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator - () const
		{
			return Vector2D(-x, -y);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Bool operator == ( const Vector2D &v )
		{
			return (x == v.x) && (y == v.y);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Bool operator != ( const Vector2D &v )
		{
			return (x != v.x) || (y != v.y);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator += ( Float f )
		{
			x += f;
			y += f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator -= ( Float f )
		{
			x -= f;
			y -= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator *= ( Float f )
		{
			x *= f;
			y *= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator /= ( Float f )
		{
			x /= f;
			y /= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator += ( const Vector2D &v )
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator -= ( const Vector2D &v )
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator *= ( const Vector2D &v )
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D& operator /= ( const Vector2D &v )
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator + ( Float f ) const
		{
		    return Vector2D(x + f, y + f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator - ( Float f ) const
		{
			f = 1.0f / f;
			return Vector2D(x - f, y - f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator * ( Float f ) const
		{
		    return Vector2D(x * f, y * f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator / ( Float f ) const
		{
			return Vector2D(x / f, y / f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator + ( const Vector2D &v ) const
		{
			return Vector2D(x + v.x, y + v.y);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator - ( const Vector2D &v ) const
		{
			return Vector2D(x - v.x, y - v.y);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator * ( const Vector2D &v ) const
		{
			return Vector2D(x * v.x, y * v.y);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector2D operator / ( const Vector2D &v ) const
		{
			return Vector2D(x / v.x, y / v.y);
		}
	};
}
}

#pragma once


/**
	@file
	@brief 3次元ベクトル
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 3次元ベクトル演算クラス
	*/
	class SELENE_DLL_API Vector3D
	{
	public:
		Float x;
		Float y;
		Float z;

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Vector3D();
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param v	[in] 入力
		*/
		Vector3D( const Vector3D &v );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param fPx	[in] x初期値
			@param fPy	[in] y初期値
			@param fPz	[in] z初期値
		*/
		Vector3D( Float fPx, Float fPy, Float fPz );
		/**
			@brief 加算
			@author 葉迩倭
			@param In1	[in] 加算用入力ベクトル
			@param In2	[in] 加算用入力ベクトル
			@return 計算結果後の*this

			@note
			this = In + In2
		*/
		Vector3D& Add( const Vector3D &In1, const Vector3D &In2 );
		/**
			@brief 減算
			@author 葉迩倭
			@param In1	[in] 加算用入力ベクトル
			@param In2	[in] 加算用入力ベクトル
			@return 計算結果後の*this

			@note
			this = In - In2
		*/
		Vector3D& Sub( const Vector3D &In1, const Vector3D &In2 );
		/**
			@brief 値設定
			@author 葉迩倭
			@param fPx	[in] x設定値
			@param fPy	[in] y設定値
			@param fPz	[in] z設定値
			@return 計算結果後の*this
		*/
		Vector3D& Set( Float fPx, Float fPy, Float fPz );
		/**
			@brief 長さの二乗取得
			@author 葉迩倭
			@return 長さの二乗
		*/
		Float LengthSq( void ) const;
		/**
			@brief 長さ取得
			@author 葉迩倭
			@return 長さ
		*/
		Float Length( void ) const;
		/**
			@brief 長さ取得
			@author 葉迩倭
			@return 長さ
		*/
		Float LengthFast( void ) const;
		/**
			@brief 内積
			@author 葉迩倭
			@param In	[in] 内積算出用入力ベクトル
			@return 内積
		*/
		Float Dot( const Vector3D &In ) const;
		/**
			@brief 正規化
			@author 葉迩倭
			@return 計算結果後の*this

			@note
			長さを1.0に正規化します。
		*/
		Vector3D &Normalize( void );
		/**
			@brief 正規化
			@author 葉迩倭
			@return 計算結果後の*this

			@note
			長さを1.0に正規化します。
		*/
		Vector3D &NormalizeFast( void );
		/**
			@brief 外積
			@author 葉迩倭
			@param In1	[in] 外積算出用入力ベクトル
			@param In2	[in] 外積算出用入力ベクトル
			@return 計算結果後の*this
		*/
		Vector3D& Cross( const Vector3D &In1, const Vector3D &In2 );
		/**
			@brief 反射ベクトルの取得
			@author 葉迩倭
			@param In1	[in] 反射対象の法線
			@param In2	[in] 入射ベクトル
			@return 計算結果後の*this
		*/
		Vector3D& Reflect( const Vector3D &In1, const Vector3D &In2 );
		/**
			@brief 拡大縮小＋回転変換
			@author 葉迩倭
			@param In1	[in] 変換元ベクトル
			@param In2	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector3D& TransformNormal( const Vector3D &In1, const Matrix &In2 );
		/**
			@brief 拡大縮小＋回転＋移動変換
			@author 葉迩倭
			@param In1	[in] 変換元ベクトル
			@param In2	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector3D& TransformCoord( const Vector3D &In1, const Matrix &In2 );
		/**
			@brief 透視変換
			@author 葉迩倭
			@param In1	[in] 変換元ベクトル
			@param In2	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector3D& TransformProjection( const Vector3D &In1, const Matrix &In2 );
		/**
			@brief 正規化
			@author 葉迩倭
			@param In	[in] 正規化元のベクトル
			@return 計算結果後の*this
		*/
		Vector3D &Normalize( const Vector3D &In );
		/**
			@brief 正規化
			@author 葉迩倭
			@param In	[in] 正規化元のベクトル
			@return 計算結果後の*this
		*/
		Vector3D &NormalizeFast( const Vector3D &In );
		/**
			@brief 外積
			@author 葉迩倭
			@param In	[in] 外積用のベクトル
			@return 計算結果後の*this
		*/
		Vector3D& Cross( const Vector3D &In );
		/**
			@brief 反射
			@author 葉迩倭
			@param In	[in] 反射用法線
			@return 計算結果後の*this
		*/
		Vector3D& Reflect( const Vector3D &In );
		/**
			@brief 拡大縮小＋回転変換
			@author 葉迩倭
			@param In	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector3D& TransformNormal( const Matrix &In );
		/**
			@brief 拡大縮小＋回転＋移動変換
			@author 葉迩倭
			@param In	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector3D& TransformCoord( const Matrix &In );
		/**
			@brief 透視変換
			@author 葉迩倭
			@param In	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector3D& TransformProjection( const Matrix &In );
		/**
			@brief X軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector3D& RotationX( Sint32 Angle );
		/**
			@brief Y軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector3D& RotationY( Sint32 Angle );
		/**
			@brief Z軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector3D& RotationZ( Sint32 Angle );
		/**
			@brief ベクトルを軸に回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@param Axis		[in] 回転軸
			@return 計算結果後の*this
		*/
		Vector3D& RotationAxis( Sint32 Angle, const Vector3D &Axis );
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator + () const
		{
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator - () const
		{
			return Vector3D(-x, -y, -z);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Bool operator == ( const Vector3D &v )
		{
			return (x == v.x) && (y == v.y) && (z == v.z);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Bool operator != ( const Vector3D &v )
		{
			return (x != v.x) || (y != v.y) || (z != v.z);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator += ( Float f )
		{
			x += f;
			y += f;
			z += f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator -= ( Float f )
		{
			x -= f;
			y -= f;
			z -= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator *= ( Float f )
		{
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator /= ( Float f )
		{
			x /= f;
			y /= f;
			z /= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator += ( const Vector3D &v )
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator -= ( const Vector3D &v )
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator *= ( const Vector3D &v )
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D& operator /= ( const Vector3D &v )
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator + ( Float f ) const
		{
			return Vector3D(x + f, y + f, z + f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator - ( Float f ) const
		{
			return Vector3D(x - f, y - f, z - f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator * ( Float f ) const
		{
			return Vector3D(x * f, y * f, z * f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator / ( Float f ) const
		{
			return Vector3D(x / f, y / f, z / f);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator + ( const Vector3D &v ) const
		{
			return Vector3D(x + v.x, y + v.y, z + v.z);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator - ( const Vector3D &v ) const
		{
			return Vector3D(x - v.x, y - v.y, z - v.z);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator * ( const Vector3D &v ) const
		{
			return Vector3D(x * v.x, y * v.y, z * v.z);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector3D operator / ( const Vector3D &v ) const
		{
			return Vector3D(x - v.x, y - v.y, z - v.z);
		}
	};
}
}

#pragma once


/**
	@file
	@brief 4次元ベクトル
	@author 葉迩倭
*/

//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 4次元ベクトル演算クラス
	*/
	class SELENE_DLL_API Vector4D
	{
	public:
		Float x;
		Float y;
		Float z;
		Float w;

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Vector4D();
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param v	[in] 入力（x, y, 0, 1)に展開
		*/
		Vector4D( const Vector2D &v );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param v	[in] 入力（x, y, z, 1)に展開
		*/
		Vector4D( const Vector3D &v );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param v	[in] 入力
		*/
		Vector4D( const Vector4D &v );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param fPx	[in] x初期値
			@param fPy	[in] y初期値
			@param fPz	[in] z初期値
			@param fPw	[in] w初期値
		*/
		Vector4D( Float fPx, Float fPy = 0.0f, Float fPz = 0.0f, Float fPw = 1.0f );
		/**
			@brief 加算
			@author 葉迩倭
			@param In1	[in] 加算用入力ベクトル
			@param In2	[in] 加算用入力ベクトル
			@return 計算結果後の*this

			@note
			this = In + In2
		*/
		Vector4D& Add( const Vector4D &In1, const Vector4D &In2 );
		/**
			@brief 減算
			@author 葉迩倭
			@param In1	[in] 加算用入力ベクトル
			@param In2	[in] 加算用入力ベクトル
			@return 計算結果後の*this

			@note
			this = In - In2
		*/
		Vector4D& Sub( const Vector4D &In1, const Vector4D &In2 );
		/**
			@brief 値設定
			@author 葉迩倭
			@param fPx	[in] x設定値
			@param fPy	[in] y設定値
			@param fPz	[in] z設定値
			@param fPw	[in] w設定値
			@return 計算結果後の*this
		*/
		Vector4D& Set( Float fPx, Float fPy, Float fPz, Float fPw = 1.0f );
		/**
			@brief 長さの二乗取得
			@author 葉迩倭
			@return 長さの二乗
		*/
		Float LengthSq( void ) const;
		/**
			@brief 長さ取得
			@author 葉迩倭
			@return 長さ
		*/
		Float Length( void ) const;
		/**
			@brief 長さ取得
			@author 葉迩倭
			@return 長さ
		*/
		Float LengthFast( void ) const;
		/**
			@brief 内積
			@author 葉迩倭
			@param In	[in] 内積算出用入力ベクトル
			@return 内積
		*/
		Float Dot( const Vector4D &In ) const;
		/**
			@brief 正規化
			@author 葉迩倭
			@return 計算結果後の*this

			@note
			長さを1.0に正規化します。
		*/
		Vector4D& Normalize( void );
		/**
			@brief 正規化
			@author 葉迩倭
			@return 計算結果後の*this

			@note
			長さを1.0に正規化します。
		*/
		Vector4D& NormalizeFast( void );
		/**
			@brief 外積
			@author 葉迩倭
			@param In1	[in] 外積算出用入力ベクトル
			@param In2	[in] 外積算出用入力ベクトル
			@return 計算結果後の*this
		*/
		Vector4D& Cross( const Vector4D &In1, const Vector4D &In2 );
		/**
			@brief 反射ベクトルの取得
			@author 葉迩倭
			@param In1	[in] 反射対象の法線
			@param In2	[in] 入射ベクトル
			@return 計算結果後の*this
		*/
		Vector4D& Reflect( const Vector4D &In1, const Vector4D &In2 );
		/**
			@brief 拡大縮小＋回転変換
			@author 葉迩倭
			@param In1	[in] 変換元ベクトル
			@param In2	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector4D& TransformNormal( const Vector4D &In1, const Matrix &In2 );
		/**
			@brief 拡大縮小＋回転＋移動変換
			@author 葉迩倭
			@param In1	[in] 変換元ベクトル
			@param In2	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector4D& TransformCoord( const Vector4D &In1, const Matrix &In2 );
		/**
			@brief 透視変換
			@author 葉迩倭
			@param In1	[in] 変換元ベクトル
			@param In2	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector4D& TransformProjection( const Vector4D &In1, const Matrix &In2 );
		/**
			@brief 正規化
			@author 葉迩倭
			@param In	[in] 正規化元のベクトル
			@return 計算結果後の*this
		*/
		Vector4D& Normalize( const Vector4D &In );
		/**
			@brief 正規化
			@author 葉迩倭
			@param In	[in] 正規化元のベクトル
			@return 計算結果後の*this
		*/
		Vector4D& NormalizeFast( const Vector4D &In );
		/**
			@brief 外積
			@author 葉迩倭
			@param In	[in] 外積用のベクトル
			@return 計算結果後の*this
		*/
		Vector4D& Cross( const Vector4D &In );
		/**
			@brief 反射
			@author 葉迩倭
			@param In	[in] 反射用法線
			@return 計算結果後の*this
		*/
		Vector4D& Reflect( const Vector4D &In );
		/**
			@brief 拡大縮小＋回転変換
			@author 葉迩倭
			@param In	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector4D& TransformNormal( const Matrix &In );
		/**
			@brief 拡大縮小＋回転＋移動変換
			@author 葉迩倭
			@param In	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector4D& TransformCoord( const Matrix &In );
		/**
			@brief 透視変換
			@author 葉迩倭
			@param In	[in] 変換用マトリックス
			@return 計算結果後の*this
		*/
		Vector4D& TransformProjection( const Matrix &In );
		/**
			@brief X軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector4D& RotationX( Sint32 Angle );
		/**
			@brief Y軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector4D& RotationY( Sint32 Angle );
		/**
			@brief Z軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 計算結果後の*this
		*/
		Vector4D& RotationZ( Sint32 Angle );
		/**
			@brief ベクトルを軸に回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@param Axis		[in] 回転軸
			@return 計算結果後の*this
		*/
		Vector4D& RotationAxis( Sint32 Angle, const Vector3D &Axis );
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Bool operator == ( const Vector4D &v )
		{
			return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Bool operator != ( const Vector4D &v )
		{
			return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator + () const
		{
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator - () const
		{
			return Vector4D(-x, -y, -z, 1.0);
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator += ( const Vector2D &v )
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator -= ( const Vector2D &v )
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator *= ( const Vector2D &v )
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator /= ( const Vector2D &v )
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator += ( const Vector3D &v )
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator -= ( const Vector3D &v )
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator *= ( const Vector3D &v )
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator /= ( const Vector3D &v )
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator += ( Float f )
		{
			x += f;
			y += f;
			z += f;
			w += f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator -= ( Float f )
		{
			x -= f;
			y -= f;
			z -= f;
			w -= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator *= ( Float f )
		{
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator /= ( Float f )
		{
			x /= f;
			y /= f;
			z /= f;
			w /= f;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator += ( const Vector4D &v )
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator -= ( const Vector4D &v )
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator *= ( const Vector4D &v )
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator /= ( const Vector4D &v )
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D& operator *= ( Matrix &m )
		{
			return TransformCoord( m );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator + ( Float f ) const
		{
			return Vector4D(x + f, y + f, z + f, w + f );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator - ( Float f ) const
		{
			return Vector4D(x - f, y - f, z - f, w - f );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator * ( Float f ) const
		{
			return Vector4D(x * f, y * f, z * f, w * f );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator / ( Float f ) const
		{
			return Vector4D(x / f, y / f, z / f, w / f );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator + ( const Vector2D &v ) const
		{
			return Vector4D(x + v.x, y + v.y, z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator - ( const Vector2D &v ) const
		{
			return Vector4D(x - v.x, y - v.y, z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator * ( const Vector2D &v ) const
		{
			return Vector4D(x * v.x, y * v.y, z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator / ( const Vector2D &v ) const
		{
			return Vector4D(x / v.x, y / v.y, z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator + ( const Vector3D &v ) const
		{
			return Vector4D(x + v.x, y + v.y, z + v.z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator - ( const Vector3D &v ) const
		{
			return Vector4D(x - v.x, y - v.y, z - v.z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator * ( const Vector3D &v ) const
		{
			return Vector4D(x * v.x, y * v.y, z * v.z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator / ( const Vector3D &v ) const
		{
			return Vector4D(x / v.x, y / v.y, z / v.z, w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator + ( const Vector4D &v ) const
		{
			return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator - ( const Vector4D &v ) const
		{
			return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator * ( const Vector4D &v ) const
		{
			return Vector4D(x * v.x, y * v.y, z * v.z, w * v.w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator / ( const Vector4D &v ) const
		{
			return Vector4D(x / v.x, y / v.y, z / v.z, w / v.w );
		}
		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Vector4D operator * ( Matrix &m ) const
		{
			Vector4D vTemp;
			return vTemp.TransformCoord( *this, m );
		}
	};
}
}

#pragma once


/**
	@file
	@brief 四元数管理
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 四元数クラス
	*/
	class SELENE_DLL_API Quaternion
	{
	public:
		Float x;
		Float y;
		Float z;
		Float w;

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Quaternion();
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param x	[in] X要素
			@param y	[in] Y要素
			@param z	[in] Z要素
			@param w	[in] W要素
		*/
		Quaternion( Float x, Float y, Float z, Float w );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param In	[in] クォータニオン
		*/
		Quaternion( const Quaternion &In );
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param mIn	[in] マトリックス
		*/
		Quaternion( const Matrix &mIn );
		/**
			@brief 加算
			@author 葉迩倭
			@param In1	[in] 入力クォータニオン１
			@param In2	[in] 入力クォータニオン２
			@return 計算結果後の*this
			@note
			二つのクォータニオンを加算します。
		*/
		Quaternion& Add( const Quaternion &In1, const Quaternion &In2 );
		/**
			@brief 減算
			@author 葉迩倭
			@param In1	[in] 入力クォータニオン１
			@param In2	[in] 入力クォータニオン２
			@return 計算結果後の*this
			@note
			二つのクォータニオンを減算します。
		*/
		Quaternion& Sub( const Quaternion &In1, const Quaternion &In2 );
		/**
			@brief 乗算
			@author 葉迩倭
			@param In1	[in] 入力クォータニオン１
			@param In2	[in] 入力クォータニオン２
			@return 計算結果後の*this
			@note
			二つのクォータニオンを乗算します。
		*/
		Quaternion& Mul( const Quaternion &In1, const Quaternion &In2 );
		/**
			@brief 線形補間
			@author 葉迩倭
			@param In0		[in] 入力クォータニオン１
			@param In1		[in] 入力クォータニオン２
			@param fRate	[in] ブレンド率(0.0～1.0f)
			@return 計算結果後の*this
			@note
			二つのクォータニオンを線形補間します
		*/
		Quaternion& Lerp( const Quaternion &In0, const Quaternion &In1, Float fRate );
		/**
			@brief 球面線形補間
			@author 葉迩倭
			@param In0		[in] 入力クォータニオン１
			@param In1		[in] 入力クォータニオン２
			@param fRate	[in] ブレンド率(0.0～1.0f)
			@return 計算結果後の*this
			@note
			二つのクォータニオンを球面線形補間します
		*/
		Quaternion& Slerp( const Quaternion &In0, const Quaternion &In1, Float fRate );
		/**
			@brief 加算
			@author 葉迩倭
			@param In	[in] 入力クォータニオン
			@note
			二つのクォータニオンを加算します。
		*/
		Quaternion& Add( const Quaternion &In );
		/**
			@brief 減算
			@author 葉迩倭
			@param In	[in] 入力クォータニオン
			@return 計算結果後の*this
			@note
			二つのクォータニオンを減算します。
		*/
		Quaternion& Sub( const Quaternion &In );
		/**
			@brief 乗算
			@author 葉迩倭
			@param In	[in] 入力クォータニオン
			@return 計算結果後の*this
			@note
			二つのクォータニオンを乗算します。
		*/
		Quaternion& Mul( const Quaternion &In );
		/**
			@brief 線形補間
			@author 葉迩倭
			@param In		[in] 入力クォータニオン
			@param fRate	[in] ブレンド率(0.0～1.0f)
			@return 計算結果後の*this
			@note
			二つのクォータニオンを線形補間します
		*/
		Quaternion& Lerp( const Quaternion &In, Float fRate );
		/**
			@brief 球面線形補間
			@author 葉迩倭
			@param In		[in] 入力クォータニオン
			@param fRate	[in] ブレンド率(0.0～1.0f)
			@return 計算結果後の*this
			@note
			二つのクォータニオンを球面線形補間します
		*/
		Quaternion& Slerp( const Quaternion &In, Float fRate );
		/**
			@brief 初期化
			@author 葉迩倭
			@return 計算結果後の*this
			@note
			xyz=0.0、w=1.0に初期化します。
		*/
		Quaternion& Identity( void );
		/**
			@brief 長さの二乗を取得
			@author 葉迩倭
			@return 長さの二乗
			@note
			長さの二乗を取得します。
		*/
		Float LengthSq( void ) const;
		/**
			@brief 長さを取得
			@author 葉迩倭
			@return 長さ
			@note
			長さを取得します。
		*/
		Float Length( void ) const;
		/**
			@brief 内積を取得
			@author 葉迩倭
			@return 内積
			@note
			内積を取得します
		*/
		Float Dot( const Quaternion &In ) const;
		/**
			@brief 共役
			@author 葉迩倭
			@return 計算結果後の*this
			@note
			クォータニオンを共役します。
		*/
		Quaternion& Conjugate( void );
		/**
			@brief 共役
			@author 葉迩倭
			@return 計算結果後の*this
			@note
			クォータニオンを共役して格納します。
		*/
		Quaternion& Conjugate( const Quaternion &In );
	};
}
}

#pragma once


/**
	@file
	@brief 行列演算
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 行列構造体
	*/
	struct SELENE_DLL_API SMatrix4x4
	{
		union {
			struct {
				Vector4D x;
				Vector4D y;
				Vector4D z;
				Vector4D w;
			};
			Float m[4][4];
		};
	};

	/**
		@brief 行列クラス
	*/
	class SELENE_DLL_API Matrix : public SMatrix4x4
	{
	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Matrix();

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Matrix( const Matrix &Mat );

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Matrix( const SMatrix4x4 &Mat );

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Matrix( const Quaternion &In );

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Matrix(
			Float _11, Float _12, Float _13, Float _14,
			Float _21, Float _22, Float _23, Float _24,
			Float _31, Float _32, Float _33, Float _34,
			Float _41, Float _42, Float _43, Float _44 );

		/**
			@brief 行列の単位化
			@author 葉迩倭
		*/
		void Identity( void );

		/**
			@brief 行列の転置化
			@author 葉迩倭
			@param In	[in] 処理元行列
		*/
		Matrix &Transpose( const Matrix &In );

		/**
			@brief 行列の逆行列化
			@author 葉迩倭
			@param In	[in] 処理元行列
		*/
		Matrix &Inverse( const Matrix &In );

		/**
			@brief 行列の合成
			@author 葉迩倭
			@param In1		[in] 合成元行列１
			@param In2		[in] 合成元行列２
		*/
		Matrix &Multiply( const Matrix &In1, const Matrix &In2 );

		/**
			@brief 行列の転置行列化
			@author 葉迩倭
		*/
		Matrix &Transpose( void );

		/**
			@brief 行列の逆行列化
			@author 葉迩倭
		*/
		Matrix &Inverse( void );

		/**
			@brief 行列の合成
			@author 葉迩倭
			@param In	[in] 合成元行列
		*/
		Matrix &Multiply( const Matrix &In );

		/**
			@brief Ｘ軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
		*/
		Matrix &RotationX( Sint32 Angle );

		/**
			@brief Ｙ軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
		*/
		Matrix &RotationY( Sint32 Angle );

		/**
			@brief Ｚ軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
		*/
		Matrix &RotationZ( Sint32 Angle );

		/**
			@brief ＺＸＹ回転行列を生成
			@author 葉迩倭
			@param AngleX	[in] Ｘ軸回転角度
			@param AngleY	[in] Ｙ軸回転角度
			@param AngleZ	[in] Ｚ軸回転角度
		*/
		Matrix &RotationZXY( Sint32 AngleX, Sint32 AngleY, Sint32 AngleZ );

		/**
			@brief 任意軸回転
			@author 葉迩倭
			@param Angle	[in] 角度
			@param Axis		[in] 回転軸
		*/
		Matrix &RotationAxis( Sint32 Angle, const Vector3D &Axis );

		/**
			@brief 拡大縮小行列生成
			@author 葉迩倭
			@param sx	[in] Ｘ方向拡大率
			@param sy	[in] Ｙ方向拡大率
			@param sz	[in] Ｚ方向拡大率
		*/
		Matrix &Scaling( Float sx, Float sy, Float sz );

		/**
			@brief 移動行列生成
			@author 葉迩倭
			@param px	[in] Ｘ位置
			@param py	[in] Ｙ位置
			@param pz	[in] Ｚ位置
		*/
		Matrix &Translation( Float px, Float py, Float pz );

		/**
			@brief 透視変換用行列生成
			@author 葉迩倭
			@param Fov		[in] 画角
			@param NearZ	[in] 近接クリップ面
			@param FarZ		[in] 遠方クリップ面
			@param Aspect	[in] アスペクト比（＝描画エリア横幅÷描画エリア縦幅）
		*/
		Matrix &Perspective( Sint32 Fov, Float NearZ, Float FarZ, Float Aspect );

		/**
			@brief 並行投影用行列生成
			@author 葉迩倭
			@param Width	[in] 横幅
			@param Height	[in] 縦幅
			@param NearZ	[in] 近接クリップ面
			@param FarZ		[in] 遠方クリップ面
		*/
		Matrix &Ortho( Float Width, Float Height, Float NearZ, Float FarZ );

		/**
			@brief 任意点注視行列作成
			@author 葉迩倭
			@param Eye	[in] 視点
			@param At	[in] 注視点
			@param Up	[in] 上方向
		*/
		Matrix &LookAt( const Vector3D &Eye, const Vector3D &At, const Vector3D &Up );

		/**
			@brief 行列の分解
			@author 葉迩倭
			@param vTranslate	[in] 移動
			@param vScale		[in] 拡大縮小
			@param qRotate		[in] 回転
		*/
		void Resolution( Vector3D &vTranslate, Vector3D &vScale, Quaternion &qRotate );

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Matrix &operator = ( const Quaternion &q )
		{
			Float xx = q.x * q.x;
			Float yy = q.y * q.y;
			Float zz = q.z * q.z;
			Float xy = q.x * q.y;
			Float xz = q.x * q.z;
			Float yz = q.y * q.z;
			Float wx = q.w * q.x;
			Float wy = q.w * q.y;
			Float wz = q.w * q.z;

			x.x = 1.0f - 2.0f * (yy + zz);
			x.y =        2.0f * (xy - wz);
			x.z =        2.0f * (xz + wy);
			x.w = 0.0f;

			y.x =        2.0f * (xy + wz);
			y.y = 1.0f - 2.0f * (xx + zz);
			y.z =        2.0f * (yz - wx);
			y.w = 0.0f;

			z.x =        2.0f * (xz - wy);
			z.y =        2.0f * (yz + wx);
			z.z = 1.0f - 2.0f * (xx + yy);
			z.w = 0.0f;

			w.x = 0.0f;
			w.y = 0.0f;
			w.z = 0.0f;
			w.w = 1.0f;

			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Matrix &operator = ( const Matrix &m )
		{
			MemoryCopy( this->m, m.m, sizeof(Float[4][4]) );
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Matrix &operator = ( const SMatrix4x4 &m )
		{
			MemoryCopy( this->m, m.m, sizeof(Float[4][4]) );
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Matrix operator * ( const Matrix &m )
		{
			Matrix mTemp;
			return mTemp.Multiply( *this, m );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Matrix &operator *= ( const Matrix &m )
		{
			return Multiply( m );
		}

		/**
			@brief 単位行列
			@author 葉迩倭
			@return 単位行列
		*/
		static Matrix GetIdentity( void )
		{
			Matrix mTemp;
			mTemp.Identity();
			return mTemp;
		}

		/**
			@brief 移動行列生成
			@author 葉迩倭
			@param px	[in] Ｘ位置
			@param py	[in] Ｙ位置
			@param pz	[in] Ｚ位置
			@return 移動行列
		*/
		static Matrix GetTranslation( Float px, Float py, Float pz )
		{
			Matrix mTemp;
			mTemp.Translation( px, py, pz );
			return mTemp;
		}

		/**
			@brief Ｘ軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 回転行列
		*/
		static Matrix GetRotationX( Sint32 Angle )
		{
			Matrix mTemp;
			mTemp.RotationX( Angle );
			return mTemp;
		}

		/**
			@brief Ｙ軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 回転行列
		*/
		static Matrix GetRotationY( Sint32 Angle )
		{
			Matrix mTemp;
			mTemp.RotationY( Angle );
			return mTemp;
		}

		/**
			@brief Ｚ軸回転
			@author 葉迩倭
			@param Angle	[in] 回転角度
			@return 回転行列
		*/
		static Matrix GetRotationZ( Sint32 Angle )
		{
			Matrix mTemp;
			mTemp.RotationZ( Angle );
			return mTemp;
		}
		
		/**
			@brief 拡大縮小行列
			@author 葉迩倭
			@param sx	[in] Ｘ方向拡大率
			@param sy	[in] Ｙ方向拡大率
			@param sz	[in] Ｚ方向拡大率
			@return 拡大縮小行列
		*/
		static Matrix GetScaling( Float sx, Float sy, Float sz )
		{
			Matrix mTemp;
			mTemp.Scaling( sx, sy, sz );
			return mTemp;
		}
	};
}
}

#pragma once


/**
	@file
	@brief 姿勢管理
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 姿勢制御クラス
	*/
	class SELENE_DLL_API Style
	{
	private:
		Vector3D m_vUp;					///< 上方向ベクトル
		Vector3D m_vRight;				///< 右方向ベクトル
		Vector3D m_vFront;				///< 前方向ベクトル
		Vector3D m_vPosition;			///< 位置ベクトル
		Vector3D m_vLocalPosition;		///< 位置ベクトル
		Vector3D m_vScale;				///< 拡大縮小ベクトル

		Bool m_IsScale;					///< 拡大縮小
		Bool m_IsLocal;					///< ローカル移動

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Style();
		/**
			@brief コンストラクタ
			@author 葉迩倭
			@param Style	[in] コピー元
		*/
		Style( const Style &Style );
		/**
			@brief 変換初期化
			@author 葉迩倭

			@note
			変換を初期化して初期値に戻します。
		*/
		virtual void TransformReset( void );
		/**
			@brief X軸回転
			@author 葉迩倭
			@param Rot		[in] 角度

			@note
			ワールド座標を基準にX軸周りに回転させます。
		*/
		virtual void RotationX( Sint32 Rot );
		/**
			@brief Y軸回転
			@author 葉迩倭
			@param Rot		[in] 角度

			@note
			ワールド座標を基準にY軸周りに回転させます。
		*/
		virtual void RotationY( Sint32 Rot );
		/**
			@brief Z軸回転
			@author 葉迩倭
			@param Rot		[in] 角度

			@note
			ワールド座標を基準にZ軸周りに回転させます。
		*/
		virtual void RotationZ( Sint32 Rot );
		/**
			@brief ピッチング
			@author 葉迩倭
			@param Rot		[in] 角度

			@note
			ローカル座標を基準にX軸周りに回転させます。
		*/
		virtual void LocalRotationX( Sint32 Rot );
		/**
			@brief ヘッディング
			@author 葉迩倭
			@param Rot		[in] 角度

			@note
			ローカル座標を基準にY軸周りに回転させます。
		*/
		virtual void LocalRotationY( Sint32 Rot );
		/**
			@brief ローリング
			@author 葉迩倭
			@param Rot		[in] 角度

			@note
			ローカル座標を基準にZ軸周りに回転させます。
		*/
		virtual void LocalRotationZ( Sint32 Rot );
		/**
			@brief クォータニオンで回転
			@author 葉迩倭
			@param Quat		[in] クォータニオン

			@note
			クォータニオンを使って回転させます。
		*/
		virtual void RotationQuaternion( const Quaternion &Quat );
		/**
			@brief 移動
			@author 葉迩倭
			@param fPx		[in] X座標
			@param fPy		[in] Y座標
			@param fPz		[in] Z座標

			@note
			指定座標へ移動させます。
		*/
		virtual void Translation( Float fPx, Float fPy, Float fPz );
		/**
			@brief 移動
			@author 葉迩倭
			@param vPos		[in] 座標

			@note
			指定座標へ移動させます。
		*/
		virtual void Translation( const Vector3D &vPos );
		/**
			@brief 移動
			@author 葉迩倭
			@param fPx		[in] X座標
			@param fPy		[in] Y座標
			@param fPz		[in] Z座標

			@note
			指定座標へ移動させます。
		*/
		virtual void LocalTranslation( Float fPx, Float fPy, Float fPz );
		/**
			@brief 移動
			@author 葉迩倭
			@param vPos		[in] 座標

			@note
			指定座標へ移動させます。
		*/
		virtual void LocalTranslation( const Vector3D &vPos );
		/**
			@brief 拡大縮小
			@author 葉迩倭
			@param fSx		[in] X軸拡大縮小率
			@param fSy		[in] Y軸拡大縮小率
			@param fSz		[in] Z軸拡大縮小率

			@note
			指定倍率で拡大縮小をさせます。
		*/
		virtual void Scaling( Float fSx, Float fSy, Float fSz );
		/**
			@brief 任意方向へ姿勢
			@author 葉迩倭
			@param vEye		[in] 視点位置
			@param vAt		[in] 目標位置
			@param vUp		[in] 上方向

			@note
			任意の場所から指定の場所を見た場合の姿勢を作成します。
		*/
		virtual void LookAt( const Vector3D &vEye, const Vector3D &vAt, const Vector3D &vUp );
		/**
			@brief 行列から変換
			@author 葉迩倭
			@param Mat		[in] 変換マトリックス

			@note
			指定のマトリックスになるような姿勢データを生成します。
		*/
		virtual void FromMatrix( const Matrix &Mat );
		/**
			@brief X軸回転角度取得
			@author 葉迩倭
			@return X軸における回転角度

			@note
			X軸における回転角度を取得します。
		*/
		virtual Sint32 GetAngleX( void ) const;
		/**
			@brief Y軸回転角度取得
			@author 葉迩倭
			@return Y軸における回転角度

			@note
			Y軸における回転角度を取得します。
		*/
		virtual Sint32 GetAngleY( void ) const;
		/**
			@brief Z軸回転角度取得
			@author 葉迩倭
			@return Z軸における回転角度

			@note
			Z軸における回転角度を取得します。
		*/
		virtual Sint32 GetAngleZ( void ) const;
		/**
			@brief 位置設定
			@author 葉迩倭
			@param Pos	[in] 位置

			@note
			指定位置に移動します。
		*/
		virtual void SetPosition( const Vector3D &Pos );
		/**
			@brief 前方設定
			@author 葉迩倭
			@param Vec	[in] 前方ベクトル

			@note
			姿勢の正面方向を設定します。
		*/
		virtual void SetFront( const Vector3D &Vec );
		/**
			@brief 右方設定
			@author 葉迩倭
			@param Vec	[in] 右方ベクトル

			@note
			姿勢の右手方向を設定します。
		*/
		virtual void SetRight( const Vector3D &Vec );
		/**
			@brief 上方設定
			@author 葉迩倭
			@param Vec	[in] 上方ベクトル

			@note
			姿勢の上手方向を設定します。
		*/
		virtual void SetUp( const Vector3D &Vec );
		/**
			@brief 位置取得
			@author 葉迩倭
			@param Pos	[out] 格納先

			@note
			指定位置に移動します。
		*/
		virtual void GetPosition( Vector3D &Pos ) const;
		/**
			@brief 前方取得
			@author 葉迩倭
			@param Vec	[out] 格納先

			@note
			姿勢の正面方向を取得します。
		*/
		virtual void GetFront( Vector3D &Vec ) const;
		/**
			@brief 右方取得
			@author 葉迩倭
			@param Vec	[out] 格納先

			@note
			姿勢の右手方向を取得します。
		*/
		virtual void GetRight( Vector3D &Vec ) const;
		/**
			@brief 上方取得
			@author 葉迩倭
			@param Vec	[out] 格納先

			@note
			姿勢の上手方向を取得します。
		*/
		virtual void GetUp( Vector3D &Vec ) const;
		/**
			@brief 姿勢行列を取得
			@author 葉迩倭
			@param Mat	[out] 格納先

			@note
			姿勢行列を取得します。<BR>
			この行列には回転要素しか含みません。
		*/
		virtual void GetStyle( Matrix &Mat ) const;
		/**
			@brief 逆姿勢行列を取得
			@author 葉迩倭
			@param Mat	[out] 格納先

			@note
			逆姿勢行列を取得します。<BR>
			この行列には回転要素しか含みません。
		*/
		virtual void GetStyleInverse( Matrix &Mat ) const;
		/**
			@brief 変換行列を取得
			@author 葉迩倭
			@param Mat	[out] 格納先

			@note
			変換行列を取得します。<BR>
			この行列には回転と位置要素しか含みません。
		*/
		virtual void GetTransform( Matrix &Mat ) const;
		/**
			@brief 逆変換行列を取得
			@author 葉迩倭
			@param Mat	[out] 格納先

			@note
			逆変換行列を取得します。<BR>
			この行列には回転と位置要素しか含みません。
		*/
		virtual void GetTransformInverse( Matrix &Mat ) const;
		/**
			@brief 変換行列を取得
			@author 葉迩倭
			@param Mat	[out] 格納先

			@note
			変換行列を取得します。<BR>
			この行列には回転と位置と拡大縮小の全要素を含みます。
		*/
		virtual void GetTransformWithScale( Matrix &Mat ) const;
		/**
			@brief 逆変換行列を取得
			@author 葉迩倭
			@param Mat	[out] 格納先

			@note
			逆変換行列を取得します。<BR>
			この行列には回転と位置と拡大縮小の全要素を含みます。
		*/
		virtual void GetTransformInverseWithScale( Matrix &Mat ) const;
	};
}
}

#pragma once


/**
	@file
	@brief 算術演算
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
/**
@brief sin取得
@author 葉迩倭
@param Angle	[in] 1周65536とした角度
@return sin値

@note
sin値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // 90度の時のsin値を取得
  Float fRet = Math::Sin( DEG_TO_ANGLE(90) );

  return 0;
}
@endcode
*/
SELENE_DLL_API Float Sin( Sint32 Angle );

/**
@brief cos取得
@author 葉迩倭
@param Angle	[in] 1周65536とした角度
@return cos値

@note
cos値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // 90度の時のcos値を取得
  Float fRet = Math::Cos( DEG_TO_ANGLE(90) );

  return 0;
}
@endcode
*/
SELENE_DLL_API Float Cos( Sint32 Angle );

/**
@brief ベクトル取得
@author 葉迩倭
@param Angle	[in] 1周65536とした角度
@return 2Dベクトル

@note
正規化された2Dベクトルを取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // 90度の方向を向く単位ベクトルを取得
  Vector2D vDir = Math::GetVector( DEG_TO_ANGLE(90) );

  return 0;
}
@endcode
*/
SELENE_DLL_API Vector2D GetVector( Sint32 Angle );

/**
@brief atan2取得
@author 葉迩倭
@param Dx	[in] Ｘ距離
@param Dy	[in] Ｙ距離
@return X-Yの成す角度

@note
X,Yから求められるatan2の値をSeleneの角度単位に<BR>
あわせた値で取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  // (x,y)=(100.0f,50.0f)における原点基準の角度を求める
  Sint32 Angle = Math::ATan2( 100.0f, 50.0f );

  return 0;
}
@endcode
*/
SELENE_DLL_API Sint32 ATan2( Float Dx, Float Dy );

	/**
		@brief 矩形データ
		@author 葉迩倭
	*/
	template <typename Type>
	class Rect2D
	{
	public:
		Type x;					///< Ｘ始点座標
		Type y;					///< Ｙ始点座標
		Type w;					///< Ｘサイズ
		Type h;					///< Ｙサイズ

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Rect2D()
		{
		}

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		template <typename TypeT>
		Rect2D( const Rect2D<TypeT> &Src )
		{
			this->x = (Type)Src.x;
			this->y = (Type)Src.y;
			this->w = (Type)Src.w;
			this->h = (Type)Src.h;
		}

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x		[in] Ｘ始点座標
			@param y		[in] Ｙ始点座標
			@param w		[in] Ｘサイズ
			@param h		[in] Ｙサイズ
		*/
		Rect2D( Type x, Type y, Type w, Type h )
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}

		/**
			@brief 値設定
			@author 葉迩倭

			@param x		[in] Ｘ中心座標
			@param y		[in] Ｙ中心座標
			@param w		[in] Ｘサイズ
			@param h		[in] Ｙサイズ
		*/
		void Set( Type x, Type y, Type w, Type h )
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}

		/**
			@brief 値設定
			@author 葉迩倭

			@param x		[in] Ｘ中心座標
			@param y		[in] Ｙ中心座標
			@param w		[in] Ｘサイズ
			@param h		[in] Ｙサイズ
			@param Scale	[in] 拡大縮小率
		*/
		void SetCenter( Type x, Type y, Type w, Type h, Float Scale = 1.0f )
		{
			this->w = (Type)(w * Scale);
			this->h = (Type)(h * Scale);
			this->x = x - (Type)(toF(this->w) * 0.5f);
			this->y = y - (Type)(toF(this->h) * 0.5f);
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		template <typename TypeT>
		Rect2D<Type> & operator = ( const Rect2D<TypeT> &Src )
		{
			this->x = (Type)Src.x;
			this->y = (Type)Src.y;
			this->w = (Type)Src.w;
			this->h = (Type)Src.h;
			return *this;
		}
	};

	/**
		@brief ２次元の点
		@author 葉迩倭
	*/
	template <typename Type>
	class Point2D
	{
	public:
		Type x;					///< Ｘ座標
		Type y;					///< Ｙ座標

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Point2D()
		{
		}

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		template <typename TypeT>
		Point2D( const Point2D<TypeT> &Src )
		{
			this->x = (Type)Src.x;
			this->y = (Type)Src.y;
		}

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x		[in] Ｘ座標
			@param y		[in] Ｙ座標
		*/
		Point2D( Type x, Type y )
		{
			this->x = x;
			this->y = y;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> operator + ( Point2D<Type> &Pt )
		{
			return Point2D<Type>( this->x + Pt.x, this->y + Pt.y );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> operator - ( Point2D<Type> &Pt )
		{
			return Point2D<Type>( this->x - Pt.x, this->y - Pt.y );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> operator * ( Type Mul )
		{
			return Point2D<Type>( this->x * Mul, this->y * Mul );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> operator / ( Type Div )
		{
			return Point2D<Type>( this->x / Div, this->y / Div );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> &operator += ( Point2D<Type> &Pt )
		{
			this->x += Pt.x;
			this->y += Pt.y;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> &operator -= ( Point2D<Type> &Pt )
		{
			this->x -= Pt.x;
			this->y -= Pt.y;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> &operator *= ( Type Mul )
		{
			this->x *= Mul;
			this->y *= Mul;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point2D<Type> &operator /= ( Type Div )
		{
			this->x /= Div;
			this->y /= Div;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		template <typename TypeT>
		Point2D<Type> & operator = ( const Point2D<TypeT> &Src )
		{
			this->x = (Type)Src.x;
			this->y = (Type)Src.y;
			return *this;
		}
	};

	/**
		@brief ３次元の点
	*/
	template <typename Type>
	struct Point3D
	{
		Type x;							///< Ｘ位置
		Type y;							///< Ｙ位置
		Type z;							///< Ｚ位置

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		Point3D()
		{
		}

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		template <typename TypeT>
		Point3D( const Point3D<TypeT> &Src )
		{
			this->x = (Type)Src.x;
			this->y = (Type)Src.y;
			this->z = (Type)Src.z;
		}

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x		[in] Ｘ座標
			@param y		[in] Ｙ座標
			@param z		[in] Ｚ座標
		*/
		Point3D( Type x, Type y, Type z )
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> operator + ( Point3D<Type> &Pt )
		{
			return Point3D<Type>( this->x + Pt.x, this->y + Pt.y, this->z + Pt.z );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> operator - ( Point3D<Type> &Pt )
		{
			return Point3D<Type>( this->x - Pt.x, this->y - Pt.y, this->z - Pt.z );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> operator * ( Type Mul )
		{
			return Point3D<Type>( this->x * Mul, this->y * Mul, this->z * Mul );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> operator / ( Type Div )
		{
			return Point3D<Type>( this->x / Div, this->y / Div, this->z / Div );
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> &operator += ( Point3D<Type> &Pt )
		{
			this->x += Pt.x;
			this->y += Pt.y;
			this->z += Pt.z;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> &operator -= ( Point3D<Type> &Pt )
		{
			this->x -= Pt.x;
			this->y -= Pt.y;
			this->z -= Pt.z;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> &operator *= ( Type Mul )
		{
			this->x *= Mul;
			this->y *= Mul;
			this->z *= Mul;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		Point3D<Type> &operator /= ( Type Div )
		{
			this->x /= Div;
			this->y /= Div;
			this->z /= Div;
			return *this;
		}

		/**
			@brief オペレーター
			@author 葉迩倭
		*/
		template <typename TypeT>
		Point3D<Type> & operator = ( const Point3D<TypeT> &Src )
		{
			this->x = (Type)Src.x;
			this->y = (Type)Src.y;
			this->z = (Type)Src.z;
			return *this;
		}
	};

	typedef Point3D<Float>		Point3DF;	///< Point3D<Float>の略称
	typedef Point3D<Sint32>		Point3DI;	///< Point3D<Sint32>の略称
	typedef Point2D<Float>		Point2DF;	///< Point2D<Float>の略称
	typedef Point2D<Sint32>		Point2DI;	///< Point2D<Sint32>の略称
	typedef Rect2D<Float>		Rect2DF;	///< Rect2D<Float>の略称
	typedef Rect2D<Sint32>		Rect2DI;	///< Rect2D<Sint32>の略称
}
}

#pragma once


/**
	@file
	@brief 疑似乱数
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Math
{
	/**
		@brief 乱数処理クラス
		@author 葉迩倭
	*/
	class SELENE_DLL_API CRandom
	{
		enum { N = 624UL };

	private:
		Sint32 m_MersenneTwister[N];		///< 乱数生成用ワーク
		Sint32 m_MersenneTwisterCount;		///< 乱数生成用ワーク

	private:
		/**
			@brief 擬似乱数生成
			@author 葉迩倭
			@return 乱数値

			@note
			32Bit整数の擬似乱数を生成します。
		*/
		virtual Sint32 GenerateInt32( void );

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CRandom();

		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CRandom( Sint32 Param );

		/**
			@brief 乱数用種設定
			@author 葉迩倭
			@param Param	[in] シード値

			@note
			乱数生成の種を設定します。
		*/
		virtual void Seed( Sint32 Param );

		/**
			@brief 32Bit整数乱数取得
			@author 葉迩倭
			@return 乱数値

			@note
			32Bit整数の乱数値を取得します。
		*/
		virtual Sint32 GetInt32( void );

		/**
			@brief 64Bit整数乱数取得
			@author 葉迩倭
			@return 乱数値

			@note
			64Bit整数の乱数値を取得します。
		*/
		virtual Uint64 GetInt64( void );

		/**
			@brief 32Bit浮動小数乱数取得
			@author 葉迩倭
			@return 乱数値

			@note
			32Bit浮動小数の乱数値を取得します。
		*/
		virtual Float GetFloat32( void );

		/**
			@brief 指定範囲乱数取得
			@author 葉迩倭
			@param Min [in] 最小値
			@param Max [in] 最大値
			@return 乱数値

			@note
			指定範囲内の整数乱数を取得します。
		*/
		virtual Sint32 GetInt( Sint32 Min, Sint32 Max );

		/**
			@brief 指定範囲乱数取得
			@author 葉迩倭
			@param Min [in] 最小値
			@param Max [in] 最大値
			@return 乱数値

			@note
			指定範囲内の浮動小数乱数を取得します。
		*/
		virtual Float GetFloat( Float Min, Float Max );
	};
}
}

#pragma once


/**
	@file
	@brief 数値補間
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Interpolation
{
/**
@brief 等速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
等速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Flat( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Flat( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(Time) / toF(TimeAll);
		return (TypeA)(toF(Start) + toF(End - Start) * fRate);
	}

/**
@brief 加速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Add( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Add( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(Time) / toF(TimeAll);
		fRate *= fRate;
		return (TypeA)(toF(Start) + toF(End - Start) * fRate);
	}

/**
@brief 減速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Sub( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Sub( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(TimeAll-Time) / toF(TimeAll);
		fRate *= fRate;
		return (TypeA)(toF(End) + toF(Start - End) * fRate);
	}

/**
@brief 加速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Add2( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Add2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(Time) / toF(TimeAll);
		fRate *= fRate;
		fRate *= fRate;
		return (TypeA)(toF(Start) + toF(End - Start) * fRate);
	}

/**
@brief 減速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Sub2( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Sub2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(TimeAll-Time) / toF(TimeAll);
		fRate *= fRate;
		fRate *= fRate;
		return (TypeA)(toF(End) + toF(Start - End) * fRate);
	}

/**
@brief 加速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Add4( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Add4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(Time) / toF(TimeAll);
		fRate *= fRate;
		fRate *= fRate;
		fRate *= fRate;
		return (TypeA)(toF(Start) + toF(End - Start) * fRate);
	}

/**
@brief 減速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Sub4( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Sub4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Float fRate = toF(TimeAll-Time) / toF(TimeAll);
		fRate *= fRate;
		fRate *= fRate;
		fRate *= fRate;
		return (TypeA)(toF(End) + toF(Start - End) * fRate);
	}

/**
@brief 加速→減速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速→減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::AddSub( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA AddSub( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Add( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Sub( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

/**
@brief 減速→加速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速→減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::SubAdd( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA SubAdd( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Sub( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Add( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

/**
@brief 加速→減速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速→減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::AddSub2( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA AddSub2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Add2( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Sub2( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

/**
@brief 減速→加速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
減速→加速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::SubAdd2( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA SubAdd2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Sub2( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Add2( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

/**
@brief 加速→減速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
加速→減速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::AddSub4( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA AddSub4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Add4( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Sub4( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

/**
@brief 減速→加速運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
減速→加速運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::SubAdd4( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA SubAdd4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Sub4( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Add4( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

/**
@brief sin運動
@author 葉迩倭
@param Start	[in] 開始値
@param End		[in] 終了値
@param TimeAll	[in] End到達時間
@param Time		[in] 現在時間
@return Timeにおける値

@note
sin波で運動を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで移動する
      Sint32 Pos = Interpolation::Sin( 0L, 100L, 60L, Time );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	template <typename TypeA, typename TypeB>
	inline TypeA Sin( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Sint32 Angle = (Sint32)(DEG_TO_ANGLE(180) * Time / TimeAll);
		Float fRate = Math::Sin( Angle );
		return (TypeA)(toF(Start) + toF(End - Start) * fRate);
	}

/**
@brief ネヴィル補間
@author 葉迩倭
@param Start	[in] 開始値
@param Center	[in] 中間値
@param End		[in] 終了値
@param fTime	[in] 現在時間（0.0～1.0）
@return Timeにおける値

@note
ネヴィル補間を行うときの補間値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore *pCore = NULL;
  Sint32 Time = 0;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      Time++;

      // 60カウントで0から100まで中間で30を経由して移動する
      Sint32 Pos = Interpolation::Neville( 0.0f, 30.0f, 100.0f, toF(Time) / 60.0f );
      if ( Time > 60L )
      {
      }
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
	inline Float Neville( Float Start, Float Center, Float End, Float fTime )
	{
		fTime *= 2.0f;
		Start = Center + (Center - Start) * (fTime - 1.0f);
		Center = End + (End - Center) * (fTime - 2.0f);
		return Center + (Center - Start) * (fTime - 2.0f) * 0.5f;
	}
}
}

#pragma once


/**
	@file
	@brief アニメーション処理
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	/**
		@brief アニメーションタイプ定義
		@author 葉迩倭
	*/
	enum eAnimationType
	{
		ANIMATION_TYPE_STEP,					///< ステップ補間
		ANIMATION_TYPE_LINEAR,					///< 線形補間
		ANIMATION_TYPE_TCB,						///< ＴＣＢスプライン補間
	};

	/**
		@brief キーフレームデータ
		@author 葉迩倭
	*/
	struct SKeyFrame
	{
		Bool IsIn;								///< スプライン時のIN係数を定数にするかどうか
		Bool IsOut;								///< スプライン時のOUT係数を定数にするかどうか
		Float fTime;							///< キー時間
		Float fParam;							///< キー値
		Float fIn;								///< IN係数の定数
		Float fOut;								///< OUT係数の定数

		SKeyFrame *pPrev;						///< 前方キーフレームデータポインタ
		SKeyFrame *pNext;						///< 後方キーフレームデータポインタ
	};

namespace Math
{
	/**
		@brief アニメーションクラス
		@author 葉迩倭
	*/
	class SELENE_DLL_API Animation
	{
	private:
		Float m_fTimeFirst;						///< 開始時間
		Float m_fTimeLast;						///< 終了時間
		SKeyFrame *m_pKeys;						///< キーフレームデータポインタ
		Sint32 m_KeyMax;						///< キーフレーム数

	private:
/**
@brief 指定時間から最も近いキーデータを取得します
@author 葉迩倭
@param pKeys		[in] キーフレームデータ
@param fTime		[in] 検索時間
@param KeyMax		[in] キー最大数
@return キーフレームデータポインタ

@note
指定時間からもっとも近いキー情報を、<BR>
バイナリサーチで検索します。
*/
		SKeyFrame *GetKeyData( SKeyFrame *pKeys, Float fTime, Sint32 KeyMax );

/**
@brief エルミート補間関数を処理します
@author 葉迩倭
@param fTime	[in] 時間
@param pH1		[out] パラメーター格納先１
@param pH2		[out] パラメーター格納先２
@param pH3		[out] パラメーター格納先３
@param pH4		[out] パラメーター格納先４

@note
エルミート補間関数を使って与えられたパラメーターから結果を算出します。
*/
		void Hermite( Float fTime, Float *pH1, Float *pH2, Float *pH3, Float *pH4 );

/**
@brief 前方のキーの出力先のパラメータを求めます
@author 葉迩倭
@param pKey0	[in] 前方キーデータ
@param pKey1	[in] 後方キーデータ
@return 算出されたパラメーター

@note
指定時間に対しての直前のキーの挙動を計算します。
*/
		Float InComing( SKeyFrame *pKey0, SKeyFrame *pKey1 );

/**
@brief 後方のキーの出力先のパラメータを求めます
@author 葉迩倭
@param pKey0	[in] 前方キーデータ
@param pKey1	[in] 後方キーデータ
@return 算出されたパラメーター

@note
指定時間に対しての直後のキーの挙動を計算します。
*/
		Float OutGoing( SKeyFrame *pKey0, SKeyFrame *pKey1 );

	public:
/**
@brief コンストラクタ
@author 葉迩倭
@param KeyMax		[in] キー最大数
*/
		Animation( Sint32 KeyMax );

/**
@brief デストラクタ
@author 葉迩倭
*/
		virtual ~Animation();

/**
@brief キーフレームデータにキーを設定します
@author 葉迩倭
@param KeyNo		[in] 設定先キー番号
@param fTime		[in] 設定時間
@param fParam		[in] 設定パラメータ
@param IsIn			[in] 入力パラメーター指定有無
@param IsOut		[in] 出力パラメーター指定有無
@param fIn			[in] 入力パラメーター
@param fOut			[in] 出力パラメーター
@return 成功時はtrue

@note
指定のキーに対して、補間用のパラメーターを設定します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;

  Math::Animation Anim( 3 );	// キー３つで作成
  Anim.SetKey( 0,  0.0f,   0.0f );	// キー１
  Anim.SetKey( 1,  5.0f,  40.0f );	// キー２
  Anim.SetKey( 2, 10.0f, 100.0f );	// キー３

  Float fTime = 0.0f;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      fTime += 0.1f;

      // 時間fTimeにおける値を取得
      // 補間方法はTCBスプライン
      Float fNow = Anim.GetParameter( fTime, ANIMATION_TYPE_TCB, 0.0f );
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Bool SetKey( Sint32 KeyNo, Float fTime, Float fParam, Bool IsIn = false, Bool IsOut = false, Float fIn = 0.0f, Float fOut = 0.0f );

/**
@brief 指定タイプのアニメーションで指定時間における値を取得します
@author 葉迩倭
@param fTime		[in] 時間
@param Type			[in] 補間タイプ
@param fDefault		[in] デフォルト値
@return		補間結果値

@note
指定の時間に対して、補間されたパラメーターを取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;

  Math::Animation Anim( 3 );	// キー３つで作成
  Anim.SetKey( 0,  0.0f,   0.0f );	// キー１
  Anim.SetKey( 1,  5.0f,  40.0f );	// キー２
  Anim.SetKey( 2, 10.0f, 100.0f );	// キー３

  Float fTime = 0.0f;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      fTime += 0.1f;

      // 時間fTimeにおける値を取得
      // 補間方法はTCBスプライン
      Float fNow = Anim.GetParameter( fTime, ANIMATION_TYPE_TCB, 0.0f );
    }
  }

EXIT:
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Float GetParameter( Float fTime, eAnimationType Type, Float fDefault );
	};
}
}

#pragma once


/**
	@file
	@brief ３次元平面
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ３次元の平面
		@author 葉迩倭
	*/
	class SELENE_DLL_API CPlane
	{
	public:
		Math::Vector3D n;		///< 一般平面方程式におけるクリップ面の a b c 係数
		Float d;				///< 一般平面方程式におけるクリップ面の d 係数

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CPlane( void );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param a	[in] 一般平面方程式におけるクリップ面の a 係数
			@param b	[in] 一般平面方程式におけるクリップ面の b 係数
			@param c	[in] 一般平面方程式におけるクリップ面の c 係数
			@param d	[in] 一般平面方程式におけるクリップ面の d 係数
		*/
		CPlane( Float a, Float b, Float c, Float d );

		/**
			@brief 法線データから生成
			@author 葉迩倭

			@param Pt		[in] 平面上の1点
			@param vNormal	[in] 平面の法線
		*/
		void FromNormal( const Math::Vector3D &Pt, const Math::Vector3D &vNormal );

		/**
			@brief 平面上の３点から生成
			@author 葉迩倭

			@param vPt0		[in] 平面上の1点
			@param vPt1		[in] 平面上の1点
			@param vPt2		[in] 平面上の1点
		*/
		void FromPoint( const Math::Vector3D &vPt0, const Math::Vector3D &vPt1, const Math::Vector3D &vPt2 );

		/**
			@brief 内積
			@author 葉迩倭

			@param Pt	[in] 任意の点

			@note
			平面と任意の点の内積をとります。
		*/
		Float Dot( const Math::Vector3D &Pt ) const;
	};
}
}

#pragma once


/**
	@file
	@brief コリジョン用球
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ３次元上の球
		@author 葉迩倭
	*/
	class SELENE_DLL_API CSphere
	{
	public:
		Math::Vector3D vCenter;				///< 球の中心
		Float fRadius;						///< 球の半径

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CSphere( void );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x		[in] Ｘ座標
			@param y		[in] Ｙ座標
			@param z		[in] Ｚ座標
			@param r		[in] 半径
		*/
		CSphere( Float x, Float y, Float z, Float r );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param c		[in] 中心座標
			@param r		[in] 半径
		*/
		CSphere( const Math::Vector3D &c, Float r );
	};
}
}

#pragma once


/**
	@file
	@brief ３次元ボックス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ３次元上の箱
		@author 葉迩倭
	*/
	class SELENE_DLL_API CBox
	{
	public:
		Math::Vector3D Points[8];		///< 箱を構成する8頂点
		CPlane Planes[6];				///< 箱を構成する6平面
		Math::Vector3D vMin;			///< AABBの最少点
		Math::Vector3D vMax;			///< AABBの最大点

	private:
		/**
			@brief ポイントデータを元にボックスの各面、境界球の再計算
			@author 葉迩倭
		*/
		void UpdateBox( void );

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CBox( void );

		/**
			@brief バウンディングボックスを構成する８頂点を指定
			@author 葉迩倭
			@param vPoints	[in] ポイント

			@note
			以下の順番に並んでいる必要があります。<BR>
			(min.x,min.y,min.z)-(min.x,max.y,min.z)-(min.x,max.y,max.z)-(min.x,min.y,max.z)-<BR>
			(max.x,min.y,min.z)-(max.x,max.y,min.z)-(max.x,max.y,max.z)-(max.x,min.y,max.z)
		*/
		void SetPoints( const Math::Vector3D vPoints[] );

		/**
			@brief ３軸指定でバウンディングボックスを構成する８頂点を更新
			@author 葉迩倭
			@param vCenter	[in] 中心
			@param vAxis	[in] 軸(X,Y,Z)
			@param fLength	[in] 軸の長さ(X,Y,Z)
		*/
		void UpdatePoints( const Math::Vector3D &vCenter, const Math::Vector3D vAxis[], const Float fLength[] );

		/**
			@brief バウンディングボックスを指定した行列で変換します。
			@author 葉迩倭
			@param Matrix	[in] ポイントを変換するための行列
		*/
		void Transform( const Math::Matrix &Matrix );

		/**
			@brief バウンディングボックスを指定した行列で変換します。
			@author 葉迩倭
			@param vPoints	[in] ポイントの配列
			@param Matrix	[in] ポイントを変換するための行列
		*/
		void Transform( const Math::Vector3D vPoints[], const Math::Matrix &Matrix );

		/**
			@brief バウンディングボックスを指定した行列で透視変換します。
			@author 葉迩倭
			@param Matrix	[in] ポイントを透視変換するための行列
		*/
		void TransformProjection( const Math::Matrix &Matrix );

		/**
			@brief バウンディングボックスを指定した行列で透視変換します。
			@author 葉迩倭
			@param vPoints	[in] ポイントの配列
			@param Matrix	[in] ポイントを透視変換するための行列
		*/
		void TransformProjection( const Math::Vector3D vPoints[], const Math::Matrix &Matrix );

		/**
			@brief バウンディングボックスを生成
			@author 葉迩倭
			@param MinPt	[in] AABBの最小値
			@param MaxPt	[in] AABBの最大値
			@param Matrix	[in] ポイントを変換するための行列
		*/
		void CreateBox( const Math::Vector3D &MinPt, const Math::Vector3D &MaxPt, const Math::Matrix &Matrix );

		/**
			@brief バウンディングボックスを生成
			@author 葉迩倭
			@param MinPt	[in] AABBの最小値
			@param MaxPt	[in] AABBの最大値
			@param Matrix	[in] ポイントを透視変換するための行列
		*/
		void CreateBoxProjection( const Math::Vector3D &MinPt, const Math::Vector3D &MaxPt, const Math::Matrix &Matrix );
	};
}
}

#pragma once


/**
	@file
	@brief ２次元線分
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ２次元の線分
		@author 葉迩倭
	*/
	class SELENE_DLL_API CLine2D
	{
	public:
		Math::Vector2D vStart;				///< 線分始点位置
		Math::Vector2D vEnd;				///< 線分終点位置

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CLine2D( void );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x0		[in] Ｘ座標
			@param y0		[in] Ｙ座標
			@param x1		[in] Ｘ座標
			@param y1		[in] Ｙ座標
		*/
		CLine2D( Float x0, Float y0, Float x1, Float y1 );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param pt0		[in] 線分の頂点１
			@param pt1		[in] 線分の頂点２
		*/
		CLine2D( Math::Vector2D &pt0, Math::Vector2D &pt1 );
	};
}
}


#pragma once


/**
	@file
	@brief ３次元線分
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ３次元の線分
		@author 葉迩倭
	*/
	class SELENE_DLL_API CLine3D
	{
	public:
		Math::Vector3D vStart;				///< 線分始点位置
		Math::Vector3D vEnd;				///< 線分終点位置

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CLine3D( void );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x0		[in] Ｘ座標
			@param y0		[in] Ｙ座標
			@param z0		[in] Ｚ座標
			@param x1		[in] Ｘ座標
			@param y1		[in] Ｙ座標
			@param z1		[in] Ｚ座標
		*/
		CLine3D( Float x0, Float y0, Float z0, Float x1, Float y1, Float z1 );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param pt0	[in] 線分の頂点１
			@param pt1	[in] 線分の頂点２
		*/
		CLine3D( const Math::Vector3D &pt0, const Math::Vector3D &pt1 );
	};
}
}

#pragma once


/**
	@file
	@brief ２次元多角形
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ２次元の多角形
		@author 葉迩倭
	*/
	class SELENE_DLL_API CPolygon2D
	{
	public:
		Sint32 Count;					///< ポイント配列のポイント数
		Math::Vector2D *pPts;			///< ポリゴンを表すためのポイント配列のポインタ

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CPolygon2D( void );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param Cnt		[in] 頂点数
			@param pPt		[in] 頂点の配列のアドレス(中でコピーはされませんので、参照元のメモリは保持しておく必要があります)

			@note
			Collisionクラスで判定を行う場合、Cntの値は4以上でpPt[0]==pPt[Cnt-1]になっており、<BR>
			pPtの内容は時計回り、あるいは反時計回りの凸形状をしている必要があります。
		*/
		CPolygon2D( Sint32 Cnt, Math::Vector2D *pPt );
	};
}
}

#pragma once


/**
	@file
	@brief ２次元円
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
	/**
		@brief ２次元上の円
		@author 葉迩倭
	*/
	class SELENE_DLL_API CCircle
	{
	public:
		Math::Vector2D vCenter;			///< 円の中心
		Float fRadius;					///< 円の半径

	public:
		/**
			@brief コンストラクタ
			@author 葉迩倭
		*/
		CCircle( void );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param x		[in] Ｘ座標
			@param y		[in] Ｙ座標
			@param r		[in] 半径
		*/
		CCircle( Float x, Float y, Float r );

		/**
			@brief コンストラクタ
			@author 葉迩倭

			@param c		[in] 中心座標
			@param r		[in] 半径
		*/
		CCircle( Math::Vector2D &c, Float r );
	};
}
}

#pragma once


/**
	@file
	@brief 衝突判定
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Collision
{
/**
@brief 2D点と2D点の衝突判定
@author 葉迩倭
@param Pt1		[in] 2D平面上の点
@param Pt2		[in] 2D平面上の点
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の2つの点の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector2D Pt1, Pt2;
  // 衝突判定
  if ( Collision::Point_Point( Pt1, Pt2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Point( const Math::Vector2D &Pt1, const Math::Vector2D &Pt2 );
/**
@brief 2D点と2D線分の衝突判定
@author 葉迩倭
@param Pt		[in] 2D平面上の点
@param Line		[in] 2D平面上の線分
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の点と線分の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector2D Pt;
  Collision::CLine2D Line;
  // 衝突判定
  if ( Collision::Point_Line( Pt, Line ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Line( const Math::Vector2D &Pt, const CLine2D &Line );
/**
@brief 2D点と2D矩形の衝突判定
@author 葉迩倭
@param Pt		[in] 2D平面上の点
@param Rect		[in] 2D平面上の矩形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の点と矩形の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector2D Pt;
  Math::Rect2D<Float> Rect;
  // 衝突判定
  if ( Collision::Point_Rect( Pt, Rect ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Rect( const Math::Vector2D &Pt, const Math::Rect2D<Float> &Rect );
/**
@brief 2D点と2D矩形の衝突判定
@author 葉迩倭
@param Pt		[in] 2D平面上の点
@param Cir		[in] 2D平面上の円
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の点と円の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector2D Pt;
  Collision::CCircle Cir;
  // 衝突判定
  if ( Collision::Point_Circle( Pt, Cir ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Circle( const Math::Vector2D &Pt, const CCircle &Cir );
/**
@brief 2D点と2D多角形の衝突判定
@author 葉迩倭
@param Pt		[in] 2D平面上の点
@param Poly		[in] 2D平面上の多角形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の点と多角形の衝突判定を行います。<BR>
CPolygon2Dに関しては、時計回り、あるいは反時計回りの<BR>
凸形状をしている必要があります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector2D Pt;
  Collision::CPolygon2D Poly;
  // 衝突判定
  if ( Collision::Point_Polygon( Pt, Poly ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Polygon( const Math::Vector2D &Pt, const CPolygon2D &Poly );
/**
@brief 2D矩形と2D矩形の衝突判定
@author 葉迩倭
@param Rect1		[in] 2D平面上の矩形
@param Rect2		[in] 2D平面上の矩形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の矩形同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Rect2D<Float> Rect1, Rect2;
  // 衝突判定
  if ( Collision::Rect_Rect( Rect1, Rect2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Rect_Rect( const Math::Rect2D<Float> &Rect1, const Math::Rect2D<Float> &Rect2 );
/**
@brief 2D矩形と2D円の衝突判定
@author 葉迩倭
@param Rect		[in] 2D平面上の矩形
@param Cir		[in] 2D平面上の円
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の矩形と円の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Rect2D<Float> Rect;
  Collision::CCircle Cir;
  // 衝突判定
  if ( Collision::Rect_Circle( Rect, Cir ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Rect_Circle( const Math::Rect2D<Float> &Rect, const CCircle &Cir );
/**
@brief 2D矩形と2D線分の衝突判定
@author 葉迩倭
@param Rect		[in] 2D平面上の矩形
@param Line		[in] 2D平面上の線分
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の矩形と線分の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Rect2D<Float> Rect;
  Collision::CLine2D Line;
  // 衝突判定
  if ( Collision::Rect_Line( Rect, Line ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Rect_Line( const Math::Rect2D<Float> &Rect, const CLine2D &Line );
/**
@brief 2D矩形と2D多角形の衝突判定
@author 葉迩倭
@param Rect		[in] 2D平面上の矩形
@param Poly		[in] 2D平面上の多角形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の矩形と多角形の衝突判定を行います。<BR>
CPolygon2Dに関しては、時計回り、あるいは反時計回りの<BR>
凸形状をしている必要があります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Rect2D<Float> Rect;
  Collision::CPolygon2D Poly;
  // 衝突判定
  if ( Collision::Rect_Polygon( Rect, Poly ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Rect_Polygon( const Math::Rect2D<Float> &Rect, const CPolygon2D &Poly );
/**
@brief 2D円と2D円の衝突判定
@author 葉迩倭
@param Cir1		[in] 2D平面上の円
@param Cir2		[in] 2D平面上の円
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の円同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CCircle Cir1, Cir2;
  // 衝突判定
  if ( Collision::Circle_Circle( Cir1, Cir2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Circle_Circle( const CCircle &Cir1, const CCircle &Cir2 );
/**
@brief 2D円と2D線分の衝突判定
@author 葉迩倭
@param Cir		[in] 2D平面上の円
@param Line		[in] 2D平面上の線分
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の円同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CCircle Cir;
  Collision::CLine2D Line;
  // 衝突判定
  if ( Collision::Circle_Line( Cir, Line ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Circle_Line( const CCircle &Cir, const CLine2D &Line );
/**
@brief 2D円と2D多角形の衝突判定
@author 葉迩倭
@param Cir		[in] 2D平面上の円
@param Poly		[in] 2D平面上の多角形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の円と多角形の衝突判定を行います。<BR>
CPolygon2Dに関しては、時計回り、あるいは反時計回りの<BR>
凸形状をしている必要があります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CCircle Cir;
  Collision::CPolygon2D Poly;
  // 衝突判定
  if ( Collision::Circle_Polygon( Cir, Poly ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Circle_Polygon( const CCircle &Cir, const CPolygon2D &Poly );
/**
@brief 2D線分と2D線分の衝突判定
@author 葉迩倭
@param Line1	[in] 2D平面上の線分
@param Line2	[in] 2D平面上の線分
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の線分同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CLine2D Line1, Line2;
  // 衝突判定
  if ( Collision::Line_Line( Line1, Line2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Line_Line( const CLine2D &Line1, const CLine2D &Line2 );
/**
@brief 2D線分と2D多角形の衝突判定
@author 葉迩倭
@param Line		[in] 2D平面上の線分
@param Poly		[in] 2D平面上の多角形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の線分と多角形の衝突判定を行います。<BR>
CPolygon2Dに関しては、時計回り、あるいは反時計回りの<BR>
凸形状をしている必要があります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CLine2D Line1;
  Collision::CPolygon2D Poly;
  // 衝突判定
  if ( Collision::Line_Polygon( Line, Poly ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Line_Polygon( const CLine2D &Line, const CPolygon2D &Poly );
/**
@brief 2D多角形と2D多角形の衝突判定
@author 葉迩倭
@param Poly1	[in] 2D平面上の多角形
@param Poly2	[in] 2D平面上の多角形
@retval false	衝突していない
@retval true	衝突している

@note
2D平面上の多角形同士のの衝突判定を行います。<BR>
CPolygon2Dに関しては、時計回り、あるいは反時計回りの<BR>
凸形状をしている必要があります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CPolygon2D Poly1, Poly2;
  // 衝突判定
  if ( Collision::Polygon_Polygon( Poly1, Poly2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Polygon_Polygon( const CPolygon2D &Poly1, const CPolygon2D &Poly2 );
/**
@brief 3D点と3D点の衝突判定
@author 葉迩倭
@param Pt1		[in] 3D空間の点
@param Pt2		[in] 3D空間の点
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の点同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector3D Pt1, Pt2;
  // 衝突判定
  if ( Collision::Point_Point_3D( Pt1, Pt2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Point_3D( const Math::Vector3D &Pt1, const Math::Vector3D &Pt2 );
/**
@brief 3D点と3D球の衝突判定
@author 葉迩倭
@param Pt		[in] 3D空間の点
@param Sph		[in] 3D空間の球
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の点と球の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector3D Pt;
  Collision::CSphere Sph;
  // 衝突判定
  if ( Collision::Point_Sphere_3D( Pt, Sph ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Sphere_3D( const Math::Vector3D &Pt, const CSphere &Sph );
/**
@brief 3D点と3D線分の衝突判定
@author 葉迩倭
@param Pt		[in] 3D空間の点
@param Line		[in] 3D空間の線分
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の点と線分の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector3D Pt;
  Collision::CLine3D Line;
  // 衝突判定
  if ( Collision::Point_Line_3D( Pt, Line ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Line_3D( const Math::Vector3D &Pt, const CLine3D &Line );
/**
@brief 3D点と3D平面の衝突判定
@author 葉迩倭
@param Pt		[in] 3D空間の点
@param Plane	[in] 3D空間の平面
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の点と平面の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector3D Pt;
  Collision::CPlane Plane;
  // 衝突判定
  if ( Collision::Point_Plane_3D( Pt, Plane ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Plane_3D( const Math::Vector3D &Pt, const CPlane &Plane );
/**
@brief 3D点と3D箱の衝突判定
@author 葉迩倭
@param Pt		[in] 3D空間の点
@param Box		[in] 3D空間の箱
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の点と平面の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Math::Vector3D Pt;
  Collision::CBox Box;
  // 衝突判定
  if ( Collision::Point_Box_3D( Pt, Box ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Point_Box_3D( const Math::Vector3D &Pt, const CBox &Box );
/**
@brief 3D球と3D球の衝突判定
@author 葉迩倭
@param Sph1		[in] 3D空間の球
@param Sph2		[in] 3D空間の球
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の球同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CSphere Sph1, Sph2;
  // 衝突判定
  if ( Collision::Sphere_Sphere_3D( Sph1, Sph2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Sphere_Sphere_3D( const CSphere &Sph1, const CSphere &Sph2 );
/**
@brief 3D球と3D線分の衝突判定
@author 葉迩倭
@param Sph		[in] 3D空間の球
@param Line		[in] 3D空間の線分
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の球と線分の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CSphere Sph;
  Collision::CLine3D Line;
  // 衝突判定
  if ( Collision::Sphere_Line_3D( Sph, Line ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Sphere_Line_3D( const CSphere &Sph, const CLine3D &Line );
/**
@brief 3D球と3D平面の衝突判定
@author 葉迩倭
@param Sph		[in] 3D空間の球
@param Plane	[in] 3D空間の平面
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の球と平面の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CSphere Sph;
  Collision::CPlane Plane;
  // 衝突判定
  if ( Collision::Sphere_Plane( Sph, Plane ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Sphere_Plane( const CSphere &Sph, const CPlane &Plane );
/**
@brief 3D線分と3D平面の衝突判定
@author 葉迩倭
@param Line			[in] 3D空間の線分
@param Plane		[in] 3D空間の平面
@param vIntersect	[out] 衝突位置
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の線分と平面の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CLine3D Line;
  Collision::CPlane Plane;
  Math::Vector3D vIntersect;
  // 衝突判定
  if ( Collision::Line_Plane_3D( Line, Plane, vIntersect ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Line_Plane_3D( const CLine3D &Line, const CPlane &Plane, Math::Vector3D &vIntersect );
/**
@brief 3D線分と3D箱の衝突判定
@author 葉迩倭
@param Line			[in] 3D空間の線分
@param Box			[in] 3D空間の箱
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の線分と箱の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CLine3D Line;
  Collision::CBox Box;
  // 衝突判定
  if ( Collision::Line_Box_3D( Line, Box ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Line_Box_3D( const CLine3D &Line, const CBox &Box );
/**
@brief 3D箱と3D箱の衝突判定
@author 葉迩倭
@param Box1		[in] 3D空間の箱
@param Box2		[in] 3D空間の箱
@retval false	衝突していない
@retval true	衝突している

@note
3D空間内の箱同士の衝突判定を行います。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Collision::CBox Box1, Box2;
  // 衝突判定
  if ( Collision::Box_Box_3D( Box1, Box2 ) )
  {
  }

  return 0;
}
@endcode
*/
	SELENE_DLL_API Bool Box_Box_3D( const CBox &Box1, const CBox &Box2 );
}
}

#pragma once


/**
	@file
	@brief インターフェイス基底クラス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	/**
		@brief インターフェース基底クラス
		@author 葉迩倭
	*/
	class IInterface
	{
	public:
		virtual ~IInterface() {}

/**
@brief 有効性チェック
@author 葉迩倭
@retval true 無効
@retval false 有効

@note
インターフェイスが有効か無効かを調べます。

@code
// 有効かチェック
if ( this->IsInvalid() )
{
	// 無効なインターフェイス
	reutrn false;
}
*/
		virtual Bool IsInvalid( void ) = 0;

/**
@brief 参照カウンタデクリメント
@author 葉迩倭
@return デクリメント後の参照カウント

@note
参照カウンタをデクリメントし、<BR>
参照カウントが0になった時点でメモリから解放します。

@code
// 解放（参照カウンタを-1するだけ。実際には参照カウントが0になるとメモリから消される）
this->Release();
@endcode
*/
		virtual Sint32 Release( void ) = 0;

/**
@brief 参照カウンタインクリメント
@author 葉迩倭
@return インクリメント後の参照カウント

@note
参照カウンタをインクリメントします。<BR>
インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
AddRef()をしたインターフェイスは必ずRelease()で解放してください。

@code
IInterface *pCopy = this;
// 他でも参照するので参照数+1
this->AddRef();
@endcode
*/
		virtual Sint32 AddRef( void ) = 0;
	};
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Fiber
{
	/**
		@brief ファイバーコントローラーインターフェイス
		@author 葉迩倭

		@note
		ファイバーの制御を行うためのインターフェイスです。
	*/
	class IFiberController : public IInterface
	{
	public:
		virtual ~IFiberController() {}

/**
@brief ファイバーシステム開始
@author 葉迩倭
@note
ファイバーの処理を開始します。
*/
		virtual void Start( void ) = 0;

/**
@brief ファイバーシステム終了
@author 葉迩倭
@note
ファイバーの処理を終了します。
*/
		virtual void Exit( void ) = 0;

/**
@brief ファイバー切り替え
@author 葉迩倭
@retval false	アプリケーションは終了している
@retval true	アプリケーションは継続中である
@note
ファイバーの切り替えを行います。
*/
		virtual Bool Switch( void ) = 0;

/**
@brief ファイバー生成
@author 葉迩倭
@param FiberId	[in] ファイバーID
@param pFiber	[in] ファイバーインターフェイス
@retval false	指定のFiberIdにすでにファイバーがある
@retval true	成功
@note
ファイバーの生成を行います。
*/
		virtual Bool Create( Sint32 FiberId, IFiber *pFiber ) = 0;
	};
}
}


#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Fiber
{
	/**
		@brief ファイバーインターフェイス
		@author 葉迩倭

		@note
		ファイバーを実装するための基底クラスです。
	*/
	class IFiber
	{
	private:
		Sint32 m_RefCount;
		IFiberController *m_pFiberCtrl;

	protected:
/**
@brief ファイバー切り替え
@author 葉迩倭
@retval true	アプリケーションは継続中
@retval false	アプリケーションが終了している

@note
登録されている次のIDのファイバーに処理を切り替えます。<BR>
返り値がfalseの場合アプリケーションが終了処理を行っているので<BR>
Controller()メソッドから直ちに抜けるように組んで下さい。
*/
		Bool Switch( void )
		{
			return m_pFiberCtrl->Switch();
		}

	public:
/**
@brief コンストラクタ
@author 葉迩倭
*/
		IFiber( void )
			: m_pFiberCtrl	( NULL )
			, m_RefCount	( 1 )
		{
		}

/**
@brief デストラクタ
@author 葉迩倭
*/
		virtual ~IFiber()
		{
			if ( m_pFiberCtrl != NULL )
			{
				m_pFiberCtrl->Release();
			}
		}

/**
@brief 参照カウンタデクリメント
@author 葉迩倭
@return デクリメント後の参照カウント

@note
参照カウンタをデクリメントし、<BR>
参照カウントが0になった時点でメモリから解放します。
*/
		Sint32 Release( void )
		{
			Sint32 Cnt = --m_RefCount;
			if ( Cnt == 0 )
			{
				delete this;
			}
			return Cnt;
		}

/**
@brief 参照カウンタインクリメント
@author 葉迩倭
@return インクリメント後の参照カウント

@note
参照カウンタをインクリメントします。<BR>
インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
AddRef()をしたインターフェイスは必ずRelease()で解放してください。
*/
		Sint32 AddRef( void )
		{
			return ++m_RefCount;
		}

/**
@brief コントローラー設定
@author 葉迩倭
@param pCtrl	[in] コントローラー
*/
		void SetController( IFiberController *pCtrl )
		{
			m_pFiberCtrl = pCtrl;
			m_pFiberCtrl->AddRef();
		}

	public:
/**
@brief ファイバー用制御関数
@author 葉迩倭

@note
ファイバーを実際に操作するための純粋仮想関数です。<BR>
この関数から抜けることでそのファイバーの処理が停止します。<BR>
Switch()メソッドでの切り替え時にfalseが帰ってきたらただちに<BR>
関数から抜けるようにして下さい。
*/
		virtual void Control( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	/**
		@brief ビデオカード管理インターフェイス
		@author 葉迩倭
	*/
	class IGraphicCard
	{
	public:
		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 16Bitカラーモードを使用
			@author 葉迩倭

			フルスクリーン時の画面カラーを32Bitではなく16Bitにします。<BR>
			フィルレートの厳しい環境で速度の向上が望めますが、<BR>
			マッハバンドなどの画質的な問題も出ます。
		*/
		virtual void EnableHighlColorScreen( void ) = 0;

		/**
			@brief ピクセルシェーダーがサポートされているか取得
			@author 葉迩倭
			@param Major	[in] メジャーバージョン
			@param Minor	[in] マイナーバージョン

			現在のデバイスで指定したピクセルシェーダーをサポート<BR>
			しているかどうかを取得します。
		*/
		virtual Bool GetPixelShaderSupport( Uint16 Major, Uint16 Minor ) = 0;

		/**
			@brief 画面解像度数取得
			@author 葉迩倭
			@param IsFullColor	[in] フルカラー(32Bit)の画面解像度か否か
			@return 画面解像度数

			使用可能な画面解像度の数を取得できます。<BR>
			IsFullColorフラグに応じて、16Bit/32Bitカラーの画面解像度数が取得出来ます。
		*/
		virtual Sint32 GetScreenModeCount( Bool IsFullColor ) = 0;

		/**
			@brief 画面解像度
			@author 葉迩倭
			@param IsFullColor	[in] フルカラー(32Bit)の画面解像度か否か
			@param No			[in] 画面解像度のインデックス（最大数はGetScreenModeCount()で取得）
			@param Width		[out] 画面横幅格納先
			@param Height		[out] 画面縦幅格納先

			使用可能な画面解像度を取得します。
		*/
		virtual void GetScreenMode( Bool IsFullColor, Sint32 No, Sint32 &Width, Sint32 &Height ) = 0;

		/**
			@brief レンダリングターゲットテクスチャサイズ数取得
			@author 葉迩倭
			@return レンダリングターゲットテクスチャサイズ数

			使用可能なレンダリングターゲットテクスチャサイズの数を取得できます。
		*/
		virtual Sint32 CreateRenderTargetTextureSizeCount( void ) = 0;

		/**
			@brief レンダリングターゲットテクスチャサイズ
			@author 葉迩倭
			@param No			[in] レンダリングターゲットテクスチャサイズのインデックス（最大数はCreateRenderTargetTextureSizeCount()で取得）
			@param pWidth		[out] 画面横幅格納先
			@param pHeight		[out] 画面縦幅格納先

			使用可能なレンダリングターゲットテクスチャサイズを取得します。
		*/
		virtual void CreateRenderTargetTextureSize( Sint32 No, Sint32 *pWidth, Sint32 *pHeight ) = 0;

		/**
			@brief グラフィックカード名称取得
			@author 葉迩倭
			@param pName		[out] 名称格納先
			@param NameSize		[in] pNameのバッファサイズ

			接続されているグラフィックカードの名称を取得します。
		*/
		virtual void GetGraphicCardName( char *pName, Sint32 NameSize ) = 0;

		/**
			@brief 頂点シェーダーのバージョンを取得
			@author 葉迩倭
			@param pName		[out] バージョン格納先
			@param NameSize		[in] pNameのバッファサイズ

			接続されているグラフィックカードの頂点シェーダーのバージョンを取得します。
		*/
		virtual void GetVertexShaderVersion( char *pName, Sint32 NameSize ) = 0;

		/**
			@brief ピクセルシェーダーのバージョンを取得
			@author 葉迩倭
			@param pName		[out] バージョン格納先
			@param NameSize		[in] pNameのバッファサイズ

			接続されているグラフィックカードのピクセルシェーダーのバージョンを取得します。
		*/
		virtual void GetPixelShaderVersion( char *pName, Sint32 NameSize ) = 0;

		/**
			@brief IRenderインターフェイス生成
			@author 葉迩倭
			@param IsLockEnableBackBuffer	[in] バックバッファのロック有無
			@param IsWaitVSync				[in] 画面更新時にVSYNCを待つ
			@return IRenderインターフェイス

			グラフィックカード上のレンダラー制御用のIRenderインターフェイスを取得します。<BR>
			IRenderインターフェイスは１つのIGraphicCardに対して１つしか存在しません。
		*/
		virtual Renderer::IRender *CreateRender( Bool IsLockEnableBackBuffer = false, Bool IsWaitVSync = true ) = 0;
	};
}

#pragma once


/**
	@file
	@brief アプリケーション管理インターフェイス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	/**
		@brief アプリケーション管理インターフェイス
		@author 葉迩倭

		@note
		ウィンドウを管理するアプリケーションのコアのインターフェイスです。
	*/
	class ICore : public IInterface
	{
	public:
		virtual ~ICore() {}

/**
@brief コアの初期化
@author 葉迩倭
@param pAppName		[in] アプリケーション名
@param FrameRate	[in] フレームレート
@retval false	失敗
@retval true	成功

@note
アプリケーションに関する初期化を行います。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Bool Initialize( const char *pAppName, eFrameRate FrameRate ) = 0;

/**
@brief ウィンドウ生成
@author 葉迩倭
@param ScreenWidth	[in] 画面横幅
@param ScreenHeight	[in] 画面縦幅
@param IsWindow		[in] ウィンドウモードで起動する場合true

@note
ウィンドウを作成し、アプリケーションを起動します。<BR>
ここでの設定で起動後変更可能なものは画面モードだけです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void Start( Sint32 ScreenWidth, Sint32 ScreenHeight, Bool IsWindow ) = 0;

/**
@brief ウィンドウの終了
@author 葉迩倭

@note
ウィンドウを解体し、アプリケーションを終了します。<BR>
この関数は終了を通知するだけで、実際にはメインループ後に<BR>
終了処理が行われます。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // 終了
      pCore->Exit();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void Exit( void ) = 0;

/**
@brief アプリケーションメイン処理
@author 葉迩倭
@param IsFullActive	[in] 常時動作フラグ
@retval false	アプリケーションは終了した
@retval true	アプリケーションは稼働中

@note
IsFullActiveにtrueを指定すると、<BR>
ウィンドウがフォーカスを失っている状態でも動作し続けます。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Bool Run( Bool IsFullActive = false ) = 0;

/**
@brief ウィンドウハンドル取得
@author 葉迩倭
@return ウィンドウのウィンドウハンドル

@note
ICoreの所有するウィンドウのウィンドウハンドルを取得します。


@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ウィンドウハンドルを取得
    HWND hWnd = pCore->GetWindowHandle();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual HWND GetWindowHandle( void ) = 0;

/**
@brief ベンチマーク用モード設定
@author 葉迩倭
@param IsEnable	[in] trueにするとすべてのウェイト処理を省きます

@note
ベンチマークのような速度チェックをする場合に使います。<BR>
このフラグをtrueにするとVSYNCやタイマーなどの処理がすべて省かれ、<BR>
出せる最高の速度で処理が回ります。<BR>
<BR>
デフォルトはfalseです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // ベンチマーク用に各種ウェイトをOFF
    pCore->SetBenchMode( true );

    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void SetBenchMode( Bool IsEnable ) = 0;

/**
@brief Sleep使用有無設定
@author 葉迩倭
@param IsEnable	[in] trueにするとSleepを使用します

@note
フレームレートの調整にSleepを使うかどうかを設定します。<BR>
環境によっては誤差が大きくフレームレートが一定にならないことがあるので、<BR>
そのような条件ではfalseを設定して下さい。<BR>
Sleepを使わない場合は当然CPUを常に占有するようになります。<BR>
<BR>
デフォルトはtrueです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // より厳密に時間計測をする（CPU使用率が跳ね上がるので注意）
    pCore->SetSleepUse( false );

    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void SetSleepUse( Bool IsEnable ) = 0;

/**
@brief 仮想画面処理のON/OFF
@author 葉迩倭
@param IsEnable		[in] true 有効 / false 無効

@note
2D描画時の仮想画面処理の有無のON/OFFを設定します。<BR>
trueにする事で2Dの描画が仮想画面に行われるようになり、<BR>
画面解像度に関係なく画面に対する同じ割合で描画されるようになります。<BR>
<BR>
デフォルトはfalseです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // 画面サイズが返られてもいいように2Dがを仮想画面に描画
    pCore->SetVirtualScreenEnable( true );

    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void SetVirtualScreenEnable( Bool IsEnable ) = 0;

/**
@brief 仮想画面設定
@author 葉迩倭
@param RealWidth		[in] 実画面の横幅
@param RealHeight		[in] 実画面の縦幅
@param VirtualWidth		[in] 仮想画面の横幅
@param VirtualHeight	[in] 仮想画面の縦幅

@note
可変画面サイズ用の設定を行います。<BR>
3D用に画面のサイズを自由に変えられるように作られている場合でも<BR>
2Dの描画は仮想画面に設定された画面サイズと見立てて描画を行います。<BR>
つまり仮想画面が(640x480)の時に(0,0)-(640,480)への全画面の2D描画を行った場合、<BR>
実際の画面のサイズが(320x240)や(1280x960)等のサイズであっても<BR>
全画面に自動的に拡大縮小して描画されるということです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // 画面サイズが返られてもいいように2Dがを仮想画面に描画
    pCore->SetVirtualScreenEnable( true );

    // 仮想画面サイズ
    pCore->SetVirtualScreenSize( 640, 480, 800, 600 );

    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void SetVirtualScreenSize( Sint32 RealWidth, Sint32 RealHeight, Sint32 VirtualWidth, Sint32 VirtualHeight ) = 0;

/**
@brief GPU負荷を取得します。
@author 葉迩倭
@return GPU負荷（％）

@note
おおまかなGPUの負荷を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // CPU使用率
      Float fCPU = pCore->GetCPU();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Float GetCPU( void ) = 0;

/**
@brief FPSを取得します。
@author 葉迩倭
@return FPS

@note
秒間のフレーム数を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // FPS
      Float fFPS = pCore->GetFPS();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Float GetFPS( void ) = 0;

/**
@brief 1フレームのポリゴン数を取得します。
@author 葉迩倭
@return PPS

@note
秒間のポリゴン数を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // ポリゴン数取得
	  Float fPPF = pCore->GetPPF();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Float GetPPF( void ) = 0;

/**
@brief OS起動からの時間取得
@author 葉迩倭
@return 1/1000秒単位の時間

@note
OSが起動してからの時間を取得します。<BR>
32Bit変数なので約48日で1周して0に戻ります。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // 時間取得
      Uint32 Time = pCore->GetMultiMediaTime();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Uint32 GetMultiMediaTime( void ) = 0;

/**
@brief CPUタイマーのカウント値取得
@author 葉迩倭
@return 1/GetSystemTimerBySec秒単位の時間

@note
CPUタイマーの現在の時間を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // 時間取得
      Uint64 Time = pCore->GetSystemTimer();

      // なんか処理する
 
      // 経過時間取得
      Time = pCore->GetSystemTimer() - Time;

      // 経過時間を1秒あたりの％に
      Float fTime = 100.0 * (double)Time / (double)pCore->GetSystemTimerBySec();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Uint64 GetSystemTimer( void ) = 0;

/**
@brief CPUタイマーの1秒のカウント値取得
@author 葉迩倭
@return GetSystemTimerの1秒のカウント値

@note
CPUタイマーの1秒の時間を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // 時間取得
      Uint64 Time = pCore->GetSystemTimer();

      // なんか処理する
 
      // 経過時間取得
      Time = pCore->GetSystemTimer() - Time;

      // 経過時間を1秒あたりの％に
      Float fTime = 100.0 * (double)Time / (double)pCore->GetSystemTimerBySec();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Uint64 GetSystemTimerBySec( void ) = 0;

/**
@brief 画面サイズ変更
@author 葉迩倭
@param Size		[in] 画面サイズ

@note
画面サイズを変更します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // 画面サイズ変更
    pCore->ResizeWindowSize( Math::Point2DI(320,240) );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void ResizeWindowSize( Math::Point2DI Size ) = 0;

/**
@brief 画面モードを変更します
@author 葉迩倭

@note
画面モードを変更します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // 画面モード変更
    pCore->ChangeScreenMode();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void ChangeScreenMode( void ) = 0;

/**
@brief ウィンドウモードチェック
@author 葉迩倭

@note
現在の画面モードがウィンドウモードかどうかを調べます

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // 画面モード取得
    if ( pCore->IsWindowMode() )
    {
    }

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Bool IsWindowMode( void ) = 0;

/**
@brief ムービー再生
@author 葉迩倭
@param pFileName	[in] ファイル名
@param IsLoop		[in] ループON/OFF
@param pCallback	[in] 強制終了用コールバック関数

@note
フル画面でムービーの再生を行います。<BR>
ムービーが終了するか、pCallback関数内でfalseを返すまで処理は戻ってきません。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ムービー再生
    if ( pCore->PlayMovie( "Sample.mpg", false, NULL ) )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Bool PlayMovie( const char *pFileName, Bool IsLoop, Bool (__stdcall *pCallback)( void ) ) = 0;

/**
@brief フレームカウントを取得します。
@author 葉迩倭
@return フレーム数

@note
起動時からの総フレーム数を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
      // 起動からのフレーム数
      Sint32 Cnt = pCore->GetSyncCount();
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Sint32 GetSyncCount( void ) = 0;

/**
@brief タイトルバーの情報表示ON/OFF
@author 葉迩倭
@param IsEnable		[in] 表示のON/OFF

@note
タイトルバーに現在の情報の表示を行うか設定します。<BR>
<BR>
デフォルトはfalseです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // タイトルバーに情報表示
    pCore->EnableDrawTitleInformation( true );

    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void EnableDrawTitleInformation( Bool IsEnable ) = 0;

/**
@brief マウスカーソルの表示ON/OFF
@author 葉迩倭
@param IsEnable		[in] 表示のON/OFF

@note
マウスカーソルの表示を行うか設定します。<BR>
<BR>
デフォルトはfalseです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // マウスカーソル消去
    pCore->EnableDrawMouseCursor( false );

    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void EnableDrawMouseCursor( Bool IsEnable ) = 0;

/**
@brief グラフィックカードインターフェイスを取得
@author 葉迩倭
@param GraphicCardNo	[in] グラフィックカード番号
@return グラフィックカードインターフェイス

@note
グラフィックカードに関しての初期化を行い、<BR>
そのインターフェイスを取得します。<BR>
通常GraphicCardNoにはGRAPHIC_CARD_DEFAULT_NOを指定します。<BR>
「NV PerfHUD」のインストールされた環境では、<BR>
GRAPHIC_CARD_NV_PERF_HUDを指定することでそれを有効に出来ます。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  IGraphicCard *pGraphicCard = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // グラフィックカードインターフェイスを生成
    pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // グラフィックカードの解放
  SAFE_RELEASE( pGraphicCard );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual IGraphicCard *CreateGraphicCard( eGraphicCardNo GraphicCardNo ) = 0;

/**
@brief ジョイスティックの数を取得
@author 葉迩倭
@return 有効なジョイスティックの数

@note
初期化に成功した有効なジョイスティックなの数を取得します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ジョイスティック数を取得
    Sint32 Count = pCore->GetJoystickCount();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Sint32 GetJoystickCount( void ) = 0;

/**
@brief マウスインターフェイスを取得
@author 葉迩倭
@return マウスインターフェイス

@note
ICoreの所有するウィンドウに関連付けられたマウスの初期化を行い、<BR>
そのインターフェイスを取得します。<BR>
マウスに関しての情報はこのインターフェイス経由で取得して下さい。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Peripheral::IMouse *pMouse = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // マウスの取得
    pMouse = pCore->GetMouse();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // マウスの解放
  SAFE_RELEASE( pMouse );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Peripheral::IMouse *GetMouse( void ) = 0;

/**
@brief キーボードインターフェイスを取得
@author 葉迩倭
@return キーボードインターフェイス

@note
ICoreの所有するウィンドウに関連付けられたキーボードの初期化を行い、<BR>
そのインターフェイスを取得します。<BR>
キーボードに関しての情報はこのインターフェイス経由で取得して下さい。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Peripheral::IKeyboard *pKeyboard = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // キーボードの取得
    pKeyboard = pCore->GetKeyboard();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // キーボードの解放
  SAFE_RELEASE( pKeyboard );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Peripheral::IKeyboard *GetKeyboard( void ) = 0;

/**
@brief ジョイスティックインターフェイスを取得
@author 葉迩倭
@param No	[in] ジョイスティック番号(0～15)
@return ジョイスティックインターフェイス

@note
ICoreの所有するウィンドウに関連付けられたジョイスティックの初期化を行い、<BR>
そのインターフェイスを取得します。<BR>
ジョイスティックに関しての情報はこのインターフェイス経由で取得して下さい。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Peripheral::IJoystick *pJoystick = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ジョイスティックの取得
    pJoystick = pCore->GetJoystick( 0 );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // ジョイスティックの解放
  SAFE_RELEASE( pJoystick );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Peripheral::IJoystick *GetJoystick( Sint32 No ) = 0;

/**
@brief 入力統合インターフェイスを取得
@author 葉迩倭
@param No					[in] 適用するジョイスティック番号
@param KeyRepeatStartTime	[in] キーリピート開始フレーム
@param KeyRepeatInterval	[in] キーリピート間隔フレーム
@return 入力統合インターフェイス

@note
ICoreの所有するウィンドウに関連付けられたジョイスティック＆キーボードの初期化を行い、<BR>
そのインターフェイスを取得します。<BR>
ジョイスティックとキーボードを統合して扱うことのできるインターフェイスです。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Peripheral::IInputController *pInputController = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // 入力コントローラーの取得
    pInputController = pCore->GetInputController( 0, 30, 5 );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // 入力コントローラーの解放
  SAFE_RELEASE( pInputController );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Peripheral::IInputController *GetInputController( Sint32 No, Sint32 KeyRepeatStartTime, Sint32 KeyRepeatInterval ) = 0;

/**
@brief ファイルマネージャーインターフェイス生成
@author 葉迩倭
@return ファイルマネージャーインターフェイス

@note
新規のファイルマネージャーインターフェイスを生成します。<BR>
ファイルのパスなどの設定は全てIFileManagerを経由して行います。
取得したファイルマネージャーインターフェイスは使用終了後には必ずRelease()して下さい。。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  File::IFileManager *pFileMgr = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ファイルマネージャーの生成
    pFileMgr = pCore->CreateFileManager();

    // ファイルマネージャーを設定
    pCore->SetFileManager( pFileMgr );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // ファイルマネージャーの解放
  SAFE_RELEASE( pFileMgr );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual File::IFileManager *CreateFileManager( void ) = 0;

/**
@brief ファイルマネージャーインターフェイス取得
@author 葉迩倭
@return ファイルマネージャーインターフェイス

@note
現在設定されているファイルマネージャーインターフェイスを取得します。<BR>
設定済みのファイルマネージャーインターフェイスへのアクセスを行う場合に使用してください。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  File::IFileManager *pFileMgr = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ファイルマネージャーの生成
    pFileMgr = pCore->CreateFileManager();

    // ファイルマネージャーを設定
    pCore->SetFileManager( pFileMgr );

    // ファイルパス設定
    pCore->GetFileMgrPointer()->SetCurrentPath( "Data\\Texture" );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // ファイルマネージャーの解放
  SAFE_RELEASE( pFileMgr );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual File::IFileManager *GetFileMgrPointer( void ) = 0;

/**
@brief ファイルマネージャーインターフェイス設定
@author 葉迩倭
@param pMgr	[in] ファイルマネージャーインターフェイス

@note
ファイルの読み込みに使われるファイルマネージャーインターフェイスを設定します。<BR>
ファイルの読み込みはこのマネージャーを経由して行うので、<BR>
ファイルの読み込みを行う場合は必ず設定してください。<BR>
既に設定されているマネージャーは内部で解放されます。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  File::IFileManager *pFileMgr = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ファイルマネージャーの生成
    pFileMgr = pCore->CreateFileManager();

    // ファイルマネージャーを設定
    pCore->SetFileManager( pFileMgr );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // ファイルマネージャーの解放
  SAFE_RELEASE( pFileMgr );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void SetFileManager( File::IFileManager *pMgr ) = 0;

/**
@brief サウンドファイル読み込み
@author 葉迩倭
@param pFileName		[in] サウンドファイル名(wav)
@param LayerCount		[in] レイヤー数（同時発音可能数）
@param IsGlobalScope	[in] サウンドのスコープ
@retval	サウンドインターフェイス

@note
ICoreの所有するウィンドウに関連付けられたサウンドを取得します。<BR>
IsGlobalScopeにtrueを渡すと、ウィンドウのフォーカスに関係なく再生されます。<BR>
LayerCountで指定した数分のレイヤーを内部で持ちます。<BR>
これは同一のバッファを別々のタイミングで再生・停止等の<BR>
操作を行うための機能ですが、１つのバッファを使いまわすので<BR>
メモリ使用量は１つの場合と同じです。<BR>
使用の終了したサウンドは必ずRelease()して下さい。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Sound::ISound *pSound = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // サウンドの生成
    pSound = CreateSoundFromFile( "Sample.wav", 4, false );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // サウンドの解放
  SAFE_RELEASE( pSound );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Sound::ISound *CreateSoundFromFile( const char *pFileName, Sint32 LayerCount, Bool IsGlobalScope ) = 0;

/**
@brief サウンドファイル読み込み
@author 葉迩倭
@param pFileName		[in] サウンドファイル名
@param IsGlobalScope	[in] サウンドのスコープ
@param pPluginName		[in] プラグイン名
@retval		サウンドインターフェイス

@note
ICoreの所有するウィンドウに関連付けられたサウンドを取得します。<BR>
IsGlobalScopeにtrueを渡すと、ウィンドウのフォーカスに関係なく再生されます。<BR>
使用の終了したサウンドは必ずRelease()して下さい。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Sound::IStreamSound *pSound = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // サウンドの生成
    pSound = CreateStreamSoundFromFile( "Sample.ogg", false, "OggVorbis" );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // サウンドの解放
  SAFE_RELEASE( pSound );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Sound::IStreamSound *CreateStreamSoundFromFile( const char *pFileName, Bool IsGlobalScope, const char *pPluginName ) = 0;

/**
@brief ファイバーコントローラー生成
@author 葉迩倭
@param Max	[in] ファイバー最大数
@return ファイバーコントローラーインターフェイス

@note
ファイバーコントローラーを取得します。<BR>
ICoreに１つしか存在せず2回目以降は既存のインターフェイスを返します。

@code
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  ICore *pCore = NULL;
  Fiber::IFiberController *pFiber = NULL;

  // システムの初期化
  System::Initialize();

  // コアの生成
  pCore = System::CreateCore();

  // 初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーション開始
    pCore->Start( 640, 480, true );

    // ファイバーコントローラーの生成
    pFiber = pCore->CreateFiberController( 8 );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

  // ファイバーの解放
  SAFE_RELEASE( pFiber );

  // コアの解放
  SAFE_RELEASE( pCore );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Fiber::IFiberController *CreateFiberController( Sint32 Max ) = 0;
	};
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
	/**
		@brief コリジョン結果保持用
		@author 葉迩倭

		コリジョンの結果を保持するための構造体です。
	*/
	struct SCollisionResult
	{
		Uint32 Attribute;				///< 属性
		Math::Vector3D vHitPosition;	///< 衝突点
		Math::Vector3D vNormal;			///< 衝突面の法線
		Math::Vector3D vTriangle[3];	///< 衝突三角形をなす頂点
		CColor Color[3];				///< 衝突三角形をなす頂点の色
	};

	/**
		@brief コリジョン結果保持用
		@author 葉迩倭

		コリジョンの結果を保持するための構造体です。
	*/
	struct SCollisionResultExtend
	{
		Uint32 Attribute;					///< 属性（Almeria上でのマテリアルの番号）
		Math::Vector3D vHitPosition;		///< 衝突点
		Math::Vector3D vNormal;				///< 衝突面の法線
		Math::Vector3D vTriangle[3];		///< 衝突三角形をなす頂点
		Math::Vector3D vRefrectDirection;	///< 反射後の方向
		Math::Vector3D vRefrectPosition;	///< 反射後の位置
		Math::Vector3D vSlidePosition;		///< 面上をスライドした場合の位置
		CColor Color[3];					///< 衝突三角形をなす頂点の色
	};

	/**
		@brief コリジョン結果保持用
		@author 葉迩倭

		コリジョンの結果を保持するための構造体です。
	*/
	struct SCollisionResultSphere
	{
		Math::Vector3D vHitPosition;	///< 衝突点
		Math::Vector3D vNormal;			///< 衝突面の法線
	};

	/**
		@brief コリジョン結果保持用
		@author 葉迩倭

		コリジョンの結果を保持するための構造体です。
	*/
	struct SCollisionResultSphereExtend
	{
		Math::Vector3D vHitPosition;		///< 衝突点
		Math::Vector3D vNormal;				///< 衝突面の法線
		Math::Vector3D vRefrectDirection;	///< 反射後の方向
		Math::Vector3D vRefrectPosition;	///< 反射後の位置
		Math::Vector3D vSlidePosition;		///< 面上をスライドした場合の位置
	};

	/**
		@brief 2D描画用頂点
		@author 葉迩倭

		2D描画用の基本頂点データです。<BR>
		主にIPoint2D/ILine2Dクラスで使用します。<BR>
		VERTEX_ELEMENT_PRIMITIVEフラグを指定して作成した<BR>
		頂点定義ではこの構造体を使います。
	*/
	struct SVertex2D
	{
		Math::Vector4D Pos;		///< 位置
		CColor Col;				///< 頂点色
	};

	/**
		@brief 2D描画用頂点
		@author 葉迩倭

		2D描画用のテクスチャUV付き頂点データです。<BR>
		主にIPrimitive2D/ISprite2D/IFontSprite2Dクラスで使用します。<BR>
		VERTEX_ELEMENT_SPRITEフラグを指定して作成した<BR>
		頂点定義ではこの構造体を使います。
	*/
	struct SVertex2DTex
	{
		Math::Vector4D Pos;		///< 位置
		CColor Col;				///< 頂点色
		Math::Vector2D Tex[2];	///< テクスチャUV
	};

	/**
		@brief 3D描画用頂点
		@author 葉迩倭

		3D描画用の基本頂点データです。<BR>
		VERTEX_ELEMENT_BASEフラグを指定して作成した<BR>
		頂点定義に対して定義されます。
	*/
	struct SVertex3DBase
	{
		Math::Vector3D Pos;		///< 位置
		CColor Col;				///< 頂点色
	};

	/**
		@brief 3D描画用頂点
		@author 葉迩倭

		3D描画用のテクスチャUV用頂点データです。<BR>
		VERTEX_ELEMENT_3DTEXTUREフラグを指定して作成した<BR>
		頂点定義に対して定義されます。
	*/
	struct SVertex3DTexture
	{
		Math::Vector2D TexColor;		///< テクスチャUV
		Math::Vector2D TexLight;		///< テクスチャUV
	};

	/**
		@brief 3D描画用頂点
		@author 葉迩倭

		3D描画用のライティング用頂点データです。<BR>
		VERTEX_ELEMENT_3DLIGHTフラグを指定して作成した<BR>
		頂点定義に対して定義されます。
	*/
	struct SVertex3DLight
	{
		Math::Vector3D Norm;		///< 法線
	};

	/**
		@brief 3D描画用頂点
		@author 葉迩倭

		3D描画用のバンプマッピング用頂点データです。<BR>
		VERTEX_ELEMENT_3DBUMPフラグを指定して作成した<BR>
		頂点定義に対して定義されます。
	*/
	struct SVertex3DBump
	{
		Math::Vector3D Tangent;	///< 接線
	};

	/**
		@brief 3D描画用頂点
		@author 葉迩倭

		3D描画用のスキニング用頂点データです。<BR>
		VERTEX_ELEMENT_3DANIMATIONフラグを指定して作成した<BR>
		頂点定義に対して定義されます。
		１つの頂点につき２つのボーンの計算が行われます。
	*/
	struct SVertex3DAnimation
	{
		Float Weight;		///< ウェイト（１番目のウェイトだけを指定、２番目はシェーダー内で1.0-Weightで算出）
		Uint8 Index[4];		///< ボーンインデックス
	};

	/**
		@brief ラインプリミティブ用頂点データ
		@author 葉迩倭
	*/
	struct SLineVertex2D
	{
		SVertex2D v1;
		SVertex2D v2;
	};

	/**
		@brief プリミティブ用頂点データ
		@author 葉迩倭
	*/
	struct SPrimitiveVertex2D
	{
		SVertex2DTex v1;
		SVertex2DTex v2;
		SVertex2DTex v3;
	};

	/**
		@brief 3Dプリミティブ基本データ
		@author 葉迩倭

		ILine3Dで描画を行う際に使用する頂点データ。
	*/
	struct SLineVertex3D
	{
		SVertex3DBase v1;			///< 位置・頂点色
		SVertex3DBase v2;			///< 位置・頂点色
	};

	/**
		@brief テクスチャ生成用コンフィグ
		@author 葉迩倭

		テクスチャ生成時に参照される<BR>
		作成用パラメーターが定義されるクラスです。
	*/
	class CTextureConfig
	{
	private:
		eSurfaceFormat m_Format;	///< サーフェイスフォーマット
		CColor m_KeyColor;			///< カラーキーの色
		Bool m_IsMipmap;			///< ミップマップ
		Bool m_IsHalfSize;			///< 半分サイズ

	public:
		/**
			@brief コンストラクタ
		*/
		CTextureConfig()
			: m_Format		( FORMAT_TEXTURE_32BIT )
			, m_KeyColor	( 0x00000000 )
			, m_IsMipmap	( false )
			, m_IsHalfSize	( false )
		{
		}

		/**
			@brief デストラクタ
		*/
		~CTextureConfig()
		{
		}

		/**
			@brief カラーキー（透明色）設定
			@author 葉迩倭
			@param Col	[in] カラーキー（透明色）に使う色<BR>0x00000000の時カラーキー（透明色）OFF

			テクスチャを読み込んだ際に指定した色のアルファを0にし、<BR>
			カラーキー処理を行ったようにそのピクセルを描画<BR>
			されないようにします。
		*/
		void SetColorKey( CColor Col )
		{
			m_KeyColor = Col;
		}

		/**
			@brief サーフェイスフォーマット設定
			@author 葉迩倭
			@param Fmt	[in] サーフェイスフォーマット

			テクスチャ読み込み時のサーフェイスフォーマットを設定します。<BR>
			無効なフォーマットを指定した場合、作成は失敗します。
		*/
		void SetFormat( eSurfaceFormat Fmt )
		{
			m_Format = Fmt;
		}

		/**
			@brief ミップマップ有無設定
			@author 葉迩倭
			@param IsEnable	[in] ミップマップ有無

			テクスチャ読み込み時のミップマップの有無を設定します。
		*/
		void SetMipmapEnable( Bool IsEnable ) 
		{
			m_IsMipmap = IsEnable;
		}

		/**
			@brief 1/2サイズテクスチャ有無設定
			@author 葉迩倭
			@param IsEnable	[in] 1/2サイズテクスチャ有無

			テクスチャ読み込み時の1/2サイズテクスチャの有無を設定します。<BR>
			このフラグは純粋にテクスチャの使用量を抑える時に使います。
		*/
		void SetHalfSizeEnable( Bool IsEnable ) 
		{
			m_IsHalfSize = IsEnable;
		}

		/**
			@brief カラーキーを取得
			@author 葉迩倭
			@retval	0		カラーキー処理を行わない
			@retval	0以外	その色をカラーキーとする
		*/
		CColor GetColorKey( void )
		{
			return m_KeyColor;
		}

		/**
			@brief サーフェイスフォーマットを取得します
			@author 葉迩倭
			@return サーフェイスフォーマット
		*/
		eSurfaceFormat GetFormat( void )
		{
			return m_Format;
		}

		/**
			@brief ミップマップの有無を取得します
			@author 葉迩倭
			@return ミップマップの有無
		*/
		Bool IsMipmap( void )
		{
			return m_IsMipmap;
		}

		/**
			@brief 1/2サイズテクスチャの有無を取得します
			@author 葉迩倭
			@return 1/2サイズテクスチャの有無
		*/
		Bool IsHalfSize( void )
		{
			return m_IsHalfSize;
		}
	};

	/**
		@brief レンダラーインターフェイス
		@author 葉迩倭

		画面に対する全ての描画処理を管理します。<BR>
		描画用の各種クラスも全てこのクラスから取得します<BR>
		またこのインターフェイスは１つのIDeviceインターフェイスに対して１つしか作成されません。
	*/
	class IRender
	{
	public:
		virtual ~IRender() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief シェーダーバージョンチェック
			@author 葉迩倭
			@retval false	未対応
			@retval true	対応

			シェーダーモデル2.0に対応しているかどうかを調べます。
		*/
		virtual Bool IsSupportedPS20( void ) = 0;

		/**
			@brief シェーダーバージョンチェック
			@author 葉迩倭
			@retval false	未対応
			@retval true	対応

			シェーダーモデル3.0に対応しているかどうかを調べます。
		*/
		virtual Bool IsSupportedPS30( void ) = 0;

		/**
			@brief ピクセルシェーダーの命令数チェック
			@author 葉迩倭
			@parma Count	[in] 命令数
			@retval false	未対応
			@retval true	対応

			指定した命令数に対応している稼動を調べます。
		*/
		virtual Bool IsPixelShaderInstructionSupport( Sint32 Count ) = 0;

		/**
			@brief ピクセルシェーダーの命令数チェック
			@author 葉迩倭
			@retval マルチレンダリングターゲット数

			マルチレンダリングターゲットの対応数を取得します。
		*/
		virtual Sint32 GetMRTs( void ) = 0;

		/**
			@brief レンダリングターゲットクリア
			@author 葉迩倭
			@param Color	[in] クリアに使う色
			@retval false	失敗
			@retval true	成功

			設定されたレンダリングターゲットを指定された色でクリアーします。
		*/
		virtual Bool Clear( CColor Color = 0x00000000 ) = 0;

		/**
			@brief 深度ステンシルバッファクリア
			@author 葉迩倭
			@retval false	失敗
			@retval true	成功

			設定された深度ステンシルバッファをクリアします。<BR>
			深度ステンシルバッファが存在しない場合失敗します。
		*/
		virtual Bool ClearDepthStencil( void ) = 0;

		/**
			@brief 描画領域シザリング
			@author 葉迩倭
			@param pRect		[in] 描画領域(NULLで解除)

			指定した描画領域内にしかレンダリングされないようになります。<BR>
			ビューポートには影響を与えません。
		*/
		virtual void SetScissorRect( const Math::Rect2DI *pRect ) = 0;

		/**
			@brief デバイスリセット時コールバック関数登録
			@author 葉迩倭
			@param pCallback	[in] コールバック関数

			デバイスロスト時にコールバックされる関数を設定します。<BR>
			通常の用途では使用する必要はありません。<BR>
			なんらかの理由で直接デバイスを操作する時にのみ使用してください。
		*/
		virtual void SetCallbackDeviceReset( void (*pCallback)( void ) ) = 0;

		/**
			@brief 利用可能なVRAM残量を取得
			@author 葉迩倭
			@return VRAM残量

			使用可能なVRAM残量を取得します。<BR>
			この値はビデオカードに搭載されているVRAM量とは違い、<BR>
			ドライバが使用可能な容量を返します。
		*/
		virtual Float GetRestVRAM( void ) = 0;

		/**
			@brief デバイスリスト時コールバック関数登録
			@author 葉迩倭
			@param pCallback	[in] コールバック関数

			デバイスリストア時にコールバックされる関数を設定します。<BR>
			通常の用途では使用する必要はありません。<BR>
			なんらかの理由で直接デバイスを操作する時にのみ使用してください。
		*/
		virtual void SetCallbackDeviceRestore( void (*pCallback)( void ) ) = 0;

		/**
			@brief カラー書き込みマスクを設定
			@author 葉迩倭
			@param IsAlpha		[in] アルファ書き込みマスクON/OFF
			@param IsRed		[in] 赤成分書き込みマスクON/OFF
			@param IsGreen		[in] 緑成分書き込みマスクON/OFF
			@param IsBlue		[in] 青成分書き込みマスクON/OFF

			フラグをtrueにした色への書き込みを行います。<BR>
			falseにするとその色への書き込みを行いません。
		*/
		virtual void SetColorWriteEnable( Bool IsAlpha, Bool IsRed, Bool IsGreen, Bool IsBlue ) = 0;

		/**
			@brief 内部デバイスを取得
			@author 葉迩倭
			@return		デバイスのポインター

			内部で使用しているデバイスを特例的に取得します。<BR>
			通常の用途では使用する必要はありません。<BR>
			なんらかの理由で直接デバイスを操作する時にのみ使用してください。<BR>
			ここで取得したデバイスを経由してステートなどの変更を行った場合、<BR>
			その後のSeleneでのステート管理はサポート外になります。<BR>
		*/
		virtual void *GetDriverDevicePointer( void ) = 0;

		/**
			@brief レンダリングターゲットの内容をPNGファイルに保存
			@author 葉迩倭
			@param pFileName	[in] ファイル名(*.png)

			レンダリングターゲットの内容をPNGファイルとして保存します。<BR>
			この機能を使うにはレンダリングターゲットがロック可能なサーフェイスでなければいけません。
		*/
		virtual void SaveToFilePNG( const char *pFileName ) = 0;

		/**
			@brief レンダリングターゲットの内容をJPGファイルに保存
			@author 葉迩倭
			@param pFileName	[in] ファイル名(*.jpg)

			レンダリングターゲットの内容をJPGファイルとして保存します。<BR>
			この機能を使うにはレンダリングターゲットがロック可能なサーフェイスでなければいけません。
		*/
		virtual void SaveToFileJPG( const char *pFileName ) = 0;

		/**
			@brief 標準のコントロールを表示するためのクリッピングのON/OFF
			@author 葉迩倭
			@param IsEnable	[in] クリップ処理のON/OFF

			trueを指定することでクリッピングが有効になりデバイス上でも正しく表示されるようになります。<BR>
		*/
		virtual void SetGUIEnable( Bool IsEnable ) = 0;

		/**
			@brief テクスチャ設定
			@author 葉迩倭
			@param Stage	[in] 設定するステージ（０～７）
			@param pTex		[in] 設定するテクスチャ（NULLで無効化）
			@retval false	失敗
			@retval true	成功

			テクスチャをレンダラーに設定します。<BR>
			一度設定されたテクスチャはNULLを設定するまで<BR>
			有効になっています。
		*/
		virtual Bool SetTexture( eTextureStage Stage, ITexture *pTex ) = 0;

		/**
			@brief 深度バッファ設定
			@author 葉迩倭
			@param pSurface	[in] 設定するサーフェイス
			@retval false	失敗
			@retval true	成功

			深度バッファを設定します。
		*/
		virtual Bool SetDepthBuffer( ITexture *pTexture ) = 0;

		/**
			@brief レンダリングターゲット設定
			@author 葉迩倭
			@retval false	失敗
			@retval true	成功

			レンダリングターゲットをバックバッファに戻します。
		*/
		virtual Bool ResetRenderTarget( void ) = 0;

		/**
			@brief レンダリングターゲット設定
			@author 葉迩倭
			@retval false	失敗
			@retval true	成功

			レンダリングターゲットとして設定します。<BR>
			CreateRenderTarget()で生成されたテクスチャ以外は失敗します。
		*/
		virtual Bool SetRenderTarget( ITexture *pTexture ) = 0;

		/**
			@brief レンダリング処理開始通知
			@author 葉迩倭

			レンダラーに対してこれからレンダリング処理を<BR>
			開始するという事を通知します。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief レンダリング処理終了通知
			@author 葉迩倭

			レンダラーに対してこれでレンダリング処理を<BR>
			終了するということを通知します。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 2D描画シザリング設定
			@author 葉迩倭
			@param pRect	[in] シザリング領域(NULLで解除)

			レンダラーに関連する２Ｄ描画のシザリングを行います。
		*/
		virtual void SetScissorPrimitive2D( const Math::Rect2DF *pRect ) = 0;

		/**
			@brief カスタムシーン管理インターフェイス生成
			@author 葉迩倭
			@param QueMax			[in] シーンの描画キューの最大数
			@return カスタムシーン管理インターフェイス

			全てをプロジェクト側で管理するカスタムシーンマネージャーを生成します。<BR>
			レンダリングシステムを自前で管理したい場合に利用して下さい。
		*/
		virtual Scene::ICustomizedSceneManager *CreateCustomizedSceneManager( Sint32 QueMax ) = 0;

		/**
			@brief シーン管理インターフェイス生成
			@author 葉迩倭
			@param QueMax				[in] シーンの描画キューの最大数
			@param IsPixelShaderEnable	[in] ピクセルシェーダーの有無
			@return シーン管理インターフェイス

			シェーダーモデル2.0ベースのシーン管理用のインターフェイスを生成します。<BR>
			3Dに関するレンダリング処理は全てこのインターフェイスを介します。
		*/
		virtual Scene::ISceneManager *CreateSceneManager( Sint32 QueMax, Bool IsPixelShaderEnable ) = 0;

		/**
			@brief 2Dポイントプリミティブインターフェイス生成
			@author 葉迩倭
			@param PointMax		[in] 内部バッファ格納可能ポイント数
			@param IsAutoResize	[in] 自動リサイズ
			@param ResizeStep	[in] リサイズ時拡張サイズ
			@return 2Dポイントプリミティブインターフェイス

			2Dの点描画用のインターフェイスを生成します。<BR>
			画面に点を打つ場合はこのインターフェイスを使います。
		*/
		virtual Object::IPoint2D *CreatePoint2D( Sint32 PointMax, Bool IsAutoResize = false, Sint32 ResizeStep = 0 ) = 0;

		/**
			@brief 2Dラインプリミティブインターフェイス生成
			@author 葉迩倭
			@param LineMax	[in] 内部バッファ格納可能ライン数
			@param IsAutoResize	[in] 自動リサイズ
			@param ResizeStep	[in] リサイズ時拡張サイズ
			@return 2Dラインプリミティブインターフェイス

			2Dの点描画用のインターフェイスを生成します。<BR>
			画面に線を引く場合はこのインターフェイスを使います。
		*/
		virtual Object::ILine2D *CreateLine2D( Sint32 LineMax, Bool IsAutoResize = false, Sint32 ResizeStep = 0 ) = 0;

		/**
			@brief 2Dポリゴンプリミティブインターフェイス生成
			@author 葉迩倭
			@param PrimitiveMax	[in] 内部バッファ格納可能頂点数（通常１つの三角形に三頂点消費する）
			@param IsAutoResize	[in] 自動リサイズ
			@param ResizeStep	[in] リサイズ時拡張サイズ
			@return 2Dポリゴンプリミティブインターフェイス

			2Dのポリゴン（三角形）を描画するためのインターフェイスを生成します。<BR>
			最小単位である三角形ごとの描画を行う場合はこのインターフェイスを使います。
		*/
		virtual Object::IPrimitive2D *CreatePrimitive2D( Sint32 PrimitiveMax, Bool IsAutoResize = false, Sint32 ResizeStep = 0 ) = 0;

		/**
			@brief 2Dスプライトインターフェイス生成
			@author 葉迩倭
			@param PrimitiveMax	[in] 内部バッファ格納可能頂点数（通常１つの三角形に三頂点消費する）
			@param pTexture		[in] スプライトに関連付けるテクスチャインターフェイス
			@param IsFiltering	[in] テクスチャフィルタを使うか否か
			@param IsOffset		[in] フィルタ使用時のテクスチャUV補正値を行うか否か
			@param IsAutoResize	[in] 自動リサイズ
			@param ResizeStep	[in] リサイズ時拡張サイズ
			@return 2Dスプライトインターフェイス

			2Dのスプライトを描画するためのインターフェイスを生成します。<BR>
			IPrimitive2Dを継承しており、内部的にはIPrimitive2Dでの描画を行っています。<BR>
			テクスチャを使った最も簡単なスプライト描画が可能なインターフェイスです。<BR>
			<BR>
			IsFilteringがtrueの時、描画にバイリニアフィルタリングが適用され、拡大縮小、回転などの時に<BR>
			補間され綺麗な描画行われますが、逆に通常の等倍表示の時にぼやけてしまうという欠点があります。<BR>
			この欠点を解消するにはIsFitlerをtrueにし、IsOffsetをfalseにする事で可能ですが、<BR>
			バイリニアフィルタの隣接ピクセルを参照してしまうという問題は画像データの方で解消する必要があります。<BR>
			具体的には周囲に１ドットに余白として何も描画されないピクセルを置くことです。
		*/
		virtual Object::ISprite2D *CreateSprite2D( Sint32 PrimitiveMax, ITexture *pTexture, Bool IsFiltering = false, Bool IsOffset = false, Bool IsAutoResize = false, Sint32 ResizeStep = 0 ) = 0;

		/**
			@brief 2D用フォントスプライトインターフェイス生成
			@author 葉迩倭
			@param pFileName	[in] フォントスプライト用定義ファイル
			@param pExt			[in] フォントスプライト用画像ファイル拡張子
			@param FontMax		[in] 最大フォント数（内部の頂点バッファ数）
			@param IsAutoResize	[in] 自動リサイズ
			@param ResizeStep	[in] リサイズ時拡張サイズ
			@return 2Dフォントスプライトインターフェイス

			2Dのフォントスプライトを描画するためのインターフェイスを生成します。<BR>
			ISprite2Dを利用しており、内部的にはISprite2Dでの描画を行っています。<BR>
			FontUtilityで生成した定義ファイルとテクスチャを使って、高速に文字列の<BR>
			描画を行うことが可能です。
		*/
		virtual Object::IFontSprite2D *CreateFontSprite2DFromFile( const char *pFileName, const char *pExt, Sint32 FontMax, Bool IsAutoResize = false, Sint32 ResizeStep = 0 ) = 0;

		/**
			@brief 3Dポイントプリミティブインターフェイス生成
			@author 葉迩倭
			@param VertexMax	[in] ポイントの最大数
			@param IsDynamic	[in] ダイナミックバッファの使用ON/OFF（頻繁に書き換える場合はtrueにして下さい）
			@return 3Dポイントプリミティブインターフェイス

			3Dのポイント描画用のインターフェイスを生成します。<BR>
			位置及び色データのみで、テクスチャやライトなどの不可効果をつけることは出来ません。
		*/
		virtual Object::IPoint3D *CreatePoint3D( Sint32 VertexMax, Bool IsDynamic ) = 0;

		/**
			@brief 3Dラインプリミティブインターフェイス生成
			@author 葉迩倭
			@param VertexMax	[in] ラインの最大数
			@param IsDynamic	[in] ダイナミックバッファの使用ON/OFF（頻繁に書き換える場合はtrueにして下さい）
			@return 3Dラインプリミティブインターフェイス

			3Dのライン描画用のインターフェイスを生成します。<BR>
			位置及び色データのみで、テクスチャやライトなどの不可効果をつけることは出来ません。
		*/
		virtual Object::ILine3D *CreateLine3D( Sint32 VertexMax, Bool IsDynamic ) = 0;

		/**
			@brief 3Dポリゴンプリミティブインターフェイス生成
			@author 葉迩倭
			@param VertexMax		[in] 内部頂点バッファの最大数
			@param IndexMax			[in] 内部インデックスバッファの最大数
			@param VertexFlag		[in] 頂点データフラグ（eVertexElementの組み合わせ）
			@param IsDynamicVertex	[in] ダイナミック頂点バッファの使用ON/OFF（頻繁に書き換える場合はtrueにして下さい）
			@param IsDynamicIndex	[in] ダイナミックインデックスバッファの使用ON/OFF（頻繁に書き換える場合はtrueにして下さい）
			@return 3Dポリゴンプリミティブインターフェイス

			3Dのポリゴン（三角形）を描画するためのインターフェイスを生成します。<BR>
			最小単位である三角形ごとの描画を行う場合はこのインターフェイスを使います。<BR>
			またインデックスを使用した効率的な描画をサポートします。<BR>
			VertexFlagは頂点のフォーマットを指定するもので eVertexElement 列挙型の中から<BR>
			VERTEX_ELEMENT_PRIMITIVE と VERTEX_ELEMENT_SPRITE を除いたものを指定します。<BR>
			複数組み合わせる場合は | 演算子で指定してください。<BR>
			利用されるシェーダーは内部で作成された固定シェーダーが利用されます。<BR>
			現在のバージョンではバンプマップに関する処理は行われません。<BR>
			<BR>
			（例）法線を持ちライティング可能な頂点 -> VERTEX_ELEMENT_3DTEXTURE | VERTEX_ELEMENT_3DLIGHT
		*/
		virtual Object::IPrimitive3D *CreatePrimitive3D( Sint32 VertexMax, Sint32 IndexMax, Sint32 VertexFlag, Bool IsDynamicVertex, Bool IsDynamicIndex ) = 0;

		/**
			@brief 3Dスプライトインターフェイス生成
			@author 葉迩倭
			@param SpriteMax		[in] 内部バッファ格納可能スプライト数
			@param pTexture			[in] スプライトに関連付けるテクスチャインターフェイス
			@return 3Dスプライトインターフェイス

			3Dのスプライトを描画するためのインターフェイスを生成します。<BR>
			IPrimitive3Dを継承しており、内部的にはIPrimitive3Dでの描画を行っています。<BR>
			テクスチャを使った最も簡単なスプライト描画が可能なインターフェイスです。
		*/
		virtual Object::ISprite3D *CreateSprite3D( Sint32 SpriteMax, ITexture *pTexture ) = 0;

		/**
			@brief パーティクルインターフェイス生成
			@author 葉迩倭
			@param ParticleMax		[in] 内部バッファ格納可能パーティクル数
			@param pTexture			[in] パーティクルに関連付けるテクスチャインターフェイス
			@param Type				[in] パーティクルに関する頂点フォーマットタイプ
			@param IsSoftBillboard	[in] ソフトパーティクル化するかどうか
			@return パーティクルインターフェイス

			パーティクルを描画するためのインターフェイスを生成します。<BR>
			IPrimitive3Dを継承しており、内部的にはIPrimitive3Dでの描画を行っています。
		*/
		virtual Object::IParticle *CreateParticle( Sint32 ParticleMax, ITexture *pTexture, eParticleType Type, Bool IsSoftBillboard = false ) = 0;

		/**
			@brief 3D用フォントスプライトインターフェイス生成
			@author 葉迩倭
			@param pFileName	[in] フォントスプライト用定義ファイル
			@param pExt			[in] フォントスプライト用画像ファイル拡張子
			@param FontMax		[in] 最大フォント数（内部の頂点バッファ数）
			@return 3Dフォントスプライトインターフェイス

			3Dのフォントスプライトを描画するためのインターフェイスを生成します。<BR>
			IParticleを利用しており、内部的にはIParticleでの描画を行っています。<BR>
			FontUtilityで生成した定義ファイルとテクスチャを使って、高速に文字列の<BR>
			描画を行うことが可能です。
		*/
		virtual Object::IFontSprite3D *CreateFontSprite3DFromFile( const char *pFileName, const char *pExt, Sint32 FontMax ) = 0;

		/**
			@brief ツリーモデルをファイルから生成
			@author 葉迩倭
			@param pFileName	[in] モデル名

			SMF形式のモデルファイルを読み込みます。<BR>
			SMFファイルはAmaryllisを使うことでXファイルから生成できます。
		*/
		virtual Object::IMapModel *CreateMapModelFromFile( const char *pFileName ) = 0;

		/**
			@brief ツリーモデルをメモリから生成
			@author 葉迩倭
			@param pData	[in] モデルデータ
			@param Size		[in] データサイズ

			SMF形式のモデルデータを読み込みます。<BR>
			SMFファイルはAmaryllisを使うことでXファイルから生成できます。
		*/
		virtual Object::IMapModel *CreateMapModelFromMemory( const Uint8 *pData, Sint32 Size ) = 0;

		/**
			@brief モデルをファイルから生成
			@author 葉迩倭
			@param pFileName	[in] モデル名

			SMF形式のモデルファイルを読み込みます。<BR>
			SMFファイルはAmaryllisを使うことでXファイルから生成できます。
		*/
		virtual Object::IModel *CreateModelFromFile( const char *pFileName ) = 0;

		/**
			@brief モデルをメモリから生成
			@author 葉迩倭
			@param pData	[in] モデルデータ
			@param Size		[in] データサイズ

			SMF形式のモデルデータを読み込みます。<BR>
			SMFファイルはAmaryllisを使うことでXファイルから生成できます。
		*/
		virtual Object::IModel *CreateModelFromMemory( const Uint8 *pData, Sint32 Size ) = 0;

		/**
			@brief テクスチャ読み込み用コンフィグデータを取得
			@author 葉迩倭
			@return テクスチャコンフィグデータ
		*/
		virtual CTextureConfig &GetTextureConfig( void ) = 0;

		/**
			@brief テクスチャ読み込み用コンフィグデータ設定
			@author 葉迩倭
			@param Conf		[in] テクスチャコンフィグデータ
		*/
		virtual void SetTextureConfig( CTextureConfig &Conf ) = 0;

		/**
			@brief デバイスからサーフェイスを生成
			@author 葉迩倭
			@param Width	[in] 横幅
			@param Height	[in] 縦幅
			@return サーフェイスインターフェイス
		*/
		virtual ITexture *CreateTextureDepthBuffer( Sint32 Width, Sint32 Height ) = 0;

		/**
			@brief デバイスからテクスチャを生成
			@author 葉迩倭
			@param Width	[in] テクスチャの横幅
			@param Height	[in] テクスチャの縦幅
			@param Format	[in] テクスチャフォーマット
			@return テクスチャインターフェイス
		*/
		virtual ITexture *CreateTextureRenderTarget( Sint32 Width, Sint32 Height, eSurfaceFormat Format ) = 0;

		/**
			@brief デバイスからテクスチャを生成
			@author 葉迩倭
			@param pFileName	[in] テクスチャファイル名
			@return テクスチャインターフェイス
		*/
		virtual ITexture *CreateTextureFromFile( const char *pFileName ) = 0;

		/**
			@brief デバイスからテクスチャを生成
			@author 葉迩倭
			@param pData	[in] データのポインタ
			@param Size		[in] データサイズ
			@return テクスチャインターフェイス
		*/
		virtual ITexture *CreateTextureFromMemory( const void *pData, Sint32 Size ) = 0;

		/**
			@brief デバイスからキューブテクスチャを生成
			@author 葉迩倭
			@param pFileName	[in] テクスチャファイル名
			@return テクスチャインターフェイス
		*/
		virtual ITexture *CreateCubeTextureFromFile( const char *pFileName ) = 0;

		/**
			@brief デバイスからキューブテクスチャを生成
			@author 葉迩倭
			@param pData	[in] データのポインタ
			@param Size		[in] データサイズ
			@return テクスチャインターフェイス
		*/
		virtual ITexture *CreateCubeTextureFromMemory( const void *pData, Sint32 Size ) = 0;

		/**
			@brief シェーダーの生成
			@author 葉迩倭
			@param pShader		[in] シェーダーデータのポインタ
			@param Size			[in] シェーダーデータのサイズ
			@param IsCompiled	[in] コンパイル済みか否か
			@return シェーダーインターフェイス

			シェーダーファイルからシェーダーインターフェイスを生成します。
		*/
		virtual Shader::IShader *CreateShaderFromMemory( const void *pShader, Sint32 Size, Bool IsCompiled = false ) = 0;

		/**
			@brief シェーダーの生成
			@author 葉迩倭
			@param pShader		[in] シェーダーファイル
			@param IsCompiled	[in] コンパイル済みか否か
			@return シェーダーインターフェイス

			シェーダーファイルからシェーダーインターフェイスを生成します。
		*/
		virtual Shader::IShader *CreateShaderFromFile( const char *pFile, Bool IsCompiled = false ) = 0;

		/**
			@brief 描画タイプ設定
			@author 葉迩倭
			@param Type		[in] 描画タイプ

			ポリゴンの描画タイプを設定します。
		*/
		virtual void SetDrawType( eDrawType Type ) = 0;

		/**
			@brief 表裏判定タイプ設定
			@author 葉迩倭
			@param Type		[in] 表裏判定タイプ

			ポリゴンの表裏判定タイプを設定します。
		*/
		virtual void SetCullType( eCullType Type ) = 0;

		/**
			@brief フィルタリングタイプ設定
			@author 葉迩倭
			@param Stage	[in] 設定ステージ
			@param Type		[in] フィルタリングタイプ

			テクスチャステージ毎のフィルタリングタイプを設定します。<BR>
			TEXTURE_FILTER_ANISOTROPY系 のフィルタがサポートされないハードウェアでは<BR>
			TEXTURE_FILTER_2D が使われます。
		*/
		virtual void SetTextureFilterType( eTextureStage Stage, eTextureFilterType Type ) = 0;

		/**
			@brief 深度テストON/OFF設定
			@author 葉迩倭
			@param Flag		[in] 深度テストON/OFF

			深度テストのON/OFFを設定します。
		*/
		virtual void SetDepthTestEnable( Bool Flag ) = 0;

		/**
			@brief 深度バッファ書き込みON/OFF設定
			@author 葉迩倭
			@param Flag		[in] 深度バッファ書き込みON/OFF

			深度バッファ書き込みのON/OFFを設定します。
		*/
		virtual void SetDepthWriteEnable( Bool Flag ) = 0;

		/**
			@brief アルファテストON/OFF設定
			@author 葉迩倭
			@param Flag		[in] アルファテストON/OFF

			アルファテストのON/OFFを設定します。
		*/
		virtual void SetAlphaTestEnable( Bool Flag ) = 0;

		/**
			@brief アルファテスト閾値設定
			@author 葉迩倭
			@param Bound	[in] アルファテスト閾値

			アルファテストの閾値を設定します。
		*/
		virtual void SetAlphaBoundary( Sint32 Bound ) = 0;

		/**
			@brief ステートの設定をスタックに退避します
			@author 葉迩倭
			@retval false	スタックオーバーフロー
			@retval true	エラー無し

			現在のステートをスタックに退避させます。<BR>
			この関数によって退避されるステートは以下の関数で設定したものです。<BR>
			・void SetDrawType( eDrawType Type )<BR>
			・void SetCullType( eCullType Type )<BR>
			・void SetTextureFilterType( Sint32 Stage, eTextureFilterType Type )<BR>
			・void SetTextureAddressType( Sint32 Stage, eTextureAddressType Type )<BR>
			・void SetDepthTestEnable( Bool Flag )<BR>
			・void SetDepthWriteEnable( Bool Flag )<BR>
			・void SetAlphaTestEnable( Bool Flag )<BR>
			・void SetAlphaBoundary( Sint32 Bound )<BR>
			・Bool SetTexture( Sint32 Stage, ITexture *pTex )
		*/
		virtual Bool StatePush( void ) = 0;

		/**
			@brief ステートの設定をスタックから復帰します
			@author 葉迩倭
			@retval false	スタックオーバーフロー
			@retval true	エラー無し

			現在のステートをスタックに退避去れているステートに戻します。
		*/
		virtual Bool StatePop( void ) = 0;

		/**
			@brief ステートの設定を初期状態にリセットします
			@author 葉迩倭

			現在のステートをスタックに退避去れているステートに戻します。<BR>
			・SetDrawType( DRAW_TYPE_NORMAL )<BR>
			・SetCullType( CULL_FRONT )<BR>
			・SetDepthTestEnable( false )<BR>
			・SetDepthWriteEnable( false )<BR>
			・void SetAlphaTestEnable( true )<BR>
			・void SetAlphaBoundary( Sint32 Bound )<BR>
			・SetTextureFilterType( \\<Stage\\>, TEXTURE_FILTER_DISABLE )<BR>
			・SetTextureAddressType( \\<Stage\\>, TEXTURE_ADDRESS_REPEAT )<BR>
			・SetTexture( \\<Stage\\>, NULL )
		*/
		virtual void StateInit( void ) = 0;

		/**
			@brief デバッグ用文字設定変更
			@author 葉迩倭
			@param pFontFace	[in] フォント種類
			@param Size			[in] フォントサイズ

			デバッグ用の文字の設定を変更します。
		*/
		virtual void ChangeDebugPrintFont( const char *pFontFace, Sint32 Size ) = 0;

		/**
			@brief デバッグ用文字描画処理
			@author 葉迩倭
			@param Pos		[in] 描画位置
			@param Color	[in] 描画色
			@param pStr		[in] 描画文字列

			デバッグ用の文字描画を行います。
		*/
		virtual void DebugPrint( Math::Point2DI &Pos, CColor Color, const char *pStr, ... ) = 0;

		/**
			@brief 簡易文字描画用フォント設定
			@author 葉迩倭
			@param pFontFace	[in] フォント種類
			@param Size			[in] フォントサイズ
			@param Layer		[in] 描画文字データレイヤー(0～7)

			簡易文字描画のフォントの設定をします
		*/
		virtual void SetDrawTextFont( const char *pFontFace, Sint32 Size, Sint32 Layer = 0 ) = 0;

		/**
			@brief 簡易文字描画処理
			@author 葉迩倭
			@param Pos		[in] 描画位置
			@param Color	[in] 描画色
			@param pStr		[in] 描画文字列

			簡易文字描画を行います。<BR>
			SetDrawTextFont()でLayer=0を指定して作ったフォントが利用されます。
		*/
		virtual void DrawText( Math::Point2DI &Pos, CColor Color, const char *pStr, ... ) = 0;

		/**
			@brief 簡易文字描画処理
			@author 葉迩倭
			@param Layer	[in] 描画文字データレイヤー(0～7)
			@param Pos		[in] 描画位置
			@param Color	[in] 描画色
			@param pStr		[in] 描画文字列

			簡易文字描画を行います。
		*/
		virtual void DrawTextLayer( Sint32 Layer, Math::Point2DI &Pos, CColor Color, const char *pStr, ... ) = 0;

		/**
			@brief 矩形レンダリング
			@author 葉迩倭
			@param Dst			[in] 描画先矩形
			@param Color		[in] 描画色

			単純な矩形を塗りつぶします。
		*/
		virtual void FillRect( const Math::Rect2DF &Dst, CColor Color ) = 0;

		/**
			@brief シェーダーを利用する場合のテクスチャを使った矩形回転付きのレンダリング
			@author 葉迩倭
			@param Dst		[in] 描画先矩形
			@param Color	[in] 描画色
			@param Src		[in] テクスチャUV矩形
			@param pTex		[in] テクスチャ
			@param Angle	[in] 回転角度
			@param Offset	[in] 回転の中心の画像の中心からのオフセット

			外部のシェーダーを利用する場合の回転付きの単純な矩形をテクスチャ対応でレンダリングします。
		*/
		virtual void DrawTextureByShader( const Math::Rect2DF &Dst, CColor Color, const Math::Rect2DF &Src, ITexture *pTex, Sint32 Angle = 0, const Math::Point2DF &Offset = Math::Point2DF(0.0f,0.0f) ) = 0;

		/**
			@brief テクスチャを使った矩形回転付きのレンダリング
			@author 葉迩倭
			@param Dst		[in] 描画先矩形
			@param Color	[in] 描画色
			@param Src		[in] テクスチャUV矩形
			@param pTex		[in] テクスチャ
			@param Angle	[in] 回転角度
			@param Offset	[in] 回転の中心の画像の中心からのオフセット

			回転付きの単純な矩形をテクスチャ対応でレンダリングします。
		*/
		virtual void DrawTexture( const Math::Rect2DF &Dst, CColor Color, const Math::Rect2DF &Src, ITexture *pTex, Sint32 Angle = 0, const Math::Point2DF &Offset = Math::Point2DF(0.0f,0.0f) ) = 0;

		/**
			@brief テクスチャを使った矩形回転付きのレンダリング
			@author 葉迩倭
			@param Dst		[in] 描画先矩形
			@param Color	[in] 描画色
			@param Src		[in] テクスチャUV矩形
			@param pTex		[in] テクスチャ
			@param Angle	[in] 回転角度
			@param Offset	[in] 回転の中心の画像の中心からのオフセット

			回転付きの単純な矩形をテクスチャ対応でレンダリングします。
		*/
		virtual void DrawTextureTile( const Math::Rect2DF &Dst, CColor Color, const Math::Rect2DF &Src, ITexture *pTex, Sint32 Angle = 0, const Math::Point2DF &Offset = Math::Point2DF(0.0f,0.0f) ) = 0;

		/**
			@brief ユニバーサルトランジション描画
			@author 葉迩倭
			@param Dst			[in] 描画先矩形
			@param fRate		[in] 透明度（0.0～1.0）
			@param SrcBase		[in] ベース画像テクスチャUV矩形
			@param pTexBase		[in] ベース画像テクスチャ
			@param SrcRule		[in] アルファ用ルールテクスチャUV矩形
			@param pTexRule		[in] アルファ用ルールテクスチャ
			@param Angle		[in] 回転角度
			@param Offset		[in] 回転の中心の画像の中心からのオフセット

			ルール画像を用いたユニバーサルトランジションを行います。<BR>
			PixelShader2.0がない環境ではDrawTextureに置き換えられます。
		*/
		virtual void DrawUniversal( const Math::Rect2DF &Dst, Float fRate, const Math::Rect2DF &SrcBase, ITexture *pTexBase, const Math::Rect2DF &SrcRule, ITexture *pTexRule, Sint32 Angle = 0, const Math::Point2DF &Offset = Math::Point2DF(0.0f,0.0f) ) = 0;

		/**
			@brief 4x4コーンぼかしフィルタ描画
			@author 葉迩倭
			@param Dst		[in] 描画先矩形
			@param Color	[in] 描画色
			@param Src		[in] テクスチャUV矩形
			@param pTex		[in] テクスチャ
			@param Angle	[in] 回転角度
			@param Offset	[in] 回転の中心の画像の中心からのオフセット

			4x4のボックスぼかしのフィルタをかけて描画します。<BR>
			PixelShader2.0がない環境ではDrawTextureに置き換えられます。
		*/
		virtual void DrawBlur( const Math::Rect2DF &Dst, CColor Color, const Math::Rect2DF &Src, ITexture *pTex, Sint32 Angle = 0, const Math::Point2DF &Offset = Math::Point2DF(0.0f,0.0f) ) = 0;

		/**
			@brief グレースケールフィルタ描画
			@author 葉迩倭
			@param Dst		[in] 描画先矩形
			@param Color	[in] 描画色
			@param Src		[in] テクスチャUV矩形
			@param pTex		[in] テクスチャ
			@param Angle	[in] 回転角度
			@param Offset	[in] 回転の中心の画像の中心からのオフセット

			グレースケールのフィルタをかけて描画します。<BR>
			PixelShader2.0がない環境ではDrawTextureに置き換えられます。
		*/
		virtual void DrawGrayscale( const Math::Rect2DF &Dst, CColor Color, const Math::Rect2DF &Src, ITexture *pTex, Sint32 Angle = 0, const Math::Point2DF &Offset = Math::Point2DF(0.0f,0.0f) ) = 0;

		/**
			@brief 魚眼レンズ描画
			@author 葉迩倭
			@param Dst		[in] 描画先矩形
			@param Color	[in] 描画色
			@param Src		[in] テクスチャUV矩形
			@param pTex		[in] テクスチャ
			@param fRate	[in] 魚眼レンズ歪み度（０で歪みなし）

			魚眼レンズのフィルタをかけて描画します。<BR>
			PixelShader2.0がない環境ではDrawTextureに置き換えられます。
		*/
		virtual void DrawFishEye( const Math::Rect2DF &Dst, CColor Color, const Math::Rect2DF &Src, ITexture *pTex, Float fRate ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	typedef void *FX_HANDLE;

namespace Renderer
{
namespace Shader
{
	class IShader : public IInterface
	{
	protected:
		virtual ~IShader() {}

	public:
		virtual Bool Begin( Sint32 *pPassMax, Bool IsSave = false ) = 0;
		virtual void End( void ) = 0;
		virtual void BeginPass( Sint32 Pass ) = 0;
		virtual void EndPass( void ) = 0;
		virtual void UpdateParameter( void ) = 0;

		virtual FX_HANDLE GetParameterByName( const char *pName ) = 0;
		virtual FX_HANDLE GetParameterBySemantic( const char *pSemantic ) = 0;

		virtual FX_HANDLE GetTechnique( const char *pName ) = 0;
		virtual void SetTechnique( FX_HANDLE Handle ) = 0;

		virtual void SetFloat( FX_HANDLE Handle, Float Param ) = 0;
		virtual void SetFloatArray( FX_HANDLE Handle, const Float *pParam, Sint32 Num ) = 0;
		virtual void SetInt( FX_HANDLE Handle, Sint32 Param ) = 0;
		virtual void SetIntArray( FX_HANDLE Handle, const Sint32 *pParam, Sint32 Num ) = 0;
		virtual void SetMatrix( FX_HANDLE Handle, const Math::SMatrix4x4 *pMatrix ) = 0;
		virtual void SetMatrixArray( FX_HANDLE Handle, const Math::SMatrix4x4 *pMatrix, Sint32 Num ) = 0;
		virtual void SetMatrixPointerArray( FX_HANDLE Handle, const Math::SMatrix4x4 **ppMatrix, Sint32 Num ) = 0;
		virtual void SetMatrixTranspose( FX_HANDLE Handle, const Math::SMatrix4x4 *pMatrix ) = 0;
		virtual void SetMatrixTransposeArray( FX_HANDLE Handle, const Math::SMatrix4x4 *pMatrix, Sint32 Num ) = 0;
		virtual void SetTexture( FX_HANDLE Handle, Renderer::ITexture *pTex ) = 0;
		virtual void SetVector( FX_HANDLE Handle, const Math::Vector4D *pVec ) = 0;
		virtual void SetVectorArray( FX_HANDLE Handle, const Math::Vector4D *pVec, Sint32 Num ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 2Dポイントプリミティブ管理インターフェイス
		@author 葉迩倭

		2Dポイント描画用のインターフェイスです。
	*/
	class IPoint2D
	{
	public:
		virtual ~IPoint2D() {}
		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 内部バッファへデータ追加
			@author 葉迩倭
			@param pPoint	[in] 頂点データ
			@param Count	[in] 追加数

			内部バッファのデータの追加を行います。<BR>
			この関数を呼び出す前に必ずBeing関数でデータ追加の開始を宣言して下さい。
		*/
		virtual Bool Push( SVertex2D *pPoint, Sint32 Count ) = 0;

		/**
			@brief レンダリング
			@author 葉迩倭

			内部バッファの内容のレンダリングを行います。
		*/
		virtual void Rendering( void ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 2Dラインプリミティブ管理インターフェイス
		@author 葉迩倭

		2Dライン描画用のインターフェイスです。
	*/
	class ILine2D
	{
	public:
		virtual ~ILine2D() {}
		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 内部バッファへデータ追加
			@author 葉迩倭
			@param pLine	[in] ラインデータ
			@param Count	[in] 追加数

			内部バッファのデータの追加を行います。<BR>
			この関数を呼び出す前に必ずBeing関数でデータ追加の開始を宣言して下さい。
		*/
		virtual Bool Push( SLineVertex2D *pLine, Sint32 Count ) = 0;

		/**
			@brief レンダリング
			@author 葉迩倭

			内部バッファの内容のレンダリングを行います。
		*/
		virtual void Rendering( void ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 2Dプリミティブ管理インターフェイス
		@author 葉迩倭

		2Dプリミティブ描画用のインターフェイスです。
	*/
	class IPrimitive2D
	{
	public:
		virtual ~IPrimitive2D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void )																													= 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void )																													= 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void )																													= 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void )																														= 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void )																														= 0;

		/**
			@brief 頂点リクエスト数取得
			@author 葉迩倭
			@return リクエストした頂点数

			Pushした最大頂点数を取得します。
		*/
		virtual Sint32 GetRequestedVertexCount( void )																									= 0;

		/**
			@brief 頂点リクエスト数取得
			@author 葉迩倭
			@return リクエストした頂点数

			Pushした最大頂点数を取得します。
		*/
		virtual Sint32 GetRequestedVertexCountMax( void )																								= 0;

		/**
			@brief 内部バッファへデータ追加
			@author 葉迩倭
			@param pPrimitive	[in] プリミティブデータ
			@param Count		[in] 追加数

			内部バッファのデータの追加を行います。<BR>
			この関数を呼び出す前に必ずBeing関数でデータ追加の開始を宣言して下さい。
		*/
		virtual Bool Push( SPrimitiveVertex2D *pPrimitive, Sint32 Count )																				= 0;

		/**
			@brief レンダリング
			@author 葉迩倭

			内部バッファの内容のレンダリングを行います。
		*/
		virtual void Rendering( void )																													= 0;

		/**
			@brief 四角形描画
			@author 葉迩倭
			@param DstRect	[in] 描画矩形データ
			@param Color	[in] 描画色

			単純な四角形を描画します。
		*/
		virtual void Square( Math::Rect2DI &DstRect, CColor Color )																					= 0;

		/**
			@brief 回転付き四角形描画
			@author 葉迩倭
			@param DstRect	[in] 描画矩形データ
			@param Color	[in] 描画色
			@param Angle	[in] 1周を65536とした回転角度

			回転付きの四角形を描画します。
		*/
		virtual void SquareRotate( Math::Rect2DI &DstRect, CColor Color, Sint32 Angle )																= 0;

		/**
			@brief 多角形描画
			@author 葉迩倭
			@param Pos		[in] 中心位置
			@param fRadius	[in] 描画半径
			@param Color	[in] 描画色
			@param Num		[in] 角数

			単純な多角形を描画します。
		*/
		virtual void Polygon( Math::Point2DI Pos, Float fRadius, CColor Color, Sint32 Num )															= 0;

		/**
			@brief 回転付き多角形描画
			@author 葉迩倭
			@param Pos		[in] 中心位置
			@param fRadius	[in] 描画半径
			@param Color	[in] 描画色
			@param Num		[in] 角数
			@param Angle	[in] 1周を65536とした回転角度

			回転付き多角形を描画します。
		*/
		virtual void PolygonRotate( Math::Point2DI Pos, Float fRadius, CColor Color, Sint32 Num, Sint32 Angle )										= 0;

		/**
			@brief リング状描画
			@author 葉迩倭
			@param Pos			[in] 中心位置
			@param fInRadius	[in] 内周半径
			@param fOutRadius	[in] 外周半径
			@param InColor		[in] 内周描画色
			@param OutColor		[in] 外周描画色
			@param Num			[in] 角数

			リング状ポリゴンを描画します。
		*/
		virtual void Ring( Math::Point2DI Pos, Float fInRadius, Float fOutRadius, CColor InColor, CColor OutColor, Sint32 Num )						= 0;

		/**
			@brief 回転付きリング状描画
			@author 葉迩倭
			@param Pos			[in] 中心位置
			@param fInRadius	[in] 内周半径
			@param fOutRadius	[in] 外周半径
			@param InColor		[in] 内周描画色
			@param OutColor		[in] 外周描画色
			@param Num			[in] 角数
			@param Angle	[in] 1周を65536とした回転角度

			回転付きリング状ポリゴンを描画します。
		*/
		virtual void RingRotate( Math::Point2DI Pos, Float fInRadius, Float fOutRadius, CColor InColor, CColor OutColor, Sint32 Num, Sint32 Angle )	= 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief DrawList用構造体
		@author 葉迩倭

		@note
		DrawListで利用する構造体です。
	*/
	struct SSpriteListData2D
	{
		Math::Vector2D Pos;
		Float Width;
		Sint32 Angle;
		CColor Color;
	};

	/**
		@brief 2Dスプライト管理インターフェイス
		@author 葉迩倭

		2Dスプライト描画用のインターフェイスです。
	*/
	class ISprite2D
	{
	public:
		virtual ~ISprite2D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief レンダリング
			@author 葉迩倭

			内部バッファの内容のレンダリングを行います。
		*/
		virtual void Rendering( void ) = 0;

		/**
			@brief 頂点リクエスト数取得
			@author 葉迩倭
			@return リクエストした頂点数

			Pushした最大頂点数を取得します。
		*/
		virtual Sint32 GetRequestedVertexCount( void ) = 0;

		/**
			@brief 頂点リクエスト数取得
			@author 葉迩倭
			@return リクエストした頂点数

			Pushした最大頂点数を取得します。
		*/
		virtual Sint32 GetRequestedVertexCountMax( void ) = 0;

		/**
			@brief 内部バッファへデータ追加
			@author 葉迩倭
			@param pPrimitive	[in] プリミティブデータ
			@param Count		[in] 追加数

			内部バッファのデータの追加を行います。<BR>
			この関数を呼び出す前に必ずBeing関数でデータ追加の開始を宣言して下さい。
		*/
		virtual Bool Push( SPrimitiveVertex2D *pPrimitive, Sint32 Count ) = 0;

		/**
			@brief 装飾用テクスチャの設定
			@author 葉迩倭

			@param pTex		[in] テクスチャインターフェイス

			装飾用のテクスチャを設定します。
		*/
		virtual void SetDecoration( ITexture *pTex ) = 0;

		/**
			@brief 四角形描画
			@author 葉迩倭

			@param DstRect		[in] 転送先スクリーンの矩形
			@param SrcRect		[in] 転送元テクスチャの矩形
			@param Color		[in] 頂点色

			最も単純なスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawSquare( Math::Rect2DF &DstRect, Math::Rect2DF &SrcRect, CColor Color ) = 0;

		/**
			@brief 回転付き四角形描画
			@author 葉迩倭

			@param DstRect		[in] 転送先スクリーンの矩形
			@param SrcRect		[in] 転送元テクスチャの矩形
			@param Color		[in] 頂点色
			@param Angle		[in] 1周65536とした回転角度

			回転付きスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawSquareRotate( Math::Rect2DF &DstRect, Math::Rect2DF &SrcRect, CColor Color, Sint32 Angle ) = 0;

		/**
			@brief 回転付き四角形描画
			@author 葉迩倭

			@param DstRect		[in] 転送先スクリーンの矩形
			@param SrcRect		[in] 転送元テクスチャの矩形
			@param Color		[in] 頂点色
			@param AngleX		[in] 1周65536とした回転角度
			@param AngleY		[in] 1周65536とした回転角度
			@param AngleZ		[in] 1周65536とした回転角度

			回転付きスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawSquareRotateXYZ( Math::Rect2DF &DstRect, Math::Rect2DF &SrcRect, CColor Color, Sint32 AngleX, Sint32 AngleY, Sint32 AngleZ ) = 0;

		/**
			@brief 行列変換四角形描画
			@author 葉迩倭

			@param mWorld		[in] 変換行列
			@param SrcRect		[in] 転送元テクスチャの矩形
			@param Color		[in] 頂点色

			行列変換付きスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawSquareMatrix( Math::Matrix &mWorld, Math::Rect2DF &SrcRect, CColor Color ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Width	[in] 頂点ごとの幅
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawList( Math::Point2DF Pos[], Sint32 Angle[], CColor Color[], Sint32 Count, Float Width, Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Width	[in] 頂点ごとの幅の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawList( Math::Point2DF Pos[], Float Width[], Sint32 Angle[], CColor Color[], Sint32 Count, Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param List		[in] リスト情報の配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawList( SSpriteListData2D List[], Sint32 Count, Math::Rect2DF &Src ) = 0;

		/**
			@brief 放射状フェード
			@author 葉迩倭

			@param SrcRect		[in] 転送元テクスチャの矩形
			@param Divide		[in] 円周のポリゴンの分割数
			@param Side			[in] １辺あたりのポリゴンの分割数
			@param Alpha		[in] 透明度

			中心に向かってフェードするエフェクトです。<BR>
			画面の切り替え時などに利用できると思います。
		*/
		virtual void CircleFade( Math::Rect2DF &SrcRect, Sint32 Divide, Sint32 Side, Sint32 Alpha ) = 0;

		/**
			@brief 円状回転フェード
			@author 葉迩倭

			@param SrcRect		[in] 転送元テクスチャの矩形
			@param Divide		[in] 円周のポリゴンの分割数
			@param Side			[in] １辺あたりのポリゴンの分割数
			@param Alpha		[in] 透明度

			時計回りにフェードするエフェクトです。<BR>
			画面の切り替え時などに利用できると思います。
		*/
		virtual void CircleRoundFade( Math::Rect2DF &SrcRect, Sint32 Divide, Sint32 Side, Sint32 Alpha ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief フォントスプライト管理インターフェイス
		@author 葉迩倭

		2Dフォント描画用のインターフェイスです。
	*/
	class IFontSprite2D
	{
	public:
		virtual ~IFontSprite2D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭
			@param Space	[in] 行間補正値

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( Sint32 Space = 0 ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief レンダリング
			@author 葉迩倭

			内部バッファの内容のレンダリングを行います。
		*/
		virtual void Rendering( void ) = 0;

		/**
			@brief 装飾用テクスチャの設定
			@author 葉迩倭

			@param pTex		[in] テクスチャインターフェイス

			装飾用のテクスチャを設定します。
		*/
		virtual void SetDecoration( ITexture *pTex ) = 0;

		/**
			@brief サイズ取得
			@author 葉迩倭

			文字サイズを取得
		*/
		virtual Sint32 GetSize( void ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Color	[in] 描画色
			@param Length	[in] 描画文字数(-1で全て)

			等幅フォントで文字列の描画を行います。<BR>
			生成時の書体が等幅フォントでない場合は正しく描画されない事があります。
		*/
		virtual void DrawString( const char *pStr, Math::Point2DF Pos, CColor Color, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Color	[in] 描画色
			@param Length	[in] 描画文字数(-1で全て)

			プロポーショナルフォントで文字列の描画を行います。<BR>
			生成時の書体がプロポーショナルフォントでなくても正しく描画されます。
		*/
		virtual void DrawStringP( const char *pStr, Math::Point2DF Pos, CColor Color, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字描画
			@author 葉迩倭
			@param pChara	[in] 描画文字
			@param Dst		[in] 描画矩形
			@param Color	[in] 描画色

			指定位置に文字を描画します。<BR>
			この関数では文字の拡大縮小をサポートします。
		*/
		virtual Sint32 DrawChara( const char *pChara, Math::Rect2DF &Dst, CColor Color ) = 0;

		/**
			@brief 回転付き文字描画
			@author 葉迩倭
			@param pChara	[in] 描画文字
			@param Dst		[in] 描画矩形
			@param Color	[in] 描画色
			@param Angle	[in] 1周を65536とした回転角度

			指定位置に文字を回転付きで描画します。<BR>
			この関数では文字の拡大縮小をサポートします。
		*/
		virtual Sint32 DrawCharaRotate( const char *pChara, Math::Rect2DF &Dst, CColor Color, Sint32 Angle ) = 0;

		/**
			@brief 文字列描画完了位置取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Length	[in] 描画文字数(-1で全て)
			@return		描画完了時の座標

			等幅フォントで文字列の描画を行った場合の最終的な描画終了位置を取得します。<BR>
			改行が含まれる場合は改行を考慮した最終位置を返すので、<BR>
			文字列の最大幅を取得したい場合は注意してください。
		*/
		virtual Math::Point2DF GetStringLastPos( const char *pStr, Math::Point2DF Pos, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字列描画完了位置取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Length	[in] 描画文字数(-1で全て)
			@return		描画完了時の座標

			プロポーショナルフォントで文字列の描画を行った場合の最終的な描画終了位置を取得します。<BR>
			改行が含まれる場合は改行を考慮した最終位置を返すので、<BR>
			文字列の最大幅を取得したい場合は注意してください。
		*/
		virtual Math::Point2DF GetStringLastPosP( const char *pStr, Math::Point2DF Pos, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字列描画サイズ取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@return		描画サイズ

			等幅フォントで文字列の描画を行った場合の縦横のサイズを取得します。<BR>
			改行が含まれる場合でも最大の横幅を取得できます。
		*/
		virtual Math::Point2DF GetStringSize( const char *pStr ) = 0;

		/**
			@brief 文字列描画サイズ取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@return		描画サイズ

			プロポーショナルフォントで文字列の描画を行った場合の縦横のサイズを取得します。<BR>
			改行が含まれる場合でも最大の横幅を取得できます。
		*/
		virtual Math::Point2DF GetStringSizeP( const char *pStr ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Color	[in] 描画色
			@param Length	[in] 描画文字数(-1で全て)

			等幅フォントで文字列の描画を行います。<BR>
			生成時の書体が等幅フォントでない場合は正しく描画されない事があります。
		*/
		virtual void DrawString( const wchar_t *pStr, Math::Point2DF Pos, CColor Color, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Color	[in] 描画色
			@param Length	[in] 描画文字数(-1で全て)

			プロポーショナルフォントで文字列の描画を行います。<BR>
			生成時の書体がプロポーショナルフォントでなくても正しく描画されます。
		*/
		virtual void DrawStringP( const wchar_t *pStr, Math::Point2DF Pos, CColor Color, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字描画
			@author 葉迩倭
			@param pChara	[in] 描画文字
			@param Dst		[in] 描画矩形
			@param Color	[in] 描画色

			指定位置に文字を描画します。<BR>
			この関数では文字の拡大縮小をサポートします。
		*/
		virtual Sint32 DrawChara( const wchar_t *pChara, Math::Rect2DF &Dst, CColor Color ) = 0;

		/**
			@brief 回転付き文字描画
			@author 葉迩倭
			@param pChara	[in] 描画文字
			@param Dst		[in] 描画矩形
			@param Color	[in] 描画色
			@param Angle	[in] 1周を65536とした回転角度

			指定位置に文字を回転付きで描画します。<BR>
			この関数では文字の拡大縮小をサポートします。
		*/
		virtual Sint32 DrawCharaRotate( const wchar_t *pChara, Math::Rect2DF &Dst, CColor Color, Sint32 Angle ) = 0;

		/**
			@brief 文字列描画完了位置取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Length	[in] 描画文字数(-1で全て)
			@return		描画完了時の座標

			等幅フォントで文字列の描画を行った場合の最終的な描画終了位置を取得します。<BR>
			改行が含まれる場合は改行を考慮した最終位置を返すので、<BR>
			文字列の最大幅を取得したい場合は注意してください。
		*/
		virtual Math::Point2DF GetStringLastPos( const wchar_t *pStr, Math::Point2DF Pos, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字列描画完了位置取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param Pos		[in] 描画座標
			@param Length	[in] 描画文字数(-1で全て)
			@return		描画完了時の座標

			プロポーショナルフォントで文字列の描画を行った場合の最終的な描画終了位置を取得します。<BR>
			改行が含まれる場合は改行を考慮した最終位置を返すので、<BR>
			文字列の最大幅を取得したい場合は注意してください。
		*/
		virtual Math::Point2DF GetStringLastPosP( const wchar_t *pStr, Math::Point2DF Pos, Sint32 Length = -1 ) = 0;

		/**
			@brief 文字列描画サイズ取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@return		描画サイズ

			等幅フォントで文字列の描画を行った場合の縦横のサイズを取得します。<BR>
			改行が含まれる場合でも最大の横幅を取得できます。
		*/
		virtual Math::Point2DF GetStringSize( const wchar_t *pStr ) = 0;

		/**
			@brief 文字列描画サイズ取得
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@return		描画サイズ

			プロポーショナルフォントで文字列の描画を行った場合の縦横のサイズを取得します。<BR>
			改行が含まれる場合でも最大の横幅を取得できます。
		*/
		virtual Math::Point2DF GetStringSizeP( const wchar_t *pStr ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 3Dポイント描画用インターフェイス
		@author 葉迩倭

		3Dポイント描画を保持するためのインターフェイスです。
	*/
	class IPoint3D
	{
	public:
		virtual ~IPoint3D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pPoint		[in] ポイントデータ
			@param PointCount	[in] ポイント数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			生成時に指定した頂点フォーマットに適合した頂点データを設定してください。
		*/
		virtual Bool Push( const SVertex3DBase *pPoint, Sint32 PointCount ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 3Dライン描画用インターフェイス
		@author 葉迩倭

		3Dライン描画を保持するためのインターフェイスです。
	*/
	class ILine3D
	{
	public:
		virtual ~ILine3D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pLine		[in] ラインデータ
			@param LineCount	[in] ライン数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			生成時に指定した頂点フォーマットに適合した頂点データを設定してください。
		*/
		virtual Bool Push( SLineVertex3D *pLine, Sint32 LineCount ) = 0;

		/**
			@brief バウンディングボックス描画リクエスト
			@author 葉迩倭
			@param Box		[in] コリジョン用のボックス
			@param Color	[in] 色

			コリジョン用のボックスデータの描画を行います。
		*/
		virtual void PushBox( Collision::CBox &Box, CColor Color ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 3Dプリミティブ描画用インターフェイス
		@author 葉迩倭

		3Dプリミティブ描画を保持するためのインターフェイスです。
	*/
	class IPrimitive3D
	{
	public:
		virtual ~IPrimitive3D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pVtx		[in] 頂点データ
			@param Count	[in] 頂点数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			適合した頂点データを設定してください。
		*/
		virtual Bool Push( const SVertex3DBase *pVtx, Sint32 Count ) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pVtx		[in] 頂点データ
			@param Count	[in] 頂点数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			適合した頂点データを設定してください。
		*/
		virtual Bool Push( const SVertex3DTexture *pVtx, Sint32 Count ) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pVtx		[in] 頂点データ
			@param Count	[in] 頂点数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			適合した頂点データを設定してください。
		*/
		virtual Bool Push( const SVertex3DLight *pVtx, Sint32 Count ) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pVtx		[in] 頂点データ
			@param Count	[in] 頂点数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			適合した頂点データを設定してください。
		*/
		virtual Bool Push( const SVertex3DBump *pVtx, Sint32 Count ) = 0;

		/**
			@brief 頂点データを追加
			@author 葉迩倭
			@param pVtx		[in] 頂点データ
			@param Count	[in] 頂点数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへ頂点データを追加します。<BR>
			適合した頂点データを設定してください。
		*/
		virtual Bool Push( const SVertex3DAnimation *pVtx, Sint32 Count ) = 0;

		/**
			@brief インデックスデータを追加
			@author 葉迩倭
			@param pIndex		[in] インデックスデータ
			@param IndexCount	[in] インデックス数
			@retval false	追加できなかった（バッファーオーバーフロー）
			@retval true	追加できた

			内部バッファへインデックスデータを追加します。<BR>
			生成時に指定したインデックスフォーマットに適合したインデックスデータを設定してください。
		*/
		virtual Bool Push( const void *pIndex, Sint32 IndexCount ) = 0;

		/**
			@brief 頂点数取得
			@author 葉迩倭
			@return 追加要求をした頂点の数

			追加要求を行ったSVertex3DBase頂点の数を取得します。<BR>
			この値は実際に追加された数ではなく、Pushを行って追加された数を<BR>
			取得するので、実際に持てる最大数以上の数を返す場合があります。
		*/
		virtual Sint32 GetVertexCount( void ) = 0;

		/**
			@brief インデックス数取得
			@author 葉迩倭
			@return 追加要求をしたインデックスの数

			追加要求を行ったインデックスの数を取得します。<BR>
			この値は実際に追加された数ではなく、Pushを行って追加された数を<BR>
			取得するので、実際に持てる最大数以上の数を返す場合があります。
		*/
		virtual Sint32 GetIndexCount( void ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭
			@param IsVertex		[in] 頂点書き込み行う
			@param IsIndex		[in] インデックス書き込み行う
			@param IsDirect		[in] バッファ直接書き込み

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( Bool IsVertex = true, Bool IsIndex = true, Bool IsDirect = true ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief DrawList用構造体
		@author 葉迩倭

		@note
		DrawListで利用する構造体です。
	*/
	struct SSpriteListData3D
	{
		Math::Vector3D Pos;
		Float Width;
		Sint32 Angle;
		CColor Color;
	};

	/**
		@brief 3Dスプライト描画用インターフェイス
		@author 葉迩倭

		3Dスプライト描画を保持するためのインターフェイスです。
	*/
	class ISprite3D
	{
	public:
		virtual ~ISprite3D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭
			@param mWorld		[in] ワールドの変換行列
			@param Size			[in] 描画サイズ
			@param SrcRect		[in] テクスチャ矩形
			@param Color		[in] 色

			内部バッファへのデータの追加を行います。<BR>
			Sizeで指定した大きさの四角形ポリゴンをmWorldで変換します。
		*/
		virtual void Draw( const Math::Matrix &mWorld, const Math::Point2DF &Size, const Math::Rect2DI &SrcRect, CColor Color ) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭
			@param mWorld		[in] ワールドの変換行列
			@param PtTbl		[in] 描画用４頂点（左上－右上－左下－右下）
			@param SrcRect		[in] テクスチャ矩形
			@param Color		[in] 色

			内部バッファへのデータの追加を行います。<BR>
			vPositionを基準位置としてPtTblで指定した大きさの矩形を作りmWorldで変換します。
		*/
		virtual void Draw( const Math::Matrix &mWorld, const Math::Vector2D PtTbl[], const Math::Rect2DI &SrcRect, CColor Color ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param vCameraPosition	[in] カメラの位置
			@param Pos				[in] 描画位置の配列
			@param Color			[in] 描画色の配列
			@param Count			[in] 各要素の配列数
			@param Width			[in] 頂点ごとの幅
			@param Src				[in] 転送元テクスチャの矩形

			カメラの位置に応じて自動回転処理が行われた<BR>
			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawPolyLine( const Math::Vector3D &vCameraPosition, const Math::Vector3D Pos[], const CColor Color[], Sint32 Count, Float Width, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param vCameraPosition	[in] カメラの位置
			@param Pos				[in] 描画位置の配列
			@param Color			[in] 描画色の配列
			@param Width			[in] 頂点ごとの幅
			@param Count			[in] 各要素の配列数
			@param Src				[in] 転送元テクスチャの矩形

			カメラの位置に応じて自動回転処理が行われた<BR>
			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawPolyLine( const Math::Vector3D &vCameraPosition, const Math::Vector3D Pos[], const CColor Color[], const Float Width[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param vCameraPosition	[in] カメラの位置
			@param List				[in] 頂点ごとの配列
			@param Count			[in] 各要素の配列数
			@param Src				[in] 転送元テクスチャの矩形

			カメラの位置に応じて自動回転処理が行われた<BR>
			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawPolyLine( const Math::Vector3D &vCameraPosition, const SSpriteListData3D List[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Width	[in] 頂点ごとの幅
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListXY( const Math::Vector3D Pos[], const Sint32 Angle[], const CColor Color[], Sint32 Count, Float Width, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Width	[in] 頂点ごとの幅
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListYZ( const Math::Vector3D Pos[], const Sint32 Angle[], const CColor Color[], Sint32 Count, Float Width, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Width	[in] 頂点ごとの幅
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListZX( const Math::Vector3D Pos[], const Sint32 Angle[], const CColor Color[], Sint32 Count, Float Width, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Width	[in] 頂点ごとの幅の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListXY( const Math::Vector3D Pos[], const Float Width[], const Sint32 Angle[], const CColor Color[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Width	[in] 頂点ごとの幅の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListYZ( const Math::Vector3D Pos[], const Float Width[], const Sint32 Angle[], const CColor Color[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param Pos		[in] 描画位置の配列
			@param Width	[in] 頂点ごとの幅の配列
			@param Angle	[in] 1周65536とした回転角度の配列
			@param Color	[in] 描画色の配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListZX( const Math::Vector3D Pos[], const Float Width[], const Sint32 Angle[], const CColor Color[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param List		[in] 頂点ごとの配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListXY( const SSpriteListData3D List[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param List		[in] 頂点ごとの配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListYZ( const SSpriteListData3D List[], Sint32 Count, const Math::Rect2DF &Src ) = 0;

		/**
			@brief 帯状四角形描画
			@author 葉迩倭

			@param List		[in] 頂点ごとの配列
			@param Count	[in] 各要素の配列数
			@param Src		[in] 転送元テクスチャの矩形

			帯状に連結したスプライトを描画することが出来ます。<BR>
			この関数はデータを描画バッファに追加するだけで<BR>
			実際のレンダリング処理はRendering関数呼び出し時に開始されます。
		*/
		virtual void DrawListZX( const SSpriteListData3D List[], Sint32 Count, const Math::Rect2DF &Src ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 3D文字描画用インターフェイス
		@author 葉迩倭

		3D文字描画を保持するためのインターフェイスです。
	*/
	class IFontSprite3D
	{
	public:
		virtual ~IFontSprite3D() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param vPos		[in] 描画座標
			@param fSize	[in] 描画サイズ
			@param Color	[in] 描画色

			等幅フォントで文字列の描画を行います。<BR>
			生成時の書体が等幅フォントでない場合は正しく描画されない事があります。
		*/
		virtual void DrawString( const char *pStr, const Math::Vector3D &vPos, const Math::Point2DF &fSize, CColor Color ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param vPos		[in] 描画座標
			@param fSize	[in] 描画サイズ
			@param Color	[in] 描画色

			プロポーショナルフォントで文字列の描画を行います。<BR>
			生成時の書体がプロポーショナルフォントでなくても正しく描画されます。
		*/
		virtual void DrawStringP( const char *pStr, const Math::Vector3D &vPos, const Math::Point2DF &fSize, CColor Color ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param vPos		[in] 描画座標
			@param fSize	[in] 描画サイズ
			@param Color	[in] 描画色

			等幅フォントで文字列の描画を行います。<BR>
			生成時の書体が等幅フォントでない場合は正しく描画されない事があります。
		*/
		virtual void DrawString( const wchar_t *pStr, const Math::Vector3D &vPos, const Math::Point2DF &fSize, CColor Color ) = 0;

		/**
			@brief 文字列描画
			@author 葉迩倭
			@param pStr		[in] 描画文字列
			@param vPos		[in] 描画座標
			@param fSize	[in] 描画サイズ
			@param Color	[in] 描画色

			プロポーショナルフォントで文字列の描画を行います。<BR>
			生成時の書体がプロポーショナルフォントでなくても正しく描画されます。
		*/
		virtual void DrawStringP( const wchar_t *pStr, const Math::Vector3D &vPos, const Math::Point2DF &fSize, CColor Color ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief 3Dスプライト描画用インターフェイス
		@author 葉迩倭

		3Dスプライト描画を保持するためのインターフェイスです。
	*/
	class IParticle
	{
	public:
		virtual ~IParticle() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 基準位置設定
			@author 葉迩倭
			@param vPos		[in] 基準位置

			内部バッファに対する全ての基準となる位置を設定します。<BR>
			ここで指定された位置はソートにも利用されます。
		*/
		virtual void SetBasePosition( const Math::Vector3D &vPos ) = 0;

		/**
			@brief データ追加開始宣言
			@author 葉迩倭

			内部バッファへのデータの追加を行うことを通知します。<BR>
			この関数を呼ばずにPush*系の関数を呼ばないようにしてください。
		*/
		virtual void Begin( void ) = 0;

		/**
			@brief データ追加終了宣言
			@author 葉迩倭

			内部バッファへのデータの追加を完了したことを通知します。<BR>
			この関数を呼ぶ前に必ずBegin関数を呼ぶようにしてください。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭
			@param vPosition	[in] スプライトの位置
			@param Size			[in] 描画サイズ
			@param SrcRect		[in] テクスチャ矩形
			@param Color		[in] 色

			内部バッファへのデータの追加を行います。<BR>
			vPositionを基準位置としてSizeで指定した大きさの矩形を作り、
			カメラの方へ向くように頂点シェーダーで計算されます。
		*/
		virtual void Draw( const Math::Vector3D &vPosition, const Math::Point2DF &Size, const Math::Rect2DI &SrcRect, CColor Color ) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭
			@param vPosition	[in] スプライトの位置
			@param PtTbl		[in] 描画用４頂点（左上－右上－左下－右下）
			@param SrcRect		[in] テクスチャ矩形
			@param Color		[in] 色

			内部バッファへのデータの追加を行います。<BR>
			vPositionを基準位置としてPtTblで指定した大きさの矩形を作り、
			カメラの方へ向くように頂点シェーダーで計算されます。
		*/
		virtual void Draw( const Math::Vector3D &vPosition, const Math::Vector2D PtTbl[], const Math::Rect2DI &SrcRect, CColor Color ) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭
			@param vPosition	[in] スプライトの位置
			@param Size			[in] 描画サイズ
			@param SrcRect		[in] テクスチャ矩形
			@param Color		[in] 色
			@param Angle		[in] 回転角度

			内部バッファへのデータの追加を行います。<BR>
			vPositionを基準位置としてSizeで指定した大きさの矩形を作り、
			カメラの方へ向くように頂点シェーダーで計算されます。
		*/
		virtual void DrawRotate( const Math::Vector3D &vPosition, const Math::Point2DF &Size, const Math::Rect2DI &SrcRect, CColor Color, Sint32 Angle ) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭
			@param vPosition	[in] スプライトの位置
			@param PtTbl		[in] 描画用４頂点（左上－右上－左下－右下）
			@param SrcRect		[in] テクスチャ矩形
			@param Color		[in] 色
			@param Angle		[in] 回転角度

			内部バッファへのデータの追加を行います。<BR>
			vPositionを基準位置としてPtTblで指定した大きさの矩形を作り、
			カメラの方へ向くように頂点シェーダーで計算されます。
		*/
		virtual void DrawRotate( const Math::Vector3D &vPosition, const Math::Vector2D PtTbl[], const Math::Rect2DI &SrcRect, CColor Color, Sint32 Angle ) = 0;
	};
}
}
}
#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief モデルデータ用インターフェイス
		@author 葉迩倭

		@note
		モデルデータを保持するためのインターフェイスです。
	*/
	class IModel : public IInterface
	{
	public:
		virtual ~IModel() {}
	};
}
}
}

#pragma once


/**
	@file
	@brief マップモデルインターフェイス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
namespace Object
{
	/**
		@brief マップモデルデータ用インターフェイス
		@author 葉迩倭

		@note
		マップモデルデータを保持するためのインターフェイスです。
	*/
	class IMapModel : public IInterface
	{
	public:
		virtual ~IMapModel() {}
	};
}
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーンプリミティブアクター用インターフェイス
		@author 葉迩倭

		シーン管理されたプリミティブアクターを扱う為のインターフェイスです。
	*/
	class IPrimitiveActor : public Math::Style
	{
	public:
		virtual ~IPrimitiveActor() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭

			ISceneManagerに対してレンダリングリクエストを発行します。<BR>
			取得元のISceneManagerがBegin()/End()中でない場合この関数は失敗します。
		*/
		virtual void RenderingRequest( void ) = 0;

		/**
			@brief 座標変換初期化
			@author 葉迩倭

			アクターの座標変換を初期化します。<BR>
			アニメーションの情報などもこの関数内ですべて初期化されます。
		*/
		virtual void TransformReset( void )	 = 0;

		/**
			@brief 座標変換更新
			@author 葉迩倭

			アクターの座標変換を更新します。<BR>
			アニメーションの情報などもこの関数内ですべて更新されます。
		*/
		virtual void TransformUpdate( void ) = 0;

		/**
			@brief ボーン変換行列を設定
			@author 葉迩倭
			@param pMatrix	[in] ボーン行列の配列
			@param Count	[in] ボーン行列の数

			アクターに対してボーンの変換行列を設定します。<BR>
			ボーンデータをもつIPrimitive3Dに対してのみ有効です。
		*/
		virtual void SetBoneMatrixArray( Math::Matrix *pMatrix, Sint32 Count ) = 0;

		/**
			@brief シェーダーを設定
			@author 葉迩倭
			@param pShader	[in] シェーダーインターフェイス

			アクターに対して任意のシェーダーを設定します。
		*/
		virtual void SetShader( Renderer::Shader::IShader *pShader ) = 0;

		/**
			@brief マテリアルの拡散反射色設定
			@author 葉迩倭
			@param vColor			[in] 色

			メッシュ内のマテリアルの拡散反射色を設定します。
		*/
		virtual void SetMaterialColor( Math::Vector4D &vColor ) = 0;

		/**
			@brief マテリアルの自己発光色設定
			@author 葉迩倭
			@param vColor			[in] 色

			メッシュ内のマテリアルの自己発光色を設定します。
		*/
		virtual void SetEmissiveColor( Math::Vector4D &vColor )										= 0;

		/**
			@brief マテリアルの鏡面反射色設定
			@author 葉迩倭
			@param vColor			[in] 色

			メッシュ内のマテリアルの鏡面反射色を設定します。
		*/
		virtual void SetSpecularColor( Math::Vector4D &vColor )										= 0;

		/**
			@brief マテリアルの鏡面反射の強さ設定
			@author 葉迩倭
			@param fParam			[in] 強さ

			メッシュ内のマテリアルの鏡面反射の強さを設定します。
		*/
		virtual void SetSpecularRefractive( Float fParam )									= 0;

		/**
			@brief マテリアルの鏡面反射の荒さ設定
			@author 葉迩倭
			@param fParam			[in] 荒さ

			メッシュ内のマテリアルの鏡面反射の荒さを設定します。
		*/
		virtual void SetSpecularRoughly( Float fParam )									= 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーンプリミティブアクター用インターフェイス
		@author 葉迩倭

		シーン管理されたプリミティブアクターを扱う為のインターフェイスです。
	*/
	class ISpriteActor : public Math::Style
	{
	public:
		virtual ~ISpriteActor() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭

			ISceneManagerに対してレンダリングリクエストを発行します。<BR>
			取得元のISceneManagerがBegin()/End()中でない場合この関数は失敗します。
		*/
		virtual void RenderingRequest( void ) = 0;

		/**
			@brief 座標変換初期化
			@author 葉迩倭

			アクターの座標変換を初期化します。<BR>
			アニメーションの情報などもこの関数内ですべて初期化されます。
		*/
		virtual void TransformReset( void )	 = 0;

		/**
			@brief 座標変換更新
			@author 葉迩倭

			アクターの座標変換を更新します。<BR>
			アニメーションの情報などもこの関数内ですべて更新されます。
		*/
		virtual void TransformUpdate( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーンスプライトアクター用インターフェイス
		@author 葉迩倭

		シーン管理されたスプライトアクターを扱う為のインターフェイスです。
	*/
	class IParticleActor
	{
	public:
		virtual ~IParticleActor() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭

			ISceneManagerに対してレンダリングリクエストを発行します。<BR>
			取得元のISceneManagerがBegin()/End()中でない場合この関数は失敗します。
		*/
		virtual void RenderingRequest( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーンモデルアクター用インターフェイス
		@author 葉迩倭

		シーン管理されたモデルアクターを扱う為のインターフェイスです。
	*/
	class IModelActor : public Math::Style
	{
	public:
		virtual ~IModelActor() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭

			ISceneManagerに対してレンダリングリクエストを発行します。<BR>
			取得元のISceneManagerがBegin()/End()中でない場合この関数は失敗します。
		*/
		virtual void RenderingRequest( void ) = 0;

		/**
			@brief カリング用バウンディングレンダリング
			@author 葉迩倭
			@param pLine	[in] ラインプリミティブインターフェイス

			バウンディングボックスを指定したラインデータに対して追加します。
		*/
		virtual void RenderingBounding( Renderer::Object::ILine3D *pLine ) = 0;

		/**
			@brief 座標変換初期化
			@author 葉迩倭

			アクターの座標変換を初期化します。<BR>
			アニメーションの情報などもこの関数内ですべて初期化されます。
		*/
		virtual void TransformReset( void ) = 0;

		/**
			@brief 座標変換更新
			@author 葉迩倭

			アクターの座標変換を更新します。<BR>
			アニメーションの情報などもこの関数内ですべて更新されます。<BR>
			処理的には　TransformUpdateCollisionOnly() と TransformUpdateActorOnly() を呼び出しています。
		*/
		virtual void TransformUpdate( void ) = 0;

		/**
			@brief 座標変換更新
			@author 葉迩倭

			アクターのコリジョンの座標変換を更新します。<BR>
			コリジョンに関係するデータがここで更新されます。
		*/
		virtual void TransformUpdateCollisionOnly( void ) = 0;

		/**
			@brief 座標変換更新
			@author 葉迩倭

			アクターの座標変換を更新します。<BR>
			描画用のアクターだけが更新されます。
		*/
		virtual void TransformUpdateActorOnly( void ) = 0;

		/**
			@brief モデルに対してアニメーションデータ数を取得
			@author 葉迩倭
			@return	アニメーション数

			アクターの所持するアニメーションの数を取得します。
		*/
		virtual Sint32 GetAnimationCount( void ) = 0;

		/**
			@brief アニメーションデータの最終時間を取得
			@author 葉迩倭
			@param AnimationNo		[in] 適用するアニメーション番号
			@return	アニメーションの最終時間

			アニメーションの最終フレームを取得します。
		*/
		virtual Float GetAnimationLastTime( Sint32 AnimationNo ) = 0;

		/**
			@brief モデルに対してアニメーションデータを更新
			@author 葉迩倭
			@param AnimationNo		[in] 適用するアニメーション番号
			@param fAnimationTime	[in] アニメーションの時間（単位は作成したツールによる）

			単一のアニメーションデータの更新をします。
		*/
		virtual void UpdateAnimation( Sint32 AnimationNo, Float fAnimationTime ) = 0;

		/**
			@brief モデルに対してアニメーションデータを更新
			@author 葉迩倭
			@param AnimationNo1		[in] 適用するアニメーション番号
			@param fAnimationTime1	[in] アニメーションの時間（単位は作成したツールによる）
			@param AnimationNo2		[in] 適用するアニメーション番号
			@param fAnimationTime2	[in] アニメーションの時間（単位は作成したツールによる）
			@param fWeight			[in] ブレンド割合（0.0fの時AnimationNo0、1.0の時AnimationNo1)

			２つのアニメーションをブレンドしてアニメーションデータを更新します。
		*/
		virtual void UpdateAnimation( Sint32 AnimationNo1, Float fAnimationTime1, Sint32 AnimationNo2, Float fAnimationTime2, Float fWeight ) = 0;

		/**
			@brief カリング処理に有無設定
			@author 葉迩倭
			@param IsEnable	[in] カリングの有無

			モデル内の各アクターに対してビューフラスタムカリングを行うかを設定します。<BR>
			ビューフラスタムカリングとはカメラの描画領域内にないアクターを描画に関する一連の処理から<BR>
			省くための処理で、行う事でCPU負荷は増えますが画面外に出るアクターに対しての描画負荷を軽減できます。
		*/
		virtual void SetCullTestEnable( Bool IsEnable ) = 0;

		/**
			@brief モデル内のフレーム数取得
			@author 葉迩倭
			@return	モデル内のフレームの数

			モデル内のフレームの数を取得します。<BR>
			スキンメッシュに関連付けられているフレームは<BR>
			扱い的にはボーンになります。
		*/
		virtual Sint32 GetFrameCount( void ) = 0;

		/**
			@brief モデル内のフレーム数取得
			@author 葉迩倭
			@return	モデル内のフレームの数

			モデル内のフレームの数を取得します。
		*/
		virtual Sint32 Frame_GetIndex( const char *pName ) = 0;

		/**
			@brief フレームの変換行列を取得
			@author 葉迩倭
			@param Index	[in] フレームの番号
			@return	フレームの変換行列

			フレームの変換行列を取得します。
		*/
		virtual Math::Matrix &Frame_GetTransformLocal( Sint32 Index ) = 0;

		/**
			@brief フレームの変換行列を取得
			@author 葉迩倭
			@param Index	[in] フレームの番号
			@return	フレームの変換行列

			フレームの変換行列を取得します。
		*/
		virtual Math::Matrix &Frame_GetTransformWorld( Sint32 Index ) = 0;

		/**
			@brief モデル内のメッシュ数取得
			@author 葉迩倭
			@return	モデル内のメッシュの数

			モデル内のメッシュの数を取得します。<BR>
			メッシュの数＝描画アクターの数になります。
		*/
		virtual Sint32 GetMeshCount( void ) = 0;

		/**
			@brief モデル内のメッシュ描画ON/OFF
			@author 葉迩倭
			@param Index		[in] メッシュ番号
			@param IsDrawEnable	[in] 描画するか否か

			モデル内のメッシュを描画するかどうかを設定します。
		*/
		virtual void GetMesh_DrawEnable( Sint32 Index, Bool IsDrawEnable ) = 0;

		/**
			@brief メッシュ内のマテリアル数取得
			@author 葉迩倭
			@param MeshIndex	[in] メッシュ番号
			@return	メッシュ内のマテリアル数

			メッシュ内のマテリアル数を取得します。
		*/
		virtual Sint32 GetMeshMaterialCount( Sint32 MeshIndex ) = 0;

		/**
			@brief マテリアルのシェーダー設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param pShader			[in] シェーダー

			メッシュ内のマテリアルのシェーダーを設定します。
		*/
		virtual void MeshMaterial_SetShader( Sint32 MeshIndex, Sint32 MaterialIndex, Renderer::Shader::IShader *pShader ) = 0;

		/**
			@brief マテリアルの拡散反射色設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param vColor			[in] 色

			メッシュ内のマテリアルの拡散反射色を設定します。
		*/
		virtual void MeshMaterial_SetMaterialColor( Sint32 MeshIndex, Sint32 MaterialIndex, Math::Vector4D &vColor ) = 0;

		/**
			@brief マテリアルの自己発光色設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param vColor			[in] 色

			メッシュ内のマテリアルの自己発光色を設定します。
		*/
		virtual void MeshMaterial_SetEmissiveColor( Sint32 MeshIndex, Sint32 MaterialIndex, Math::Vector4D &vColor )										= 0;

		/**
			@brief マテリアルの鏡面反射色設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param vColor			[in] 色

			メッシュ内のマテリアルの鏡面反射色を設定します。
		*/
		virtual void MeshMaterial_SetSpecularColor( Sint32 MeshIndex, Sint32 MaterialIndex, Math::Vector4D &vColor )										= 0;

		/**
			@brief マテリアルの鏡面反射の強さ設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param fParam			[in] 強さ

			メッシュ内のマテリアルの鏡面反射の強さを設定します。
		*/
		virtual void MeshMaterial_SetSpecularRefractive( Sint32 MeshIndex, Sint32 MaterialIndex, Float fParam )									= 0;

		/**
			@brief マテリアルの鏡面反射の荒さ設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param fParam			[in] 荒さ

			メッシュ内のマテリアルの鏡面反射の荒さを設定します。
		*/
		virtual void MeshMaterial_SetSpecularRoughly( Sint32 MeshIndex, Sint32 MaterialIndex, Float fParam )									= 0;

		/**
			@brief マテリアルの描画タイプ設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param Type				[in] 描画タイプ

			メッシュ内のマテリアルの描画タイプを設定します。
		*/
		virtual void MeshMaterial_SetDrawType( Sint32 MeshIndex, Sint32 MaterialIndex, eDrawType Type )											= 0;

		/**
			@brief マテリアルのαテスト閾値設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param Boundary			[in] αテスト閾値

			メッシュ内のマテリアルのαテスト閾値を設定します。
		*/
		virtual void MeshMaterial_SetAlphaBoundary( Sint32 MeshIndex, Sint32 MaterialIndex, Sint32 Boundary )									= 0;

		/**
			@brief マテリアルのテクスチャ設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param pTexture			[in] テクスチャ

			メッシュ内のマテリアルのテクスチャを設定します。
		*/
		virtual void MeshMaterial_SetTexture( Sint32 MeshIndex, Sint32 MaterialIndex, Renderer::ITexture *pTexture )										= 0;

		/**
			@brief マテリアルのテクスチャUVのオフセット設定
			@author 葉迩倭
			@param MeshIndex		[in] メッシュ番号
			@param MaterialIndex	[in] マテリアル番号
			@param vOffset			[in] オフセット（0.0->1.0)

			メッシュ内のマテリアルのテクスチャUVのオフセットを設定します。
		*/
		virtual void MeshMaterial_SetTextureOffset( Sint32 MeshIndex, Sint32 MaterialIndex, Math::Vector2D &vOffset )									= 0;

		/**
			@brief コリジョン生成
			@author 葉迩倭

			アクターに対してのコリジョン操作用のインターフェイスを生成します。<BR>
			これを行うとCollision_***()系の関数が有効になります。
		*/
		virtual void Collision_Create( void ) = 0;

		/**
			@brief コリジョン有無設定
			@author 葉迩倭
			@param Index	[in] フレーム番号
			@param IsEnable	[in] trueの時コリジョンON（デフォルトはON）

			指定番号のフレームのコリジョンをON/OFFします。
		*/
		virtual void Collision_SetEnable( Sint32 Index, Bool IsEnable ) = 0;

		/**
			@brief コリジョン有無設定
			@author 葉迩倭
			@param pName	[in] 名称
			@param IsEnable	[in] trueの時コリジョンON（デフォルトはON）

			指定名のフレームのコリジョンをON/OFFします。
		*/
		virtual void Collision_SetEnable( const char *pName, Bool IsEnable ) = 0;

		/**
			@brief コリジョンデータ描画
			@author 葉迩倭
			@param pLine	[in] 描画用ラインプリミティブインターフェイス

			当たり判定データをレンダリングするための関数です。
		*/
		virtual void Collision_Rendering( Renderer::Object::ILine3D *pLine ) = 0;

		/**
			@brief コリジョンの結果取得
			@author 葉迩倭
			@param FrameNo	[in,out] 衝突したフレーム番号格納先
			@param vPos		[in,out] 衝突した位置格納先

			当たり判定データを取得します。<BR>
			現時点では衝突した大まかな位置と衝突した<BR>
			フレーム番号しか取得することはできません。
		*/
		virtual void Collision_GetResult( Sint32 &FrameNo, Math::Vector3D &vPos ) = 0;

		/**
			@brief コリジョンチェック
			@author 葉迩倭
			@param pSrc	[in] チェックするIModelActor

			指定されたデータとの衝突検出を行います。<BR>
			衝突の結果はCollision_GetResult()で取得できます。
		*/
		virtual Bool Collision_Check( IModelActor *pSrc ) = 0;

		/**
			@brief コリジョンチェック
			@author 葉迩倭
			@param Src	[in] チェックするボックスデータ

			指定されたデータとの衝突検出を行います。<BR>
			衝突の結果はCollision_GetResult()で取得できます。
		*/
		virtual Bool Collision_Check( Collision::CBox &Src ) = 0;

		/**
			@brief コリジョンチェック
			@author 葉迩倭
			@param Src	[in] チェックするラインデータ

			指定されたデータとの衝突検出を行います。<BR>
			衝突の結果はCollision_GetResult()で取得できます。
		*/
		virtual Bool Collision_Check( Collision::CLine3D &Src ) = 0;

		/**
			@brief コリジョンチェック
			@author 葉迩倭
			@param Src	[in] チェックする点データ

			指定されたデータとの衝突検出を行います。<BR>
			衝突の結果はCollision_GetResult()で取得できます。
		*/
		virtual Bool Collision_Check( Math::Vector3D &Src ) = 0;

		/**
			@brief コリジョンチェック
			@author 葉迩倭
			@param Src			[in] チェックする点データ
			@param pSceneMgr	[in] レンダリングを行ったシーンのインターフェイス

			スクリーン座標との衝突検出を行います。<BR>
			衝突の結果はCollision_GetResult()で取得できます。
		*/
		virtual Bool Collision_CheckOnScreen( Math::Vector2D &Src, ISceneManager *pSceneMgr ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResult &Out ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResult &Out, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResultExtend &Out ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResultExtend &Out, Collision::CBox &Box ) = 0;

		/**
			@brief コリジョンモデル表示
			@author 葉迩倭
			@param pLine		[in] ラインプリミティブインターフェイス

			マップモデルが持つコリジョンデータをRenderer::Object::ILine3Dを使って<BR>
			可視出来るように描画します。
		*/
		virtual void CreateCollisionDrawPrimitive( Renderer::Object::ILine3D *pLine ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーンアクター用インターフェイス
		@author 葉迩倭

		シーン管理されたオブジェクトを扱う為のインターフェイスです。
	*/
	class IMapActor
	{
	public:
		virtual ~IMapActor() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief 描画リクエスト
			@author 葉迩倭

			ISceneManagerに対してレンダリングリクエストを発行します。<BR>
			取得元のISceneManagerがBegin()/End()中でない場合この関数は失敗します。
		*/
		virtual void RenderingRequest( void ) = 0;

		/**
			@brief カリング用バウンディングレンダリング
			@author 葉迩倭
			@param pLine	[in] ラインプリミティブインターフェイス

			バウンディングボックスを指定したラインデータに対して追加します。
		*/
		virtual void RenderingBounding( Renderer::Object::ILine3D *pLine ) = 0;

		/**
			@brief セルの全体数取得
			@author 葉迩倭
			@return セルの全体数

			マップ内のセルの数を取得します。
		*/
		virtual Sint32 GetCellCount( void ) = 0;

		/**
			@brief セルの描画数取得
			@author 葉迩倭
			@return セルの描画数

			マップ内の描画されたセルの数を取得します。
		*/
		virtual Sint32 GetRenderingCellCount( void ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResult &Out ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResult &Out, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResultExtend &Out ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param Out			[out] 衝突結果
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
		virtual Bool HitCheckByRay( const Collision::CLine3D &Ray, Renderer::SCollisionResultExtend &Out, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param fRadius		[in] 衝突半径
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
//		virtual Bool HitCheckBySphere( const Collision::CLine3D &Ray, Float fRadius ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param fRadius		[in] 衝突半径
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
//		virtual Bool HitCheckBySphere( const Collision::CLine3D &Ray, Float fRadius, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param fRadius		[in] 衝突半径
			@param Out			[out] 衝突結果
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
//		virtual Bool HitCheckBySphere( const Collision::CLine3D &Ray, Float fRadius, Renderer::SCollisionResultSphere &Out ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param fRadius		[in] 衝突半径
			@param Out			[out] 衝突結果
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
//		virtual Bool HitCheckBySphere( const Collision::CLine3D &Ray, Float fRadius, Renderer::SCollisionResultSphere &Out, Collision::CBox &Box ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param fRadius		[in] 衝突半径
			@param Out			[out] 衝突結果
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
//		virtual Bool HitCheckBySphere( const Collision::CLine3D &Ray, Float fRadius, Renderer::SCollisionResultSphereExtend &Out ) = 0;

		/**
			@brief 衝突判定
			@author 葉迩倭
			@param Ray			[in] 始点から終点を結ぶ線分
			@param fRadius		[in] 衝突半径
			@param Out			[out] 衝突結果
			@param Box			[out] 衝突したメッシュのOBB
			@retval false	衝突しない
			@retval true	衝突する

			マップモデルが持つコリジョンデータに対して、<BR>
			点がRayで示す移動をした場合のコリジョンを処理します。
		*/
//		virtual Bool HitCheckBySphere( const Collision::CLine3D &Ray, Float fRadius, Renderer::SCollisionResultSphereExtend &Out, Collision::CBox &Box ) = 0;

		/**
			@brief コリジョンモデル表示
			@author 葉迩倭
			@param pLine		[in] ラインプリミティブインターフェイス

			マップモデルが持つコリジョンデータをRenderer::Object::ILine3Dを使って<BR>
			可視出来るように描画します。
		*/
		virtual void CreateCollisionDrawPrimitive( Renderer::Object::ILine3D *pLine ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーンカメラ操作用インターフェイス
		@author 葉迩倭

		シーンのカメラを操作するためのインターフェイスです。
	*/
	class ICamera
	{
	public:
		virtual ~ICamera() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief ワールド→スクリーン座標変換行列取得
			@author 葉迩倭
			@return 変換行列

			ワールド空間からスクリーン座標への逆変換行列を取得します。
		*/
		virtual const Math::Matrix &WorldToScreen( void ) = 0;

		/**
			@brief ワールド→ビュー変換行列取得
			@author 葉迩倭
			@return 変換行列

			ワールド空間からカメラ空間への逆変換行列を取得します。
		*/
		virtual const Math::Matrix &WorldToView( void ) = 0;

		/**
			@brief スクリーン→ワールド変換行列取得
			@author 葉迩倭
			@return 変換行列

			スクリーン座標からワールド空間への逆変換行列を取得します。
		*/
		virtual const Math::Matrix &ScreenToWorld( void ) = 0;

		/**
			@brief ビュー→ワールド変換行列取得
			@author 葉迩倭
			@return 変換行列

			カメラ空間からワールド空間への逆変換行列を取得します。
		*/
		virtual const Math::Matrix &ViewToWorld( void ) = 0;

		/**
			@brief カメラ位置取得
			@author 葉迩倭
			@return カメラ位置ベクトル

			ワールド空間でのカメラの位置を取得します。
		*/
		virtual const Math::Vector3D &Position( void ) = 0;

		/**
			@brief ニアクリップ値取得
			@author 葉迩倭

			@return ニアクリップ値

			ニアクリップ値を取得します。
		*/
		virtual Float GetNearClip( void ) = 0;

		/**
			@brief ファークリップ値取得
			@author 葉迩倭

			@return ファークリップ値

			ファークリップ値を取得します。
		*/
		virtual Float GetFarClip( void ) = 0;

		/**
			@brief プロジェクション行列更新
			@author 葉迩倭

			@param fNearClip	[in] ニアクリップ値
			@param fFarClip		[in] ファークリップ値
			@param FovAngle		[in] 画角（1周65536とした角度）
			@param Width		[in] 表示横幅
			@param Height		[in] 表示縦幅

			プロジェクション行列を更新します。<BR>
			カメラを使用する場合には必ずこの関数でプロジェクションを作成して下さい。<BR>
			またFovAngleに0を指定すると平行投影になります。
		*/
		virtual void UpdateProjection( Float fNearClip, Float fFarClip, Sint32 FovAngle, Sint32 Width, Sint32 Height ) = 0;

		/**
			@brief カメラデータ初期化
			@author 葉迩倭

			カメラデータを初期化します。<BR>
			座標(0,0,0)からZ軸+方向を向いた状態になります。
		*/
		virtual void Reset( void ) = 0;

		/**
			@brief カメラデータ更新
			@author 葉迩倭

			カメラのデータを更新します。<BR>
			各種行列やバウンディングボックスなどは<BR>
			この関数を呼び出すことで作成されます。
		*/
		virtual void Update( void ) = 0;

		/**
			@brief カメラを変換
			@author 葉迩倭

			@param vPosition	[in] カメラの現在位置
			@param vTarget		[in] カメラのターゲット位置
			@param Bank			[in] バンク(傾き)角度

			CStyleを使わず直接カメラの姿勢情報を設定します。
		*/
		virtual void SetTransformSimple( Math::Vector3D &vPosition, Math::Vector3D &vTarget, Sint32 Bank ) = 0;

		/**
			@brief カメラを変換
			@author 葉迩倭

			@param Style	[in] カメラ変換用Style

			CStyleデータで定義された変換処理を<BR>
			カメラに適用します。<BR>
			カメラで使用されるのは移動/回転になります。
		*/
		virtual void SetTransform( Math::Style &Style ) = 0;

		/**
			@brief カリング用バウンディングレンダリング
			@author 葉迩倭
			@param pLine	[in] ラインプリミティブインターフェイス

			バウンディングボックスを指定したラインデータに対して追加します。
		*/
		virtual void RenderingBounding( Renderer::Object::ILine3D *pLine ) = 0;
	};
}
}

#pragma once


/**
	@file
	@brief シーン管理系クラス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief シーン管理用インターフェイス
		@author 葉迩倭

		シーン管理を行うためのインターフェイスです。
	*/
	class ISceneManager : public IInterface
	{
	protected:
		virtual ~ISceneManager() {}

	public:
		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IModelActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIModelActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual IModelActor *CreateActor( Renderer::Object::IModel *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IMapActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIMapActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/

		virtual IMapActor *CreateActor( Renderer::Object::IMapModel *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IPrimitiveActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIPrimitiveActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual IPrimitiveActor *CreateActor( Renderer::Object::IPoint3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IPrimitiveActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIPrimitiveActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual IPrimitiveActor *CreateActor( Renderer::Object::ILine3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@param IsLightmap		[in] 
			@pamra IsNormalmap		[in] 
			@param IsSpecularmap	[in] 
			@param IsEnvironmentmap	[in] 
			@return	IPrimitiveActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIPrimitiveActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual IPrimitiveActor *CreateActor( Renderer::Object::IPrimitive3D *pObject, Bool IsLightmap = false, Bool IsNormalmap = false, Bool IsSpecularmap = false, Bool IsEnvironmentmap = false ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	ISpriteActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はISpriteActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual ISpriteActor *CreateActor( Renderer::Object::ISprite3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IParticleActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIParticleActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual IParticleActor *CreateActor( Renderer::Object::IFontSprite3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IParticleActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIParticleActorを経由して行います。<BR>
			<B>必ずInitParameterUpdateの後に実行してください。</B>
		*/
		virtual IParticleActor *CreateActor( Renderer::Object::IParticle *pObject ) = 0;

		/**
			@brief シーンカメラ取得
			@author 葉迩倭
			@return カメラ

			シーンに関連付けられたカメラを取得します。
		*/
		virtual ICamera *GetCamera( void ) = 0;

		/**
			@brief シーン初期化
			@author 葉迩倭

			シーンのライトやフォグなどのデータをリセットします。<BR>
			この関数はBegin()-End()内で呼び出すと失敗します。
		*/
		virtual void Reset( void ) = 0;

		/**
			@brief シーン開始
			@author 葉迩倭
			@param IsSort	[in] シーン内のオブジェクトをソートするかどうか

			シーン管理を開始します。<BR>
			この時点でカメラのデータが確定しますので<BR>
			この関数を呼び出したあとのカメラの更新は全て無効です。
		*/
		virtual void Begin( Bool IsSort ) = 0;

		/**
			@brief シーン終了
			@author 葉迩倭

			シーンの管理を完了します。
		*/
		virtual void End( void ) = 0;

		/**
			@brief 終了待機
			@author 葉迩倭

			ISceneManagerでレンダリング処理の完了を完全に待機します。<BR>
			Begin()-End()以降並列でデータの構築処理が行われますので、<BR>
			シーン、あるいはシーンで利用しているリソースを解放する前は<BR>
			この関数でシーンの構築処理の終了を待って下さい。<BR>
			なおアプリケーションの終了時(ICore::Run()=false)時には内部で全てのシーンの<BR>
			Abort()が自動的に呼ばれます。
		*/
		virtual void Abort( void ) = 0;

		/**
			@brief シーンレンダリング
			@author 葉迩倭
			@param IsDrawBuffer	[in] 内部用バッファの表示

			シーンの管理で構築されたシーンをレンダリングします。<BR>
			この関数をコールした段階で内部で描画処理が開始されます。<BR>
			必ずIRender::Begin()～IRender::End()間で呼び出してください。
		*/
		virtual void Rendering( Bool IsDrawBuffer = false ) = 0;

		/**
			@brief アンビエントライト設定
			@author 葉迩倭
			@param vColorSky	[in] シーン内の天球の環境光の色
			@param vColorEarth	[in] シーン内の地表の環境光の色

			シーンの環境光を設定します。<BR>
			半球ライティングを行うために、天球と地表の色を設定します。<BR>
			この関数はBegin()-End()内で呼び出すと失敗します。
		*/
		virtual void SetLightParameter_Ambient( const Math::Vector3D &vColorSky, const Math::Vector3D &vColorEarth ) = 0;

		/**
			@brief ディレクションライト設定
			@author 葉迩倭
			@param vDirect	[in] シーン内の平行光源の方向
			@param vColor	[in] シーン内の平行光源の色

			シーンに大して１つだけ平行光源を割り当てることができます。<BR>
			太陽のように遥か遠方に存在し、オブジェクトの位置によって<BR>
			光の方向が変わらないようなライトに向いています。<BR>
			この関数はBegin()-End()内で呼び出すと失敗します。<BR>
			またシーンモードの影生成を指定している時はvDirectは無効です。<BR>
			方向に関してはSetParameter_Shadow()で指定した値が使用されます。
		*/
		virtual void SetLightParameter_Directional( const Math::Vector3D &vDirect, const Math::Vector3D &vColor ) = 0;

		/**
			@brief ポイントライト追加
			@author 葉迩倭
			@param vPosition	[in] シーン内の点光源の位置
			@param vColor		[in] シーン内の点光源の色
			@param fDistance	[in] シーン内の点光源の影響距離

			シーンに対して点光源を追加します<BR>
			最大で32個のライトを設定でき、そのうち最もオブジェクトに近い<BR>
			4つのライトがオブジェクトに適用されます。<BR>
			この関数はBegin()-End()内で呼び出すと失敗します。
		*/
		virtual void SetLightParameter_AddPoint( const Math::Vector3D &vPosition, const Math::Vector3D &vColor, Float fDistance ) = 0;

		/**
			@brief レンダリングリクエスト数取得
			@author 葉迩倭
			@return レンダリングリクエスト数

			シーンにリクエストレンダリング数を取得します。
		*/
		virtual Sint32 GetResult_RenderingRequestActorCount( void ) = 0;

		/**
			@brief レンダリング数取得
			@author 葉迩倭
			@return レンダリング数

			シーンで実行されたレンダリング数を取得します。<BR>
			2Pass系の処理等もカウントされるので、<BR>
			リクエスト数以上の値になることもあります。
		*/
		virtual Sint32 GetResult_RenderingActorCount( void ) = 0;

		/**
			@brief シーンの構築にかかった時間を取得
			@author 葉迩倭
			@return シーンの構築にかかった時間（１フレームに対する％）

			１フレームあたりのシーン構築にかかった時間を％で取得します。
		*/
		virtual Sint32 GetResult_BackgroundThreadTime( void ) = 0;

		/**
			@brief シーンの描画にかかった時間を取得
			@author 葉迩倭
			@return シーンの描画にかかった時間（１フレームに対する％）

			１フレームあたりのシーン描画にかかった時間を％で取得します。
		*/
		virtual Sint32 GetResult_RenderingTme( void ) = 0;

		/**
			@brief モーションブラーレベルの設定
			@author 葉迩倭
			@param Level		[in] 使用するレベル

			モーションブラー処理のレベルを設定します。<BR>
			MOTION_BLUR_FULL指定の場合はモデルデータが閉じていて法線を保持している必要があります。
		*/
		virtual void SetActorParameter_MotionBlur( eSceneMotionBlurQuarity MotionBlurQuarity ) = 0;

		/**
			@brief 影の生成オプションのON/OFF
			@author 葉迩倭
			@param Type		[in] 影処理の方法
			@param Priority	[in] 影のプライオリティ
			@param fRadius	[in] 丸影の場合の大きさ

			シーンに適用される影処理が「SHADOW_TYPE_PROJECTION_*」か「SHADOW_TYPE_SOFT_PROJECTION_*」の時に<BR>
			レンダリングするモデルが影を落とすかどうかを設定します。<BR>
			影を落とすモデルには他のモデルの影が落ちず、影を落とさないモデルには他のモデルの影が落ちます。
		*/
		virtual void SetActorParameter_ProjectionShadow( eProjectionShadowType Type, eProjectionShadowPriority Priority, Float fRadius = 0.0f ) = 0;

		/**
			@brief シーン全体の明るさの設定
			@author 葉迩倭
			@param vBrightness	[in] 色

			シーンにライトマップを基準とした明るさを設定します。<BR>
			1.0fを指定することでAmaryllis上で設定したパラメーターそのままの明るさになり、<BR>
			それ以上で明るく、それ以下で暗くすることが出来ます。
		*/
		virtual void SetParameter_SceneBrightness( const Math::Vector3D &vBrightness ) = 0;

		/**
			@brief 線形フォグ情報設定
			@author 葉迩倭
			@param vColor	[in] フォグの色
			@param fNear	[in] フォグ開始点
			@param fFar		[in] フォグ終了点

			シーンに適用される線形フォグのパラメーターを設定します。<BR>
			この関数はBegin()-End()内で呼び出すと失敗します。
		*/
		virtual void SetParameter_Fog( Math::Vector3D &vColor, Float fNear, Float fFar ) = 0;

		/**
			@brief 被写界深度用のフォーカス位置を設定します。
			@author 葉迩倭

			@param fForcusZ	[in] フォーカスのＺ（カメラ基準）
			@param fPower	[in] 被写界深度強度

			被写界深度のフォーカス位置を設定します。
		*/
		virtual void SetParameter_DepthOfField( Float fForcusZ, Float fPower ) = 0;

		/**
			@brief ソフトパーティクル用スケール値
			@author 葉迩倭

			@param fSoftParticleScale	[in] ソフトパーティクルのアルファ値用のスケール

			ソフトパーティクル時の深度値の差異からアルファを算出するときのスケール値です。<BR>
			この値が大きいほどアルファの境界がシャープになります。
		*/
		virtual void SetParameter_SoftParticleScale( Float fSoftParticleScale ) = 0;

		/**
			@brief HDRエフェクト設定
			@author 葉迩倭
			@param HdrEffect	[in] HDRエフェクトの種類
			@param fHDRPower	[in] HDRの倍率
			@param fHDRBoundary	[in] HDRとして扱う閾値（1.0が白）

			シーンの描画時のHDRエフェクトの設定をします。
		*/
		virtual void SetParameter_HighDynamicRange( Float fHDRPower, Float fHDRBoundary ) = 0;

		/**
			@brief トゥーン用パラメーター設定
			@author 葉迩倭
			@param vHatchingColor	[in] ハッチング用の斜線の色
			@param fToonPower		[in] トゥーンマップ用の影部分の暗さ（0.0～1.0)

			トゥーンレンダリング用のパラメーターを設定します。
		*/
		virtual void SetParameter_Toon( Math::Vector3D &vHatchingColor, Float fToonPower ) = 0;

		/**
			@brief トゥーン用エッジ検出パラメーター設定
			@author 葉迩倭
			@param fEdgeNormalPower	[in] 法線エッジの検出パラメーター
			@param fEdgeDepthPower	[in] 深度エッジの検出パラメーター
			@param IsToonBold		[in] 輪郭線を太くするか否か

			トゥーン用のエッジ検出用のパラメーターを設定します。<BR>
			いずれのパラメーターも大きいほど境界線が引かれる範囲が増えます。
		*/
		virtual void SetParameter_ToonEdge( Float fEdgeNormalPower, Float fEdgeDepthPower, Bool IsToonBold ) = 0;

		/**
			@brief シャドウ用カメラ設定
			@author 葉迩倭
			@param vPosition	[in] カメラ位置
			@param vTarget		[in] カメラ注視点
			@param fSize		[in] レンダリングサイズ
			@param fNear		[in] 近クリップ面
			@param fFar			[in] 遠クリップ面
			@param fBias		[in] シャドウマップ用深度バイアス
			@param fPower		[in] 影の部分の明るさ(0.0から1.0まで、1.0の時影は完全なアンビエントになる）

			シャドウ用のカメラデータを設定します。
		*/
		virtual void SetParameter_Shadow( Math::Vector3D &vPosition, Math::Vector3D &vTarget, Float fSize, Float fNear, Float fFar, Float fBias, Float fPower ) = 0;

		/**
			@brief レンダリングターゲットの矩形を設定
			@author 葉迩倭
			@param Dst		[in] レンダリング矩形

			シーンのレンダリングターゲットの矩形を指定します。<BR>
			最終的に表示される位置に関係します。
		*/
		virtual void SetParameter_RenderRect( Math::Rect2DF &Dst ) = 0;

		/**
			@brief 背景塗りつぶし色設定
			@author 葉迩倭
			@param ClearColor	[in] バッファクリア色
			@param IsClear		[in] クリア有無

			シーンの描画時の背景の塗りつぶし色を設定します。
		*/
		virtual void SetParameter_BGColor( Math::Vector4D &ClearColor, Bool IsClear = true ) = 0;

		/**
			@brief 背景塗りつぶし色設定
			@author 葉迩倭
			@param ClearColor	[in] バッファクリア色
			@param IsClear		[in] クリア有無

			シーンの描画時の背景の塗りつぶし色を設定します。
		*/
		virtual void SetParameter_BGColor( CColor ClearColor, Bool IsClear = true ) = 0;

		/**
			@brief 背景塗りつぶしテクスチャ設定
			@author 葉迩倭
			@param pTex		[in] テクスチャ

			シーンの描画時の背景のテクスチャを設定します。
		*/
		virtual void SetParameter_BGTexture( Renderer::ITexture *pTex ) = 0;

		/**
			@brief シーン情報を更新
			@author 葉迩倭

			シーンをこれまでの情報を元に更新します。<BR>
			必要な設定後にこの関数を呼び出さない限りIActorを生成することが出来ません。
		*/
		virtual Bool InitParameter_Update( void ) = 0;

		/**
			@brief レンダリングターゲットのサイズを設定
			@author 葉迩倭
			@param pTarget				[in] レンダリングターゲットのテクスチャ（NULLでバックバッファへ直接レンダリング）
			@param Size					[in] サイズ（pTargetが有効な場合はpTargetのサイズに自動設定されます）
			@param IsTextureSizePow2	[in] サイズを強制的に２の累乗にするかどうか

			シーンのレンダリングターゲットのサイズを指定します。<BR>
			レンダリング使われる各種バッファのサイズに関係します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_RenderTargetData( Renderer::ITexture *pTarget, Math::Point2DI Size = Math::Point2DI(0,0), Bool IsTextureSizePow2 = false ) = 0;

		/**
			@brief シェーディング用パラメーターの初期化
			@author 葉迩倭
			@param ShadingType		[in] シェーディングの種類

			シーンで行うシェーディングの処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_Shading( eSceneShadingType ShadingType ) = 0;

		/**
			@brief アンチエイリアス用パラメーターの初期化
			@author 葉迩倭
			@param AntiAliasType	[in] アンチエイリアスの種類
			@param fPower			[in] アンチエイリアスのかかり具合(0.0～1.0)

			シーンで行うアンチエイリアスの処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_AntiAliasType( eSceneAntiAliasType AntiAliasType, Float fPower ) = 0;

		/**
			@brief 影用パラメーターの初期化
			@author 葉迩倭
			@param ShadowType			[in] 影の種類
			@param ShadowQuarity		[in] 影の品質
			@param IsSoftShadow			[in] ソフトシャドウを使うかどうか
			@param IsHardwareShadowmap	[in] ハードウェアシャドウマップを使うかどうか

			シーンで行う影の処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_Shadow( eSceneShadowType ShadowType, eSceneShadowQuarity ShadowQuarity, Bool IsSoftShadow = false, Bool IsHardwareShadowmap = true ) = 0;

		/**
			@brief モーションブラー用パラメーターの初期化
			@author 葉迩倭
			@param MotionBlurType	[in] モーションブラーの種類

			シーンで行うモーションブラーの処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_MotionBlur( eSceneMotionBlurType MotionBlurType ) = 0;

		/**
			@brief 被写界深度用パラメーターの初期化
			@author 葉迩倭
			@param DofType	[in] 被写界深度の種類

			シーンで行う被写界深度の処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_DepthOfField( eSceneDepthOfFieldType DofType ) = 0;

		/**
			@brief フォグ用パラメーターの初期化
			@author 葉迩倭
			@param FogType		[in] フォグの種類
			@param FogEffect	[in] フォグのエフェクト

			シーンで行うフォグの処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_Fog( eSceneFogType FogType, eSceneFogEffect FogEffect ) = 0;

		/**
			@brief HDRレンダリング用パラメーターの初期化
			@author 葉迩倭
			@param HdrType		[in] HDRレンダリングの種類
			@param HdrEffect	[in] HDRレンダリングのエフェクト

			シーンで行うHDRレンダリングの処理を初期化します。<BR>
			<B>必ずInitParameterUpdateの前に実行してください。</B>
		*/
		virtual void InitParameter_HighDynamicRange( eSceneHighDynamicRangeType HdrType, eSceneHighDynamicRangeEffect HdrEffect ) = 0;

		/**
			@brief スカイドームの生成
			@author 葉迩倭
			@param fRadius		[in] 半径
			@param TopColor		[in] 天球の頭頂部の色
			@param BottomColor	[in] 天球の地面部分の色
			@param pTexCloud	[in] 雲テクスチャ

			シーンに適用するスカイドームを生成します。<BR>
		*/
		virtual Bool SceneSkydoom_Create( Float fRadius, CColor TopColor, CColor BottomColor, Renderer::ITexture *pTexCloud ) = 0;

		/**
			@brief スカイドームのレンダリング
			@author 葉迩倭

			シーンに登録されているスカイドームのレンダリングをします。
		*/
		virtual void SceneSkydoom_Rendering( void ) = 0;

		/**
			@brief スカイドームの雲テクスチャの色を設定
			@author 葉迩倭
			@param Color		[in] 色

			シーンに適用するスカイドームの雲の色を設定します。
		*/
		virtual void SceneSkydoom_SetCloudColor( const CColor &Color ) = 0;

		/**
			@brief スカイドームの雲テクスチャの移動量オフセット
			@author 葉迩倭
			@param vOffset		[in] 移動量(1.0でテクスチャサイズ)

			シーンに適用するスカイドームの雲の流れを制御します。<BR>
			この関数で与えたオフセット分雲が移動します。
		*/
		virtual void SceneSkydoom_SetCloudOffset( const Math::Vector2D &vOffset ) = 0;

		/**
			@brief スクリーン座標に変換
			@author 葉迩倭
			@param vPosition	[in] ワールド座標
			@return スクリーン座標

			ワールド空間の座標をスクリーン座標に変換します。
		*/
		virtual Math::Vector3D TransformToScreen( const Math::Vector3D &vPosition ) = 0;

		/**
			@brief スクリーン座標から変換
			@author 葉迩倭
			@param vPosition	[in] スクリーン座標
			@return ワールド座標

			スクリーン座標をワールド空間の座標に変換します。
		*/
		virtual Math::Vector3D TransformFromScreen( const Math::Vector3D &vPosition ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief カスタムシーン管理用インターフェイス
		@author 葉迩倭

		@note
		カスタムシーン管理を行うためのインターフェイスです。
	*/
	class ICustomizedSceneManager : public IInterface
	{
	protected:
		virtual ~ICustomizedSceneManager() {}

	public:
		/**
			@brief 終了待機
			@author 葉迩倭

			ISceneManagerでレンダリング処理の完了を完全に待機します。<BR>
			Begin()-End()以降並列でデータの構築処理が行われますので、<BR>
			シーン、あるいはシーンで利用しているリソースを解放する前は<BR>
			この関数でシーンの構築処理の終了を待って下さい。<BR>
			なおアプリケーションの終了時(ICore::Run()=false)時には内部で全てのシーンの<BR>
			Abort()が自動的に呼ばれます。
		*/
		virtual void Abort( void ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IModelActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIModelActorを経由して行います。
		*/
		virtual IModelActor *CreateActor( Renderer::Object::IModel *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IMapActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIMapActorを経由して行います。
		*/

		virtual IMapActor *CreateActor( Renderer::Object::IMapModel *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IPrimitiveActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIPrimitiveActorを経由して行います。
		*/
		virtual IPrimitiveActor *CreateActor( Renderer::Object::IPoint3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IPrimitiveActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIPrimitiveActorを経由して行います。
		*/
		virtual IPrimitiveActor *CreateActor( Renderer::Object::ILine3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@param IsLightmap		[in] 
			@pamra IsNormalmap		[in] 
			@param IsSpecularmap	[in] 
			@param IsEnvironmentmap	[in] 
			@return	IPrimitiveActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIPrimitiveActorを経由して行います。
		*/
		virtual IPrimitiveActor *CreateActor( Renderer::Object::IPrimitive3D *pObject, Bool IsLightmap = false, Bool IsNormalmap = false, Bool IsSpecularmap = false, Bool IsEnvironmentmap = false ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	ISpriteActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はISpriteActorを経由して行います。
		*/
		virtual ISpriteActor *CreateActor( Renderer::Object::ISprite3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IParticleActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIParticleActorを経由して行います。
		*/
		virtual IParticleActor *CreateActor( Renderer::Object::IFontSprite3D *pObject ) = 0;

		/**
			@brief シーンアクター取得
			@author 葉迩倭
			@param pObject	[in] アクターに関連付ける描画インターフェイス
			@return	IParticleActorインターフェイス

			ISceneManagerでレンダリングを行うためのアクターを取得します。<BR>
			ワールド変換やレンダリングの操作はIParticleActorを経由して行います。
		*/
		virtual IParticleActor *CreateActor( Renderer::Object::IParticle *pObject ) = 0;

		/**
			@brief シーンカメラ取得
			@author 葉迩倭
			@return カメラ

			シーンに関連付けられたカメラを取得します。
		*/
		virtual ICamera *GetCamera( void ) = 0;

		/**
			@brief シーン初期化
			@author 葉迩倭

			シーンのライトやフォグなどのデータをリセットします。<BR>
			この関数はBegin()-End()内で呼び出すと失敗します。
		*/
		virtual void Reset( void ) = 0;

		/**
			@brief シーン開始
			@author 葉迩倭
			@param IsSort	[in] シーン内のオブジェクトをソートするかどうか

			シーン管理を開始します。<BR>
			この時点でカメラのデータが確定しますので<BR>
			この関数を呼び出したあとのカメラの更新は全て無効です。
		*/
		virtual void Begin( Bool IsSort ) = 0;

		/**
			@brief シーン終了
			@author 葉迩倭

			シーンの管理を完了します。
		*/
		virtual void End( void ) = 0;

		/**
			@brief シーンレンダリング開始
			@author 葉迩倭
			@retval false	シーンにレンダリングするオブジェクトがない
			@retval	true	シーンにレンダリングするオブジェクトがある

			@note
			シーンのレンダリングを開始する事を宣言します。<BR>
			この関数がtrueを返した場合は必ずRedneringEnd()をコールして下さい。
		*/
		virtual Bool RenderingStart( void ) = 0;

		/**
			@brief シーンレンダリング終了
			@author 葉迩倭

			@note
			シーンのレンダリングを終了する事を宣言します。
		*/
		virtual void RenderingExit( void ) = 0;

		/**
			@brief スクリーン座標に変換
			@author 葉迩倭
			@param vPosition	[in] ワールド座標
			@return スクリーン座標

			ワールド空間の座標をスクリーン座標に変換します。
		*/
		virtual Math::Vector3D TransformToScreen( const Math::Vector3D &vPosition ) = 0;

		/**
			@brief スクリーン座標から変換
			@author 葉迩倭
			@param vPosition	[in] スクリーン座標
			@return ワールド座標

			スクリーン座標をワールド空間の座標に変換します。
		*/
		virtual Math::Vector3D TransformFromScreen( const Math::Vector3D &vPosition ) = 0;

		/**
			@brief アクターオブジェクトのレイヤー数取得
			@author 葉迩倭
			@return アクターオブジェクトのレイヤー数

			@note
			レンダリングするアクターオブジェクトのレイヤー数を取得します。<BR>
			レイヤー数自体は3で固定されており、内部的に下のように固定されています<BR>
			0=未使用<BR>
			1=不透明モデル<BR>
			2=半透明モデル
		*/
		virtual Sint32 GetRenderObjectLayerCount( void ) = 0;

		/**
			@brief アクターオブジェクトの取得
			@author 葉迩倭
			@param Layer	[in] オブジェクトレイヤー
			@return アクターオブジェクト

			@note
			レンダリングするアクターオブジェクトを取得します。
		*/
		virtual ICustomizedSceneRenderObject *GetRenderObject( Sint32 Layer ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief カスタムシーン描画オブジェクトインターフェイス
		@author 葉迩倭

		@note
		カスタムシーンの描画オブジェクトを扱うためのインターフェイスです。
	*/
	class ICustomizedSceneRenderObject : public IInterface
	{
	protected:
		virtual ~ICustomizedSceneRenderObject() {}

	public:
		/**
			@brief ボーン用頂点の有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトがボーン用頂点を持っているかを取得します。
		*/
		virtual Bool IsSupportVertexBone( void ) = 0;

		/**
			@brief テクスチャ用頂点の有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトがテクスチャ用頂点を持っているかを取得します。
		*/
		virtual Bool IsSupportVertexTexture( void ) = 0;

		/**
			@brief 法線用頂点の有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトが法線用頂点を持っているかを取得します。
		*/
		virtual Bool IsSupportVertexNormal( void ) = 0;

		/**
			@brief バンプ用接線用頂点の有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトがバンプ用接線用頂点を持っているかを取得します。
		*/
		virtual Bool IsSupportVertexBump( void ) = 0;

		/**
			@brief マテリアル数取得
			@author 葉迩倭
			@return マテリアル数

			@note
			描画オブジェクトが所有するマテリアル数を取得します。
		*/
		virtual Sint32 GetMaterialCount( void ) = 0;

		/**
			@brief 描画マテリアル取得
			@author 葉迩倭
			@param Index	[in] マテリアル番号

			@note
			描画オブジェクトが所有するマテリアルデータを取得します。
		*/
		virtual ICustomizedSceneRenderMaterial *GetMaterialPointer( Sint32 Index ) = 0;

		/**
			@brief ボーン数を取得
			@author 葉迩倭
			@return ボーン数

			@note
			描画オブジェクトが所持しているボーン数を取得します。
		*/
		virtual Sint32 GetBoneCount( void ) = 0;

		/**
			@brief ワールド行列取得
			@author 葉迩倭
			@return ワールド行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldMatrix( void ) = 0;


		/**
			@brief 前フレームのワールド行列取得
			@author 葉迩倭
			@return ワールド行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldMatrixPrev( void ) = 0;

		/**
			@brief ワールド＞ビュー行列取得
			@author 葉迩倭
			@return ワールド＞ビュー行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldViewMatrix( void ) = 0;

		/**
			@brief 前フレームのワールド＞ビュー行列取得
			@author 葉迩倭
			@return ワールド＞ビュー行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldViewPrevMatrix( void ) = 0;

		/**
			@brief ワールド＞ビュー＞プロジェクション行列取得
			@author 葉迩倭
			@return ワールド＞ビュー＞プロジェクション行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldViewProjectionMatrix( void ) = 0;

		/**
			@brief 前フレームのワールド＞ビュー＞プロジェクション行列取得
			@author 葉迩倭
			@return ワールド＞ビュー＞プロジェクション行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldViewProjectionPrevMatrix( void ) = 0;

		/**
			@brief ワールド逆行列取得
			@author 葉迩倭
			@return ワールド逆行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldInverseMatrix( void ) = 0;

		/**
			@brief 前フレームのワールド逆行列取得
			@author 葉迩倭
			@return ワールド逆行列

			@note
			描画オブジェクトの行列データを取得します。
		*/
		virtual const Math::Matrix &GetWorldInverseMatrixPrev( void ) = 0;

		/**
			@brief ボーン用変換行列取得
			@author 葉迩倭
			@return ボーン変換行列

			@note
			描画オブジェクトのボーン用の行列データを取得します。
		*/
		virtual const Math::SMatrix4x4 *GetBoneMatrixArray( void ) = 0;

		/**
			@brief 前フレームのボーン用変換行列取得
			@author 葉迩倭
			@return ボーン変換行列

			@note
			描画オブジェクトのボーン用の行列データを取得します。
		*/
		virtual const Math::SMatrix4x4 *GetBoneMatrixArrayPrev( void ) = 0;

		/**
			@brief レンダリング用頂点データ設定
			@author 葉迩倭
			@param pRender		[in] 関連するIRenderインターフェイス

			@note
			レンダリングを行うための頂点ストリームの設定を行います。
		*/
		virtual void SetStreamSource( Renderer::IRender *pRender ) = 0;

		/**
			@brief 通常レンダリング
			@author 葉迩倭
			@param MaterialNo	[in] 描画するマテリアル番号
			@param pRender		[in] 関連するIRenderインターフェイス

			@note
			通常のレンダリング処理を行います。
		*/
		virtual Sint32 Rendering( Sint32 MaterialNo, Renderer::IRender *pRender ) = 0;

		/**
			@brief 速度マップ用縮退ポリゴン入りレンダリング
			@author 葉迩倭
			@param pRender	[in] 関連するIRenderインターフェイス

			@note
			速度マップ描画用の縮退ポリゴン入りのレンダリングを行います。
		*/
		virtual Sint32 Rendering_Velocity( Renderer::IRender *pRender ) = 0;

		/**
			@brief 頂点変換タイプを取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトの頂点変換タイプを取得します。
		*/
		virtual eTransformType GetTransformType( void ) = 0;

		/**
			@brief ソフトパーティクルの有無を取得
			@author 葉迩倭
			@retval false	ソフトパーティクルではない
			@retval true	ソフトパーティクル

			@note
			描画オブジェクトがソフトパーティクルかどうかを取得します。
		*/
		virtual Bool GetSoftBillboardEnable( void ) = 0;

		/**
			@brief 視差バンプマップの有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトが視差バンプマップを持っているかを取得します。
		*/
		virtual Bool GetParallaxEnable( void ) = 0;

		/**
			@brief スペキュラー設定のの有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトがスペキュラー設定のを持っているかを取得します。
		*/
		virtual Bool GetSpecularEnable( void ) = 0;

		/**
			@brief 環境マップ設定のの有無を取得
			@author 葉迩倭
			@retval false	無い
			@retval true	有る

			@note
			描画オブジェクトが環境マップ設定のを持っているかを取得します。
		*/
		virtual Bool GetEnvironmentEnable( void ) = 0;

		/**
			@brief リストにつながれている次の描画オブジェクトを取得
			@author 葉迩倭
			@return 描画オブジェクトインターフェイス

			@note
			リストにつながれている描画オブジェクトの自分の次の描画オブジェクトを取得します。
		*/
		virtual ICustomizedSceneRenderObject *GetNextObject( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Scene
{
	/**
		@brief カスタムシーンマテリアルインターフェイス
		@author 葉迩倭

		@note
		カスタムシーンのアクターのマテリアルを扱うためのインターフェイスです。
	*/
	class ICustomizedSceneRenderMaterial
	{
	protected:
		virtual ~ICustomizedSceneRenderMaterial() {}

	public:
		/**
			@brief シェーダーインターフェイス取得
			@author 葉迩倭
			@return シェーダーインターフェイス

			@note
			マテリアルに関連付けられているシェーダーを取得します。
		*/
		virtual Renderer::Shader::IShader *GetShader( void ) = 0;

		/**
			@brief テクスチャインターフェイス取得
			@author 葉迩倭
			@return テクスチャインターフェイス

			@note
			マテリアルに関連付けられているテクスチャを取得します。
		*/
		virtual Renderer::ITexture *GetTexture( Sint32 Stage ) = 0;

		/**
			@brief 拡散反射色取得
			@author 葉迩倭
			@return 拡散反射色

			@note
			マテリアルに関連付けられている拡散反射色を取得します。
		*/
		virtual Math::Vector4D &GetDiffuseColor( void ) = 0;

		/**
			@brief 自己発光色取得
			@author 葉迩倭
			@return 自己発光色

			@note
			マテリアルに関連付けられている自己発光色を取得します。
		*/
		virtual Math::Vector4D &GetEmissiveColor( void ) = 0;

		/**
			@brief 鏡面反射色取得
			@author 葉迩倭
			@return 鏡面反射色

			@note
			マテリアルに関連付けられている鏡面反射色を取得します。
		*/
		virtual Math::Vector4D &GetSpecularColor( void ) = 0;

		/**
			@brief テクスチャUVオフセット取得
			@author 葉迩倭
			@return テクスチャUVオフセット

			@note
			マテリアルに関連付けられているテクスチャUVオフセットを取得します。
		*/
		virtual Math::Vector2D &GetTextureOffset( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual Float GetSpecularRefractive( void ) = 0;

		/**
			@brief 鏡面反射荒さ取得
			@author 葉迩倭
			@return 鏡面反射荒さ

			@note
			マテリアルに関連付けられている鏡面反射荒さを取得します。
		*/
		virtual Float GetSpecularRoughly( void ) = 0;

		/**
			@brief 視差マップ深度取得
			@author 葉迩倭
			@return 視差マップ深度

			@note
			マテリアルに関連付けられている視差マップ深度を取得します。
		*/
		virtual Float GetParallaxDepth( void ) = 0;

		/**
			@brief 鏡面反射タイプ取得
			@author 葉迩倭
			@return 鏡面反射タイプ

			@note
			マテリアルに関連付けられている鏡面反射タイプを取得します。
		*/
		virtual eSpecularType GetSpecularType( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual eBumpType GetBumpType( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual eDrawType GetDrawType( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual eCullType GetCullType( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual Sint32 GetAlphaBoundary( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual Bool GetAlphaTestEnable( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual Bool GetZTestEnable( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual Bool GetZWriteEnable( void ) = 0;

		/**
			@brief 鏡面反射反射率取得
			@author 葉迩倭
			@return 鏡面反射反射率

			@note
			マテリアルに関連付けられている鏡面反射反射率を取得します。
		*/
		virtual Bool GetLightEnable( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Dynamics
{
	struct SSurfaceParameter
	{
		eContactMode	Mode;				///< モード
		Float			Friction;			///< 摩擦係数
		Float			FrictionEx;			///< 摩擦係数
		Float			Bounce;				///< 反射係数
		Float			BounceVelocity;		///< 反射最低速度
		Float			SoftErp;
		Float			SoftCfm;
		Float			Motion;
		Float			MotionEx;
		Float			Slip;
		Float			SlipEx;
	};

	/**
		@brief 接触点処理用インターフェイス
		@author 葉迩倭

		ダイナミクスの接触点を管理するためのインターフェイスです。
	*/
	class IContactPoint : public IInterface
	{
	public:
		virtual ~IContactPoint() {}

		virtual Sint32 GetCount( void ) = 0;
		virtual void SetSurfaceParameter( Sint32 Index, SSurfaceParameter &Param ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
	enum eBodyType
	{
		BODY_TYPE_GEOMETRY,
		BODY_TYPE_MODEL,
		BODY_TYPE_MAP,
	};

namespace Dynamics
{
	/**
		@brief ダイナミクス用剛体インターフェイス
		@author 葉迩倭

		ダイナミクスを処理する剛体のインターフェイスです。
	*/
	class IRigidBody : public IInterface
	{
	public:
		virtual ~IRigidBody() {}

		virtual void SetUserData( void *pUser ) = 0;
		virtual void *GetUserData( void ) = 0;
		virtual eBodyType GetType( void ) = 0;

		virtual void SetPosition( Math::Vector3D &vPos ) = 0;
		virtual void SetRotation( Math::Matrix &mRot ) = 0;
		virtual void SetLinearSpeed( Math::Vector3D &vDir ) = 0;
		virtual void SetAngularSpeed( Math::Vector3D &vDir ) = 0;

		virtual Math::Matrix &GetMatrix( void ) = 0;
		virtual void GetPosition( Math::Vector3D &vPos ) = 0;

		virtual void SetFixed( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Dynamics
{
	/**
		@brief ダイナミクス用剛体インターフェイス
		@author 葉迩倭

		ダイナミクスを処理する剛体のインターフェイスです。
	*/
	class IRigidModel : public IInterface
	{
	public:
		virtual ~IRigidModel() {}
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Dynamics
{
	typedef void (*CollisionProc)( ISimulationWorld *pWorld, IRigidBody *pIn1, IRigidBody *pIn2, IContactPoint *pContact );

	/**
		@brief ダイナミクス管理インターフェイス
		@author 葉迩倭

		ダイナミクスを処理するワールドを管理するインターフェイスです。
	*/
	class ISimulationWorld : public IInterface
	{
	public:
		virtual ~ISimulationWorld() {}

		virtual void SetContactPointCount( Sint32 Count ) = 0;
		virtual void SetStepNumIterations( Sint32 Count ) = 0;

		virtual void CreateEarth( void ) = 0;
		virtual void SetGravity( Math::Vector3D &vDir ) = 0;
		virtual void Update( Float fTime, CollisionProc Proc ) = 0;

		virtual void RenderingRequest( void ) = 0;

		virtual IRigidBody *CreateSphere( Float fRadius, Float fMass, Bool IsBody ) = 0;
		virtual IRigidBody *CreateCapsule( Float fRadius, Float fLength, Float fMass, Bool IsBody ) = 0;
		virtual IRigidBody *CreateBox( Math::Point3DF &Size, Float fMass, Bool IsBody ) = 0;
		virtual IRigidModel *CreateMap( Renderer::Object::IMapModel *pMap ) = 0;
		virtual IRigidModel *CreateModel( Renderer::Object::IModel *pModel, Bool IsBody ) = 0;
	};
}
}

#pragma once


/**
	@file
	@brief テクスチャインターフェイス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Renderer
{
	/**
		@brief 深度バッファタイプ
		@author 葉迩倭
	*/
	enum eDepthBufferType
	{
		DEPTH_BUFFER_SURFACE,			///< 深度バッファはサーフェイス
		DEPTH_BUFFER_TEXTURE_NVIDIA,	///< 深度バッファはテクスチャ（NVIDIA仕様）
		DEPTH_BUFFER_TEXTURE_ATI,		///< 深度バッファはテクスチャ（ATI仕様）
	};

	/**
		@brief テクスチャタイプ
		@author 葉迩倭
	*/
	enum eTextureType
	{
		TEXTURE_TYPE_DEFAULT,		///< 通常の描画用テクスチャ
		TEXTURE_TYPE_TARGET,		///< 描画ターゲット用テクスチャ
		TEXTURE_TYPE_DEPTH,			///< 深度バッファ用テクスチャ
		TEXTURE_TYPE_NONE,			///< 特になし
	};

	/**
		@brief テクスチャインターフェイス
		@author 葉迩倭

		@note
		テクスチャを操作するためのインターフェイスです。<BR>
		IRenderインターフェイスから取得できます。
	*/
	class ITexture : public IInterface
	{
	public:
		virtual ~ITexture() {}

/**
@brief 画像ファイルとして保存
@author 葉迩倭
@param pFileName	[in] テクスチャファイル名
@retval false	失敗
@retval true	成功

@note
テクスチャの内容をTGA画像ファイルとして保存します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;
  IGraphicCard *pGraphicCard = NULL;
  IRender      *pRender      = NULL;
  ITexture     *pTex         = NULL;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // ビデオカード初期化
    IGraphicCard *pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
    if ( pGraphicCard == NULL ) goto EXIT;      // IGraphicCardにはNullDeviceがないので失敗するとNULLが返る

    // レンダラーの生成
    IRender *pRender = pGraphicCard->CreateRender();
    if ( pRender == NULL ) goto EXIT;           // IRenderにはNullDeviceがないので失敗するとNULLが返る

    // テクスチャの生成
    pTex = pRender->CreateTextureFromFile( "sample.bmp" );

    // テクスチャを画像として保存
    pTex->SaveToTGA( "hogehoge.tga" );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

EXIT:
  SAFE_RELEASE( pTex );                // テクスチャの解放
  SAFE_RELEASE( pRender );             // レンダラーの解放
  SAFE_RELEASE( pGraphicCard );        // ビデオカードの解放
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Bool SaveToTGA( const char *pFileName ) = 0;

/**
@brief 画像ファイルとして保存
@author 葉迩倭
@param pFileName	[in] テクスチャファイル名
@retval false	失敗
@retval true	成功

@note
テクスチャの内容をPNG画像ファイルとして保存します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;
  IGraphicCard *pGraphicCard = NULL;
  IRender      *pRender      = NULL;
  ITexture     *pTex         = NULL;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // ビデオカード初期化
    IGraphicCard *pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
    if ( pGraphicCard == NULL ) goto EXIT;      // IGraphicCardにはNullDeviceがないので失敗するとNULLが返る

    // レンダラーの生成
    IRender *pRender = pGraphicCard->CreateRender();
    if ( pRender == NULL ) goto EXIT;           // IRenderにはNullDeviceがないので失敗するとNULLが返る

    // テクスチャの生成
    pTex = pRender->CreateTextureFromFile( "sample.bmp" );

    // テクスチャを画像として保存
    pTex->SaveToPNG( "hogehoge.png" );

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

EXIT:
  SAFE_RELEASE( pTex );                // テクスチャの解放
  SAFE_RELEASE( pRender );             // レンダラーの解放
  SAFE_RELEASE( pGraphicCard );        // ビデオカードの解放
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Bool SaveToPNG( const char *pFileName ) = 0;

/**
@brief 実テクスチャサイズ取得
@author 葉迩倭
@return テクスチャの実サイズ

@note
テクスチャのサイズを取得します。<BR>
２の累乗でないテクスチャを作成した際に<BR>
デバイスがそのサイズをサポートしていないときは<BR>
内包できる大きさの２の累乗のサイズになっています。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;
  IGraphicCard *pGraphicCard = NULL;
  IRender      *pRender      = NULL;
  ITexture     *pTex         = NULL;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // ビデオカード初期化
    IGraphicCard *pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
    if ( pGraphicCard == NULL ) goto EXIT;      // IGraphicCardにはNullDeviceがないので失敗するとNULLが返る

    // レンダラーの生成
    IRender *pRender = pGraphicCard->CreateRender();
    if ( pRender == NULL ) goto EXIT;           // IRenderにはNullDeviceがないので失敗するとNULLが返る

    // テクスチャの生成
    pTex = pRender->CreateTextureFromFile( "sample.bmp" );

    // テクスチャのサイズを取得
    // テクスチャの実サイズ
    // デバイスによっては生成時に要求通りにサイズはならない
    Math::Point2DI Size = pTex->GetSize();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

EXIT:
  SAFE_RELEASE( pTex );                // テクスチャの解放
  SAFE_RELEASE( pRender );             // レンダラーの解放
  SAFE_RELEASE( pGraphicCard );        // ビデオカードの解放
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Math::Point2DI GetSize( void ) = 0;

/**
@brief 要求テクスチャサイズ取得
@author 葉迩倭
@return テクスチャのサイズ

@note
テクスチャの元サイズを取得します。<BR>
作成時に指定した大きさを取得します。<BR>
この大きさは実際のテクスチャサイズとは違う場合があります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;
  IGraphicCard *pGraphicCard = NULL;
  IRender      *pRender      = NULL;
  ITexture     *pTex         = NULL;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // ビデオカード初期化
    IGraphicCard *pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
    if ( pGraphicCard == NULL ) goto EXIT;      // IGraphicCardにはNullDeviceがないので失敗するとNULLが返る

    // レンダラーの生成
    IRender *pRender = pGraphicCard->CreateRender();
    if ( pRender == NULL ) goto EXIT;           // IRenderにはNullDeviceがないので失敗するとNULLが返る

    // テクスチャの生成
    pTex = pRender->CreateTextureFromFile( "sample.bmp" );

    // 生成時にリクエストしたテクスチャのサイズ
    // ファイル読み込みの場合ファイル上ので画像のサイズ
    Math::Point2DI Size = pTex->GetOriginalSize();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

EXIT:
  SAFE_RELEASE( pTex );                // テクスチャの解放
  SAFE_RELEASE( pRender );             // レンダラーの解放
  SAFE_RELEASE( pGraphicCard );        // ビデオカードの解放
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Math::Point2DI GetOriginalSize( void ) = 0;

/**
@brief テクスチャ変換用パラメーター取得
@author 葉迩倭
@return 変換用の値

@note
テクスチャの元画像の座標からテクスチャの値を取得するための変換用の値を取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;
  IGraphicCard *pGraphicCard = NULL;
  IRender      *pRender      = NULL;
  ITexture     *pTex         = NULL;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // ビデオカード初期化
    IGraphicCard *pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
    if ( pGraphicCard == NULL ) goto EXIT;      // IGraphicCardにはNullDeviceがないので失敗するとNULLが返る

    // レンダラーの生成
    IRender *pRender = pGraphicCard->CreateRender();
    if ( pRender == NULL ) goto EXIT;           // IRenderにはNullDeviceがないので失敗するとNULLが返る

    // テクスチャの生成
    pTex = pRender->CreateTextureFromFile( "sample.bmp" );

    // 元画像の(100,50)座標をUVへ変換
    Math::Point2DF Trans = pTex->GetPixelToTexelTransform();
    Float u = 100.0f * Trans.x;
    Float v =  50.0f * Trans.y;

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

EXIT:
  SAFE_RELEASE( pTex );                // テクスチャの解放
  SAFE_RELEASE( pRender );             // レンダラーの解放
  SAFE_RELEASE( pGraphicCard );        // ビデオカードの解放
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Math::Point2DF GetPixelToTexelTransform( void ) = 0;

/**
@brief レンダリングターゲット内容を保存
@author 葉迩倭
@retval true	成功
@retval false	失敗

@note
レンダリングターゲットの現在の内容をバックアップします。<BR>
バックアップされた内容はデバイスのロスト時に自動的に復元されます。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  ICore        *pCore        = NULL;
  IGraphicCard *pGraphicCard = NULL;
  IRender      *pRender      = NULL;
  ITexture     *pTex         = NULL;

  // システムの起動
  if ( !System::Initialize() )
  {
    return 0;
  }

  // ICoreの生成
  pCore = System::CreateCore();
  if ( pCore == NULL ) goto EXIT;               // ICoreにはNullDeviceがないので失敗するとNULLが返る

  // コアの初期化
  if ( pCore->Initialize( "Sample Program", FRAME_RATE_60 ) )
  {
    // アプリケーションの開始
    pCore->Start( 640, 480, true );

    // ビデオカード初期化
    IGraphicCard *pGraphicCard = pCore->CreateGraphicCard( GRAPHIC_CARD_DEFAULT_NO );
    if ( pGraphicCard == NULL ) goto EXIT;      // IGraphicCardにはNullDeviceがないので失敗するとNULLが返る

    // レンダラーの生成
    IRender *pRender = pGraphicCard->CreateRender();
    if ( pRender == NULL ) goto EXIT;           // IRenderにはNullDeviceがないので失敗するとNULLが返る

    // テクスチャの生成
    pTex = pRender->CreateTextureRenderTarget( 256, 256 );

    // デバイスロスト時に内容が復元できるように
    // 現在の状態をバックアップしておく。
    //   通常は何らかのレンダリングを行った時に、
    //   その内容を保存しておきたい場合に使う。
    pTex->BackupTargetBuffer();

    // メインループ
    while ( pCore->Run() )
    {
    }
  }

EXIT:
  SAFE_RELEASE( pTex );                // テクスチャの解放
  SAFE_RELEASE( pRender );             // レンダラーの解放
  SAFE_RELEASE( pGraphicCard );        // ビデオカードの解放
  SAFE_RELEASE( pCore );               // コアの解放

  System::Finalize();                  // システムの終了処理を行う

  return 0;
}
@endcode
*/
		virtual Bool BackupTargetBuffer( void ) = 0;

/**
@brief 種類を取得
@author 葉迩倭
@return	テクスチャがどの種類に属するかを取得します。

@note
テクスチャの種類を取得します。<BR>
IRender::SetRenderTarget()に使えるのはTEXTURE_TYPE_TARGETだけで<BR>
IRender::SetDepthBuffer()に使えるのはTEXTURE_TYPE_DEPTHだけです。
*/
		virtual eTextureType GetType( void ) = 0;

/**
@brief 深度バッファの種類を取得
@author 葉迩倭
@return	深度バッファの種類

@note
深度バッファの種類を取得します。<BR>
返り値がDEPTH_BUFFER_TEXTUREの場合はテクスチャとして深度バッファが作られています。
*/
		virtual eDepthBufferType GetDepthBufferType( void ) = 0;

/**
@brief フィルタリング処理の是非を取得
@author 葉迩倭
@retval false	フィルタリング＆アルファブレンド出来ない
@retval true	フィルタリング＆アルファブレンド出来る

@note
フィルタリング可能なフォーマットかを取得します。
*/
		virtual Bool IsFilterEnable( void ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Network
{
	/**
		@brief	ホスト情報
		@author 葉迩倭
	*/
	struct SHostInfo
	{
		char Name[64];						///< ホスト名
		char Alias[16][64];					///< ホスト別名テーブル
		char Address[16][64];				///< IPアドレステーブル
		Sint32 Type;						///< データタイプ
		Sint32 Length;						///< データサイズ
		Sint32 AliasCount;					///< エイリアス数
		Sint32 AddressCount;				///< IPアドレス数
	};

	/**
		@brief ネットワーク管理クラス
		@author 葉迩倭

		@note
		Seleneで使用するネットワークの管理を行います。
	*/
	class INetworkManager : public IInterface
	{
	public:
		virtual ~INetworkManager() {}

/**
@brief エラー文字列取得
@author 葉迩倭
@return	文字列のポインタ

@note
一連のネットワーク処理発生したエラーメッセージを取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Network::INetworkManager	*pNetworkMgr	= NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーにTCPを使って接続する
  pTCP = pNetworkMgr->ConnectByTCP( "127.0.0.1", 80 );
  if ( pTCP == NULL )
  {
    // エラーメッセージ
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual const char *GetLastErrorString( void ) = 0;

/**
@brief クライアント生成
@author 葉迩倭
@param pTargetHost	[in] 接続先のホスト名（IPアドレス）
@param TargetPort	[in] 接続先のポート
@retval	NULL		失敗
@retval	NULL以外	クライアントのインターフェイス

@note
指定したホストへのTCP接続を試みます。<BR>
接続できた場合はTCP操作用のインターフェイスが返ります。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Network::INetworkManager	*pNetworkMgr	= NULL;
  Network::ISocketTCP		*pTCP			= NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーにTCPを使って接続する
  pTCP = pNetworkMgr->ConnectByTCP( "127.0.0.1", 80 );
  if ( pTCP == NULL )
  {
    // エラーメッセージ
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // TCP接続を解放
  SAFE_RELEASE( pTCP );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual ISocketTCP *ConnectByTCP( const char *pTargetHost, Uint16 TargetPort ) = 0;

/**
@brief サーバー機能を生成します
@author 葉迩倭
@param Port			[in] ポート番号
@param ConnectMax	[in] 接続最大数
@retval	NULL		失敗
@retval	NULL以外	サーバーのインターフェイス

@note
サーバー機能を生成し、そのインターフェイスを取得します。<BR>
接続できた場合はTCP操作用のインターフェイスが返ります。

@code

using namespace Selene;

int main()
{
  Network::INetworkManager  *pNetworkMgr  = NULL;
  Network::IServerTCP      *pServer    = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーを生成する
  pServer = pNetworkMgr->CreateServer( HOST_PORT, 16 );
  if ( pServer == NULL )
  {
    // 失敗
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // キー入力で終了
  getchar();

  // サーバーを解放
  SAFE_RELEASE( pServer );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
	virtual IServerTCP *CreateServer( Uint16 Port, Uint32 ConnectMax = 32 ) = 0;

/**
@brief ホスト情報取得
@author 葉迩倭
@param Info		[out] ホスト情報

@note
一連のネットワーク処理発生したエラーメッセージを取得します。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Network::INetworkManager	*pNetworkMgr	= NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // アプリケーションが動作しているPCのホスト情報を取得
  SHostInfo Info;
  pNetworkMgr->GetHostInfo( Info );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual void GetHostInfo( SHostInfo &Info ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Network
{
	/**
		@brief TCP接続クライアントクラス
		@author 葉迩倭

		@note
		Seleneで使用するネットワーク処理を行います。
	*/
	class ISocketTCP : public IInterface
	{
	public:
		virtual ~ISocketTCP() {}

/**
@brief データ送信
@author 葉迩倭
@param pData	[in] データ格納バッファ
@param Size		[in] 送信サイズ
@retval false	失敗
@retval true	成功

@note
データの送信を行います。<BR>
指定サイズの送信が完了するまで関数から抜けてきません。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Network::INetworkManager	*pNetworkMgr	= NULL;
  Network::ISocketTCP		*pTCP			= NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーにTCPを使って接続する
  pTCP = pNetworkMgr->ConnectByTCP( "127.0.0.1", 80 );
  if ( pTCP == NULL )
  {
    // エラーメッセージ
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  const Uint32 MAX_PACKET_SIZE = 256;
  Sint8 PacketBuffer[MAX_PACKET_SIZE];

  // データ送信
  if ( !pTCP->Send( PacketBuffer, sizeof(PacketBuffer) ) )
  {
    // エラーメッセージ
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // TCP接続を解放
  SAFE_RELEASE( pTCP );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Bool Send( const void *pData, Sint32 Size ) = 0;

/**
@brief データ受信
@author 葉迩倭
@param pData	[in] データ格納バッファ
@param Size		[in] 受信サイズ
@retval false	失敗
@retval true	成功

@note
データの受信を行います。<BR>
指定サイズの受信が完了するまで関数から抜けてきません。

@code

using namespace Selene;

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
  Network::INetworkManager	*pNetworkMgr	= NULL;
  Network::ISocketTCP		*pTCP			= NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーにTCPを使って接続する
  pTCP = pNetworkMgr->ConnectByTCP( "127.0.0.1", 80 );
  if ( pTCP == NULL )
  {
    // エラーメッセージ
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  const Uint32 MAX_PACKET_SIZE = 256;
  Sint8 PacketBuffer[MAX_PACKET_SIZE];

  // データ受信
  if ( !pTCP->Recv( PacketBuffer, sizeof(PacketBuffer) ) )
  {
    // エラーメッセージ
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // TCP接続を解放
  SAFE_RELEASE( pTCP );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}
@endcode
*/
		virtual Bool Recv( void *pData, Sint32 Size ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Network
{
	typedef void (*ClientControlCallback)( IServerClientTCP *pClient );		///< クライアント制御用

	/**
		@brief TCP接続サーバークラス
		@author 葉迩倭

		@note
		Seleneで使用するネットワーク処理を行います。
	*/
	class IServerTCP : public IInterface
	{
	public:
		virtual ~IServerTCP() {}

/**
@brief サーバー機能を開始
@author 葉迩倭
@param pCallback	[in] クライアント処理用コールバック関数

@note
サーバー機能を開始し、クライアントからの接続を待ちます。<BR>
クライアントが接続されるたびにスレッドが作られ、その中からpCallbackで<BR>
指定した関数に処理がきます。

@code

using namespace Selene;

static void ClientRecieve( Network::IServerClientTCP *pClient );

int main()
{
  Network::INetworkManager  *pNetworkMgr  = NULL;
  Network::IServerTCP      *pServer    = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーを生成する
  pServer = pNetworkMgr->CreateServer( 80, 16 );
  if ( pServer == NULL )
  {
    // 失敗
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // サーバー処理開始
  pServer->Start( ClientRecieve );

  // キー入力で終了
  getchar();

  // サーバーを解放
  SAFE_RELEASE( pServer );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}

void ClientRecieve( Network::IServerClientTCP *pClient )
{
  // 接続元クライアント情報
  Network::SHostInfo HostInfo;
  pClient->GetHostInfo( HostInfo );

  // クライアント用メイン通信ループ
  for ( ; ; )
  {
  }
}
@endcode
*/
		virtual Bool Start( ClientControlCallback pCallback ) = 0;

/**
@brief データ送信
@author 葉迩倭
@param pData	[in] データ格納バッファ
@param Size		[in] 送信サイズ
@retval false	失敗
@retval true	成功

@note
接続されている全てのクライアントにデータの送信を行います。<BR>
指定サイズの送信が完了するまで関数から抜けてきません。

@code

using namespace Selene;

static void ClientRecieve( Network::IServerClientTCP *pClient );

int main()
{
  Network::INetworkManager  *pNetworkMgr  = NULL;
  Network::IServerTCP      *pServer    = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーを生成する
  pServer = pNetworkMgr->CreateServer( 80, 16 );
  if ( pServer == NULL )
  {
    // 失敗
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // サーバー処理開始
  pServer->Start( ClientRecieve );

  // 全てのクライアントに送信
  pServer->SendAllClient( "めっせーじ", strlen("めっせーじ") + 1 );

  // キー入力で終了
  getchar();

  // サーバーを解放
  SAFE_RELEASE( pServer );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}

void ClientRecieve( Network::IServerClientTCP *pClient )
{
  // 接続元クライアント情報
  Network::SHostInfo HostInfo;
  pClient->GetHostInfo( HostInfo );

  // クライアント用メイン通信ループ
  for ( ; ; )
  {
  }
}
@endcode
*/
		virtual Bool SendAllClient( const void *pData, Sint32 Size ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Network
{
	/**
		@brief TCP接続サーバーに接続されたクライアントクラス
		@author 葉迩倭

		@note
		Seleneで使用するネットワーク処理を行います。
	*/
	class IServerClientTCP : public IInterface
	{
	public:
		virtual ~IServerClientTCP() {}

/**
@brief データ送信
@author 葉迩倭
@param pData	[in] データ格納バッファ
@param Size		[in] 送信サイズ
@retval false	失敗
@retval true	成功

@note
データの送信を行います。<BR>
指定サイズの送信が完了するまで関数から抜けてきません。

@code

using namespace Selene;

static void ClientRecieve( Network::IServerClientTCP *pClient );

int main()
{
  Network::INetworkManager  *pNetworkMgr  = NULL;
  Network::IServerTCP      *pServer    = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーを生成する
  pServer = pNetworkMgr->CreateServer( 80, 16 );
  if ( pServer == NULL )
  {
    // 失敗
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // サーバー処理開始
  pServer->Start( ClientRecieve );

  // キー入力で終了
  getchar();

  // サーバーを解放
  SAFE_RELEASE( pServer );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}

void ClientRecieve( Network::IServerClientTCP *pClient )
{
  // 接続元クライアント情報
  Network::SHostInfo HostInfo;
  pClient->GetHostInfo( HostInfo );

  // クライアント用メイン通信ループ
  for ( ; ; )
  {
    const Uint32 MAX_PACKET_SIZE = 256;
    Sint8 PacketBuffer[MAX_PACKET_SIZE];

    // クライアントへパケットを送る
    if ( !pClient->Send( PacketBuffer, sizeof(PacketBuffer) ) )
    {
      // エラーor切断
      return;
    }
  }
}
@endcode
*/
		virtual Bool Send( const void *pData, Sint32 Size ) = 0;

/**
@brief データ受信
@author 葉迩倭
@param pData	[in] データ格納バッファ
@param Size		[in] 受信サイズ
@retval false	失敗
@retval true	成功

@note
データの受信を行います。<BR>
指定サイズの受信が完了するまで関数から抜けてきません。

@code

using namespace Selene;

static void ClientRecieve( Network::IServerClientTCP *pClient );

int main()
{
  Network::INetworkManager  *pNetworkMgr  = NULL;
  Network::IServerTCP      *pServer    = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーを生成する
  pServer = pNetworkMgr->CreateServer( 80, 16 );
  if ( pServer == NULL )
  {
    // 失敗
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // サーバー処理開始
  pServer->Start( ClientRecieve );

  // キー入力で終了
  getchar();

  // サーバーを解放
  SAFE_RELEASE( pServer );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}

void ClientRecieve( Network::IServerClientTCP *pClient )
{
  // 接続元クライアント情報
  Network::SHostInfo HostInfo;
  pClient->GetHostInfo( HostInfo );

  // クライアント用メイン通信ループ
  for ( ; ; )
  {
    const Uint32 MAX_PACKET_SIZE = 256;
    Sint8 PacketBuffer[MAX_PACKET_SIZE];

    // クライアントからのパケットを待つ
    if ( !pClient->Recv( PacketBuffer, sizeof(PacketBuffer) ) )
    {
      // エラーor切断
      return;
    }
  }
}
@endcode
*/
		virtual Bool Recv( void *pData, Sint32 Size ) = 0;

/**
@brief 接続されたクライアントの情報取得
@author 葉迩倭
@param Info	[in] クライアント情報

@note
接続されたクライアントの情報を取得します。

@code

using namespace Selene;

static void ClientRecieve( Network::IServerClientTCP *pClient );

int main()
{
  Network::INetworkManager  *pNetworkMgr  = NULL;
  Network::IServerTCP      *pServer    = NULL;

  // システムの初期化
  System::Initialize();

  // ネットワークマネージャーの生成
  pNetworkMgr = Network::CreateManager();

  // サーバーを生成する
  pServer = pNetworkMgr->CreateServer( 80, 16 );
  if ( pServer == NULL )
  {
    // 失敗
    ::MessageBox( NULL, pNetworkMgr->GetLastErrorString(), "ERROR", MB_ICONERROR );
  }

  // サーバー処理開始
  pServer->Start( ClientRecieve );

  // キー入力で終了
  getchar();

  // サーバーを解放
  SAFE_RELEASE( pServer );

  // ネットワークマネージャーの解放
  SAFE_RELEASE( pNetworkMgr );

  // システムの解放
  System::Finalize();

  return 0;
}

void ClientRecieve( Network::IServerClientTCP *pClient )
{
  // 接続元クライアント情報
  Network::SHostInfo HostInfo;
  pClient->GetHostInfo( HostInfo );

  // クライアント用メイン通信ループ
  for ( ; ; )
  {
    const Uint32 MAX_PACKET_SIZE = 256;
    Sint8 PacketBuffer[MAX_PACKET_SIZE];

    // クライアントからのパケットを待つ
    if ( !pClient->Recv( PacketBuffer, sizeof(PacketBuffer) ) )
    {
      // エラーor切断
      return;
    }
  }
}
@endcode
*/
		virtual void GetHostInfo( SHostInfo &Info ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace File
{
	/**
		@brief ファイル管理クラス
		@author 葉迩倭

		Seleneで使用するファイルの管理を行います。
	*/
	class IFileManager
	{
	public:
		virtual ~IFileManager() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief ファイルロードパス設定
			@author 葉迩倭
			@param Priority		[in] 検索プライオリティ
			@param pRootPath	[in] ルートディレクトリ
			@param pPassword	[in] パックファイルの場合のパスワード（ない場合はNULL）

			ファイルの読み込みを行うルートのディレクトリを設定します。<BR>
			ここで設定された パス および パス.Pack ファイルは同列に扱われます。<BR>
			つまりプログラムの変更なく双方にまったく同じようにアクセス可能です。
		*/
		virtual void SetRootPath( Sint32 Priority, const char *pRootPath, const char *pPassword = NULL ) = 0;

		/**
			@brief カレントディレクトリ設定
			@author 葉迩倭
			@param pCurrentDir	[in] ディレクトリ名

			ファイル検索を行う際のカレントのディレクトリを設定します。<BR>
			ここで設定されたディレクトリをルートとしてファイルの検索を行います。<BR>
			<BR>
			SetRootPath( 0, "Data", "Data.Pack" ); という設定が行われいて、<BR>
			SetCurrentPath( "texture" ); となっているとき「sample.bmp」と指定して読み込みした場合<BR>
			<BR>
			「Data\texture\sample.bmp」を探しに行き、見つからない場合は<BR>
			「Data.Pack」ファイル内の「texture\sample.bmp」ファイルを探しに行きます。
		*/
		virtual void SetCurrentPath( const char *pCurrentDir ) = 0;

		/**
			@brief ファイルロード
			@author 葉迩倭
			@param pFile	[in] ファイル名
			@param ppData	[out] ファイルデータ格納先
			@param pSize	[out] ファイルサイズ格納先
			@retval true	成功
			@retval false	失敗

			ファイルをロードし、メモリに展開します。
		*/
		virtual Bool Load( const char *pFile, void **ppData, Sint32 *pSize ) = 0;

		/**
			@brief データ解放
			@author 葉迩倭
			@param pData	[in] データ

			Load()関数で取得したデータをメモリから解放します。<BR>
			この関数を使った方法以外での解放は環境依存するため、<BR>
			正しく解放されない可能性があります。
		*/
		virtual void Free( void *pData ) = 0;

		/**
			@brief ファイルオープン
			@author 葉迩倭
			@param pFile		[in] ファイル名
			@retval NULL		失敗
			@retval NULL以外	ファイルインターフェイス

			リソースファイルをオープンします。<BR>
			ここでいうリソースファイルはゲームで使われるデータ全般の事です。<BR>
			つまりパックファイルの内容、およびルートディレクトリ以下のデータです。<BR>
			SetRootPath()で指定されている[フォルダ]→[パックファイル]の<BR>
			順にファイルを検索します。<BR>
			オープンしたファイルはパックファイルであっても、<BR>
			通常のファイルと同じようにアクセスすることが出来ます。<BR>
			またこの関数から取得したIFIleインターフェイスは読み取り専用です。<BR>
			使用が終了したIFileインターフェイスはRelease()で解放してください。
		*/
		virtual IResourceFile *FileOpen( const char *pFile ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace File
{
	/**
		@brief リソースファイル操作
		@author 葉迩倭
	*/
	class IResourceFile
	{
	public:
		virtual ~IResourceFile() {};

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void	) = 0;

		/**
			@brief ファイルへの読み込み
			@author 葉迩倭
			@param pData	[in] 読み込みデータ
			@param Size		[in] 読み込みデータサイズ
			@return 実際に書き込んだバイト数

			ファイルへの読み込みを行います。<BR>
			読み込み可能なファイルはインターフェイス生成時に<BR>
			FILE_OPEN_TYPE_READかFILE_OPEN_TYPE_READ_WRITEフラグを<BR>
			指定する必要があります。
		*/
		virtual Sint32 Read( void *pData, Sint32 Size ) = 0;

		/**
			@brief ファイル名取得
			@author 葉迩倭
			@return ファイル名の先頭アドレス

			ファイル名の先頭アドレスを取得します。
		*/
		virtual const char *GetNamePointer( void ) = 0;

		/**
			@brief ファイルサイズ取得
			@author 葉迩倭
			@return ファイルサイズ

			ファイルのサイズを取得します。
		*/
		virtual Sint32 GetFileSize( void ) = 0;

		/**
			@brief ファイルポインター位置取得
			@author 葉迩倭
			@return ファイルポインターの位置

			現在のファイルポインタの位置を取得します。
		*/
		virtual Sint32 GetFilePosition( void ) = 0;

		/**
			@brief ファイルシーク
			@author 葉迩倭
			@param Offset	[in] 移動量
			@return ファイルポインターの位置

			ファイルポインターの位置をファイルの先頭からOffsetバイト移動します。
		*/
		virtual Sint32 SeekStart( Sint32 Offset ) = 0;

		/**
			@brief ファイルシーク
			@author 葉迩倭
			@param Offset	[in] 移動量
			@return ファイルポインターの位置

			ファイルポインターの位置をファイルの終端からOffsetバイト移動します。
		*/
		virtual Sint32 SeekEnd( Sint32 Offset ) = 0;

		/**
			@brief ファイルシーク
			@author 葉迩倭
			@param Offset	[in] 移動量
			@return ファイルポインターの位置

			ファイルポインターの位置を現在の位置からOffsetバイト移動します。
		*/
		virtual Sint32 Seek( Sint32 Offset ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Peripheral
{
	/**
		@brief マウス管理インターフェイス
		@author 葉迩倭

		マウスを扱うためのインターフェイスです。
	*/
	class IMouse
	{
	public:
		virtual ~IMouse() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief マウスのX座標取得
			@author 葉迩倭
			@return X座標

			マウスポインターのスクリーン座標上のX座標を取得します。
		*/
		virtual Sint32 GetPosX( void ) = 0;

		/**
			@brief マウスのY座標取得
			@author 葉迩倭
			@return Y座標

			マウスポインターのスクリーン座標上のY座標を取得します。
		*/
		virtual Sint32 GetPosY( void ) = 0;

		/**
			@brief マウスのホイール回転量取得
			@author 葉迩倭
			@return ホイール回転量

			マウスホイールの回転量を取得します。
		*/
		virtual Sint32 GetPosW( void ) = 0;

		/**
			@brief マウスのX移動量取得
			@author 葉迩倭
			@return X移動量

			マウスポインターのスクリーン上のX移動量を取得します。
		*/
		virtual Sint32 GetMoveX( void ) = 0;

		/**
			@brief マウスのY移動量取得
			@author 葉迩倭
			@return Y移動量

			マウスポインターのスクリーン上のY移動量を取得します。
		*/
		virtual Sint32 GetMoveY( void ) = 0;

		/**
			@brief マウスのホイール移動量取得
			@author 葉迩倭
			@return ホイール移動量

			マウスポインターのホイール移動量を取得します。
		*/
		virtual Sint32 GetMoveW( void ) = 0;

		/**
			@brief マウスの左クリック状態取得
			@author 葉迩倭
			@retval false	左ボタンは押されていない
			@retval true	左ボタンは押されている

			マウスの左ボタンの状態を取得します。<BR>
			この関数ではON/OFFしか取得できません。
		*/
		virtual Bool GetClickL( void ) = 0;

		/**
			@brief マウスの右クリック状態取得
			@author 葉迩倭
			@retval false	右ボタンは押されていない
			@retval true	右ボタンは押されている

			マウスの右ボタンの状態を取得します。<BR>
			この関数ではON/OFFしか取得できません。
		*/
		virtual Bool GetClickR( void ) = 0;

		/**
			@brief マウスのホイールクリック状態取得
			@author 葉迩倭
			@retval false	ホイールボタンは押されていない
			@retval true	ホイールボタンは押されている

			マウスのホイールボタンの状態を取得します。<BR>
			この関数ではON/OFFしか取得できません。
		*/
		virtual Bool GetClickW( void ) = 0;

		/**
			@brief マウスの左ダブルクリック状態取得
			@author 葉迩倭
			@retval false	左ボタンはダブルクリックされていない
			@retval true	左ボタンはダブルクリックされた

			マウスの左ボタンがダブルクリックされたかを取得します。<BR>
			この関数ではON/OFFしか取得できません。
		*/
		virtual Bool GetDoubleClickL( void ) = 0;

		/**
			@brief マウスの右ダブルクリック状態取得
			@author 葉迩倭
			@retval false	右ボタンはダブルクリックされていない
			@retval true	右ボタンはダブルクリックされた

			マウスの右ボタンがダブルクリックされたかを取得します。<BR>
			この関数ではON/OFFしか取得できません。
		*/
		virtual Bool GetDoubleClickR( void ) = 0;

		/**
			@brief マウスのホイールダブルクリック状態取得
			@author 葉迩倭
			@retval false	ホイールボタンはダブルクリックされていない
			@retval true	ホイールボタンはダブルクリックされた

			マウスのホイールボタンがダブルクリックされたかを取得します。<BR>
			この関数ではON/OFFしか取得できません。
		*/
		virtual Bool GetDoubleClickW( void ) = 0;

		/**
			@brief マウスの左ボタン状態取得
			@author 葉迩倭
			@return		左ボタンの状態

			マウスの左ボタンの詳細な情報を取得します。
		*/
		virtual eMouseState GetStateL( void ) = 0;

		/**
			@brief マウスの右ボタン状態取得
			@author 葉迩倭
			@return		右ボタンの状態

			マウスの右ボタンの詳細な情報を取得します。
		*/
		virtual eMouseState	GetStateR( void ) = 0;

		/**
			@brief マウスのホイールボタン状態取得
			@author 葉迩倭
			@return		ホイールボタンの状態

			マウスのホイールボタンの詳細な情報を取得します。
		*/
		virtual eMouseState	GetStateW( void ) = 0;

		/**
			@brief マウスの位置を設定
			@author 葉迩倭
			@param Pos	[in] スクリーン座標

			マウスの位置を任意の場所に設定します。
		*/
		virtual void SetPosition( Math::Point2DI &Pos ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Peripheral
{
	/**
		@brief キーボード管理インターフェイス
		@author 葉迩倭

		キーボードを扱うためのインターフェイスです。
	*/
	class IKeyboard
	{
	public:
		virtual ~IKeyboard() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief キー入力バッファフラッシュ
			@author 葉迩倭

			キー入力バッファに蓄積した入力バッファをクリアします。
		*/
		virtual void ClearKeyBuffer( void ) = 0;

		/**
			@brief キー入力バッファからデータ取得
			@author 葉迩倭
			@return 入力されたキーのキーコード

			キー入力バッファに蓄積されたデータを取り出します。<BR>
			押されたキーを全て取り出す時はwhile()文等で処理してください。
		*/
		virtual eVirtualKeyCode GetKeyBuffer( void ) = 0;

		/**
			@brief キーが押されているかチェックする
			@author 葉迩倭
			@retval false	押されていない
			@retval true	押されている

			指定されたキーが現在押されているかチェックします。<BR>
			純粋にキーのON/OFFのみ取得できます。
		*/
		virtual Bool GetKeyData( eVirtualKeyCode Key ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Peripheral
{
	/**
		@brief ジョイスティック管理インターフェイス
		@author 葉迩倭

		ジョイスティックを扱うためのインターフェイスです。
	*/
	class IJoystick
	{
	public:
		virtual ~IJoystick() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief 軸情報を取得
			@author 葉迩倭
			@param Type		[in] 軸の種類
			@param Dir		[in] 軸の方向
			@return 軸の現在値

			指定された種類、方向の軸の状態を取得します。<BR>
			アナログスティックの場合は-4096～+4096の値が返り、<BR>
			デジタルスティックの場合は-4096か+4096の値が返ります。
		*/
		virtual Sint32 GetAxis( ePadAxisType Type, ePadAxisDirection Dir ) = 0;

		/**
			@brief スライダー情報を取得
			@author 葉迩倭
			@param Type		[in] スライダーのタイプ
			@return スライダーの現在値

			指定された種類のスライダーの状態を取得します。<BR>
			-4096～+4096の値が返ります。
		*/
		virtual Sint32 GetSlider( ePadSliderType Type ) = 0;

		/**
			@brief ボタン情報を取得
			@author 葉迩倭
			@param Type		[in] ボタンのタイプ
			@retval false	押されてない
			@retval true	押されている

			指定された種類のボタンの状態を取得します。
		*/
		virtual Bool GetButton( ePadButtonType Type ) = 0;
	};
}
}

#pragma once


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Peripheral
{
	/**
		@brief 入力デバイス管理クラス
		@author 葉迩倭

		キーボードとパッドを統一して扱うためのインターフェイスです。
	*/
	class IInputController
	{
	public:
		virtual ~IInputController() {}

		/**
			@brief 有効性チェック
			@author 葉迩倭
			@retval true 無効
			@retval false 有効

			インターフェイスが有効か無効かを調べます。
		*/
		virtual Bool IsInvalid( void ) = 0;

		/**
			@brief 参照カウンタデクリメント
			@author 葉迩倭
			@return デクリメント後の参照カウント

			参照カウンタをデクリメントし、<BR>
			参照カウントが0になった時点でメモリから解放します。
		*/
		virtual Sint32 Release( void ) = 0;

		/**
			@brief 参照カウンタインクリメント
			@author 葉迩倭
			@return インクリメント後の参照カウント

			参照カウンタをインクリメントします。<BR>
			インクリメントした参照カウントはRelease()を呼ぶことによりデクリメントされます。<BR>
			AddRef()をしたインターフェイスは必ずRelease()で解放してください。
		*/
		virtual Sint32 AddRef( void ) = 0;

		/**
			@brief 入力情報更新
			@author 葉迩倭

			入力情報から最新の状態へ更新します。<BR>
			必ず１フレームに１回呼ぶようにして下さい。
		*/
		virtual void Refresh( void ) = 0;

		/**
			@brief X軸入力設定
			@author 葉迩倭
			@param Type		[in] 入力管理用の軸タイプ
			@param AxisX	[in] 適用するジョイスティックの軸
			@param AxisDirX	[in] 適用するジョイスティックの軸方向

			ジョイスティックの軸データを入力管理用のX軸へマッピングします。
		*/
		virtual void SetAxisX( eInputAxisType Type, ePadAxisType AxisX, ePadAxisDirection AxisDirX ) = 0;

		/**
			@brief Y軸入力設定
			@author 葉迩倭
			@param Type		[in] 入力管理用の軸タイプ
			@param AxisY	[in] 適用するジョイスティックの軸
			@param AxisDirY	[in] 適用するジョイスティックの軸方向

			ジョイスティックの軸データを入力管理用のY軸へマッピングします。
		*/
		virtual void SetAxisY( eInputAxisType Type, ePadAxisType AxisY, ePadAxisDirection AxisDirY ) = 0;

		/**
			@brief ボタン入力設定
			@author 葉迩倭
			@param Type		[in] 入力管理用のボタンタイプ
			@param Button	[in] 適用するジョイスティックのボタン

			ジョイスティックのボタンデータを入力管理用のボタンへマッピングします。
		*/
		virtual void SetButton( eInputButtonType Type, ePadButtonType Button ) = 0;

		/**
			@brief キーボード経由入力設定
			@author 葉迩倭
			@param Type		[in] 入力管理用のボタンタイプ
			@param Key		[in] 適用するキーボードのキー

			キーボードのキーをジョイスティックの別名として入力管理用のボタンへマッピングします。
		*/
		virtual void SetAlias( eInputButtonType Type, eVirtualKeyCode Key ) = 0;

		/**
			@brief どれか１つでもボタンが押されているかチェック
			@author 葉迩倭
			@retval false	ボタンは１つも押されていない
			@retval true	ボタンは１つ以上押されている

			なんらかのボタンが押されているかどうかをチェックします。
		*/
		virtual Bool IsPushAnyKey( void ) = 0;

		/**
			@brief ボタンの状態を初期化
			@author 葉迩倭

			すべてのステートをOFF状態にします。
		*/
		virtual void ClearState( void ) = 0;

		/**
			@brief ボタンの状態を取得
			@author 葉迩倭
			@param PadState	[in] 取得する状態
			@param Type		[in] 取得するボタン
			@retval false	ボタンは条件を満たしていない
			@retval true	ボタンは条件を満たしている

			ボタンTypeが状態PadStateのときにtrueが返ります。<BR>
			キーリピートを設定している場合は、指定間隔毎にBUTTON_STATE_PUSH<BR>
			状態に自動的になります。
		*/
		virtual Bool GetState( eInputButtonState PadState, eInputButtonType Type ) = 0;

		/**
			@brief ボタンの状態を設定
			@author 葉迩倭
			@param PadState	[in] 設定する状態
			@param Type		[in] 設定するボタン

			ボタンTypeを状態PadStateに設定します。<BR>
			外部からキーを操作し、自動進行などを行わせることが可能です。
		*/
		virtual void SetState( eInputButtonState PadState, eInputButtonType Type ) = 0;

		/**
			@brief カーソル上下移動
			@author 葉迩倭
			@param AxisNo		[in] 使用する軸番号
			@param Cursor		[in,out] カーソル
			@param Max			[in] カーソルのとりうる最大値
			@retval BUTTON_DISABLE			ボタンは押されていない
			@retval BUTTON_DISABLE以外		ボタンは上or下が押されている

			指定した軸に対してカーソルを上下に動かします。<BR>
			またこの時にカーソルはリピートされます。
		*/
		virtual eInputButtonType CursorRepeatUpDown( Sint32 AxisNo, Sint32 &Cursor, Sint32 Max ) = 0;

		/**
			@brief カーソル左右移動
			@author 葉迩倭
			@param AxisNo		[in] 使用する軸番号
			@param Cursor		[in,out] カーソル
			@param Max			[in] カーソルのとりうる最大値
			@retval BUTTON_DISABLE			ボタンは押されていない
			@retval BUTTON_DISABLE以外		ボタンは左or右が押されている

			指定した軸に対してカーソルを左右に動かします。<BR>
			またこの時にカーソルはリピートされます。
		*/
		virtual eInputButtonType CursorRepeatLeftRight( Sint32 AxisNo, Sint32 &Cursor, Sint32 Max ) = 0;

		/**
			@brief カーソル上下移動
			@author 葉迩倭
			@param AxisNo		[in] 使用する軸番号
			@param Cursor		[in,out] カーソル
			@param Max			[in] カーソルのとりうる最大値
			@retval BUTTON_DISABLE			ボタンは押されていない
			@retval BUTTON_DISABLE以外		ボタンは上or下が押されている

			指定した軸に対してカーソルを上下に動かします。<BR>
			またこの時にカーソルはクランプされます。
		*/
		virtual eInputButtonType CursorClampUpDown( Sint32 AxisNo, Sint32 &Cursor, Sint32 Max ) = 0;

		/**
			@brief カーソル左右移動
			@author 葉迩倭
			@param AxisNo		[in] 使用する軸番号
			@param Cursor		[in,out] カーソル
			@param Max			[in] カーソルのとりうる最大値
			@retval BUTTON_DISABLE			ボタンは押されていない
			@retval BUTTON_DISABLE以外		ボタンは左or右が押されている

			指定した軸に対してカーソルを左右に動かします。<BR>
			またこの時にカーソルはクランプされます。
		*/
		virtual eInputButtonType CursorClampLeftRight( Sint32 AxisNo, Sint32 &Cursor, Sint32 Max ) = 0;

		/**
			@brief キーリピート処理の時間設定を更新
			@author 葉迩倭
			@param KeyRepeatStartTime	[in] キーリピート開始フレーム
			@param KeyRepeatInterval	[in] キーリピート間隔フレーム

			キーリーピートに関する処理の更新をします。
		*/
		virtual void UpdateRepeatParameter( Sint32 KeyRepeatStartTime, Sint32 KeyRepeatInterval ) = 0;
	};
}
}

#pragma once


/**
	@file
	@brief サウンドインターフェイス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Sound
{
	/**
		@brief サウンド管理クラス
		@author 葉迩倭

		@note
		サウンドの処理を行うクラスです。<BR>
		未圧縮Waveのみのオンメモリ多重再生に対応しています。<BR>
		SEの再生用途を想定しています。
	*/
	class ISound : public IInterface
	{
	public:
		virtual ~ISound() {}

/**
@brief 再生
@author 葉迩倭
@param Layer	[in] レイヤー番号（-1指定で全レイヤーに対して）
@param IsLoop	[in] ループ有無フラグ

@note
読み込んだサウンドデータの再生を開始します。<BR>
ループの有無を指定できます。

@code
ISound *pSnd;
// 同一サウンドであってもLayerが違うものは多重再生が出来る。
// 下の３つは10ms毎に多重再生される。
pSnd->Play( 0, false );
Sleep( 10 );
pSnd->Play( 1, false );
Sleep( 10 );
pSnd->Play( 2, false );
@endcode
*/
		virtual void Play( Sint32 Layer, Bool IsLoop = false ) = 0;

/**
@brief 停止
@author 葉迩倭
@param Layer	[in] レイヤー番号（-1指定で全レイヤーに対して）

@note
再生中のサウンドを停止します。

@code
ISound *pSnd;
// 同一ファイル=サウンドであってもLayerが違えば別のものとして扱われる
pSnd->Play( 0, false );
Sleep( 10 );
pSnd->Play( 1, false );
Sleep( 10 );
// この場合Layer=0のサウンドは停止するが、Layer=1のサウンドは再生されたままになる
pSnd->Stop( 0, false );
@endcode
*/
		virtual void Stop( Sint32 Layer ) = 0;

/**
@brief 一時停止/解除
@author 葉迩倭
@param Layer	[in] レイヤー番号（-1指定で全レイヤーに対して）

@note
再生中のサウンドを一時停止、<BR>
一時停止中のサウンドを再生します。

@code
ISound *pSnd;
// 同一ファイル=サウンドであってもLayerが違えば別のものとして扱われる
pSnd->Play( 0, false );
Sleep( 10 );
pSnd->Play( 1, false );
Sleep( 10 );
// この場合Layer=0のサウンドは一時停止するが、Layer=1のサウンドは再生されたままになる
pSnd->Pause( 0, false );
@endcode
*/
		virtual void Pause( Sint32 Layer ) = 0;

/**
@brief ボリューム変更
@author 葉迩倭
@param Layer	[in] レイヤー番号（-1指定で全レイヤーに対して）
@param fVolume	[in] ボリューム（0～100%）

@note
ボリュームの変更を行います。

@code
ISound *pSnd;
// 同一ファイル=サウンドであってもLayerが違えば別のものとして扱われる
pSnd->Play( 0, false );
Sleep( 10 );
pSnd->Play( 1, false );
Sleep( 10 );
// この場合Layer=0のサウンドは音量が半分になるが、Layer=1のサウンドはそのままになる
pSnd->SetVolume( 0, 50.0f );
@endcode
*/
		virtual void SetVolume( Sint32 Layer, Float fVolume ) = 0;

/**
@brief パン移動
@author 葉迩倭
@param Layer	[in] レイヤー番号（-1指定で全レイヤーに対して）
@param fPan	[in] パン（左:-100～右:+100）

@note
パンの移動を行います。

@code
ISound *pSnd;
// 同一ファイル=サウンドであってもLayerが違えば別のものとして扱われる
pSnd->Play( 0, false );
Sleep( 10 );
pSnd->Play( 1, false );
Sleep( 10 );
// この場合Layer=0のサウンドは左からのみ鳴るが、Layer=1のサウンドはそのままになる
pSnd->SetPan( 0, -100.0f );
@endcode
*/
		virtual void SetPan( Sint32 Layer, Float fPan ) = 0;

/**
@brief 再生チェック
@author 葉迩倭
@param Layer	[in] レイヤー番号
@retval false	再生していない
@retval true	再生している

@note
現在再生中のサウンドかどうか調べます。

@code
ISound *pSnd;
// サウンドのLayer=1が再生中か調べる
if ( pSnd->IsPlay( 1 ) )
{
	// 再生中
}
@endcode
*/
		virtual Bool IsPlay( Sint32 Layer ) = 0;
	};
}
}
#pragma once


/**
	@file
	@brief ストリームサウンドインターフェイス
	@author 葉迩倭
*/


//-----------------------------------------------------------------------------------
// INCLUDE
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// NAMESPACE
//-----------------------------------------------------------------------------------
namespace Selene
{
namespace Sound
{
	static const Float PLAY_TIME_AUTO = -1.0f;

	/**
		@brief ストリームサウンド再生パラメーター
		@author 葉迩倭

		@note
		ストリームサウンドの再生を行うためのテーブルデータです。
	*/
	struct SPlayBlock
	{
		Sint32 LoopCount;		// このテーブルのループ数（-1で無限ループ）
		Float fStartTime;		// 再生開始位置（秒指定）（PLAY_TIME_AUTO指定で最初から）
		Float fEndTime;			// 再生終了位置（秒指定）（PLAY_TIME_AUTO指定で最後まで）
	};

	/**
		@brief ストリームサウンド管理クラス
		@author 葉迩倭

		@note
		ストリームサウンドの処理を行うクラスです。<BR>
		未圧縮Wave/圧縮Wave/OggVorbisの再生に対応しています。<BR>
		BGMや音声の再生用途を想定しています。
	*/
	class IStreamSound : public IInterface
	{
	public:
		virtual ~IStreamSound() {}

/**
@brief 再生
@author 葉迩倭
@param pTbl			[in] 再生テーブル
@param Count		[in] 再生テーブル数
@retval false		処理に失敗
@retval true		処理に成功

@note
読み込んだサウンドデータの再生を開始します。<BR>
再生テーブルの定義にそって再生されていきます。

@code
IStreamSound *pSnd;
// 再生テーブル
Sound::SPlayBlock Tbl[] = {
  {
    3,				// 3回
    0.0f,			// 0.0秒から
    5.0f,			// 5.0秒まで
  },
};
pSnd->Play( Tbl, sizeof(Tbl) / sizeof(Sound::SPlayBlock) );
@endcode
*/
		virtual Bool Play( SPlayBlock *pTbl, Sint32 Count ) = 0;

/**
@brief 再生
@author 葉迩倭
@param LoopCount	[in] ループ数(-1で無限)
@retval false		処理に失敗
@retval true		処理に成功

@note
読み込んだサウンドデータの再生を開始します。

@code
IStreamSound *pSnd;
// 普通に1回再生
pSnd->Play();
@endcode
*/
		virtual Bool Play( Sint32 LoopCount = 0 ) = 0;

/**
@brief 停止
@author 葉迩倭

@retval false		処理に失敗
@retval true		処理に成功

@note
再生中のサウンドを停止します。

@code
IStreamSound *pSnd;
// 再生停止
pSnd->Stop();
@endcode
*/
		virtual Bool Stop( void ) = 0;

/**
@brief 一時停止/解除
@author 葉迩倭

@retval false		処理に失敗
@retval true		処理に成功

@note
再生中のサウンドを一時停止、<BR>
一時停止中のサウンドを再生します。

@code
IStreamSound *pSnd;
// 一時停止
pSnd->Pause();
@endcode
*/
		virtual Bool Pause( void ) = 0;

/**
@brief ボリューム変更
@author 葉迩倭
@param fVolume	[in] ボリューム（0～100）
@retval false		処理に失敗
@retval true		処理に成功

@note
ボリュームの変更を行います。

@code
IStreamSound *pSnd;
// ボリューム最大に
pSnd->SetVolume( 100.0f );
@endcode
*/
		virtual Bool SetVolume( Float fVolume ) = 0;

/**
@brief パン移動
@author 葉迩倭
@param fPan	[in] パン（-100～+100）
@retval false		処理に失敗
@retval true		処理に成功

@note
パンの移動を行います。

@code
IStreamSound *pSnd;
// 右からだけ音を鳴らす
pSnd->SetPan( +100.0f );
@endcode
*/
		virtual Bool SetPan( Float fPan ) = 0;

/**
@brief 再生チェック
@author 葉迩倭
@retval false	再生していない
@retval true	再生している

@note
現在再生中のサウンドかどうか調べます。

@code
IStreamSound *pSnd;
// 再生中か調べる
if ( !pSnd->IsPlay() )
{
	// 再生終了
}
@endcode
*/
		virtual Bool IsPlay( void ) = 0;
	};
}
}

