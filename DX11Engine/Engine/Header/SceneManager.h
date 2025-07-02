#pragma once

#include "epch.h"
#include "Scene.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneManager final 
{
	SINGLETONCLASS(CSceneManager);

public:
	HRESULT Initialize();
	void Release();

public:
	CScene* CreateScene(CScene* _newScene, wstring _name);
	CScene* Get_CrtScene();
	CScene* Get_TempScene();
	const _bool Is_Loading() const;

	void LoadScene(wstring _scene);
	void LoadComplete();

public:
	class CEditorCamera* Get_EditorCamera();

private:
	CScene* m_pCrtScene;
	CScene* m_pTempScene;
	map<wstring, CScene*> m_mSceneList;

	_bool m_bLoading;

	class CGameObject* m_pEditorCamObj;
	class CEditorCamera* m_pEditorCamera;
};

NS_END

