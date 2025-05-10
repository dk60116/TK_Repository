#pragma once

#include "Object.h"
#include "Engine_Define.h"
#include "CLight.h"
#include "Camera.h"
#include "CDebug.h"

BEGIN(Engine)

class CManagement;
class CGameObject;

struct SceneOptions
{
    _bool lighting;
};

class ENGINE_DLL CScene
    : public UObject
{
protected:
    CScene();
    ~CScene();

public:
    virtual void EditorInit();
    virtual void Awake();
    virtual void Start();
    void UpdateEditor();
    virtual void Update();
    virtual void FixedUpdate();
    void LateUpdateEditor();
    virtual void LateUpdate();
    virtual void Render_Editor();
    virtual void Render_Game();
    virtual void SceneRelease();
    virtual void Destroy();

public:
    void SetName(const wstring _name) { m_strSceneName = _name; }
    const wstring& getName() const { return m_strSceneName; }

    CGameObject* AddObject(wstring _objName, Layer _layer);

    void AddCamera(CCamera* _cam);
    void AddLight(CLight* _light);
    list<CCamera*>& getCamList() { return m_lCameraList; }
    list<CLight*>& getLightList() { return m_lLightList; }
    CCamera* getCamera() const { return m_lCameraList.back(); }
    void UpdateAllLight();
    void UpdateSceneCameraResolution(const vector2Int _resolution);
    void UpdateAllCameraResolution(const vector2Int _resolution);
    SceneOptions& getOptions() { return m_sOptions; }

    void SetGraphicDev(LPDIRECT3DDEVICE9 _device) { m_pGraphicDev = _device; }

    vector<CGameObject*> getRootObjects();
    CGameObject* FindGameObject(const wstring _name);

private:
    void SafeDestroyObject(CGameObject* _obj);
    void Render_Grid();

protected:
    wstring m_strSceneName;

    list<CGameObject*> m_lObjectList;
    list<CCamera*> m_lCameraList;
    list<CLight*> m_lLightList;

    SceneOptions m_sOptions;

    LPDIRECT3DDEVICE9 m_pGraphicDev;

    UINT m_iObjIndex;
};

END