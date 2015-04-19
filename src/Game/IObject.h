
#pragma once

// �I�u�W�F�N�g�C���^�[�t�F�[�X
//  *�C���X�^���X�𐶐��ł��Ȃ��ł����B�ƌ������A�S��abstract�ł����B

#include "Selene.h"

class IObject
{
public:
	IObject(){}
	virtual ~IObject(){}

	virtual void Release()=0;
	virtual void Draw()=0;
	virtual bool Update(/*���ۂ͂����Ɉ���������Ǝv����I*/) = 0;
	virtual IObject *Clone()=0;
	
	// �R���W�����n�A������n���ǉ�����邩�ȁH
};
