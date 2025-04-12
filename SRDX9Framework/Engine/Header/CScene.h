#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "CGameObject.h"
#include "CCamera.h"

BEGIN(Engine)

class ENGINE_DLL CScene
{
public:
    CScene();
    ~CScene();

public:
    virtual void Awake();
    virtual void Start();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void LateUpdate();
    virtual void Render();
    virtual void Release();

public:
    void SetName(const wstring _name) { m_strSceneName = _name; }
    wstring& getName() { return m_strSceneName; }

    CGameObject* AddObject(LPDIRECT3DDEVICE9 _device, wstring _objName, Layer _layer);

    CCamera* getCamera(const int _index) { return m_vCameraList[_index]; }
    void UpdateAllCameraResolution();

protected:
    wstring m_strSceneName;

    list<CGameObject*> m_lObjectList[Layer::LAYER_END];

    vector<CCamera*> m_vCameraList;
};

END