#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "CCamera.h"
#include "CLight.h"

BEGIN(Engine)

class CManagement;
class CGameObject;

struct SceneOptions
{
    _bool lighting;
};

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

    CGameObject* AddObject(wstring _objName, Layer _layer);

    void AddCamera(CCamera* _cam);
    void AddLight(CLight* _light);
    vector<CCamera*>& getCamList() { return m_vCameraList; }
    vector<CLight*>& getLightList() { return m_vLightList; }
    CCamera* getCamera(const int _index) const { return m_vCameraList[_index]; }
    CCamera* getCamera() const { return m_vCameraList.back(); }
    void UpdateAllLight();
    void UpdateAllCameraResolution();
    SceneOptions& getOptions() { return m_sOptions; }

    void SetGraphicDev(LPDIRECT3DDEVICE9 _device) { m_pGraphicDev = _device; }

protected:
    wstring m_strSceneName;

    list<CGameObject*> m_lObjectList[Layer::LAYER_END];
    vector<CCamera*> m_vCameraList;
    vector<CLight*> m_vLightList;

    SceneOptions m_sOptions;

    LPDIRECT3DDEVICE9 m_pGraphicDev;
};

END