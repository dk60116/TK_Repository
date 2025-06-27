#include "epch.h"
#include "Resources.h"

CResources::CResources()
{
}

CResources::~CResources()
{
	Release();
}

CResources& CResources::GetInstance()
{
	static CResources inst;
	return inst;
}

void CResources::Release()
{
}
