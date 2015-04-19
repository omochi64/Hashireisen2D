
#pragma once

#include <vector>
#include <map>
#include "Game/CModelObject.h"
#include "IManager.h"

#include "TinyXML/TinyXML.h"


namespace Manager
{

//#define MAX_OBJECT_NUM_IN_A_COURSE	2048

// IObjectManagerがインターフェースとしてくるかな？
// シングルトンパターン適用
class CModelObjectManager:public IManager
{
protected:
//	CModelObjectManager();
	virtual ~CModelObjectManager();
//	CModelObjectManager( const CModelObjectManager & );
//	CModelObjectManager & operator =( const CModelObjectManager & );

	// XMLパーサ
	std::map<ATOM,TiXmlDocument *>				m_XMLParser;
	// オブジェクトのマスタ
	std::map<ATOM,CModelObject *>	m_objectMaster;
	// ネームリスト
	std::vector<ATOM>				m_addedObject;
	std::vector<ATOM>				m_readObject;

private:
	bool AnalysisForAnimation(TiXmlElement *animation_root, CModelObject *object);
	bool AnalysisForMotionOfAnimation(TiXmlElement *motion_root , CAnimation::SCRIPT_ACTION &sa ); 
public:

	void Release();

	static CModelObjectManager &getInstance();

	bool AddDefinitionFile( LPCTSTR pFileName );
	bool LoadObject( ATOM name );

	CModelObject *GetOriginal( ATOM name );
	CModelObject *GetCopy( ATOM name );
};

};
