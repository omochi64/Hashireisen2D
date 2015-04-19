
#pragma once

namespace Manager
{

class IManager
{
protected:
	IManager(){}
	IManager( const IManager & ){}
	IManager &operator=(const IManager &){return *this;}
};

};