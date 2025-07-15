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
    void Set_SceneName(const wstring _name);
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
    const list <CCamera*>& Get_CameraList();
    CCamera* Add_Camera(CCamera* _camera);

    class CCanvas* Get_Canvas(const _int _index) const;
    const list <CCanvas*>& Get_CanvasList();
    CCanvas* Add_Canvas(CCanvas* _canvas);

    CGameObject* Instantiate(CGameObject* _gameObject);
    HRESULT SaveScene(const wstring& _filePath);
    const _uint Get_UniqueObjectCount() const;

public:
    ID3D11DepthStencilState* Get_MeshStencillState() const;
    ID3D11DepthStencilState* Get_UIStencillState() const;

protected:
    HRESULT PreLoadResources();

protected:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;

protected:
    UINT m_iSceneIndex;
    wstring m_strSceneName;
    list <CGameObject*> m_lObjectList;
    list <CCamera*> m_lCameraList;
    list<CCanvas*> m_lCanvasList;

    CCamera* m_pEditorCamera;

    unordered_map<wstring, CEngineResource*> m_mResourceList;
    unordered_map<wstring, CEngineResource*> m_mTempResourceList;

protected:
    _uint m_iUniqueObjectCount;

    ID3D11DepthStencilState* m_pMeshDepthStencilState, * m_pUIDepthStencilState;
};

NS_END

