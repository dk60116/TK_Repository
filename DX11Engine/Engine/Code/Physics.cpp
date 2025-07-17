#include "epch.h"
#include "Physics.h"

CPhysics::CPhysics()
{
}

CPhysics::~CPhysics()
{
}

CPhysics& CPhysics::GetInstance()
{
	static CPhysics inst;

	return inst;
}
