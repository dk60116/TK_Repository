#include "CResource.h"

CResource::CResource()
	: m_bLoaded(false)
{
	m_strName = L"Resource";
}

CResource::~CResource()
{
}

void CResource::Release()
{
}
