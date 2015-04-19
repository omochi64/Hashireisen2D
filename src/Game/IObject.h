
#pragma once

// オブジェクトインターフェース
//  *インスタンスを生成できないですぜ。と言うより、全てabstractですぜ。

#include "Selene.h"

class IObject
{
public:
	IObject(){}
	virtual ~IObject(){}

	virtual void Release()=0;
	virtual void Draw()=0;
	virtual bool Update(/*実際はここに引数が来ると思うよ！*/) = 0;
	virtual IObject *Clone()=0;
	
	// コリジョン系、可視判定系が追加されるかな？
};
