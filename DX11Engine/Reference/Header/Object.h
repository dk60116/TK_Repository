#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL UObject abstract
{
public:
	UObject();
	virtual ~UObject();

public:
	void AddRef();
	UINT Release();

public:
	const wstring& Get_UName() const;
	const UINT Get_RefCnt() const;

protected:
	wstring m_strName;
	UINT m_iInstanceId;

private:
	static atomic<UINT> s_iNextInstanceID;
	atomic<UINT> m_iRefCount;
};

NS_END