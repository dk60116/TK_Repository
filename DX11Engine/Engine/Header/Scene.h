#pragma once

#include <array>

#include "Object.h"
#include "Renderer.h"
#include "SkinnedMeshBuffer.h"
#include "NaviMesh.h"

NS_BEGIN(Engine)

class ENGINE_DLL CScene abstract : public UObject
{
    friend class CSceneManager;
    friend class CSceneLoader;

public:
    typedef struct ObjectsRectTransfomInfo
    {
        _float2 anchoredPos = {};
        _float2 widthHeight = {};
        _float2 pivot = {};
        _float2 anchorMin = {};
        _float2 anchorMax = {};
    }SCENERECTINFO;

    typedef struct ObjectsTransformInfo
    {
        _uint objID = 0;
        wstring objName = L"";
        _float3 localPos = {};
        _float4 localQuaternion = {};
        _float3 localScale = {};
        _bool isRect = false;
        SCENERECTINFO rectInfo = {};
    }SCENETRANSFORMINFO;

public:
    struct LightSettings
    {
        wstring skyBox = L"DefaultSky (SkyBox)";
        _float ambient = 0.1f;
    };

protected:
    CScene();
    ~CScene();

public:
    virtual HRESULT Initialize();
    virtual void Awake();
    virtual void Start();
    virtual void Update_Editor();
    void PrevUpdate();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdateEditor();
    virtual void LateUpdate();
    virtual void Render_Editor();
    virtual void Render_Game();
    virtual void SceneRelease();

protected:
    void RenderSkyBox(CCamera* _camera);

public:
    void Set_SceneName(const wstring _name);
    const wstring& Get_SceneName() const;
    const _bool IsStarted() const;

public:
    vector<SCENETRANSFORMINFO> Convert_ObjectsTransformInfo() const;
    void Bind_ObjectsTransform(const vector<SCENETRANSFORMINFO> _infoList);
    vector<CGameObject*> Get_NavigationStaticObjects();

public:
    class CEngineResource* Add_Resource(const wstring& _name, CEngineResource* _resource);
    class CEngineResource* Find_Resource(const wstring& _name);
    vector<MeshBundle> Find_MeshInfoResource(const wstring& _name);
    vector<SkinnedMeshBundle> Find_SkinnedMeshInfoResource(const wstring& _name);
    vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> Find_SkinnedBonesResource(const wstring& _name);
    class CEngineResource* Add_TempResource(const wstring& _name, CEngineResource* _resource);
    void Add_MeshBundle(const wstring& _name, vector<MeshBundle> _resource);
    void Add_SkinnedBundle(const wstring& _name, vector<SkinnedMeshBundle> _resource);
    void Add_TempMeshBundle(const wstring& _name, vector<MeshBundle> _resource);
    void Add_TempSkinnedBundle(const wstring& _name, vector<SkinnedMeshBundle> _resource);
    void Add_SkinnedMeshBone(const wstring& _name, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> _resource);
    void Add_TempSkinnedMeshBone(const wstring& _name, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> _resource);
    class CEngineResource* Add_CloneResourece(CEngineResource* _resource);
    class CGameObject* Add_GameObject(wstring _name);
    list<CGameObject*>& Get_ObjectList();
    vector<CGameObject*> Get_RootObjects();
    vector<CRenderer*> Get_MeshObjects(const _uint _layerMask = 0xFFFFFFFF);
    vector<EngineAI::CNaviMesh*> Get_NavMeshes();

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

    HRESULT SaveScene(const wstring& _filePath);
    const _uint Get_UniqueObjectCount() const;

	const _float Get_PassedTime() const;

public:
    ID3D11DepthStencilState* Get_MeshStencillState() const;
    ID3D11DepthStencilState* Get_TransparentStencillState() const;
    ID3D11DepthStencilState* Get_UIStencillState() const;

    ID3D11RasterizerState* Get_NoneBlendingResterState() const;
    ID3D11RasterizerState* Get_NoneBlendingNoneCullResterState() const;
    ID3D11RasterizerState* Get_BlendingResterState() const;

    ID3D11BlendState* Get_NoneBlendingState() const;
    ID3D11BlendState* Get_BlendingState() const;

protected:
    HRESULT PreLoadResources();
    HRESULT InitializeDeferredResources();
    HRESULT CreateGBuffer(_uint _width, _uint _height);
    void ReleaseGBuffer();
    void BindGBufferTargets();
    void ClearGBufferTargets(const ColorValue& _clearColor);
    void RenderDeferredLighting(CCamera* _camera);

protected:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;

protected:
    UINT m_iSceneIndex;
    wstring m_strSceneName;
    _bool m_bSceneStarted;
    LightSettings m_sLightSettings;
    class CSkyBox* m_pSkyBox;
    list <CGameObject*> m_lObjectList;
    list <CCamera*> m_lCameraList;
    list <CLight*> m_lLightList;
    list<CCanvas*> m_lCanvasList;

    CCamera* m_pEditorCamera;

    unordered_map<wstring, CEngineResource*> m_mResourceList, m_mTempResourceList;
    unordered_map<wstring, vector<MeshBundle>> m_mMeshBundleList, m_mTempMeshBundleList;
    unordered_map<wstring, vector<SkinnedMeshBundle>> m_mSkinnedBundleList, m_mTempSkinnedBundleList;
    unordered_map<wstring, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL>> m_mSkinnedBoneList, m_mTempSkinnedBoneList;
    vector<CEngineResource*> m_vCloneResourceList;

    EngineAI::CNaviMesh* m_pSceneNaviMesh_Walkable, * m_pSceneNaviMesh_WalkUnable;

protected:
    _uint m_iUniqueObjectCount;
    _bool m_bUseNavi;

    ID3D11DepthStencilState* m_pSkyBoxDepthStencillState, * m_pMeshDepthStencilState, * m_pTransparentDepthStencilState, * m_pUIDepthStencilState;
    ID3D11RasterizerState* m_pSkyBoxResterizerState, * m_pMeshResterizerState, * m_pNoneCullResterizeState, *m_pBlendResteraizerState, * m_pUIResterizerState;
    ID3D11BlendState* m_pBlendingState, * m_pNoneBlendingState;
    
    _float m_fPassedTime;

    struct GBufferTarget
    {
        ID3D11Texture2D* texture = nullptr;
        ID3D11RenderTargetView* rtv = nullptr;
        ID3D11ShaderResourceView* srv = nullptr;
    };

    array<GBufferTarget, 3> m_gbufferTargets = {};
    ID3D11Texture2D* m_pGBufferDepth = nullptr;
    ID3D11DepthStencilView* m_pGBufferDSV = nullptr;
    ID3D11ShaderResourceView* m_pGBufferDepthSRV = nullptr;
    _uint m_gbufferWidth = 0;
    _uint m_gbufferHeight = 0;
    _bool m_bUseDeferred = true;
    class CShader* m_pDeferredGBufferShader = nullptr;
    class CMaterial* m_pDeferredLightingMaterial = nullptr;
    class CMeshBuffer* m_pDeferredQuad = nullptr;
};

NS_END
