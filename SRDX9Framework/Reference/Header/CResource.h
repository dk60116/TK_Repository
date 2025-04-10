#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CResource
{
public:
	CResource();
	~CResource();

public:
	virtual HRESULT Load();
	virtual void Release();

public:
	const wstring& getPath() { return m_strFilePath; };
	const wstring& getName() { return m_strFilePath; };

private:
	wstring m_strFilePath;
	wstring m_strName;
	_bool m_bLoaded;
};

END
