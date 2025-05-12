#include "Object.h"

atomic<UINT> UObject::s_iNextInstanceID = 1;

UObject::UObject()
    : m_iRefCount(1)
{
    m_iInstanceId = s_iNextInstanceID++;
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

void UObject::Serialize(CSerialzer& _s)
{
}
