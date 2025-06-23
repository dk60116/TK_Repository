#include "epch.h"
#include "EngineResource.h"

CResource::CResource()
{
}

CResource::~CResource()
{
}

CResource& CResource::GetInstance()
{
	static CResource inst;
	return inst;
}
