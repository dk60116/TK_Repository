#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CScene abstract : public UObject
{
    friend class CSceneManager;

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
    class CCamera* Get_Camera() const;
    class CCamera* Get_Camera(const _int _index) const;
    class CCamera* Get_EditorCamera() const;
    list <CCamera*>& Get_CameraList();
    class CCamera* Add_Camera(class CCamera* _camera);
    class CGameObject* Instantiate(class CGameObject* _gameObject);

private:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;

private:
    UINT m_iSceneIndex;
    wstring m_strSceneName;
    list <class CGameObject*> m_lObjectList;
    list <class CCamera*> m_lCameraList;

    class CCamera* m_pEditorCamera;

    unordered_map<wstring, class CEngineResource*> m_mResourceList;
    unordered_map<wstring, class CEngineResource*> m_mTempResourceList;
};

NS_END

