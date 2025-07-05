#pragma once

#include "epch.h"
#include "Texture.h"
#include "Animation.h"

#include <filesystem>
namespace fs = std::filesystem;

NS_BEGIN(Engine)

class ENGINE_DLL CResources final
{
	SINGLETONCLASS(CResources);

private:
	void Release();

public:
	template<typename T>
	T* CreateResource(const wstring& _name, const wstring& _path, void* _desc = nullptr, const _bool _tempScene = false);

	template<typename T>
	T* LoadOnScene(const wstring& _name);

	static _bool FileExists(wstring& _path);
	static _bool FileExists(string& _path);

private:
	wstring m_strDefaultAssetPath;
	wstring m_strEngineFilePath;
};

NS_END

template<typename T>
inline T* CResources::CreateResource(const wstring& _name, const wstring& _path, void* _desc, const _bool _tempScene)
{
	T* newResource = T::Create(m_strDefaultAssetPath + _path);

	if (FAILED(newResource->Initialize(_name, m_strDefaultAssetPath + _path, _desc)))
	{
		delete newResource;
		return nullptr;
	}

	if (!_tempScene)
		CSceneManager::GetInstance().Get_CrtScene()->Add_Resource(_name, newResource);
	else
		CSceneManager::GetInstance().Get_TempScene()->Add_TempResource(_name, newResource);

	return newResource;
}

template<typename T>
inline T* CResources::LoadOnScene(const wstring& _name)
{
	CEngineResource* r = CSceneManager::GetInstance().Get_CrtScene()->Find_Resource(_name);

	T* resultResource = dynamic_cast<T*>(r);

	if (!r)
	{
		CEngineResource* r = CSceneManager::GetInstance().Get_TempScene()->Find_Resource(_name);

		T* resultResource = dynamic_cast<T*>(r);
	}

	return resultResource;
}
