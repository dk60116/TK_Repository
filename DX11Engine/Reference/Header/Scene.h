#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CScene abstract : public UObject
{
    friend class CSceneManager;

protected:
    CScene();
    ~CScene();

public:
    virtual HRESULT Initialize();
    virtual void Awake();
    virtual void Start();
    virtual void UpdateEditor();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdateEditor();
    virtual void LateUpdate();
    virtual void Render_Editor();
    virtual void Render_Game();
    virtual void SceneRelease();

public:
    void Set_Name(const wstring _name);
    const wstring& Get_Name() const;

public:
    class CGameObject* Add_GameObject(wstring _name);
    class CCamera* Get_Camera() const;
    class CCamera* Get_Camera(const _int _index) const;
    list <class CCamera*>& Get_CameraList();
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
};

NS_END

