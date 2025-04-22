#pragma once

#include "UObject.h"
#include "Engine_Define.h"
#include "CScene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement
{
	SINGLETONCLASS(CManagement);

public:
	void CreateScene(CScene* _newScene, wstring _name);
	CScene* getCrtScene() { return m_pCrtScene; }
	HRESULT LoadScene(wstring _scene);

	void Destroy();

public:
	LPDIRECT3DDEVICE9 getGraphicDevice() { return m_pGraphicDev; }
	void SetGraphicDevice(LPDIRECT3DDEVICE9 _gd);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CScene* m_pCrtScene;
	map<wstring, CScene*> m_mSceneList;
};

END