#include "epch.h"
#include "Object.h"
#include "Scene.h"

UObject::UObject()
	: m_iInstanceId(0)
	, m_iRefCount(0)
{
}

UObject::~UObject()
{
}

void UObject::AddRef()
{
	++m_iRefCount;
}

UINT UObject::Release()
{
    int prevRefCount = m_iRefCount;
    int refCount = --m_iRefCount;

    if (refCount == 0)
        delete this;

    return prevRefCount;
}

const UINT UObject::Get_RefCnt()
{
    return m_iRefCount;
}
