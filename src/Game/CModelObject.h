
#pragma once

#include "IObject.h"
#include "CAnimation.h"
#include <map>
#include <vector>

class CModelObject:public IObject
{
protected:
	std::map<ATOM,CAnimation *>	m_animations;
	CAnimation	*m_now_animation;
	CAnimation	*m_default_animation;
	std::vector<ATOM>	m_animation_list;
	ATOM		m_object_name_atom;
	ATOM		m_model_name_atom;
//	Selene::Scene::IModelActor *m_pActor;
	int			m_actor_id;
	bool		m_is_pausing;

private:
	void initialize();

public:
	CModelObject(ATOM object_name_atom);
	CModelObject(LPCTSTR object_name);
	virtual ~CModelObject();

	ATOM GetObjectNameAtom(){return m_object_name_atom;}

	// ロード系
	bool LoadModel( LPCTSTR model_name );

	// アニメーションの追加
	void AddAnimation( ATOM animation_name , CAnimation *add_animation );
	// アニメーション全削除
	void ClearAnimation();

	// 現行アニメーション設定
	bool SetAnimation( ATOM animation_name , bool will_reset = true );
	bool SetDefaultAnimation( ATOM animation_name );
	void SetAnimationToDefault( bool will_reset = true );
	// アニメーション操作
	void PauseAnimation();
	void ResumeAnimation();
	// アニメーションのゲッターは要らないかな？

	virtual void Draw();
	virtual void Release();
	virtual bool Update();
	virtual IObject *Clone();

	// 問題はコリジョン処理
	// できればコリジョン検出後の処理はスクリプト言語に任せたい
	// うどんげの処理はプログラム中に埋め込んでもいいが…敵の種類が増えるたびにプログラムを書き換えるのは面倒
};
