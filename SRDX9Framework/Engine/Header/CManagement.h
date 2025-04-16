#pragma once

#include "Engine_Define.h"
#include "CScene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement
{
	SINGLETONCLASS(CManagement);

public:
	template<typename T>
	void CreateScene(wstring _name);
	CScene* getCrtScene() { return m_pCrtScene; }
	HRESULT ChangeScene(wstring _scene);

	void Release();

public:
	LPDIRECT3DDEVICE9 getGraphicDevice() { return m_pGraphicDev; }
	void SetGraphicDevice(LPDIRECT3DDEVICE9 _gd) { m_pGraphicDev = _gd; }

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	CScene* m_pCrtScene;
	map<wstring, CScene*> m_mSceneList;
};

END

template<typename T>
inline void CManagement::CreateScene(wstring _name)
{
	T* newScene = new T;

	CScene* scene = dynamic_cast<CScene*>(newScene);

	if (scene)
	{
		scene->SetName(_name);
		scene->SetGraphicDev(m_pGraphicDev);

		m_mSceneList.insert({ _name, scene });
	}
}
