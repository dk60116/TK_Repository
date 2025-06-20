#pragma once

#include "Object.h"
#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CScene final : public UObject
{
protected:
    CScene(const wstring _name, ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    ~CScene();

public:
    HRESULT Initialize() override;

public:
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
    void SetName(const wstring _name);
    const wstring& getName() const;

public:
    CGameObject* Add_GameObject(wstring _name);
    CGameObject* Instantiate(CGameObject* _gameObject);

private:
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pContext;

private:
    UINT m_iSceneIndex;
    wstring m_strSceneName;
    list<CGameObject*> m_lObjectList;
};

NS_END

