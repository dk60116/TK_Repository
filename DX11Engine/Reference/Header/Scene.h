#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CScene abstract : public UObject
{
    friend class CSceneManager;
    friend class CSceneLoader;

protected:
    CScene();
    ~CScene();

protected:
    HRESULT PreLoadResources();

public:
    virtual HRESULT Initialize();
    virtual void Awake();
    virtual void Start();
    virtual void Update_Editor();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdateEditor();
    virtual void LateUpdate();
    virtual void Render_Editor();
    virtual void Render_Game();
    virtual void SceneRelease();

public:
    void Set_Name(const wstring _name);
    const wstring& Get_SceneName() const;

public:
    class CEngineResource* Add_Resource(const wstring& _name, class CEngineResource* _resource);
    class CEngineResource* Find_Resource(const wstring& _name);
    class CEngineResource* Add_TempResource(const wstring& _name, class CEngineResource* _resource);
    class CGameObject* Add_GameObject(wstring _name);
    vector<CGameObject*> Get_RootObjects();
    class CCamera* Get_Camera() const;
    CCamera* Get_Camera(const _int _index) const;
    CCamera* Get_EditorCamera() const;
    list <CCamera*>& Get_CameraList();
    CCamera* Add_Camera(CCamera* _camera);
    CGameObject* Instantiate(CGameObject* _gameObject);
    HRESULT SaveScene(const wstring& _filePath);

protected:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;

protected:
    UINT m_iSceneIndex;
    wstring m_strSceneName;
    list <CGameObject*> m_lObjectList;
    list <CCamera*> m_lCameraList;

    CCamera* m_pEditorCamera;

    unordered_map<wstring, CEngineResource*> m_mResourceList;
    unordered_map<wstring, CEngineResource*> m_mTempResourceList;

protected:
    _uint m_iUniqueObjectCount;
};

NS_END

