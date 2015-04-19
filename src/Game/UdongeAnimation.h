
#pragma once

#include "Game/Objects.h"
#include "System/Manager/TextureManager.h"
#include "System/Manager/MultipleAnimationManager.h"
#include "GunController.h"

#define		DEFAULT_HP			3
#define		MAX_LIFE			127
#define		DEFAULT_INITIALIZE_LIFE		3


#define MAX_HP	3

typedef struct _marisa_actions{
	int stand;
	int walk;
	int run;
	int jump;
	int sit_down;
	int game_over;
	int reduce_hp;
	int shooting_gun;
}UDONGE_ACTIONS;

typedef enum _action_type{
	STAND , WALK , RUN , JUMP , SIT_DOWN , GAME_OVER
}ACTION_TYPE;

class CMultipleAnimationManager;

class UdongeAnimation:public MultipleAnimation
{
	UDONGE_ACTIONS m_udonge_actions;
	CTextureManager *m_tex_manager;
	DWORD			m_no_enemy_time;		// 無敵時間
	DWORD			m_no_enemy_mode_rest_frame;	// 無敵期間開始時間( = 0 で無敵期間ではない)
	DWORD			m_no_enemy_mode_blink_period;	// 無敵期間の点滅間隔
	bool			m_will_draw;
	bool			m_is_shooting_gun;
	int				m_hp;
	int				m_rest_life;
	int				m_default_rest_life;
	int				m_default_hp;
	int				m_point;
	int				m_coin;

	// Gunの描画は、Udongeの描画と大きく関わるので、UdongeがGunを持っておく
	std::vector<GunController *>	m_guns;
	int				m_now_gun;
	int				m_gun_num;
	int				m_normal_gun_no;

public:
	UdongeAnimation( CTextureManager *tex_manager , int default_hp = DEFAULT_HP , int default_rest_life = DEFAULT_INITIALIZE_LIFE );
	virtual ~UdongeAnimation();
	bool LoadUdongeFromFile( char *file_name , Renderer::IRender *render , File::IFileManager *pFileManager );
	bool LoadGunFromFile( char *file_name , CMultipleAnimationManager *animation_manager , File::IFileManager *pFileManager );

	void SetAnimationToStand();
	void SetAnimationToWalk();
	void SetAnimationToRun();
	void SetAnimationToJump();
	void SetAnimationToSitDown();
	void SetAnimationToGameOver();
	void SetAnimationToReduceHP();
	void SetAnimationToShootingGun();

	const UDONGE_ACTIONS & GetAnimations();

	int GetHP( void ){return m_hp;}
	void IncreaseHP( void );	// HP増加
	bool ReduceHP( void );		// false→HPがなくなった
	void SetHPToDefault( void );
	int GetPoint( void ){return m_point;}
	void ResetPoint( void ){m_point = 0;}
	void IncreasePoint( int inc_size );
	void DecreasePoint( int dec_size );

	virtual void GetCollisionRect( Math::Rect2DF &rect ){
		MultipleAnimation::GetCollisionRect(rect);
	};

	int GetRestLife( void ){return m_rest_life;}
	void IncreaseRestLife( void ){m_rest_life++;}
	bool ReduceRestLife( void )
	{
		m_rest_life--;
		if( m_rest_life <= 0 )
		{
			return false;
		}
		return true;
	}
	void SetRestLifeToDefault( void )
	{
		m_rest_life = m_default_rest_life;
	}

	// 残機がアップするならtrueを返す
	bool IncreaseCoin( void )
	{
		if( ++m_coin >= 100 )
		{
			m_coin = 0;
			IncreaseRestLife();
			return true;
		}
		return false;
	}
	int GetCoin(){return m_coin;}
	void SetCoinToDefault(){m_coin=0;}

	bool IsNoEnemyMode( void );
	void EndNoEnemyMode( void );
	bool IsShootingGun( void );

	void ResetAllState();

	// 銃関連
	Bullet *ShootTheBullet();
	void SetNextBullet();
	void SetPreviousBullet();


	virtual void Draw();
	// アニメーションをアップデート
	virtual bool UpdateAnimation();

	virtual void Release();
};
