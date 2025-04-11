#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CResource
{
public:
	CResource();
	~CResource();

public:
	virtual HRESULT Load(LPDIRECT3DDEVICE9 _device) PURE;
	virtual void Release() PURE;

public:
	const wstring& getPath() { return m_strFilePath; };
	void SetPath(const wstring _path) { m_strFilePath = _path; }
	const wstring& getName() { return m_strName; };
	void SetName(const wstring _name) { m_strName = _name; }

protected:
	wstring m_strFilePath;
	wstring m_strName;
	_bool m_bLoaded;
};

END
