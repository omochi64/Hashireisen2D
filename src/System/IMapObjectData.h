
#pragma once

#include "AllObject.h"
#include "Selene.h"

using namespace Selene;

// �}�b�v�f�[�^�̊�b
class IMapObjectData:public AllObject
{
public:
	Scene::IModelActor	*m_pActor;

	IMapObjectData();
	virtual ~IMapObjectData();
	virtual void Release();
	virtual void Copy( IMapObjectData *copy_to );
	virtual bool Compare( IMapObjectData *object );
};
