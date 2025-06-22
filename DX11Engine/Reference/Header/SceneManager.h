#pragma once

#include "epch.h"
#include "Scene.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneManager final 
{
	SINGLETONCLASS(CSceneManager);

public:
	void Release();

public:
	CScene* CreateScene(CScene* _newScene, wstring _name);
	CScene* Get_CrtScene();

	void LoadScene(wstring _scene);

private:
	CScene* m_pCrtScene;
	map<wstring, CScene*> m_mSceneList;
};

NS_END

