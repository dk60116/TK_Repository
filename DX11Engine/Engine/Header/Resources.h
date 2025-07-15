#pragma once

#include "epch.h"
#include "Shader.h"
#include "Texture.h"
#include "AnimationClip.h"

#include <filesystem>
namespace fs = std::filesystem;

NS_BEGIN(Engine)

class ENGINE_DLL CResources final
{
	SINGLETONCLASS(CResources);

public:
    HRESULT Initialize();

private:
	void Release();

public:
    static void LoadComplete_Game(const CEngineResource* _ptr);
    static void LoadComplete_Scene(const CEngineResource* _ptr);

public:
    template<typename T>
    T* CreateGameResource(const wstring& _name, const wstring& _path, void* _desc = nullptr);

	template<typename T>
	T* CreateSceneResource(const wstring& _name, const wstring& _path, void* _desc = nullptr, const _bool _tempScene = false);

    template<typename T>
    T* LoadOnGame(const wstring& _name);

    template<typename T>
    T* CloneOnGame(const wstring& _name);

    template<typename T>
	T* LoadOnScene(const wstring& _name);

	static _bool FileExists(const wstring& _path);
	static _bool FileExists(const string& _path);

    unordered_map<wstring, CEngineResource*> m_mEditorResourceList;
    unordered_map<wstring, CEngineResource*> m_mGameResourceList;

private:
	wstring m_strDefaultAssetPath;
	wstring m_strEngineFilePath;
};

NS_END

template<typename T>
inline T* CResources::CreateGameResource(const wstring& _name, const wstring& _path, void* _desc)
{
    T* newResource = T::Create(m_strDefaultAssetPath + _path);

    if (!newResource)
        return nullptr;

    if (FAILED(newResource->Initialize(_name, m_strDefaultAssetPath + _path, _desc)))
    {
        delete newResource;
        return nullptr;
    }

    m_mGameResourceList.emplace(_name, newResource);
	newResource->AddRef();

    return newResource;
}

template<typename T>
inline T* CResources::CreateSceneResource(const wstring& _name, const wstring& _path, void* _desc, const _bool _tempScene)
{
	T* newResource = T::Create(m_strDefaultAssetPath + _path);

    if (FAILED(newResource->Initialize(_name, m_strDefaultAssetPath + _path, _desc)))
    {
        delete newResource;
        return nullptr;
    }

	CScene* targetScene = _tempScene ? CSceneManager::GetInstance().Get_TempScene() :
		CSceneManager::GetInstance().Get_CrtScene();

    if (!_tempScene)
        targetScene->Add_Resource(_name, newResource);
    else
        targetScene->Add_TempResource(_name, newResource);

    return newResource;
}

template<typename T>
inline T* CResources::LoadOnGame(const wstring& _name)
{
    auto iter = m_mGameResourceList.find(_name);

    if (iter == m_mGameResourceList.end())
        return nullptr;

    T* resultResource = dynamic_cast<T*>(iter->second);

    return resultResource;
}

template<typename T>
inline T* CResources::CloneOnGame(const wstring& _name)
{
    T* proto = LoadOnGame<T>(_name);

    return T::Clone(*proto);
}

template<typename T>
inline T* CResources::LoadOnScene(const wstring& _name)
{
    CEngineResource* r = nullptr;

    if (CSceneManager::GetInstance().Get_CrtScene())
        r = CSceneManager::GetInstance().Get_CrtScene()->Find_Resource(_name);

	T* resultResource = dynamic_cast<T*>(r);

	if (!r)
	{
		if (!CSceneManager::GetInstance().Get_TempScene())
			return nullptr;

		CEngineResource* r = CSceneManager::GetInstance().Get_TempScene()->Find_Resource(_name);

		T* resultResource = dynamic_cast<T*>(r);
	}

	return resultResource;
}
