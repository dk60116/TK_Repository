#pragma once

#include "Object.h"
#include "Renderer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CScene abstract : public UObject
{
    friend class CSceneManager;
    friend class CSceneLoader;

public:
    struct LightSettings
    {
        float ambient = 0.1f;
    };

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
    class CEngineResource* Add_Resource(const wstring& _name, CEngineResource* _resource);
    class CEngineResource* Find_Resource(const wstring& _name);
    vector<MeshBundle> Find_MeshInfoResource(const wstring& _name);
    class CEngineResource* Add_TempResource(const wstring& _name, CEngineResource* _resource);
    void Add_MeshBundle(const wstring& _name, vector<MeshBundle> _resource);
    void Add_TempMeshBundle(const wstring& _name, vector<MeshBundle> _resource);
    class CEngineResource* Add_CloneResourece(CEngineResource* _resource);
    class CGameObject* Add_GameObject(wstring _name);
    vector<CGameObject*> Get_RootObjects();
    vector<CRenderer*> Get_MeshObjects();

    const LightSettings& Get_LightSetting();

    class CCamera* Get_Camera() const;
    CCamera* Get_Camera(const _int _index) const;
    CCamera* Get_EditorCamera() const;
    const list <CCamera*>& Get_CameraList();
    CCamera* Add_Camera(CCamera* _camera);

    const list<class CLight*>& Get_LightList();
    CLight* Add_Light(CLight* _light);

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
    LightSettings m_sLightSettings;
    list <CGameObject*> m_lObjectList;
    list <CCamera*> m_lCameraList;
    list <CLight*> m_lLightList;
    list<CCanvas*> m_lCanvasList;

    CCamera* m_pEditorCamera;

    unordered_map<wstring, CEngineResource*> m_mResourceList, m_mTempResourceList;
    unordered_map<wstring, vector<MeshBundle>> m_mMeshBundleList, m_mTempMeshBundleList;
    vector<CEngineResource*> m_vCloneResourceList;

protected:
    _uint m_iUniqueObjectCount;

    ID3D11DepthStencilState* m_pMeshDepthStencilState, * m_pUIDepthStencilState;
};

NS_END

