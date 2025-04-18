#pragma once

#include "UObject.h"
#include "Engine_Define.h"
#include "CResource.h"
#include "CDebug.h"

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

BEGIN(Engine)

class ENGINE_DLL CResources
	: public UObject
{
	SINGLETONCLASS(CResources);

public:
	HRESULT LoadAllFiles(LPDIRECT3DDEVICE9 _device);
	void Release();

public:
	template<typename T>
	void CreateResource(LPDIRECT3DDEVICE9 _device, wstring _name, wstring _path);

	template<typename T>
	shared_ptr<T> getResource(wstring _name);

public:
	wstring m_strBasePath;

private:
	unordered_map<type_index, unordered_map<wstring, shared_ptr<CResource>>> m_resourceMap;

	template<typename T>
	unordered_map<wstring, shared_ptr<CResource>>& GetContainer()
	{
		type_index type = typeid(T);
		if (m_resourceMap.find(type) == m_resourceMap.end())
			m_resourceMap[type] = unordered_map<wstring, shared_ptr<CResource>>();

		return m_resourceMap[type];
	}
};

END

template<typename T>
inline void CResources::CreateResource(LPDIRECT3DDEVICE9 _device, wstring _name, wstring _path)
{
	auto& container = GetContainer<T>();

	if (container.find(_name) == container.end())
	{
		auto resource = make_shared<T>();

		CResource* res = dynamic_cast<CResource*>(resource.get());
		if (!res)
			return;

		wstring fullPath = m_strBasePath + L"\\" + _path;

		res->SetName(_name);
		res->SetPath(fullPath);

		if (res->Load(_device) == S_OK)
			container[_name] = resource;
		else
			CDebug::Log(L"[Fail] Could not load: " + fullPath);
	}
}

template<typename T>
inline shared_ptr<T> CResources::getResource(wstring _name)
{
	auto& container = GetContainer<T>();

	auto iter = container.find(_name);

	if (iter != container.end())
		return static_pointer_cast<T>(iter->second);
	else
		return nullptr;
}
