#pragma once

#include "epch.h"
#include "Scene.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneManager final 
{
	SINGLETONCLASS(CSceneManager);

public:
	struct LayerFlag
	{
		_uint index = 0;
		_uint value = 0;
		wstring name = L"";
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
	static void Add_Layer(const _uint _index, const wstring& _name);
	static void Add_Tag(const _uint _index, const wstring& _name);
	static _uint NameToLayer(const wstring& _name);
	static wstring LayerToName(const _uint _layer);
	static _uint LayerToIndex(const _uint _layer);
	static _uint NameToIndex(const wstring& _name);
	static wstring TagToName(const _uint _tag);
	static _bool LayerMaskResult(const _uint _source, const _uint _mask);
	static _bool CompareTag(const _uint _tag, const wstring& _tagName);

public:
	class CCamera* Get_EditorCamera();

private:
	CScene* m_pCrtScene;
	CScene* m_pTempScene;
	map<wstring, CScene*> m_mSceneList;

	_bool m_bLoading;

	class CGameObject* m_pEditorCamObj;
	class CEditorCamera* m_pEditorCamera;

	vector<LayerFlag> m_vLayerFlags;
	map<_uint, wstring> m_mTagFlags;
};

NS_END

