#pragma once

#include "Engine_Define.h"
#include "Serialzer.h"

BEGIN(Engine)

class ENGINE_DLL UObject abstract
{
public:
	UObject();
	virtual ~UObject();

	UINT GetInstanceID() const { return m_iInstanceId; }
	const wstring& getName() const { return m_strName; };

public:
	void AddRef();
	UINT Release();

public:
	_bool operator==(const UObject& _other) const { return m_iInstanceId == _other.m_iInstanceId; }
	_bool operator!=(const UObject& _other) const { return !(*this == _other); }

public:
	const UINT getRefCnt() const { return m_iRefCount; }

public:
	virtual void Serialize(CSerialzer& _s);

protected:
	wstring m_strName;
	UINT m_iInstanceId;

private:
	static atomic<UINT> s_iNextInstanceID;
	atomic<UINT> m_iRefCount;
};

END