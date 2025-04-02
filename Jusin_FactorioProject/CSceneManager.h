#pragma once

#include "CMonoBehaviour.h"
#include "CScene.h"

class CSceneManager
{
	SingletonClass(CSceneManager);

public:
	void Init();
	void Release();

public:
	void AddScene(CScene* _cscene);
	CScene* getCurrentscene() { return m_pCurrentScene; }
	void LoadScene(const wstring _scene);
	void ChangeScene(const wstring _scene);

private:
	CScene* m_pCurrentScene;
	map<wstring, CScene*> m_mSceneList;
};

