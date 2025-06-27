#pragma once

#include "epch.h"
#include "Texture.h"
#include "Animation.h"

NS_BEGIN(Engine)

class ENGINE_DLL CResources final
{
	SINGLETONCLASS(CResources);

private:
	void Release();

public:
	template<typename T>
	T* CreateResource(const wstring& _path);
};

NS_END

template<typename T>
inline T* CResources::CreateResource(const wstring& _path)
{
	T* newResource = T::Create(_path);

	if (FAILED(newResource->Initialize(_path)))
	{
		delete newResource;
		return nullptr;
	}

	CSceneManager::GetInstance().Get_CrtScene()->Add_Resource(newResource);

	return newResource;
}
