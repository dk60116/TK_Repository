#pragma once

#include "UObject.h"
#include "Engine_Define.h"
#include "CScene.h"
#include "CGameObject.h"
#include "CEditorCamera.h"

BEGIN(Engine)

class ENGINE_DLL CManagement
{
	SINGLETONCLASS(CManagement);

public:
	void CreateScene(CScene* _newScene, wstring _name);
	CScene* getCrtScene() { return m_pCrtScene; }
	HRESULT LoadScene(wstring _scene);

public:
	void EditorUpdate();
	void Destroy();

public:
	LPDIRECT3DDEVICE9 getGraphicDevice() { return m_pGraphicDev; }
	void SetGraphicDevice(LPDIRECT3DDEVICE9 _gd);

	CEditorCamera& getEditorCamera() { return *m_pEditorCam; }

private:
 	void CreateEditorCamera();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CScene* m_pCrtScene;
	map<wstring, CScene*> m_mSceneList;

	CGameObject* m_cEditorCamObj;
	CEditorCamera* m_pEditorCam;
};

END