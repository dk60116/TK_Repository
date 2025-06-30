#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEngineResource abstract : public UObject
{
	friend class CResources;

protected:
		explicit CEngineResource();
		~CEngineResource();

protected:
	virtual HRESULT Initialize(const wstring& _filePath, void* _desc);
	virtual void OnDestroy();

public:
	const wstring& Get_ResourceName() const;

public:
	HRESULT Load(const wstring& path);

protected:
	wstring m_strResourceName;
	wstring m_strFilePath;
};

NS_END

