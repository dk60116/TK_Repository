#pragma once

#include "epch.h"
#include "Scene.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneManager final 
{
	SINGLETONCLASS(CSceneManager);

public:
	enum LightMapSizeOptions { Low, Middle, High, SuperHigh, UltraHigh };

	struct LightSettings
	{
		_float ambient = 0.2f;
		_uint shadowMapSize = 2048;
	};

public:
	HRESULT Initialize();
	void Release();

public:
	CScene* CreateScene(CScene* _newScene, wstring _name);
	CScene* Get_CrtScene();
	CScene* Get_TempScene();
	const map<wstring, CScene*>& Get_SceneList();
	const _bool Is_Loading() const;

	void LoadScene(wstring _scene);
	void LoadScene(CScene* _scene);
	void LoadComplete();

public:
	const LightSettings& Get_LightSettings();

public:
	class CCamera* Get_EditorCamera();

public:
	void Set_ShadowMapSize(const LightMapSizeOptions size);

private:
	CScene* m_pCrtScene;
	CScene* m_pTempScene;
	map<wstring, CScene*> m_mSceneList;

	_bool m_bLoading;

	class CGameObject* m_pEditorCamObj;
	class CEditorCamera* m_pEditorCamera;

	LightSettings m_sLightSettings;
	LightMapSizeOptions m_eShadowQuality;
};

NS_END

